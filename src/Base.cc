#include "Base.h"
#include <cassert>
void Base::construct(){
//    EV<<"Base initial"<<endl;
    network_ = getModuleByPath("<root>");           //Network模块
    energy_ = registerSignal("Energy");             //统计能耗数据使用的信号
    InitEnergy_ = network_->par("InitEnergy");      //网络初始能耗
    NodeNumber_ = network_->par("NodeNumber");
//    NodeDieNumber_ = network_->par("NodeDieNumber");
    Probability_ = network_->par("Probability");
    SlicingNumber_ = network_->par("SlicingNumber");
    E_DA_ = network_->par("E_DA");
    E_mp_ = network_->par("E_mp");
    E_fs_ = network_->par("E_fs");
    D0_ = network_->par("D0");
    E_elec_ = network_->par("E_elec");
    ControlPacketLength_ = network_->par("ControlPacketLength");
    DataMessageLength_ = network_->par("DataMessageLength");
    BitRate_ =  network_->par("BitRate");
    MaxDistance_ = network_->par("MaxDistance");
}
void Base::consumeAndCheckLive(double energy){
    InitEnergy_ -= energy;
    emit(energy_,this->InitEnergy_);
    if(InitEnergy_ <=0 && this->isLive_ == true){
        this->isLive_ = false;
//        int NodeDieNumber = network_->par("NodeDieNumber");
        NodeDieNumber_+=1;
        networkRestoration();
    }
}

void Base::calcEnergyConsumption(cModule* destModule,Type type){
    if(isLive_ == false || InitEnergy_ <=0)return;
    double distance = calculateDistance(check_and_cast<Base*>(destModule));
    double energyCusm = 0;
    int k;
    switch(type){
    case Type::dataAggregation:
        k = DataMessageLength_ *8;
        energyCusm = E_DA_*k;
        break;
    case Type::receiveMsg:{
        k = ControlPacketLength_*8;
        energyCusm = E_elec_*k;
        break;
    }
    case Type::receiveData:{
        k = (ControlPacketLength_+DataMessageLength_)*8;
        energyCusm = E_elec_*k;
        break;
    }
    case Type::sendMsg:{
        k =  ControlPacketLength_*8;
        energyCusm = calcEnergyCusmForDistance(k,distance);
        break;
    }
    case Type::sendData:{
        k = (DataMessageLength_+ControlPacketLength_)*8;
        energyCusm = calcEnergyCusmForDistance(k,distance);
        break;
    }
    default:{
        throw std::invalid_argument("Type 类型输入错误");
        break;
    }
    }
    consumeAndCheckLive(energyCusm);
}

double Base::calcEnergyCusmForDistance(int k,int distance){
    //E_elec * k + E_fs*k*d^2
    return distance<D0_?E_elec_ * k +  E_fs_ * k * distance *distance :
    //E_elec * k + E_mp*k*d^4
            E_elec_ * k + E_mp_ * k * distance *distance *distance *distance;
}
void Base::connectToSendModule(cModule * sendModule){
    std::string str = "child" + std::to_string(getId()) +"_"+std::to_string(sendModule->getId());
    cGate * inGate = sendModule->addGate(str.c_str(), cGate::INPUT);
    cGate * outGate = this->addGate("parent", cGate::OUTPUT);
    outGate->connectTo(inGate);
    this->isJoinOtherCluster_ = true;//该节点加入簇
    //网络中的节点数量+1
    appendNetworkNodeNumber_ +=1;
}
double Base::calculateDistance(Base *receive){
    return std::sqrt((x_-receive->x_) * (x_-receive->x_) + (y_-receive->y_) * (y_-receive->y_));
}
void Base::sendForDelayAndCalcEnergyConsumption(cMessage * msg,Base * destMoudule,Type type){
    if(isLive_ == false || InitEnergy_ <=0)return;
    int packetLength;
    switch(type){
    case Type::sendMsg :{
        packetLength = ControlPacketLength_;
        break;
    }
    case Type::sendData :{
        packetLength = ControlPacketLength_ + DataMessageLength_;
        break;
    }
    default : throw std::logic_error("输入Type参数错误");
    }
    //计算发送时延，传播时延
    double delayTime = calculateDistance(destMoudule)/3e8 + packetLength/BitRate_;
    sendDirect(msg,delayTime,0,destMoudule->gate("in"));
    calcEnergyConsumption(destMoudule, type);
}

void Base::deleteAGateToBGate(cModule * childNodeHandle,cModule * parentNodeHandle){
    //删除A的parent门和B对应的child门
    cGate * childGateHandle = childNodeHandle->gate("parent")->getPathEndGate();
    childGateHandle->disconnect();
    childNodeHandle->gate("parent")->disconnect();
    parentNodeHandle->deleteGate(childGateHandle->getName());
    childNodeHandle->deleteGate("parent");
}
void Base::FindNodeWithinDistance(){
    //SMART方案里，分发切片时需要考虑是否分发给Server，这里分发
    //不是server节点，需要计算server节点之间的距离
    if(strcmp(this->getClassName(),"Server")!=0){
        Base *object = check_and_cast<Base *>(getModuleByPath("server"));
        double distance = calculateDistance(object);
        otherNodeList_.push_back(std::make_pair(distance,object));
    }
    for(int i=0;i<NodeNumber_;++i){
        Base *object = check_and_cast<Base *>(getParentModule()->getSubmodule("nodes", i));
        //该节点不是自己，且不是自己 (this->calculateDistance(object) !=0)
        double distance = calculateDistance(object);
        if(this->getId()!=object->getId()){
            otherNodeList_.push_back(std::make_pair(distance,object));
        }
    }
    std::sort(otherNodeList_.begin(),otherNodeList_.end());
    assert(NodeNumber_== otherNodeList_.size());
    auto iter = otherNodeList_.begin();
    while(iter->first<=D0_){
        iter++;
    }

    otherNodeListWithinD0_.assign(otherNodeList_.begin(),iter);
    otherNodeListNotD0_.assign(iter, otherNodeList_.end());

    while(iter->first<=MaxDistance_){
        iter++;
    }
    otherNodeListWithinMaxDistance_.assign(otherNodeList_.begin(),iter);
    otherNodeListNotMaxDistance_.assign(iter, otherNodeList_.end());
}

MyMsg* Base::gengerateMyMsg(const char * name,int length){
    MyMsg* myMsg = new MyMsg(name);
    myMsg->setMyMsgArraySize(length);
    for(int i=0;i<length;++i){
        myMsg->setMyMsg(i, i);
    }
    return myMsg;
}

void Base::aggregationAndCalcEnergyConsumption(cMessage * msg){
    MyMsg* myMsg = check_and_cast<MyMsg*>(msg);
    for(int i=0;i<myMsg->getMyMsgArraySize();++i){
         myData_[i] += myMsg->getMyMsg(i);
    }
    calcEnergyConsumption(this, Type::dataAggregation);
}

void Base::sendAggregationDataAndCalcEnergyConsumption(const char * name){
    if(InitEnergy_<=0||isLive_ ==false) return;
    MyMsg* myMsg = new MyMsg(name);
    myMsg->setMyMsgArraySize(myData_.size());
    for(int i=0;i<myData_.size();++i){
      myMsg->setMyMsg(i, myData_[i]);
    }
    sendForDelayAndCalcEnergyConsumption(myMsg, check_and_cast<Base*>(this->gate("parent")->getPathEndGate()->getOwnerModule()), Type::sendData);
    myData_.clear();
    myData_.resize(DataMessageLength_);
}

void Base::networkRestoration(){
    if(this->getGateNames().size() <=1)return;
    //第一步：获取该节点的子节点和父节点
    cModule * parentNode = this->gate("parent")->getPathEndGate()->getOwnerModule();
    //避免孤立节点没有加入网络的，却要读取父节点进行删除。
    deleteAGateToBGate(this, parentNode);
    //判断有没有子节点,>1说明有子节点,则执行一下代码,即对子节点进行修复.
    //为什么是1不是2，因为上一行代码刚刚把parent节点删除了
    if(this->getGateNames().size() == 1)return;
    //两种修复方式，一种是寻找周围最近的在网络中的节点加入，第二种是加入父节点
    //这里实现第二种
    //获取子节点的module
    std::vector<Base *> childNodeList = getChildNodeList();
    for(auto& childNode : childNodeList){
        deleteAGateToBGate(childNode, this);
        connectModuleAToB(childNode, parentNode);
    }
}


std::vector<Base *> Base::getChildNodeList(){
    //获取this的所有叶子节点列表
    std::vector<std::string> allGateName = this->getGateNames();
    std::vector<Base *>leafList;
    for(auto gateName : allGateName){
        if(gateName == "parent" || gateName == "in"){
            continue ;
        }
        cGate * gateHandle = this->gate(gateName.c_str());
        leafList.push_back(check_and_cast<Base*>(gateHandle->getPathStartGate()->getOwnerModule()));
    }
    return leafList;
}


void Base::connectModuleAToB(cModule*child,cModule*parent){
    std::string str1 = "child" + std::to_string(child->getId()) +"_"+std::to_string(parent->getId());
    cGate * inGate = parent->addGate(str1.c_str(), cGate::INPUT);
    std::string str2 = "parent";
    cGate * outGate = child->addGate(str2.c_str(), cGate::OUTPUT);
    outGate->connectTo(inGate);
}

