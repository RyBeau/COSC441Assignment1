#ifndef __COSC441ASSIGNMENT1_TRANSMITTER_H_
#define __COSC441ASSIGNMENT1_TRANSMITTER_H_

#include <omnetpp.h>

using namespace omnetpp;

class Transmitter : public cSimpleModule
{
  public:
    virtual void initialize();
    virtual void handleMessage(cMessage *msg);

  private:
    int64_t numberOverheadBits;
    int64_t numberUserBits;
    int outGateId;
    int inGateId;
    int sequenceNumber = 0;
};

#endif
