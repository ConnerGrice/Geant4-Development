/*
 * DevSensitiveDetector.cpp
 *
 *  Created on: Mar 10, 2023
 *      Author: local1
 */

#include "DevSensitiveDetector.h"

DevSensitiveDetector::DevSensitiveDetector(G4String name,G4String collection):
G4VSensitiveDetector(name),pHitCollection(0){
	collectionName.insert(collection);
}

DevSensitiveDetector::~DevSensitiveDetector() {
}

void DevSensitiveDetector::Initialize(G4HCofThisEvent* hce) {
	pHitCollection = new DevHitsCollection(SensitiveDetectorName,collectionName[0]);

	G4int hcID = G4SDManager::GetSDMpointer()->GetCollectionID(pHitCollection);
	hce->AddHitsCollection(hcID,pHitCollection);
}

G4bool DevSensitiveDetector::ProcessHits(G4Step* aStep,G4TouchableHistory*) {
	//Gets preStep point and Post step point of current step
	G4StepPoint* prePoint = aStep->GetPreStepPoint();

	//Touchable of current step
	const G4VTouchable* touchable = prePoint->GetTouchable();

	//Stave number
	G4int stave = touchable->GetCopyNumber(5);

	//Variables for getting volume position
	G4ThreeVector digitised;
	G4ThreeVector temp;
	G4double tempRot;
	G4double rot = 0;

	//Traverse volume tree
	for (int i=5;i>-1;i--) {
		//Get volume along tree of volumes
		G4VPhysicalVolume* vol = touchable->GetVolume(i);

		//Get volume Position relative to mother
		temp = vol->GetObjectTranslation();

		//Rotate this relative to mother volume rotation
		temp.rotateZ(rot);

		//Add current volume rotation to rotation
		tempRot = vol->GetObjectRotationValue().getDelta();

		//Rotation must negate if the stave is on the left side
		//For octagon: (stave >= 2 && stave <= 5)
		//For hexagon: (stave >= 2 && stave <= 4)
		if (stave >= 2 && stave <= 5)
			rot -= tempRot;
		else
			rot += tempRot;

		//Add rotated position vector
		digitised += temp;
	}

	//Get position of particle as it leaves the pixel
	G4ThreeVector exact = prePoint->GetPosition();
	//Gets the particle ID
	G4int trackID = aStep->GetTrack()->GetTrackID();

	//Creates a hit and fills it with data
	auto hit = new DevHit();
	hit->SetTrackID(trackID);
	hit->SetExactPosition(exact);
	hit->SetDigitisedPosition(digitised);

	pHitCollection->insert(hit);

	return true;
}

void DevSensitiveDetector::EndOfEvent(G4HCofThisEvent*) {

}
