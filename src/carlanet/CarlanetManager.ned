package carlanet;

simple CarlanetManager
{
    parameters:
        @class(CarlanetManager);

        string host = default("localhost");  // server hostname
        string protocol = default("tcp");  // server hostname
        double simulationTimeStep @unit("s") = default(10ms);
        int port = default(5555);  // server port (-1: automatic)
        //int seed = default(-1); // seed value to set in launch configuration, if missing (-1: current run number)
        int communicationTimeoutms = default(1000);
        //bool autoShutdown = default(true);  // Shutdown module as soon as no more vehicles are in the simulation
		object extraInitParams = default(parseJSON("{}"));
		
		
		//Parameters for dynamically generated actors
		// IMPORTANT: all the node types have to use CarlaInetMobility (or module that inherits from it) as mobility module
		string networkActiveModuleType;  		// TODO allow multiple type based on prefixes
		string networkPassiveModuleType = default("inet.node.base.NodeBase");  // TODO allow multiple type based on prefixes
		
		
        @display("i=block/cogwheel");
}

