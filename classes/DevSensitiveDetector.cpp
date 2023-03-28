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

	ChipDigitiserMap* map = ChipDigitiserMap::getInstance();
	G4ThreeVector position = map->getPosition(pixels);

	G4int trackID = aStep->GetTrack()->GetTrackID();

	G4cout<<SensitiveDetectorName<<": "<<G4endl;
	G4cout<<"    Stave:"<<stave<<G4endl;
	G4cout<<"    Unit:"<<unit<<G4endl;
	G4cout<<"    Segment:"<<segment<<G4endl;
	G4cout<<"    yPixel:"<<yPixel<<G4endl;
	G4cout<<"    zPixel:"<<zPixel<<G4endl;
	G4cout<<"    Coords: ("<<position.getX()<<","<<position.getY()<<","<<position.getZ()<<")"<<G4endl;


	//Creates a hit and fills it with data
	auto hit = new DevHit();
	hit->SetTrackID(trackID);

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
