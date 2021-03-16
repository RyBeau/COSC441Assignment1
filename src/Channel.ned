package cosc441assignment1;

simple Channel
{
    parameters:
        //Distance between transmitter and receiver nodes
        int nodeDistance;
        //Path loss exponent for log-distance channel model
        int pathLossExponent;
        //Raidated Power in dBm
        int txPowerDBm;
        //Transmission bitrate
        int bitRate;
        //Noise power
        int noisePowerDBm;
        //Probability channel remain in good state after being in good state
        double transProbGoodGood;
        //Probability channel remains in bad state after being in bad state
        double tansProbBadBad;
        //channel gain in the good state
        int channelGainGoodDB;
        //channel gain in the bad state
        int channelGainBadDB;
        
        gates:
            input receiveGate;
            output transmitGate;
}
