#include "Transmitter.h"
#include "PacketRecord_m.h"

Define_Module(Transmitter);

void Transmitter::initialize()
{
    numberOverheadBits = 100;
    numberUserBits = 1000;
    outGateId = findGate("transmitGate");
    inGateId = findGate("receiveGate");
}

void Transmitter::handleMessage(cMessage *msg)
{
    if (msg->arrivedOn(inGateid)) {

        PacketRecord *packetRecord = new PacketRecord;

        EV << "Transmitter::handleMessage: generating a new packet record at time "
                << simTime() << " with sequence number "
                << sequenceNumber << " and with packet size";

        packetRecord->setSequenceNumber(sequenceNumber++);
        packetRecord->setOvhdBits(numberOverheadBits);
        packetRecord->setUserBits(numberUserBits);
        packetRecord->setErrorFlag(false);

        send(packetRecord, outGateId);
        delete msg;

    } else {
        error("Transmitter:: Received Unexpected Message");
    }
}

void Transmitter::~Transmitter()
{
    // TODO
}
