/*
 * DevRunAction.cpp
 *
 *  Created on: Mar 13, 2023
 *      Author: local1
 */

#include "DevRunAction.h"

DevRunAction::DevRunAction(): bTotal(0),cTotal(0),dTotal(0),nGood(0),nAlright(0),nBad(0) {
}

DevRunAction::~DevRunAction() {
}

void DevRunAction::BeginOfRunAction(const G4Run*) {
}

void DevRunAction::printCount() {
	G4cout<<"StaveB Count: "<<bTotal<<G4endl;
	G4cout<<"StaveD Count: "<<dTotal<<G4endl;
	G4cout<<"StaveC Count: "<<cTotal<<G4endl;

	G4cout<<"Good Count: "<<nGood<<G4endl;
	G4cout<<"Alright Count: "<<nAlright<<G4endl;
	G4cout<<"Bad Count: "<<nBad<<G4endl;
	G4cout<<"Total Count: "<<nGood+nAlright+nBad+nInvalid<<G4endl;
}

void DevRunAction::EndOfRunAction(const G4Run*) {
	G4double dEff = dTotal/200480.0;
	G4cout<<"StaveD Eff: "<<dTotal<<"/200480"<<" = "<<dEff<<G4endl;
	G4double bEff = bTotal/200480.0;
	G4cout<<"StaveB Eff: "<<bTotal<<"/200480"<<" = "<<bEff<<G4endl;
	G4double cEff = cTotal/200480.0;
	G4cout<<"StaveC Eff: "<<cTotal<<"/200480"<<" = "<<cEff<<G4endl;

	G4int successful = nGood + nAlright + nBad;
	G4double succEff = successful/100240.0;
	G4cout<<"Valid: "<<successful<<"/100240 = "<<succEff<<G4endl;
	G4cout<<"Invalid: "<<nInvalid<<"/100240"<<G4endl;

	G4double goodEff = nGood/(G4double)successful;
	G4cout<<"Good: "<<nGood<<"/"<<successful<<" = "<<goodEff<<G4endl;

	G4double alrightEff = nAlright/(G4double)successful;
	G4cout<<"Good: "<<nAlright<<"/"<<successful<<" = "<<alrightEff<<G4endl;

	G4double badEff = nBad/(G4double)successful;
	G4cout<<"Good: "<<nBad<<"/"<<successful<<" = "<<badEff<<G4endl;


}


