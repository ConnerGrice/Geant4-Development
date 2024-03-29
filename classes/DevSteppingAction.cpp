/*
 * DevSteppingAction.cpp
 *
 *  Created on: Apr 18, 2023
 *      Author: local1
 */

#include "DevSteppingAction.h"

DevSteppingAction::~DevSteppingAction() {
}

void DevSteppingAction::UserSteppingAction(const G4Step* aStep) {
	//Gets the current run object
	DevRun* run = static_cast<DevRun*>(G4RunManager::GetRunManager()->GetNonConstCurrentRun());

	//The track that will be used multiple times
	G4Track* aTrack = aStep->GetTrack();
	auto aTrack1 = aStep->GetPreStepPoint();

	G4LogicalVolume* currentVol =
			aTrack1->GetTouchable()->GetVolume()->GetLogicalVolume();

	const DevDetectorConstruction* construction =
			static_cast<const DevDetectorConstruction*> (G4RunManager::GetRunManager()->GetUserDetectorConstruction());
	G4LogicalVolume* scoringVol = construction->getScoringVolume();

	//Checks if particle is in CALIFA
	if (currentVol != scoringVol)
		return;

	G4int particleID = aTrack->GetTrackID();
	G4double energy = aTrack1->GetKineticEnergy();
	G4cout<<"K"<<particleID<<" ENERGY: "<<energy<<G4endl;


	//Determines which particle needs which energy
	switch(particleID) {
	case(1):
			//Sets energy and califa flag in DevRun
			run->setp1Energy(energy);
			run->p1Hit(true);
			break;
	case(2):
			run->setp2Energy(energy);
			run->p2Hit(true);
			break;
	default:
		break;
	}

	//Destroy particles after hitting CALIFA to speed up process
	aTrack->SetTrackStatus(fStopAndKill);
}

