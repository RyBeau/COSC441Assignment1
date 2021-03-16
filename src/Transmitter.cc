#include "Transmitter.h"
#include "PacketRecord_m.h"

Define_Module(Transmitter);

void Transmitter::initialize()
{
    numberOverheadBits = par("numberOverheadBits");
    numberUserBits = par("numberUserBits");
    outGateId = findGate("transmitGate");
    inGateId = findGate("receiveGate");

    EV << "Transmitter initialized with:\n numberOverheadBits: " << numberOverheadBits
            << "\nnumberUserBits: " << numberUserBits <<
            "\noutGateId:" << outGateId <<
            "\ninGateId: " << inGateId << "\n";
}

void Transmitter::handleMessage(cMessage *msg)
{
    if (msg->arrivedOn(inGateId)) {

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
