//
// Copyright (C) 2000 Institut fuer Telematik, Universitaet Karlsruhe
// Copyright (C) 2004,2011 OpenSim Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later
//


#include "carlanet/lightcontrol/CarlaAgentApp.h"
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


Define_Module(CarlaAgentApp);


CarlaAgentApp::~CarlaAgentApp()
{
    //cancelAndDelete(updateStatusSelfMessage);
}

void CarlaAgentApp::initialize(int stage)
{
    ApplicationBase::initialize(stage);
    if (stage == INITSTAGE_LOCAL) {
        carlanetManager = check_and_cast<CarlanetManager*>(getParentModule()->getParentModule()->getSubmodule("carlanetManager"));
        firstCommandMsg = new cMessage("firstCommandMsg");
        commandStartTime = par("sendInterval");
        /*actorId = getParentModule()->getName();
        carlaCommunicationManager = check_and_cast<TodCarlanetManager*>(
                getParentModule()->getParentModule()->getSubmodule("carlaCommunicationManager"));

        updateStatusSelfMessage = new cMessage("UpdateStatus");
        statusUpdateInterval = par("statusUpdateInterval");
        EV_INFO << "****** => " << statusUpdateInterval << endl;*/
    }


    //if (stage == INITSTAGE_APPLICATION_LAYER){}
}

void CarlaAgentApp::refreshDisplay() const{}

void CarlaAgentApp::finish()
{
    ApplicationBase::finish();
}

void CarlaAgentApp::handleStartOperation(LifecycleOperation *operation)
{

    L3AddressResolver().tryResolve(par("destAddress"), destAddress);
    destPort = par("destPort");

    socket.setOutputGate(gate("socketOut"));
    socket.bind(destPort);
    //socket.setTos(0b00011100);
    socket.setCallback(this);

    // wait statusUpdateInterval more before start to let Carla be ready
    //simtime_t firstStatusUpdate = simTime() + carlaCommunicationManager->getCarlaInitialCarlaTimestamp() + statusUpdateInterval;

    //EV_INFO << "First update will be at: " << firstStatusUpdate << endl;

    simtime_t firstCommandTime = simTime() + carlanetManager->getCarlaInitialCarlaTimestamp() + commandStartTime;


    scheduleAt(firstCommandTime, firstCommandMsg);
}


void CarlaAgentApp::handleStopOperation(LifecycleOperation *operation)
{
    socket.close();
}

void CarlaAgentApp::handleCrashOperation(LifecycleOperation *operation)
{
    if (operation->getRootModule() != getContainingNode(this)) // closes socket when the application crashed only
        socket.destroy(); // TODO  in real operating systems, program crash detected by OS and OS closes sockets of crashed programs.
    socket.setCallback(nullptr);
}


void CarlaAgentApp::handleMessageWhenUp(cMessage* msg){

    if (msg->isSelfMessage()){

        if (msg == firstCommandMsg){
            sendNewLightCommand();
            //sendUpdateStatusPacket();
            // this time contains all the parameters needed to generate status message, TODO: create ad hoc message
            // Note, you have to add the same time for all the UDP pkt of one frame
            //scheduleAfter(commandUpdateInterval, msg);

        }
    }else if(socket.belongsToSocket(msg)){
            socket.processMessage(msg);
    }

}


void CarlaAgentApp::socketDataArrived(UdpSocket *socket, Packet *packet){
    emit(packetReceivedSignal, packet);
    EV_INFO << "Received packet: " << UdpSocket::getReceivedPacketInfo(packet) << endl;

    processPacket(packet);

    delete packet;
    numReceived++;
}


void CarlaAgentApp::socketErrorArrived(UdpSocket *socket, Indication *indication){}


void CarlaAgentApp::socketClosed(UdpSocket *socket){}

void CarlaAgentApp::sendPacket(Packet *packet){
    emit(packetSentSignal, packet);
    socket.sendTo(packet, destAddress, destPort);
    numSent++;
}



void CarlaAgentApp::processPacket(Packet *pk){

    if (pk->hasData<LightStatusMessage>()){
        auto message_sp = pk->peekData<LightStatusMessage>();
        EV_INFO << "Received a new light status: " << message_sp->getLightCurrState() << endl;

        current_light_state = message_sp->getLightCurrState();
        sendNewLightCommand();
    }
    else {
        EV_WARN << "Received an unexpected packet "<< UdpSocket::getReceivedPacketInfo(pk) <<endl;
    }
}

void CarlaAgentApp::sendNewLightCommand(){
    EV_INFO << "Send status update" << endl;

    light_update lightStateMsg;
    lightStateMsg.light_curr_state = current_light_state;

    auto lightCommandMsg = carlanetManager->sendToAndGetFromCarla<light_update, light_command>(lightStateMsg);
    const int fragmentLength = std::min((int) par("commandMsgLength"), (int) UDP_MAX_MESSAGE_SIZE-10);
    auto packet = new Packet("LightCommand_");
    auto data = makeShared<LightCommandMessage>();
    data->setChunkLength(B(fragmentLength));
    data->setLightNextState(lightCommandMsg.light_next_state.c_str());
    auto creationTimeTag = data->addTag<CreationTimeTag>(); // add new tag
    creationTimeTag->setCreationTime(simTime()); // store current time
    packet->insertAtBack(data);
    socket.sendTo(packet, destAddress, destPort);
}

