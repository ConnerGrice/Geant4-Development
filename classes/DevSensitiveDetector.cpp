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



	G4int zPixel = touchable->GetCopyNumber();
	G4int yPixel = touchable->GetCopyNumber(1);
	G4int segment = touchable->GetCopyNumber(2);
	G4int unit = touchable->GetCopyNumber(4);
	G4int stave = touchable->GetCopyNumber(5);

	std::pair<int,int> pixels = std::make_pair(zPixel,yPixel);

	//ChipDigitiserMap* map = ChipDigitiserMap::getInstance();


	//G4VPhysicalVolume* yPixelVol = touchable->GetVolume(1);
	//G4VPhysicalVolume* segmentVol = touchable->GetVolume(2);
	//G4VPhysicalVolume* segmentVol2 = touchable->GetVolume(3);
	//G4VPhysicalVolume* stripVol = touchable->GetVolume(4);
	//G4VPhysicalVolume* staveVol = touchable->GetVolume(5);

	G4ThreeVector test;
	G4ThreeVector temp;
	G4double tempRot = 0;
	G4String name;
	for (int i=5;i>-1;i--) {
		temp = touchable->GetVolume(i)->GetObjectTranslation();

		/*
		if (i == 4) {
			temp.rotateZ(tempRot);
		}

		if (i < 4) {
			temp.rotateZ(M_PI+tempRot);
		}
		*/
		temp.rotateZ(tempRot);
		tempRot += touchable->GetVolume(i)->GetObjectRotationValue().getDelta();
		test += temp;
		name = touchable->GetVolume(i)->GetName();
		//G4cout<<name<<": "<<tempRot<<" ("<<temp.getX()<<","<<temp.getY()<<","<<temp.getZ()<<")"<<G4endl;
	}
	G4cout<<"Digit: ("<<test.getX()<<","<<test.getY()<<","<<test.getZ()<<")"<<G4endl;

	G4int trackID = aStep->GetTrack()->GetTrackID();


	//Detector construction instance
	DevDetectorConstruction construction;

	//Parameters
	G4double glueThickness = 40*um;
	G4double chipThickness = 50*um;
	G4double width = 0;
	G4double radius;

	//Position of pixel
	G4ThreeVector pos;
/*
	//Sets initial x position
	if (SensitiveDetectorName == "StaveB") {
		width = construction.getPlateWidth(2);
		G4cout<<"B: ";
	} else if (SensitiveDetectorName == "StaveC") {
		width = construction.getPlateWidth(3);
		G4cout<<"C: ";
	} else if (SensitiveDetectorName == "StaveD") {
		width = construction.getPlateWidth(4);
		G4cout<<"D: ";
	}
*/
	//Sets point to centre of stave 0 of layer
	radius = construction.getPlateRadius(width);
	pos = G4ThreeVector(radius,0,0);

	//Distance from cold plate to chip layer
	G4double offset = construction.plateThickness/2.0 +
			construction.fleeceThickness + glueThickness +
			chipThickness/2.0;

	//unit is evet = unit is on back of cold plate
	G4String testing = "Inside";
	if ((unit+1)%2 != 0) {
		offset = -offset;
		testing = "Outside";
	}
	pos += G4ThreeVector(offset,0,0);

	//CORRECT UPTO THIS POINT

	//Set point to correct unit
	offset = -width + PADDING + (construction.HCIWidth-1.2*mm)/2.0;
	pos += G4ThreeVector(0,offset + unit*construction.HCIWidth,0);

	//Set point in centre of correct segment
	G4double segmentLength = 30*mm;
	G4double segmentGap = 0.2*mm;
	offset = -(construction.HCILength/2.0)+segmentLength/2.0 + HCIZ;
	pos += G4ThreeVector(0,0,offset+segment*(segmentLength+segmentGap));

	//Sets point in chip to (0,0)
	pos += G4ThreeVector(0,(construction.HCIWidth-1.2*mm)/2.0-construction.HCIPixelSide/2.0,
			-segmentLength/2.0+construction.HCIPixelSide/2.0);

	//Set point to correct chip position
	pos += G4ThreeVector(0,yPixel*construction.HCIPixelSide,zPixel*construction.HCIPixelSide);


	//Set point to correct stave
	pos.rotateZ(stave*construction.angle);

	G4ThreeVector exact = aStep->GetPostStepPoint()->GetPosition();
/*
	G4cout<<testing<<":";
	G4cout<<"Unit("<<unit<<"): ";
	G4cout<<"Segment("<<segment<<"): ";
	G4cout<<"Pixel(0,"<<yPixel<<","<<zPixel<<"): ";
	G4cout<<"Stave("<<stave<<")"<<G4endl;
*/
	G4cout<<"Exact: ("<<exact.getX()<<","<<exact.getY()<<","<<exact.getZ()<<")"<<G4endl;
	//G4cout<<"Digit: ("<<pos.getX()<<","<<pos.getY()<<","<<pos.getZ()<<")"<<G4endl;

	G4double xDiff = exact.getX()-test.getX();
	G4double yDiff = exact.getY()-test.getY();
	G4double zDiff = exact.getZ()-test.getZ();
	G4cout<<"Diff : ("<<xDiff<<","<<yDiff<<","<<zDiff<<")"<<G4endl;
	G4cout<<"============================================="<<G4endl;

	//Creates a hit and fills it with data
	auto hit = new DevHit();
	hit->SetTrackID(trackID);
	hit->SetExactPosition(exact);
	hit->SetDigitisedPosition(pos);

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
