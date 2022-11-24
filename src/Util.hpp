#ifndef __TEST_PROJECT_UTIL_H_
#define __TEST_PROJECT_UTIL_H_
#include <omnetpp.h>
#include <string>

using namespace omnetpp;

//A连接到B，也就是A有个父亲节点B
void connectModuleAToB(cModule*child,cModule*parent){
    std::string str1 = "child" + std::to_string(child->getId()) +"_"+std::to_string(parent->getId());
    cGate * inGate = parent->addGate(str1.c_str(), cGate::INPUT);
    std::string str2 = "parent";
    cGate * outGate = child->addGate(str2.c_str(), cGate::OUTPUT);
    outGate->connectTo(inGate);
}


#endif
