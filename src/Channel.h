#ifndef __COSC441ASSIGNMENT1_CHANNEL_H_
#define __COSC441ASSIGNMENT1_CHANNEL_H_

#include <omnetpp.h>
#include "PacketRecord_m.h"
#include "Channel.h"
#include <math.h>



using namespace omnetpp;

class Channel : public cSimpleModule
{
  public:
    void initialize();
    void handleMessage(cMessage *msg);
    void transmitMessage(PacketRecord *packetRecord);
    void completeTransmission();
    void setNextChannelState();
    void processPacket(PacketRecord *packetRecord);
    double convertToDb(double n);
    double convertToNormal(double db);
    double calculatePathLoss();
    ~Channel();
  protected:
    int nodeDistance, pathLossExponent, bitRate, txPowerDBm,
    channelGainGoodDB, channelGainBadDB, outGateId, inGateId, requestGateId;
    double noisePowerDBm, transProbGoodGood, transProbBadBad;
    bool currentState, nextState = true;
    cMessage* requestTransmission;
    cMessage* transmitted;
    PacketRecord* currentPacket;
};

#endif
