#include "Server.h"
#include "Util.hpp"
void Server::initialize(){
    Base::construct();
//    EV<<"server initial"<<endl;
    this->x_ = par("x").doubleValue();
    this->y_ = par("y").doubleValue();
    //server作为网络的起点，默认在网络中
    isJoinOtherCluster_ = true;
    packetLossRate_ = registerSignal("PacketLossRate");
    //Server的能耗是无限的
    InitEnergy_ = 99999999;

    myData_.resize(DataMessageLength_);

    getDisplayString().setTagArg("p", 0, x_);
    getDisplayString().setTagArg("p", 1, y_);
    scheduleAfter(0.01,new cMessage("initial"));
    scheduleAt(1,new cMessage("BuildNetworkSignal"));
    scheduleAt(1.1, new cMessage("ReBuildNetworkSignal"));
    for(int i=1;i<1000;++i){
        scheduleAt(i+0.3,new cMessage("DataSlicingSignal"));
        scheduleAt(i+0.6,new cMessage("DataAggregateSignal"));
        scheduleAt(i+0.9,new cMessage("ReadDateSignal"));
    }

}
void Server::handleMessage(cMessage *msg){
    if(isLive_ == false || InitEnergy_ <=0)return;
    //这里应该除以加入网络的节点数量
    if(NodeDieNumber_/(double)NodeNumber_ >=0.8){
        EV<<"仿真时间："<<simTime()<<endl;
        endSimulation();
    }
    if(strcmp(msg->getName(),"initial") == 0){
        FindNodeWithinDistance();
    }
    //如果接收到构建网络的消息
    if(strcmp(msg->getName(),"BuildNetworkSignal") == 0){
        //向周围节点广播，按照一定概率选取簇头节点，其余的当作叶子节点。
        //这里以距离为依据，构建簇网络,这部分由节点完成。
        for(int i=0;i<this->otherNodeListWithinMaxDistance_.size();++i){
            sendForDelayAndCalcEnergyConsumption(new cMessage("Hello"),otherNodeListWithinMaxDistance_[i].second,Type::sendMsg);
        }
    }

    if(strcmp(msg->getName(),"ReBuildNetworkSignal") == 0){
        //第一步，得到所有的parent节点列表，也就是非叶子节点列表
        std::vector<Base *> parentList;
        for(int i=0;i<NodeNumber_;++i){
            cModule* mod = getParentModule()->getSubmodule("nodes", i);
            //判断条件，有一个in门和parent门以及其他child门
            if(mod->getGateNames().size() > 2){
                parentList.push_back(check_and_cast<Base *>(mod));
            }
        }
        //第二步，对每个parent节点的子节点进行提取操作。
        for(int i=0;i<parentList.size();++i){
            Base * parentNode = parentList[i];
            //获取parentNode的所有叶子节点列表
            std::vector<std::string> allGateName = parentNode->getGateNames();
            std::vector<Base *>leafList;
            for(auto gateName : allGateName){
                if(gateName == "parent" || gateName == "in"){
                    continue ;
                }
                cGate * gateHandle = parentNode->gate(gateName.c_str());
                leafList.push_back(check_and_cast<Base*>(gateHandle->getPathStartGate()->getOwnerModule()));
            }

            //现在我们拿到了叶子列表leafList，就可以开始操作
            //第三步，断开所有叶子节点和父节点的连接，方便后面操作
            //EV<<"leafList.size():"<<leafList.size()<<endl;
            for(auto& leafNodeHandle : leafList){
//                EV<<leafNodeHandle->getFullName()<<endl;
                //拿到父节点
                cModule * parentNodeHandle = leafNodeHandle->gate("parent")->getPathEndGate()->getOwnerModule();
                //拿到parent们对应的父节点的child门，然后删除对应的门
                cGate * childGateHandle = leafNodeHandle->gate("parent")->getPathEndGate();
                //断开两边的连接
                childGateHandle->disconnect();
                leafNodeHandle->gate("parent")->disconnect();
                parentNodeHandle->deleteGate(childGateHandle->getName());
                leafNodeHandle->deleteGate("parent");
            }
            //第四步、重构按照论文算法
            //是否加入簇，加入为true，不加入为false,这就是(Li-li)
            std::vector<Base*>isAppendCluster(leafList.begin(),leafList.end());
            int randNum = intrand(isAppendCluster.size());//获取[0 - leafList.size())内的一个随机数
            Base * head = isAppendCluster[randNum]; //Ns = Random(Li),Ns初始化成一个链表li
            Base * last = head;                     //尾插法，就是last
            Base * Ns = head;
            isAppendCluster.erase(isAppendCluster.begin()+randNum);
            //待定
//          connectModuleAToB(Ns, parentNode);  //该节点作为链表头节点，连接上父节点。
            //


            while(isAppendCluster.size()>0){
                //1.从Ns的周围广播节点中，找出广播范围内，且和leafList交集的部分。
                std::vector<Base *>Setn = getIntersectionOfTwo(Ns->otherNodeListWithinMaxDistance_,isAppendCluster);
                if(Setn.size() > 0 ){
                    int randtemp = intrand(Setn.size());
                    //既然是模拟，直接加入，并且计算能耗，一定是距离最近的先相应的,也就是tempList[0]
                    last->calcEnergyConsumption(Setn[randtemp], Type::sendMsg);//按照论文，就是rebuild消息的能耗
                    Setn[randtemp]->calcEnergyConsumption(last,Type::sendMsg); //按照论文，Join消息的能耗
                    last->calcEnergyConsumption(Setn[randtemp], Type::sendMsg);//按照论文，就是Accept消息的能耗
                    connectModuleAToB(Setn[randtemp], last);
                    last = Setn[randtemp];
                    Ns = Setn[randtemp];

                    bool flag = false;
                    for(int i=0;i<isAppendCluster.size();++i){
                        if(isAppendCluster[i] == Setn[randtemp]){
                            isAppendCluster.erase(isAppendCluster.begin()+i);
                            flag = true;
                            break;
                        }
                    }
                    if(flag!=true){
                        throw std::logic_error("找不到Setn[0]");
                    }

                }
                else{
                    //NS = random(Li-li);
                    int randNum1 = intrand(isAppendCluster.size());//获取[0 - leafList.size())内的一个随机数
                    Ns = isAppendCluster[randNum1]; //Ns = Random(Li),Ns初始化成一个链表li
                    isAppendCluster.erase(isAppendCluster.begin()+randNum1);
                    //此时Ns成为链表头，原先的成为链表，采用头插法
                    Ns->calcEnergyConsumption(head, Type::sendMsg);//按照论文，就是rebuild消息的能耗
                    head->calcEnergyConsumption(Ns,Type::sendMsg); //按照论文，Join消息的能耗
                    Ns->calcEnergyConsumption(head, Type::sendMsg);//按照论文，就是Accept消息的能耗
                    connectModuleAToB(head,Ns);
                    head = Ns;
                }
            }
            connectModuleAToB(head, parentNode);
        }
    }
    else if(strcmp(msg->getName(),"DataSlicingSignal") == 0){
        for(int i=0;i<this->otherNodeList_.size();++i){
            sendForDelayAndCalcEnergyConsumption(new cMessage("DataSlicing"),otherNodeList_[i].second,Type::sendMsg);
        }
    }
    else if(strcmp(msg->getName(),"DataAggregateSignal") == 0){
        //这里应该给所有的叶子节点发聚合数据消息
        for(int i=0;i<otherNodeList_.size();++i){
            if(otherNodeList_[i].second->getGateNames().size()==2)
                sendForDelayAndCalcEnergyConsumption(new cMessage("DataAggregate"),otherNodeList_[i].second,Type::sendMsg);
        }
    }
    else if(strcmp(msg->getName(),"SlicingData") == 0){
        calcEnergyConsumption(msg->getSenderModule(), Type::receiveData);//计算能耗
        aggregationAndCalcEnergyConsumption(msg);
    }
    //接受到来自子节点的数据,Server已经是根节点了，就不需要记录子节点数目，然后继续上传数据了
    else if(strcmp(msg->getName(),"AggregateData") == 0){
        calcEnergyConsumption(msg->getSenderModule(), Type::receiveData);
        aggregationAndCalcEnergyConsumption(msg);
    }
    else if(strcmp(msg->getName(),"ReadDateSignal") == 0){
        emit(packetLossRate_,1-myData_[1]/(double)(NodeNumber_*3));
        for(auto i:myData_){
            EV<<i<<" ";
        }
        EV<<endl;
        EV<<"加入网络的节点数量:"<<appendNetworkNodeNumber_<<endl;
        EV<<"死亡的节点数量："<<NodeDieNumber_<<endl;
        myData_.clear();
        myData_.resize(DataMessageLength_);
    }
}
void Server::finish(){

}
//得到的会是一个有序的按照节点之间距离排序好的数组
std::vector<Base*> Server::getIntersectionOfTwo(std::vector<std::pair<double,Base *>> T1,std::vector<Base *> T2){
    std::vector<Base*> res;
    for(auto& [t,i]:T1){
        for(auto&  j: T2){
            //取两者交集的部分
            if(i == j){
                res.push_back(i);
                break;
            }
        }
    }
    return res;
}


