#ifndef __COSC441ASSIGNMENT1_CHANNEL_H_
#define __COSC441ASSIGNMENT1_CHANNEL_H_

#include <omnetpp.h>

using namespace omnetpp;

class Channel : public cSimpleModule
{
  public:
    virtual void initialize();
    virtual void handleMessage(cMessage *msg);
    ~Channel();
  protected:
    int nodeDistance, pathLossExponent, bitRate, txPowerDBm,
    channelGainGoodDB, channelGainBadDB, outGateId, inGateId, requestGateId;
    double noisePowerDBm, transProbGoodGood, transProbBadBad;
    bool goodState = true;
    cMessage* startSim;
};

#endif
