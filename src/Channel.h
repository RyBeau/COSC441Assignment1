#ifndef __COSC441ASSIGNMENT1_CHANNEL_H_
#define __COSC441ASSIGNMENT1_CHANNEL_H_

#include <omnetpp.h>
#include "PacketRecord_m.h"


using namespace omnetpp;

class Channel : public cSimpleModule
{
  public:
    void initialize();
    void handleMessage(cMessage *msg);
    void transmitMessage(PacketRecord *packetRecord);
    void completeTransmission();
    ~Channel();
  protected:
    int nodeDistance, pathLossExponent, bitRate, txPowerDBm,
    channelGainGoodDB, channelGainBadDB, outGateId, inGateId, requestGateId;
    double noisePowerDBm, transProbGoodGood, transProbBadBad;
    bool goodState = true;
    cMessage* requestTransmission;
    cMessage* transmitted;
    PacketRecord* currentPacket;
};

#endif
