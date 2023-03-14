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
	G4StepPoint* stepPoint = aStep->GetPreStepPoint();
	//Gets the copy number of hit detector
	const G4VTouchable* touchable = stepPoint->GetTouchable();
	G4int copyNo = touchable->GetCopyNumber();

	auto hit = new DevHit();
	hit->SetCopyNo(copyNo);

	pHitCollection->insert(hit);

	return true;
}

void DevSensitiveDetector::EndOfEvent(G4HCofThisEvent*) {
	G4int numHits = pHitCollection->entries();

	G4cout<<SensitiveDetectorName<<": "<<numHits<<" (";
	for (int i=0;i<numHits;i++) {
		G4int copyNo = (*pHitCollection)[i]->GetCopyNo();
		 G4cout<<copyNo<<",";
	}
	G4cout<<")"<<G4endl;
}
