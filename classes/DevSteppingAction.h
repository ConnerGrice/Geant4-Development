/*
 * DevSteppingAction.h
 *
 *  Created on: Apr 18, 2023
 *      Author: local1
 */

#ifndef GEANT4_DEVELOPMENT_CLASSES_DEVSTEPPINGACTION_H_
#define GEANT4_DEVELOPMENT_CLASSES_DEVSTEPPINGACTION_H_

#include <G4UserSteppingAction.hh>

#include <G4Step.hh>
#include <G4Track.hh>
#include <G4RunManager.hh>
#include <G4LogicalVolume.hh>

#include "DevRun.h"
#include "DevDetectorConstruction.h"

class DevSteppingAction: public G4UserSteppingAction {
public:
	DevSteppingAction() {};
	virtual ~DevSteppingAction();

	void UserSteppingAction(const G4Step* aStep) override;
};

#endif /* GEANT4_DEVELOPMENT_CLASSES_DEVSTEPPINGACTION_H_ */
