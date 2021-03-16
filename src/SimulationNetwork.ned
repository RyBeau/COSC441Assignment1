package cosc441assignment1;

network SimulationNetwork
{
    submodules:
        transmitter: Transmitter;
        pktChannel: Channel;
        receiver: Receiver;
    connections:
        transmitter.transmitGate --> pktChannel.receiveGate;
        pktChannel.transmitGate --> receiver.receiveGate;
        pktChannel.requestGate --> transmitter.receiveGate;
}
