/*
 * DevEventAction.cpp
 *
 *  Created on: Mar 13, 2023
 *      Author: local1
 */

#include "DevEventAction.h"

DevEventAction::DevEventAction(DevRunAction* runAction) {
	rAction = runAction;

}

DevEventAction::~DevEventAction() {
	// TODO Auto-generated destructor stub
}

void DevEventAction::BeginOfEventAction(G4Event* anEvent) {

}

void DevEventAction::EndOfEventAction(G4Event* anEvent) {
	G4SDManager* senDetManager = G4SDManager::GetSDMpointer();

	G4String cName = "StaveCCollection";
	G4String dName = "StaveDCollection";
	G4String bName = "StaveBCollection";

}

