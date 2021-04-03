#include "Channel.h"
#include "PacketRecord_m.h"
#include <math.h>

Define_Module(Channel);

/**
 * Initialize all properties from omnetpp.ini.
 */
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
    nodeDistance = par("nodeDistance");
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
            << "\nnodeDistance: " << nodeDistance
            << endl;
}

/**
 * Callback for received messages, checks message type and sends pointer
 * to the correct handling method.
 */
void Channel::handleMessage(cMessage *msg)
{
    if(msg == transmitted){
        EV << "Channel Received Transmitted Message" << endl;
        completeTransmission();
    } else if (dynamic_cast<PacketRecord*>(msg)){
        PacketRecord* packetRecord = (PacketRecord*) msg;
        EV << "Channel has received a valid message: "
                << "\nSequence Number: " << packetRecord->getSequenceNumber()
                << "\nOverhead Bits: " << packetRecord->getOvhdBits()
                << "\nUser Bits: " << packetRecord->getUserBits()
                << "\nError Flag: " << packetRecord->getErrorFlag()
                << endl;
        transmitMessage(packetRecord);
    } else {
        delete msg;
        error("Channel: Received unexpected packet");
    }
}

/**
 * Schedules messages received on the receiveGate to be sent on the transmitGate
 * at the correct future time in the sim considering bit rate and packet length.
 */
void Channel::transmitMessage(PacketRecord *packetRecord)
{
    if (currentPacket)
    {
        EV << "Channel:: Dropping incoming packet as there is already on in transmission." << endl;
        delete packetRecord;
    } else {
        currentPacket = packetRecord;
        processPacket(currentPacket);
        double packetLength = (double)currentPacket->getBitLength();
        scheduleAt(simTime() + (packetLength / bitRate), transmitted);
    }
}

/**
 * Handles the calculation of bit errors for a received packet in a
 * bit by bit fashion.
 */
void Channel::processPacket(PacketRecord *packetRecord)
{
    double pathLossDb = convertToDb(calculatePathLoss());
    for (int i = 0; i < packetRecord->getBitLength(); i++)
    {
        setNextChannelState();
        double receivedPower = txPowerDBm + ((currentState) ? channelGainGoodDB : channelGainBadDB) - pathLossDb;
        double eHat = receivedPower - (noisePowerDBm + convertToDb(bitRate));
        double bitErrorRate = erfc(sqrt(convertToNormal(eHat))) / 2.0;
        double randomNumber = uniform(0, 1);
        EV << randomNumber << " " << bitErrorRate << endl;
        if (randomNumber < bitErrorRate){
            currentPacket->setErrorFlag(true);
            EV << "Channel:: Bit error occurred. Set errorFlag to true" << endl;
        }
    }
}

/**
 * Returns the path loss for the defined nodeDistance.
 */
double Channel::calculatePathLoss()
{
    if(nodeDistance <= 1){
        return 1;
    } else {
        return std::pow((double)nodeDistance, (double)pathLossExponent);
    }
}

/**
 * Updates the current channel state currentState and sets the nextState for the
 * following bit.
 */
void Channel::setNextChannelState()
{
    currentState = nextState;
    double randomNumber = uniform(0, 1);
    if (currentState){
        nextState = (randomNumber < transProbGoodGood) ? true : false;
    } else {
        nextState = (randomNumber < transProbBadBad) ? false : true;
    }
    if(currentState != nextState){
        EV << "Channel:: Next State is: " << ((nextState) ? "Good" : "Bad") << endl;
    }
}

/**
 * Converts dB domain to normal domain
 */
double Channel::convertToNormal(double db){
    return std::pow(10.0, (db / 10.0));
}

/**
 * Converts normal domain to dB domain
 */
double Channel::convertToDb(double n){
    return 10.0 * log10(n);
}

/**
 * Sends a packet now ready for transmission to the receiver through
 * the transmitGate.
 */
void Channel::completeTransmission()
{
    if (currentPacket)
    {
        send(currentPacket, outGateId);
        currentPacket = nullptr;
        cMessage *pkt = new cMessage("Transmit");
        send(pkt, requestGateId);
        EV << "Channel:: Message sent through the channel" << endl;
    } else {
        EV << "Channel:: No packet to transmit" << endl;
    }

}

/**
 * Cancels and delete all potentially defines messages.
 */
Channel::~Channel()
{
    cancelAndDelete(transmitted);
    if (currentPacket){
        delete currentPacket;
    }
}
