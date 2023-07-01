package carlanet.lightcontrol;
import inet.applications.contract.IApp;

simple CarlaAgentApp like IApp
{
    parameters:
        @class(CarlaAgentApp);
        @display("i=block/app");
        
        int destPort = default(-1);  // local port (-1: use ephemeral port)
        string destAddress = default("");
        
        volatile int commandMsgLength @unit(B) = default(1000B); // length of messages to generate, in bytes
        volatile double sendInterval @unit(s) = default(1s); // should usually be a random value, e.g. exponential(1)
        
        
        
        @signal[packetReceived](type=inet::Packet);
	gates:
        input socketIn @labels(UdpControlInfo/up);
        output socketOut @labels(UdpControlInfo/down);
}

