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
	G4cout<<testing<<":";

	//Set point to correct unit
	offset = -width + PADDING + construction.HCIWidth/2.0;
	pos += G4ThreeVector(0,offset + unit*construction.HCIWidth);
	G4cout<<"Unit("<<unit<<"): ";

	//Set point in centre of correct segment
	G4double segmentLength = 30*mm;
	G4double segmentGap = 0.2*mm;
	offset = -(construction.HCILength/2.0)+segmentLength/2.0;
	pos += G4ThreeVector(0,0,offset+segment*(segmentLength+segmentGap));
	G4cout<<"Segment("<<segment<<"): ";

	//Sets point in chip to (0,0)
	pos += G4ThreeVector(0,construction.HCIWidth/2.0-construction.HCIPixelSide/2.0,
			-segmentLength/2.0+construction.HCIPixelSide/2.0);

	//Set point to correct chip position
	pos += G4ThreeVector(0,yPixel*construction.HCIPixelSide,zPixel*construction.HCIPixelSide);

	G4cout<<"Pixel(0,"<<yPixel<<","<<zPixel<<"): ";

	//Set point to correct stave
	pos.rotateZ(stave*construction.angle);

	G4cout<<"Stave("<<stave<<")"<<G4endl;

	G4ThreeVector exact = aStep->GetPostStepPoint()->GetPosition();
	G4cout<<"("<<exact.getX()<<","<<exact.getY()<<","<<exact.getZ()<<")"<<G4endl;
	G4cout<<"("<<pos.getX()<<","<<pos.getY()<<","<<pos.getZ()<<")"<<G4endl;

	G4ThreeVector detPos = touchable->GetVolume()->GetTranslation();

	//Creates a hit and fills it with data
	auto hit = new DevHit();
	hit->SetTrackID(trackID);
	hit->SetExactPosition(detPos);
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
