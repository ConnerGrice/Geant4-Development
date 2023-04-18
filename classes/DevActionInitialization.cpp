/*
 * DevActionInitialization.cpp
 *
 *  Created on: Feb 24, 2023
 *      Author: local1
 */
#include "DevActionInitialization.h"

DevActionInitialization::DevActionInitialization() {
}

DevActionInitialization::~DevActionInitialization() {
}

void DevActionInitialization::Build() const{
	SetUserAction(new DevPrimaryGeneratorAction);

	DevRunAction* runAction = new DevRunAction();
	SetUserAction(runAction);

	DevEventAction* eventAction = new DevEventAction();
	SetUserAction(eventAction);

	DevStackingAction* stackingAction = new DevStackingAction();
	SetUserAction(stackingAction);

	DevSteppingAction* steppingAction = new DevSteppingAction();
	SetUserAction(steppingAction);
}

void DevActionInitialization::BuildForMaster() const {
	SetUserAction(new DevRunAction());
}

