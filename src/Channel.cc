#include "Channel.h"
#include "PacketRecord_m.h"
#include <stdlib.h>

Define_Module(Channel);

void Channel::initialize()
{
    pathLossExponent = par("pathLossExponent");
    txPowerDBm = par("txPowerDBm");
    noisePowerDBm = par("noisePowerDBm");
    bitRate = par("bitRate");
    outGateId = findGate("transmitGate");
    inGateId = findGate("receiveGate");
    requestGateId = findGate("requestGate");
    transProbGoodGood = par("transProbGoodGood");
    transProbBadBad = par("transProbBadBad");
    channelGainGoodDB = par("channelGainGoodDB");
    channelGainBadDB = par("channelGainBadDB");
    transmitted = new cMessage ("Transmitted");
    currentPacket = nullptr;

    EV << "Channel initialized with: "
            << "\npathLossExponent: " << pathLossExponent
            << "\ntxPowerDBm: " << txPowerDBm
            << "\nnoisePowerDBm: " << noisePowerDBm
            << "\nbitRate: " << bitRate
            << "\noutGateId: " << outGateId
            << "\ninGateId: " << inGateId
            << "\nrequestGateId: " << requestGateId
            << "\ntransProbGoodGood: " << transProbGoodGood
            << "\ntransProbBadBad: " << transProbBadBad
            << "\nchannelGainGoodDB: " << channelGainGoodDB
            << "\nchannelGainBadDB: " << channelGainBadDB
            << endl;
}

void Channel::handleMessage(cMessage *msg)
{
    if(msg == transmitted){
        EV << "Channel Received Transmitted Message";
        completeTransmission();
    } else if (dynamic_cast<PacketRecord*>(msg)){
        PacketRecord* packetRecord = (PacketRecord*) msg;
        EV << "Channel has received a valid message: "
                << "\nSequence Number: " << packetRecord->getSequenceNumber()
                << "\nOverhead Bits: " << packetRecord->getOvhdBits()
                << "\nUser Bits: " << packetRecord->getUserBits()
                << "\nError Flag: " << packetRecord->getErrorFlag();
        transmitMessage(packetRecord);
    } else {
        error("Channel: Received unexpected packet");
    }
}

void Channel::transmitMessage(PacketRecord *packetRecord)
{
    if (currentPacket)
    {
        EV << "Channel:: Dropping incoming packet as there is already on in transmission.";
    } else {
        currentPacket = packetRecord;

        if((rand() % 100) < 50)
        {
            currentPacket->setErrorFlag(true);
            EV << "Channel:: Bit error occurred. Set errorFlag to true";
        }
        double packetLength = (double)currentPacket->getBitLength();
        scheduleAt(simTime() + (packetLength / bitRate), transmitted);
    }
}

void Channel::completeTransmission(){
    if (currentPacket){
        send(currentPacket, outGateId);
        currentPacket = nullptr;
        cMessage *pkt = new cMessage("Transmit");
        send(pkt, requestGateId);
        EV << "Channel:: Message sent through the channel";
    } else {
        EV << "Channel:: No packet to transmit";
    }

}

Channel::~Channel(){
    cancelAndDelete(transmitted);
    if (currentPacket){
        delete currentPacket;
    }
}
