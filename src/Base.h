#ifndef __TEST_PROJECT_BASE_H_
#define __TEST_PROJECT_BASE_H_
#include <omnetpp.h>
#include <exception>
#include <vector>
#include <utility>
#include <algorithm>
#include "MyMsg_m.h"
using namespace omnetpp;
class Base :public cSimpleModule{
public:

    Base(){}

    /* *
     * 参数的构造函数
     * */
    void construct();
    /* *
     * 找到该节点周围的节点，放到vector里
     *
     * */
    virtual void FindNodeWithinDistance();
    void consumeAndCheckLive(double energy);

    enum class Type{
        receiveMsg,
        receiveData,
        sendData,
        sendMsg,
        dataAggregation
    };

    void calcEnergyConsumption(cModule* destModule,Type type);
    /* *
     * 能耗计算公式,当d<d0时
     * result = E_elec * k + E_fs*k*d^2
     * 否则 result = E_elec * k + E_mp*k*d^4
     * 具体见LEACH的论文
     * */
    double calcEnergyCusmForDistance(int k,int distance);
    /* *
     * 连接自己模块和消息发送的模块
     * sendModule : 消息发送的模块
     * 结果 : this 模块添加一个out门("parent"),消息发送的模块添加一个in门
     * */
    void connectToSendModule(cModule * sendModule);
    /* *
     * 计算当前模块和其他模块的距离
     * receive ：目标模块
     * 隐藏参数 : this
     * */
    double calculateDistance(Base *receive);
    /* *
     * 计算发送的时间时延,能耗也一并就算在内
     * msg : 发送的消息
     * destMoudule : 发送的目的地的模块
     * length : 发送的数据长度 单位（B）
     * */
    void sendForDelayAndCalcEnergyConsumption(cMessage * msg,Base * destMoudule,Type type);
    /* *
     * 删除A的parent门和对应的B的in门 ---- 应该拆分到工具类
     * childNodeHandle : 孩子节点的句柄，即A的模块
     * parentNodeHandle: 父亲节点的句柄，即B的模块
     * */
    void deleteAGateToBGate(cModule * childNodeHandle,cModule * parentNodeHandle);

    /* *
     * 产生一个带有数据的消息，返回消息的指针
     * */
    MyMsg* gengerateMyMsg(const char * name,int length);
    /* *
     * 聚合一个带有数据的消息，同时计算聚合数据的能耗。
     * */
    void aggregationAndCalcEnergyConsumption(cMessage * msg);
    /***
     * 留下参数便于代码阅读
     * 把myData的数据发送给父节点
     * 并且计算发送的能耗
     */
    void sendAggregationDataAndCalcEnergyConsumption(const char * name);

    /***
     * 网络修复
     * */
    void networkRestoration();

    /***
     * 得到所有的子节点
     * */
    std::vector<Base *> getChildNodeList();

    /***
     * A连接到B，也就是A有个父亲节点B
     * */
    void connectModuleAToB(cModule*child,cModule*parent);

public:
    bool isLive_ = true;                                //节点是否存活，即能量是否消耗完毕
    bool isJoinOtherCluster_ = false;                   //是否加入簇
    int x_ = 0;                                         //x坐标
    int y_ = 0;                                         //y坐标

    simsignal_t energy_ ;                               //统计能耗数据使用的信号
    std::vector<std::pair<double,Base *>> otherNodeList_;        //所有的其他节点列表，按照第一个参数升序排序
    std::vector<std::pair<double,Base *>> otherNodeListWithinD0_;//d0距离内的节点列表，按照第一个参数升序排序
    std::vector<std::pair<double,Base *>> otherNodeListNotD0_;   //前两者的差集
    std::vector<std::pair<double,Base *>> otherNodeListNotMaxDistance_;//全集和小于最大距离节点之间的差集
    std::vector<std::pair<double,Base *>> otherNodeListWithinMaxDistance_;//<MaxDistanc的集合
    std::vector<int> myData_;                           //持有的数据

    inline static int appendNetworkNodeNumber_ = 0;
    inline static int NodeDieNumber_ = 0;

    cModule * network_;                                 //Network模块


    double InitEnergy_;

    int NodeNumber_;

    double Probability_;
    int SlicingNumber_;
    double E_DA_;
    double E_mp_;
    double E_fs_;
    double D0_;
    double E_elec_;
    double MaxDistance_;
    int ControlPacketLength_;
    int DataMessageLength_;
    int BitRate_;

};
#endif
