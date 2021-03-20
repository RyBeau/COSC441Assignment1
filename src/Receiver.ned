package cosc441assignment1;

simple Receiver
{
    parameters:
        @signal[packetError](type=bool);
        @statistic[packetError](title="Packet Errors"; record=vector, stats; interpolationmode=none);
    gates:
        //Gate connected to channel
        input receiveGate;
}
