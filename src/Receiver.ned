package cosc441assignment1;

simple Receiver
{
    parameters:
        @signal[packetError](type=bool);
        @statistic[packetError](record=stats);
                //Length of packet overhead
    gates:
        //Gate connected to channel
        input receiveGate;
}
