package cosc441assignment1;

simple Transmitter
{
    parameters:
        //Length of packet overhead
        int numberOverheadBits;
        //Packet data length
        int numberUserBits;
    gates:
        //Gate connected to channel
        output transmitGate;
        //Gate connected to the channel
        input receiveGate;
}
