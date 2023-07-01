//
// Copyright (C) 2000 Institut fuer Telematik, Universitaet Karlsruhe
// Copyright (C) 2004,2011 OpenSim Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later
//
#include "inet/networklayer/common/L3AddressTag_m.h"
#include "inet/transportlayer/common/L4PortTag_m.h"

#include "carlanet/lightcontrol/CarlaCarApp.h"
#include "carlanet/lightcontrol/Messages_m.h"

#include <math.h>


#include "inet/common/ModuleAccess.h"
#include "inet/common/TagBase_m.h"
#include "inet/common/TimeTag_m.h"
#include "inet/common/lifecycle/ModuleOperations.h"
#include "inet/common/packet/Packet.h"
#include "inet/common/packet/chunk/ByteCountChunk.h"
#include "inet/networklayer/common/FragmentationTag_m.h"
#include "inet/networklayer/common/L3AddressResolver.h"
#include "inet/transportlayer/udp/Udp.h"
#include "inet/transportlayer/contract/udp/UdpControlInfo_m.h"
//#include "carla_omnet/TodCarlanetManager.h"
//#include "messages/TodMessages_m.h"

using namespace omnetpp;
using namespace inet;


Define_Module(CarlaCarApp);


CarlaCarApp::~CarlaCarApp()
{
    //cancelAndDelete(updateStatusSelfMessage);
}

void CarlaCarApp::initialize(int stage)
{
    ApplicationBase::initialize(stage);
    if (stage == INITSTAGE_LOCAL) {
        carlanetManager = check_and_cast<CarlanetManager*>(getParentModule()->getParentModule()->getSubmodule("carlanetManager"));

        //carlanetManager = check_and_cast<CarlanetManager*>(getParentModule()->getParentModule()->getSubmodule("carlanetManager"));
        //newCommandMsg = new cMessage("NewCommand");
        //actorId = getParentModule()->getName();

               /*;
                statusUpdateInterval = par("statusUpdateInterval");
        EV_INFO << "****** => " << statusUpdateInterval << endl;*/
    }


    //if (stage == INITSTAGE_APPLICATION_LAYER){}
}

void CarlaCarApp::refreshDisplay() const{}

void CarlaCarApp::finish()
{
    ApplicationBase::finish();
}

void CarlaCarApp::handleStartOperation(LifecycleOperation *operation)
{

    L3Address localAddress;
    L3AddressResolver().tryResolve(par("localAddress"), localAddress);
    int localPort = par("localPort");
    socket.setOutputGate(gate("socketOut"));
    socket.bind(localAddress, localPort);
    socket.setCallback(this);
    // wait statusUpdateInterval more before start to let Carla be ready

    //EV_INFO << "First update will be at: " << firstStatusUpdate << endl;

    //const simtime_t firstStatusUpdate = simTime() + carlanetManager->getCarlaInitialCarlaTimestamp() + statusUpdateInterval;
    //scheduleAt(firstStatusUpdate, updateStatusSelfMessage);
}


void CarlaCarApp::handleStopOperation(LifecycleOperation *operation)
{
    socket.close();
}

void CarlaCarApp::handleCrashOperation(LifecycleOperation *operation)
{
    if (operation->getRootModule() != getContainingNode(this)) // closes socket when the application crashed only
        socket.destroy(); // TODO  in real operating systems, program crash detected by OS and OS closes sockets of crashed programs.
    socket.setCallback(nullptr);
}


void CarlaCarApp::handleMessageWhenUp(cMessage* msg){

    if (msg->isSelfMessage()){

    }else if(socket.belongsToSocket(msg)){
        socket.processMessage(msg);
    }

}




void CarlaCarApp::socketDataArrived(UdpSocket *socket, Packet *packet){
    //emit(packetReceivedSignal, packet);
    EV_INFO << "Received packet: " << UdpSocket::getReceivedPacketInfo(packet) << endl;

    processPacket(packet);

    delete packet;
    //numReceived++;
}


void CarlaCarApp::socketErrorArrived(UdpSocket *socket, Indication *indication){}


void CarlaCarApp::socketClosed(UdpSocket *socket){}

void CarlaCarApp::sendPacket(Packet *packet){
    emit(packetSentSignal, packet);
    socket.sendTo(packet, destAddress, destPort);
    //numSent++;
}



void CarlaCarApp::processPacket(Packet *pk){
    if (pk->hasData<LightCommandMessage>()){
        L3Address destAddress = pk->getTag<L3AddressInd>()->getSrcAddress();
        int destPort = pk->getTag<L4PortInd>()->getSrcPort();
        pk->clearTags();
        pk->trim();

        auto message_sp = pk->peekData<LightCommandMessage>();
        EV_INFO << "Received a new light control command: " << message_sp->getLightNextState() << endl;

        light_command lightCommandMsg;
        lightCommandMsg.light_next_state = message_sp->getLightNextState();

        light_update lightUpdateMsg = carlanetManager->sendToAndGetFromCarla<light_command, light_update>(lightCommandMsg);

        auto packet = new Packet("LightStatus_");
        auto data = makeShared<LightStatusMessage>();
        const int fragmentLength = std::min((int) par("statusMsgLength"), (int) UDP_MAX_MESSAGE_SIZE-10);
        data->setChunkLength(B(fragmentLength));
        data->setLightCurrState(lightUpdateMsg.light_curr_state.c_str());
        auto creationTimeTag = data->addTag<CreationTimeTag>(); // add new tag
        creationTimeTag->setCreationTime(simTime()); // store current time
        packet->insertAtBack(data);
        socket.sendTo(packet, destAddress, destPort);
    }
    else{
        EV_WARN << "Received an unexpected packet "<< UdpSocket::getReceivedPacketInfo(pk) <<endl;
    }
}

