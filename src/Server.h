#ifndef __TEST_PROJECT_Server_H_
#define __TEST_PROJECT_Server_H_

#include "Base.h"

class Server : public Base{
public:

protected:
  virtual void initialize() override;
  virtual void handleMessage(cMessage *msg) override;
  virtual void finish() override;

  std::vector<Base*> getIntersectionOfTwo(std::vector<std::pair<double,Base *>> T1,std::vector<Base *> T2);

private:
  simsignal_t packetLossRate_;
};


Define_Module(Server);
#endif
