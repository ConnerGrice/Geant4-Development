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
}

