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
	G4StepPoint* stepPoint = aStep->GetPreStepPoint();
	const G4VTouchable* touchable = stepPoint->GetTouchable();

	//Get the copy number of volumes in the volume tree
	G4int zPixel = touchable->GetCopyNumber();
	G4int yPixel = touchable->GetCopyNumber(1);
	G4int segment = touchable->GetCopyNumber(2);
	G4int unit = touchable->GetCopyNumber(4);
	G4int stave = touchable->GetCopyNumber(5);

	//Variables for getting volume position
	G4ThreeVector digitised;	//Digitised position
	G4ThreeVector temp;			//Volumes position wrt mother volume
	G4double tempRot = 0;		//Sum of volume rotations

	//Debugging variables
	G4String name;	//Name of current volume
	G4int copy;		//Copy number of current volume

	//Loops through each volume in the tree
	for (int i=5;i>-1;i--) {
		//Get volume Position relative to mother
		temp = touchable->GetVolume(i)->GetObjectTranslation();

		//Get copy number
		copy = touchable->GetCopyNumber(i);

		//Rotate this relative to mother volume rotation
		temp.rotateZ(tempRot);

		//Add current volume rotation to rotation
		if (stave >= 2 && stave <= 4) {
			tempRot -= touchable->GetVolume(i)->GetObjectRotationValue().getDelta();
		} else {
			tempRot += touchable->GetVolume(i)->GetObjectRotationValue().getDelta();
		}

		//Add rotated position vector
		digitised += temp;

		//Get name of volume
		name = touchable->GetVolume(i)->GetName();

		//G4cout<<name<<"("<<copy<<"): "<<tempRot<<" ("<<temp.getX()<<","<<temp.getY()<<","<<temp.getZ()<<")"<<G4endl;
	}

	//Get position of particle as it leaves the pixel
	G4ThreeVector exact = aStep->GetPreStepPoint()->GetPosition();
	//Gets the particle ID
	G4int trackID = aStep->GetTrack()->GetTrackID();
/*
	//TODO: REMOVE DEBUGGING LOGS
	G4cout<<"Pixel: "<<stave<<"("<<yPixel<<","<<zPixel<<")"<<G4endl;
	G4cout<<"Digit: ("<<digitised.getX()<<","<<digitised.getY()<<","<<digitised.getZ()<<")"<<G4endl;
	G4cout<<"Exact: ("<<exact.getX()<<","<<exact.getY()<<","<<exact.getZ()<<")"<<G4endl;


	G4double xDiff = exact.getX()-digitised.getX();
	G4double yDiff = exact.getY()-digitised.getY();
	G4double zDiff = exact.getZ()-digitised.getZ();
	G4cout<<"Diff : ("<<xDiff<<","<<yDiff<<","<<zDiff<<")"<<G4endl;
	G4cout<<"============================================="<<G4endl;
*/
	//Creates a hit and fills it with data
	auto hit = new DevHit();
	hit->SetTrackID(trackID);
	hit->SetExactPosition(exact);
	hit->SetDigitisedPosition(digitised);

	pHitCollection->insert(hit);

	return true;
}

void DevSensitiveDetector::EndOfEvent(G4HCofThisEvent*) {
	G4int numHits = pHitCollection->entries();

	//Prints the detector and which section was hit
	G4cout<<SensitiveDetectorName<<": "<<numHits<<" (";
	for (int i=0;i<numHits;i++) {
		/*
		G4int copyNo = (*pHitCollection)[i]->GetCopyNo();
		G4int copyNo2 = (*pHitCollection)[i]->GetCopyNo2();
		G4int copyNo3 = (*pHitCollection)[i]->GetCopyNo3();
		G4cout<<copyNo<<"("<<copyNo2<<"["<<copyNo3<<"]),";
		*/
	}
	G4cout<<")"<<G4endl;
}
