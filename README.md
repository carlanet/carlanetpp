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

- **CarlanetManager**: CarlanetManager is the gateway for the communication with pyCARLANeT. Every in/out message pass from this component. You need to include this component in the root of your network to be visible to all the subcomponents.

- **CarlaInetMobility**: CarlaInetMobility is the mobility of CARLA world, each node which has a correspondent actor in CARLA needs to change the mobility in this way:
  `<node>.mobility.typename = "CarlaInetMobility"`

## Usage

Each CARLA node must set its mobility to "CarlaInetMobility". The default implementation subscribes itself to CarlanetManager in order to allow synchronization between the two sides of CARLANeT.

### Messages

There are three types of messages:

- **INIT**: It's the message sent at the beginning of the simulation. It handles the synchronization between the two CARLANeT of the simulators. In addition to parts containing the actors, another part can be defined by the developer to pass other parameters. It is possible to set these additional parameters inside the parameter of CarlanetManager "extraInitParams" in a JSON format.

- **SIMULATION STEP**: It is the message used to keep the co-simulation (in terms of time) consistent. This message is sent for each simulation time step defined in CarlanetManager "simulationTimeStep". pyCARLANeT waits for this message to perform a simulation step. The simulation time step parameter is sent during the initialization phase and is set accordingly in CARLA settings to ensure consistency between the two worlds.

- **GENERIC MESSAGE**: It is the custom message defined by the devoper of the application. All the custom message of the aplpication fall in this set of messages. It is generally used to communicate to the python application of carla which a message is received by an actor.

- **Generic Message**: Refers to the custom message defined by the developer of the application. It encompasses all the custom messages specific to the application. This message type is generally used to communicate to the Python application of CARLA that a message has been received by an actor.

To see an example of the usage of the Generic Message, please refer to the `car-light-control` example.


## Example


This repository provides an example of co-simulation between CARLA and OMNeT++ using CARLANeT. The sample code demonstrates a simple application that includes a car and an application agent controlling the car's lights remotely. You can find the corresponding code of pyCARLANeT in the respective repository.

Please note that this example works only if the CARLA service is already active with pyCARLANeT listening. If you haven't set up the CARLA service with pyCARLANeT, please visit [pyCARLANeT repository](https://github.com/carlanet/pycarlanet) for a tutorial on how to do it. Once you have the CARLA service and pyCARLANeT set up, follow the instructions below to run the example.

Once you have opened the project with OMNeT++ IDE, go to the "simulations" folder and open the `omnetpp.ini` file. Locate the line `*.carlanetManager.host` in the file and replace it with the IP address where pyCARLANeT is running. Save the changes to the file and then execute it to start the simulation.

Note: [ToD-simulator](https://github.com/connets/tod-simulator/tree/dev) is another project that extensively utilizes CARLANeT, although its documentation is not comprehensive.

## Citation
If you use the code or ideas in this repository for your research, please consider citing the following paper:

    ```
    @inproceedings{10136340,
      author={Cislaghi, Valerio and Quadri, Christian and Mancuso, Vincenzo and Marsan, Marco Ajmone},
      booktitle={2023 IEEE Vehicular Networking Conference (VNC)}, 
      title={Simulation of Tele-Operated Driving over 5G Using CARLA and OMNeT++}, 
      year={2023},
      volume={},
      number={},
      pages={81-88},
      doi={10.1109/VNC57357.2023.10136340}
    }
    ``` 

## License
CARLANeT is distributed under the MIT License. See LICENSE for more information.
