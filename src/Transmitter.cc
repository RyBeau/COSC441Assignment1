#include "Transmitter.h"
#include "PacketRecord_m.h"

Define_Module(Transmitter);

/**
 * Initialize all properties from omnetpp.ini.
 * Schedules start message to begin simulation.
 */
void Transmitter::initialize()
{
    numberOverheadBits = par("numberOverheadBits");
    numberUserBits = par("numberUserBits");
    outGateId = findGate("transmitGate");
    inGateId = findGate("receiveGate");
    startSim = new cMessage("Start");

    EV << "Transmitter initialized with: "
            << "\n numberOverheadBits: " << numberOverheadBits
            << "\nnumberUserBits: " << numberUserBits
            << "\noutGateId:" << outGateId
            << "\ninGateId: " << inGateId
            << endl;

    scheduleAt(simTime(), startSim);
}

/**
 * Callback for received messages, checks message type and handles
 * appropriately. Any expected message received creates PacketRecord to
 * be sent to the Channel.
 */
void Transmitter::handleMessage(cMessage *msg)
{
    if (msg->arrivedOn(inGateId)) {
        EV << "Transmitter::Received Request Message" << endl;
        generateAndSend();
        delete msg;
    } else if (msg == startSim) {
        EV << "Transmitter::Received Start Simulation Message" << endl;
        generateAndSend();
    } else {
        delete msg;
        error("Transmitter:: Received Unexpected Message");
    }
}

/**
 * Generates the packet record based on the property values and
 * sends it to the Channel.
 */
 void Transmitter::generateAndSend()
 {
     PacketRecord *packetRecord = new PacketRecord;

     EV << "Transmitter::handleMessage: generating a new packet record at time "
             << simTime() << " with sequence number "
             << sequenceNumber << " and with packet size"
             << numberOverheadBits + numberUserBits
             << endl;

     packetRecord->setSequenceNumber(sequenceNumber++);
     packetRecord->setOvhdBits(numberOverheadBits);
     packetRecord->setUserBits(numberUserBits);
     packetRecord->setErrorFlag(false);
     packetRecord->setBitLength(numberUserBits + numberOverheadBits);

     send(packetRecord, outGateId);
 }

 /**
  * Cancel and delete startSim
  */
Transmitter::~Transmitter(){
    cancelAndDelete(startSim);
}
