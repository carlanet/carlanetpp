//
// Copyright (C) 2000 Institut fuer Telematik, Universitaet Karlsruhe
// Copyright (C) 2004,2011 OpenSim Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later
//


#include "CarlaAgentApp.h"

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

    //scheduleAt(firstStatusUpdate, updateStatusSelfMessage);
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

    /*if (msg->isSelfMessage()){

        if (msg == updateStatusSelfMessage){
            retrieveStatusData();
            //sendUpdateStatusPacket();
            // this time contains all the parameters needed to generate status message, TODO: create ad hoc message
            // Note, you have to add the same time for all the UDP pkt of one frame
            scheduleAfter(statusUpdateInterval, msg);

        }
        else if (msg->getKind() == CREATION_STATUS_DATA_MSG_KIND) {
            sendUpdateStatusPacket(simTime());
        }
    }else if(socket.belongsToSocket(msg)){
            socket.processMessage(msg);
    }*/

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
    /*if (pk->hasData<TODMessage>()){
        if (pk->peekData<TODMessage>()->getMessageType() == TODMessageType::INSTRUCTION){
            //CARLA apply instruction
            auto message = pk->peekData<TodInstructionMessage>();
            carlaCommunicationManager->applyInstruction(message->getActorId(), message->getInstructionId());
        }
        else{
            EV_WARN << "Received an unexpected TOD Message " <<  pk->peekData<TODMessage>()->getMessageType()  << " check your implementation"<< endl;
        }
    }
    else{
        EV_WARN << "Received an unexpected packet "<< UdpSocket::getReceivedPacketInfo(pk) <<endl;
    }*/
}

