#include "Node.h"
void Node::initialize(){
    Base::construct();

    this->x_ = par("x").doubleValue();
    this->y_ = par("y").doubleValue();
    getDisplayString().setTagArg("p", 0, x_);
    getDisplayString().setTagArg("p", 1, y_);

    //选举簇头
    if(dblrand() <= Probability_){
        this->isClusterHead_ = true;
    }

    myData_.resize(DataMessageLength_);

    scheduleAfter(0.01,new cMessage("initial"));
}
//多个if-else可用策略模式+工厂模式解耦,或者构建反射+策略模式
void Node::handleMessage(cMessage *msg){
  if(isLive_ == false)return;
  //一个自我消息，初始化过程的延续，不可以直接调用，有的节点还没初始化完呢，需要加个延迟
  if(strcmp(msg->getName(),"initial") == 0){
      FindNodeWithinDistance();
  }
  //Hello消息，也就是构建路由的消息,只处理没有父节点的节点
  else if(strcmp(msg->getName(),"Hello") == 0 && isJoinOtherCluster_ == false){
      calcEnergyConsumption(msg->getSenderModule(),Type::receiveMsg);//计算能耗，这里是接收Hello消息的能耗
      //得到Hello消息，构建路由,因为自己没有加入簇里，所以加入簇
      connectToSendModule(msg->getSenderModule());
      if(isClusterHead_ == true){
          for(int i=0;i<this->otherNodeListWithinMaxDistance_.size();++i){
              sendForDelayAndCalcEnergyConsumption(new cMessage("Hello"), otherNodeListWithinMaxDistance_[i].second,Type::sendMsg);
          }
      }
  }
  //接收到数据切片的消息,这里假定每隔一段时间，节点自动会进行数据切片,这是个self-message
  //只有叶子节点切片
  else if(strcmp(msg->getName(),"DataSlicing") == 0){
    //叶子节点，执行切片
    if(this->getGateNames().size() == 2){
        //因为一段时间会自动数据切片，因而不需要接收消息能耗
        //一份数据给自己
        myData_.resize(DataMessageLength_);
        for(int i=0;i<myData_.size();++i){
            myData_[i] = i;
        }
        //其余给周围节点。可能会出现多份数据给同一个节点
        for(int i=1;i<SlicingNumber_;++i){
            MyMsg * myMsg = gengerateMyMsg("SlicingData",DataMessageLength_);
            int randNum = intrand(otherNodeListWithinMaxDistance_.size());
            //这里可能出现发送给死亡节点导致的丢包
            sendForDelayAndCalcEnergyConsumption(myMsg, otherNodeListWithinMaxDistance_[randNum].second, Type::sendData);
        }
    }
    //非叶子节点不切片
    else{
        myData_.resize(DataMessageLength_);
        for(int i=0;i<myData_.size();++i){
            myData_[i] = 3*i;
        }
    }
  }
  //如果接收到其他节点发送来的切片数据
  else if(strcmp(msg->getName(),"SlicingData") == 0){
      calcEnergyConsumption(msg->getSenderModule(), Type::receiveData);//计算能耗
      aggregationAndCalcEnergyConsumption(msg);
  }
  //数据聚合，只有叶子节点需要处理这个消息，同样的，这是个self-message
  else if(strcmp(msg->getName(),"DataAggregate") == 0){
      //是否是叶子节点,叶子节点只有两个门，一个parent门，一个in@direct门
      if(this->getGateNames().size() == 2){
          sendAggregationDataAndCalcEnergyConsumption("AggregateData");
      }
  }
  //接受到来自子节点的数据
  else if(strcmp(msg->getName(),"AggregateData") == 0){
      calcEnergyConsumption(msg->getSenderModule(), Type::receiveData);
      aggregationAndCalcEnergyConsumption(msg);
      ++childNodeNumber_;
      //非叶子节点有额外的两个门，一个parent门，一个in@direct门
      if(childNodeNumber_ == getGateNames().size() -2){
          sendAggregationDataAndCalcEnergyConsumption("AggregateData");
          childNodeNumber_ = 0;
      }
  }

}

void Node::finish(){}
