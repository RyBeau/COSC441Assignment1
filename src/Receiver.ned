package cosc441assignment1;

simple Receiver
{
    parameters:
        @signal[packetError](type=bool);
        @statistic[packetError](record=mean);
    gates:
        //Gate connected to channel
        input receiveGate;
}
