# CARLANeTpp

CARLANeTpp is the OMNeT++ side of the open source library [CARLANeT](https://github.com/carlanet) for co-simulation between CARLA and OMNeT++.

## Requirements

- [OMNeT++ 6.0](https://omnetpp.org/) or higher
- [INET 4.2](https://inet.omnetpp.org/) or higher
- [nlohmann/json](https://github.com/nlohmann/json)
- [zeromq](https://zeromq.org/)

## Installation

CARLANeTpp can be compiled on any platform supported by OMNeT++.

1. Make sure your OMNeT++ installation works OK (e.g., try running the samples) and it is in the path (to test, try the command `which omnetpp`, it should print the path of the executable).
2. Extract the downloaded tarball into a directory of your choice (usually into your workspace directory if you are using the IDE).
3. Open the OMNeT++ IDE and choose the workspace where you have extracted the CARLANeTpp directory. The extracted directory must be a subdirectory of the workspace dir.
4. Import the project using: `File` | `Import` | `General` | `Existing projects into Workspace`.
5. Select the workspace dir as the root directory, and be sure NOT to check the "Copy projects into workspace" box. Click `Finish`.
6. Open the project (if not already open). Now you can build the project by pressing `CTRL-B` (`Project` | `Build all`).
7. To run an example from the IDE, open the simulations' directory in the Project Explorer view.
8. Find the corresponding `omnetpp.ini` file. Right-click on it and select `Run As` / `Simulation`. This should create a Launch Configuration for this example.


## Architecture

CARLANeTpp is composed by two main components, to develop a working application you need to understand what these three components do.

- **CarlanetManager**: CarlanetManager is the gateway for the communication with [pyCARLANeT](https://github.com/carlanet/pycarlanet). Every in/out message pass from this component. You need to include this component in the root of your network to be visible to all the subcomponents.

- **CarlaInetMobility**: CarlaInetMobility is the mobility of CARLA world, each node which has a correspondent actor in CARLA needs to change the mobility in this way:
  `<node>.mobility.typename = "CarlaInetMobility"`

## Usage

Each CARLA node must set its mobility to "CarlaInetMobility". The default implementation subscribes itself to CarlanetManager in order to allow synchronization between the two sides of CARLANeT.

### Messages

There are three types of messages:

- **INIT**: It's the message sent at the beginning of the simulation. It handles the synchronization between the two CARLANeT of the simulators. In addition to parts containing the actors, another part can be defined by the developer to pass other parameters. It is possible to set these additional parameters inside the parameter of CarlanetManager "extraInitParams" in a JSON format.

- **SIMULATION STEP**: It is the message used to keep the co-simulation (in terms of time) consistent. This message is sent for each simulation time step defined in CarlanetManager "simulationTimeStep". [pyCARLANeT](https://github.com/carlanet/pycarlanet) waits for this message to perform a simulation step. The simulation time step parameter is sent during the initialization phase and is set accordingly in CARLA settings to ensure consistency between the two worlds.

- **GENERIC MESSAGE**: Refers to the custom message defined by the developer of the application. It Includes all application-specific custom messages. This message type is generally used to communicate to the Python application of CARLA that a message has been received by an actor.

To see an example of the usage of the Generic Message, please refer to the [`car-light-control` example](https://github.com/carlanet/carlanetpp/tree/main/src/carlanet/lightcontrol).


## Example


This repository provides an example of co-simulation between CARLA and OMNeT++ using CARLANeT. The sample code demonstrates a simple application that includes a car and an application agent controlling the car's lights remotely. You can find the corresponding code of [pyCARLANeT](https://github.com/carlanet/pycarlanet) in the respective repository.

Please note that this example works only if the CARLA service is already active with pyCARLANeT listening. If you haven't set up the CARLA service with pyCARLANeT, please visit [pyCARLANeT repository](https://github.com/carlanet/pycarlanet) for a tutorial on how to do it. Once you have the CARLA service and pyCARLANeT set up, follow the instructions below to run the example.

Once you have opened the project with OMNeT++ IDE, go to the "simulations" folder and open the `omnetpp.ini` file. Locate the line `*.carlanetManager.host` in the file and replace it with the IP address where pyCARLANeT is running. Save the changes to the file and then execute it to start the simulation.

Note: [ToD-simulator](https://github.com/connets/tod-simulator/tree/dev) is another project that extensively utilizes CARLANeT, although its documentation is not comprehensive.

## Disclaimer

If you use this software or part of it for your research, please cite 
our work:
  
V. Cislaghi, C. Quadri, V. Mancuso and M. A. Marsan, "Simulation of Tele-Operated Driving over 5G Using CARLA and OMNeT++," 2023 IEEE Vehicular Networking Conference (VNC), Istanbul, Turkiye, 2023, pp. 81-88, doi: [10.1109/VNC57357.2023.10136340](https://doi.org/10.1109/VNC57357.2023.10136340).

If you include this software or part of it within your own software, 
README and LICENSE files cannot be removed from it and must be included 
in the root directory of your software package.


## License
CARLANeT is distributed under the MIT License. See LICENSE for more information.
