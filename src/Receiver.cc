//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see http://www.gnu.org/licenses/.
// 

#include "Receiver.h"
#include "PacketRecord_m.h"

Define_Module(Receiver);

simsignal_t packetErrorSignalId  =  cComponent::registerSignal("packetError");

void Receiver::initialize()
{
    inGateId = findGate("receiveGate");
    count = 0;
    totalErrors = 0;
    userBits = 0;
    ovhdBits =  0;
    EV << "Receiver initialized with: "
       << "inGateId: " << inGateId << "\n"
       << "count: " << count << "\n"
       << "userBits: " << userBits << "\n"
       << "ovhdBits: " << ovhdBits << "\n"
       << endl;
}

void Receiver::handleMessage(cMessage *msg)
{
    if(dynamic_cast<PacketRecord*>(msg)){
        PacketRecord* packetRecord = (PacketRecord*) msg;
        EV << "Receiver has received a valid message: "
                << "\nSequence Number: " << packetRecord->getSequenceNumber()
                << "\nOverhead Bits: " << packetRecord->getOvhdBits()
                << "\nUser Bits: " << packetRecord->getUserBits()
                << "\nError Flag: " << packetRecord->getErrorFlag()
                << endl;
        emit(packetErrorSignalId, packetRecord->getErrorFlag());
        count++;
        bool packetValue = packetRecord->getErrorFlag();
        if(packetValue){
            totalErrors++;
        }
        if(userBits == 0 || ovhdBits == 0){
            userBits = packetRecord->getUserBits();
            ovhdBits = packetRecord->getOvhdBits();
        }
        delete msg;
    } else {
        delete msg;
        error("Receiver:: Received unexpected packet");
    }
}

void Receiver::finish()
{
    EV << "Receiver finished with: "
       << "inGateId: " << inGateId << "\n"
       << "count: " << count << "\n"
       << "userBits: " << userBits << "\n"
       << "ovhdBits: " << ovhdBits << "\n"
       << "totalErrors: " << totalErrors << "\n"
       << endl;
    double packetLossRate = (double)totalErrors / (double)count;
    recordScalar("AveragePacketLoss", (double)totalErrors / (double)count);
    recordScalar("Goodput", ((1 - packetLossRate) * userBits) / (userBits + ovhdBits));
}
