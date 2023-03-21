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

	DevEventAction* eventAction = new DevEventAction(runAction);
	SetUserAction(eventAction);

	DevStackingAction* stackingAction = new DevStackingAction();
	SetUserAction(stackingAction);
}
/*
void DevActionInitialization::BuildForMaster() const {
	SetUserAction(new DevRunAction());
}
*/
