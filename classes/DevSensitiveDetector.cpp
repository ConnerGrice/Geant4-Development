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
	//Gets the copy number of hit detector
	G4StepPoint* prePoint = aStep->GetPreStepPoint();
	G4StepPoint* postPoint = aStep->GetPostStepPoint();

	const G4VTouchable* touchable = prePoint->GetTouchable();

	//Get the copy number of volumes in the volume tree
	/*
	G4int zPixel = touchable->GetCopyNumber();
	G4int yPixel = touchable->GetCopyNumber(1);
	G4int segment = touchable->GetCopyNumber(2);
	G4int unit = touchable->GetCopyNumber(4);
	*/

	G4int stave = touchable->GetCopyNumber(5);

	//Variables for getting volume position
	G4ThreeVector digitised;	//Digitised position
	G4ThreeVector temp;			//Volumes position wrt mother volume
	G4double tempRot;
	G4double rot = 0;//Sum of volume rotations

	for (int i=5;i>-1;i--) {
		//Get volume long tree of volumes
		G4VPhysicalVolume* vol = touchable->GetVolume(i);

		//Get volume Position relative to mother
		temp = vol->GetObjectTranslation();

		//Rotate this relative to mother volume rotation
		temp.rotateZ(rot);

		//Add current volume rotation to rotation
		tempRot = vol->GetObjectRotationValue().getDelta();
		if (stave >= 2 && stave <= 4)
			rot -= tempRot;
		else
			rot += tempRot;

		//Add rotated position vector
		digitised += temp;
	}


	//Get position of particle as it leaves the pixel
	G4ThreeVector exact = postPoint->GetPosition();
	//Gets the particle ID
	G4int trackID = aStep->GetTrack()->GetTrackID();

	//Creates a hit and fills it with data
	auto hit = new DevHit();
	hit->SetTrackID(trackID);
	hit->SetExactPosition(exact);
	hit->SetDigitisedPosition(digitised);
	//hit->SetEventID(eventID);

	pHitCollection->insert(hit);

	return true;
}

void DevSensitiveDetector::EndOfEvent(G4HCofThisEvent*) {

}
