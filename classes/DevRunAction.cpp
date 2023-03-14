/*
 * DevRunAction.cpp
 *
 *  Created on: Mar 13, 2023
 *      Author: local1
 */

#include "DevRunAction.h"

DevRunAction::DevRunAction(): bTotal(0),cTotal(0),dTotal(0) {
}

DevRunAction::~DevRunAction() {
}

void DevRunAction::BeginOfRunAction(const G4Run*) {
}

void DevRunAction::printCount() {
	G4cout<<"StaveC Count: "<<cTotal<<G4endl;
	G4cout<<"StaveD Count: "<<dTotal<<G4endl;
	G4cout<<"StaveB Count: "<<bTotal<<G4endl;
}

void DevRunAction::EndOfRunAction(const G4Run*) {
	G4double cEff = cTotal/200480.0;
	G4cout<<"StaveC Eff: "<<cTotal<<"/200480"<<" = "<<cEff<<G4endl;
	G4double dEff = dTotal/200480.0;
	G4cout<<"StaveD Eff: "<<dTotal<<"/200480"<<" = "<<dEff<<G4endl;
	G4double bEff = bTotal/200480.0;
	G4cout<<"StaveB Eff: "<<bTotal<<"/200480"<<" = "<<bEff<<G4endl;
}


