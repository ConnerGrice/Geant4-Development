/*
 * DevActionInitialization.cpp
 *
 *  Created on: Feb 24, 2023
 *      Author: local1
 */
#include "DevActionInitialization.h"

DevActionInitialization::DevActionInitialization() {
	// TODO Auto-generated constructor stub

}

DevActionInitialization::~DevActionInitialization() {
	// TODO Auto-generated destructor stub
}

void DevActionInitialization::Build() const{
	SetUserAction(new DevPrimaryGeneratorAction);

	DevRunAction* runAction = new DevRunAction();
	SetUserAction(runAction);

	DevEventAction* eventAction = new DevEventAction(runAction);
	SetUserAction(eventAction);
}

void DevActionInitialization::BuildForMaster() const {
	SetUserAction(new DevRunAction());
}
