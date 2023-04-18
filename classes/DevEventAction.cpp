/*
 * DevEventAction.cpp
 *
 *  Created on: Mar 13, 2023
 *      Author: local1
 */

#include "DevEventAction.h"

DevEventAction::DevEventAction(): p1Energy(0),p2Energy(0) {
}

DevEventAction::~DevEventAction() {
}

void DevEventAction::BeginOfEventAction(const G4Event*) {
	G4cout<<"EVENT START==================================="<<G4endl;

}

void DevEventAction::EndOfEventAction(const G4Event* anEvent) {
	//Counter for the number of events
	G4cout<<"Event: "<<anEvent->GetEventID()<<"/100240"<<G4endl;
	G4cout<<"EVENT END==========================================="<<G4endl;
}

