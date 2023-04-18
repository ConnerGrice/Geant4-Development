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
	//The track that will be used multiple times
	G4Track* aTrack = aStep->GetTrack();

	G4LogicalVolume* currentVol =
			aTrack->GetTouchable()->GetVolume()->GetLogicalVolume();

	const DevDetectorConstruction* construction =
			static_cast<const DevDetectorConstruction*> (G4RunManager::GetRunManager()->GetUserDetectorConstruction());
	G4LogicalVolume* scoringVol = construction->getScoringVolume();

	//Checks if particle is in CALIFA
	if (currentVol != scoringVol)
		return;

	G4int particleID = aTrack->GetTrackID();
	G4double energy = aTrack->GetTotalEnergy();

	DevRun* run = static_cast<DevRun*>(G4RunManager::GetRunManager()->GetNonConstCurrentRun());

	//Determines which particle needs which energy
	switch(particleID) {
	case(1):
			run->setp1Energy(energy);
			break;
	case(2):
			run->setp2Energy(energy);
			break;
	default:
		break;
	}

	//Destroy particles after hitting CALIFA to speed up process
	aTrack->SetTrackStatus(fStopAndKill);
}

