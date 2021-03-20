#ifndef __COSC441ASSIGNMENT1_TRANSMITTER_H_
#define __COSC441ASSIGNMENT1_TRANSMITTER_H_

#include <omnetpp.h>

using namespace omnetpp;

class Transmitter : public cSimpleModule
{
  public:
    void initialize();
    void handleMessage(cMessage *msg);
    void generateAndSend();
    ~Transmitter();

  private:
    int64_t numberOverheadBits;
    int64_t numberUserBits;
    int outGateId;
    int inGateId;
    int sequenceNumber = 0;
    cMessage* startSim;
};

#endif
