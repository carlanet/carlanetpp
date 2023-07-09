// MIT License
// Copyright (c) 2023 Valerio Cislaghi, Christian Quadri

package carlanet.lightcontrol;
import inet.applications.contract.IApp;

simple CarlaCarApp like IApp
{
    parameters:
        @class(CarlaCarApp);
        @display("i=block/app");
        
        int localPort = default(-1);  // local port (-1: use ephemeral port)
        string localAddress = default("");
        
        volatile int statusMsgLength @unit(B) = default(8kB); // length of messages to generate, in bytes
        
        
        @signal[packetReceived](type=inet::Packet);
	gates:
        input socketIn @labels(UdpControlInfo/up);
        output socketOut @labels(UdpControlInfo/down);
}

