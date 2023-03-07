//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see http://www.gnu.org/licenses/.
// 

#include "CarlanetMobility.h"

#include "CarlanetManager.h"

Define_Module(CarlanetMobility);

void CarlanetMobility::initialize(int stage)
{
    MobilityBase::initialize(stage);
    if (stage == inet::INITSTAGE_LOCAL){
        carlaActorType = par("carlaActorType").stdstringValue();
        carlaActorConfiguration = check_and_cast<cValueMap*>(par("carlaActorConfiguration").objectValue()); //.cValueMap(); // .objectValue();
        auto carlaManager = check_and_cast<CarlanetManager*>(getModuleByPath("<root>.CarlanetManager"));
        // register to carlaManager
        carlaManager->registerMobilityModule(this);
    }
}

void CarlanetMobility::setInitialPosition(){
    if (!preInitialized){
        MobilityBase::setInitialPosition();
    }
}

void CarlanetMobility::preInitialize(const inet::Coord& position, const inet::Coord& velocity,  const inet::Quaternion& rotation){
    preInitialized = true;
    lastPosition = position;
    lastVelocity = velocity;
    lastOrientation = rotation;
}


void CarlanetMobility::nextPosition(const inet::Coord& position, const inet::Coord& velocity,  const inet::Quaternion& rotation){
    lastPosition = position;
    lastVelocity = velocity;
    lastOrientation = rotation;

    emitMobilityStateChangedSignal();
}


const inet::Coord& CarlanetMobility::getCurrentPosition()
{
    return lastPosition;
}

const inet::Coord& CarlanetMobility::getCurrentVelocity()
{
    return lastVelocity;
}

const inet::Coord& CarlanetMobility::getCurrentAcceleration()
{
    throw cRuntimeError("Invalid operation");
}

const inet::Quaternion& CarlanetMobility::getCurrentAngularPosition()
{
    return lastOrientation;
}

const inet::Quaternion& CarlanetMobility::getCurrentAngularVelocity()
{
    return lastAngularVelocity;
}

const inet::Quaternion& CarlanetMobility::getCurrentAngularAcceleration()
{
    throw cRuntimeError("Invalid operation");
}


void CarlanetMobility::handleSelfMessage(cMessage* msg){
}
