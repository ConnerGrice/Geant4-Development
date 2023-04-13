/*
 * DevRun.cpp
 *
 *  Created on: Apr 13, 2023
 *      Author: local1
 */

#include "DevRun.h"

DevRun::DevRun() {
	G4cout<<"RECORDING"<<G4endl;

}

DevRun::~DevRun() {
	delete G4AnalysisManager::Instance();
}


G4VHitsCollection* DevRun::getHitCollection(const G4Event* anEvent, G4String hcName) {

	G4HCofThisEvent* hce = anEvent->GetHCofThisEvent();

	//Checks if hit collections for the event exist
	if (!hce) {
		G4ExceptionDescription msg;
		msg<<"No hits collection of this event found."<<G4endl;
		G4Exception("EventAction::EndOfEventAction()",
				"Code001", JustWarning,msg);
		return nullptr;
	}

	//Gets specific hit collection
	G4SDManager* senDetManager = G4SDManager::GetSDMpointer();
	G4int hcID = senDetManager->GetCollectionID(hcName);
	auto hc = hce->GetHC(hcID);

	//Checks if hit collection of a given name exists
	if (!hc) {
		G4ExceptionDescription msg;
		msg<<"Hits collection "<<hcID<<" of This event not found." <<G4endl;
		G4Exception("EventAction::EndOfEventAction()",
				"Code001", JustWarning, msg);
	}
	return hc;
}

hitContainer DevRun::doBothHit(G4VHitsCollection* collection) {
	G4int hitCount = collection->GetSize();

	DevHit* hit = new DevHit();
	G4int particle1 = 0;
	G4int particle2 = 0;
	hitContainer result {particle1,particle2};

	//Get how many times each particle hit the layer
	for (int i = 0;i < hitCount;i++) {

		if (result[0] == 1 && result[1] == 1)
			break;

		hit = (DevHit*)collection->GetHit(i);
		G4int whichParticle = hit->GetTrackID();
		if (whichParticle == 1) {
			result[0] = 1;
		} else if (whichParticle == 2) {
			result[1] = 1;
		}
	}
	return result;
}

DevRun::EventSuccess DevRun::classifyParticle(G4int particleID,hitContainer& bHits,hitContainer& cHits,hitContainer& dHits) {
	G4int staveB = bHits[particleID];
	G4int staveC = cHits[particleID];
	G4int staveD = dHits[particleID];
	G4int total = staveB + staveC + staveD;

	if ((total == 3) || (staveB && staveC)) {
		return GoodEvent;
	} else if (staveB && staveD) {
		return BadEvent;
	}else if (staveC && staveD) {
		return AlrightEvent;
	} else {
		return InvalidEvent;
	}

}

DevRun::EventSuccess DevRun::classifyEvent(G4VHitsCollection* dCol,G4VHitsCollection* bCol,G4VHitsCollection* cCol) {

	hitContainer bHits = doBothHit(bCol);
	hitContainer cHits = doBothHit(cCol);
	hitContainer dHits = doBothHit(dCol);

	DevRun::EventSuccess p1Level = classifyParticle(0,bHits,cHits,dHits);
	DevRun::EventSuccess p2Level = classifyParticle(1,bHits,cHits,dHits);

	if (p1Level == InvalidEvent || p2Level == InvalidEvent) {
		nInvalid++;
		return InvalidEvent;
	} else if (p1Level == BadEvent || p2Level == BadEvent) {
		nBad++;
		return BadEvent;
	} else if (p1Level == AlrightEvent || p2Level == AlrightEvent) {
		nAlright++;
		return AlrightEvent;
	} else if (p1Level == GoodEvent || p2Level == GoodEvent) {
		nGood++;
		return GoodEvent;
	}
	return InvalidEvent;

}

void DevRun::recordStaveData(const G4Event* anEvent,
		G4VHitsCollection* collection, G4int tupleID) {

	G4AnalysisManager* manager = G4AnalysisManager::Instance();
	//Flag to pick which data will be recorded
	G4bool errorFlag = false;	//Difference between exact* and digitised position
	G4bool positionFlag = true;	//Digitised position

	G4int hitCount = collection->GetSize();
	DevHit* hit = new DevHit();

	G4int p1Count = 0;
	G4int p2Count = 0;
	G4ThreeVector p1Pos;
	G4ThreeVector p1Ext;
	G4ThreeVector p2Pos;
	G4ThreeVector p2Ext;

	G4int eventID = anEvent->GetEventID();

	for (int i=0;i<hitCount;i++) {
		hit = (DevHit*)collection->GetHit(i);
		G4int trackID = hit->GetTrackID();
		G4ThreeVector position = hit->GetDigitisedPosition();
		G4ThreeVector exact = hit->GetExactPosition();

		if (trackID == 1) {
			p1Count++;
			p1Pos += position;
			p1Ext += exact;
		} else if (trackID == 2) {
			p2Count++;
			p2Pos += position;
			p2Ext += exact;
		}

	}

	if (p1Count == 0 || p2Count == 0)
		return;

	G4ThreeVector p1AvgPos = p1Pos/(G4double)p1Count;
	G4ThreeVector p2AvgPos = p2Pos/(G4double)p2Count;
	G4ThreeVector p1AvgExt = p1Ext/(G4double)p1Count;
	G4ThreeVector p2AvgExt = p2Ext/(G4double)p2Count;

	if (errorFlag) {
		G4ThreeVector p1Diff = p1AvgPos - p1AvgExt;
		G4ThreeVector p2Diff = p2AvgPos - p2AvgExt;
		manager->FillNtupleDColumn(tupleID,0,p1Diff.getX());
		manager->FillNtupleDColumn(tupleID,1,p1Diff.getY());
		manager->FillNtupleDColumn(tupleID,2,p1Diff.getZ());

		manager->FillNtupleDColumn(tupleID,3,p2Diff.getX());
		manager->FillNtupleDColumn(tupleID,4,p2Diff.getY());
		manager->FillNtupleDColumn(tupleID,5,p2Diff.getZ());

	}

	if (positionFlag) {
		manager->FillNtupleDColumn(tupleID,6,p1AvgPos.getX());
		manager->FillNtupleDColumn(tupleID,7,p1AvgPos.getY());
		manager->FillNtupleDColumn(tupleID,8,p1AvgPos.getZ());

		manager->FillNtupleDColumn(tupleID,9,p2AvgPos.getX());
		manager->FillNtupleDColumn(tupleID,10,p2AvgPos.getY());
		manager->FillNtupleDColumn(tupleID,11,p2AvgPos.getZ());
	}

	manager->FillNtupleIColumn(tupleID,12,eventID);
	manager->AddNtupleRow(tupleID);
}

void DevRun::recordData(const G4Event* anEvent,
		G4VHitsCollection* dCol,G4VHitsCollection* bCol,G4VHitsCollection* cCol) {

	recordStaveData(anEvent,bCol,0);
	recordStaveData(anEvent,cCol,1);
	recordStaveData(anEvent,dCol,2);
}

void DevRun::RecordEvent(const G4Event* anEvent) {
	//Gets hit collection for each layer
	G4String dName = "StaveDCollection";
	G4VHitsCollection* dHitsCol = getHitCollection(anEvent,dName);

	G4String bName = "StaveBCollection";
	G4VHitsCollection* bHitsCol = getHitCollection(anEvent,bName);

	G4String cName = "StaveCCollection";
	G4VHitsCollection* cHitsCol = getHitCollection(anEvent,cName);

	DevRun::EventSuccess outcome = classifyEvent(dHitsCol,bHitsCol,cHitsCol);

	if (outcome != InvalidEvent)
		recordData(anEvent,dHitsCol,bHitsCol,cHitsCol);

	G4Run::RecordEvent(anEvent);
}

void DevRun::Merge(const G4Run* aRun) {
	const DevRun* localRun = static_cast<const DevRun*>(aRun);
	nGood += localRun->nGood;
	nAlright += localRun->nAlright;
	nBad += localRun->nBad;
	nInvalid += localRun->nInvalid;
	G4Run::Merge(aRun);
}


