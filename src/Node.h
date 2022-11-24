#ifndef __TEST_PROJECT_NODE_H_
#define __TEST_PROJECT_NODE_H_

#include "Base.h"

class Node : public Base{
public:

protected:
    virtual void initialize() override;
    virtual void handleMessage(cMessage *msg) override;
    virtual void finish() override;

    virtual void FindNodeWithinD0(){}

protected:
    bool isClusterHead_ = false;
    int childNodeNumber_ = 0;                           //子节点的数量标记
};

Define_Module(Node);
#endif
