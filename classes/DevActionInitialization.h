/*
 * DevActionInitialization.h
 *
 *  Created on: Feb 24, 2023
 *      Author: local1
 */

#ifndef INCLUDE_DEVACTIONINITIALIZATION_H_
#define INCLUDE_DEVACTIONINITIALIZATION_H_

#include <G4VUserActionInitialization.hh>

#include "DevPrimaryGeneratorAction.h"
#include "DevRunAction.h"
#include "DevEventAction.h"
#include "DevStackingAction.h"
#include "DevSteppingAction.h"

class DevActionInitialization: public G4VUserActionInitialization {
public:
	DevActionInitialization();
	virtual ~DevActionInitialization();

public:
	void Build() const override;
	void BuildForMaster() const override;
};

#endif /* INCLUDE_DEVACTIONINITIALIZATION_H_ */
