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

	//const G4VTouchable* touchable = aStep->GetPreStepPoint()->GetTouchable();



	G4int preDepth = prePoint->GetTouchable()->GetHistory()->GetDepth();
	G4int postDepth = postPoint->GetTouchable()->GetHistory()->GetDepth();

/*
	if (postDepth == 6) {
		prePoint = postPoint;
	}
	*/

	const G4VTouchable* touchable = prePoint->GetTouchable();

	G4String test = postPoint->GetTouchable()->GetHistory()->GetTopVolume()->GetName();
	G4ThreeVector testLoc = postPoint->GetTouchable()->GetHistory()->GetTopVolume()->GetTranslation();
	G4int testCopy = postPoint->GetTouchable()->GetHistory()->GetTopReplicaNo();
	G4int testCopy2 = postPoint->GetTouchable()->GetHistory()->GetReplicaNo(5);

		//G4NavigationHistory

	G4String test1 = prePoint->GetTouchable()->GetHistory()->GetTopVolume()->GetName();
	G4ThreeVector test1Loc = prePoint->GetTouchable()->GetHistory()->GetTopVolume()->GetTranslation();
	G4int test1Copy = prePoint->GetTouchable()->GetHistory()->GetTopVolume()->GetCopyNo();
	G4int test1Copy2 = prePoint->GetTouchable()->GetHistory()->GetReplicaNo(5);

	//G4cout<<" Pre: "<<test1<<"["<<test1Copy<<","<<test1Copy2<<"]"<<test1Loc<<"| Post: "<<test<<"["<<testCopy<<","<<testCopy2<<"]"<<testLoc<<G4endl;

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

	for (int i=5;i>-1;i--) {
	//for (int i=1;i<7;i++) {
		//Get volume Position relative to mother
		//temp = aStep->GetPostStepPoint()->GetTouchable()->GetHistory()->GetVolume(i)->GetObjectTranslation();
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

		if (i < 1) {
		//if (i == 6) {
			//G4cout<<name<<"("<<yPixel<<","<<zPixel<<"): "<<" ("<<temp.getX()<<","<<temp.getY()<<","<<temp.getZ()<<")"<<G4endl;
			//G4cout<<name<<"("<<yPixel<<","<<zPixel<<"): "<<" ("<<digitised.getX()<<","<<digitised.getY()<<","<<digitised.getZ()<<")"<<G4endl;
		}
	}
	//Get position of particle as it leaves the pixel
	G4ThreeVector exact2 = prePoint->GetPosition();
	G4ThreeVector exact = postPoint->GetPosition();

	//G4cout<<exact1<<G4endl;
	//G4cout<<exact2<<G4endl;
	G4ThreeVector exact1 = (exact2+exact1)/2.0;
	//Gets the particle ID
	G4int trackID = aStep->GetTrack()->GetTrackID();


	//TODO: REMOVE DEBUGGING LOGS
	G4cout<<"Digit: ("<<digitised.getX()<<","<<digitised.getY()<<","<<digitised.getZ()<<")"<<G4endl;
	G4cout<<"Exact: ("<<exact.getX()<<","<<exact.getY()<<","<<exact.getZ()<<")"<<G4endl;
	G4cout<<"Pixel: "<<stave<<"("<<yPixel<<","<<zPixel<<")"<<G4endl;

	G4double xDiff = exact.getX()-digitised.getX();
	G4double yDiff = exact.getY()-digitised.getY();
	G4double zDiff = exact.getZ()-digitised.getZ();
	G4cout<<"Diff : ("<<xDiff<<","<<yDiff<<","<<zDiff<<")"<<G4endl;
	G4cout<<"============================================="<<G4endl;

	//Creates a hit and fills it with data
	auto hit = new DevHit();
	hit->SetTrackID(trackID);
	hit->SetExactPosition(exact);
	hit->SetDigitisedPosition(digitised);

	pHitCollection->insert(hit);

	return true;
}

void DevSensitiveDetector::EndOfEvent(G4HCofThisEvent*) {
	/*
	G4int numHits = pHitCollection->entries();

	//Prints the detector and which section was hit
	G4cout<<SensitiveDetectorName<<": "<<numHits<<" (";
	for (int i=0;i<numHits;i++) {

		G4int copyNo = (*pHitCollection)[i]->GetCopyNo();
		G4int copyNo2 = (*pHitCollection)[i]->GetCopyNo2();
		G4int copyNo3 = (*pHitCollection)[i]->GetCopyNo3();
		G4cout<<copyNo<<"("<<copyNo2<<"["<<copyNo3<<"]),";

	}
	G4cout<<")"<<G4endl;
	*/
}
