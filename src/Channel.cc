#include "Channel.h"
#include "PacketRecord_m.h"

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
    startSim = new cMessage ("Start");

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

    scheduleAt(simTime(), startSim);
}

void Channel::handleMessage(cMessage *msg)
{
    if(msg == startSim){
        EV << "Channel Received Start Message";
        cMessage *pkt = new cMessage("Start transmission");
        send(pkt, requestGateId);

    } else if (dynamic_cast<PacketRecord*>(msg)){
        PacketRecord* packetRecord = (PacketRecord*) msg;
        EV << "Channel has received a valid message: "
                << "\nSequence Number: " << packetRecord->getSequenceNumber()
                << "\nOverhead Bits: " << packetRecord->getOvhdBits()
                << "\nUser Bits: " << packetRecord->getUserBits()
                << "\nError Flag: " << packetRecord->getErrorFlag();

        if((double) rand() > 0.5){
            packetRecord->setErrorFlag(true);
        }

        send(packetRecord, outGateId);
    } else {
        error("Channel: Received unexpected packet");
    }
}

Channel::~Channel(){
    cancelAndDelete(startSim);
}
