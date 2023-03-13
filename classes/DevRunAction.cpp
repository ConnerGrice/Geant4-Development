/*
 * DevRunAction.cpp
 *
 *  Created on: Mar 13, 2023
 *      Author: local1
 */

#include "DevRunAction.h"

DevRunAction::DevRunAction() {
	// TODO Auto-generated constructor stub

}

DevRunAction::~DevRunAction() {
	// TODO Auto-generated destructor stub
}

void DevRunAction::BeginOfRunAction(const G4Run*) {

}

void DevRunAction::EndOfRunAction(const G4Run* aRun) {
	auto HCtable = aRun->GetHCtable();
	G4int nCollections = HCtable->entries();
	for (int i = 0; i<nCollections;i++) {
		G4String name = HCtable->GetHCname(i);
		G4int id = HCtable->GetCollectionID(i);
		G4cout<<"Name: "<<name;
		G4cout<<" ID: "<<id<<G4endl;
	}
}

