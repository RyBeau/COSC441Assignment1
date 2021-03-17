#include "Channel.h"

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
            <<"\n";
}

void Channel::handleMessage(cMessage *msg)
{
    // TODO - Generated method body
}
