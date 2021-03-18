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

void Receiver::initialize()
{
    inGateId = findGate("receiveGate");
    EV << "Receiver initialized with: "
       << "inGateId: " << inGateId
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
        delete msg;
    } else {
        delete msg;
        error("Receiver:: Received unexpected packet");
    }
}
