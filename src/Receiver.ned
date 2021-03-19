package cosc441assignment1;

simple Receiver
{
    parameters:
        @signal[packetError](type=bool);
        @statistic[packetError](record=stats);
    gates:
        //Gate connected to channel
        input receiveGate;
}
