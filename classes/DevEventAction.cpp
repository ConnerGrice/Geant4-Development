/*
 * DevEventAction.cpp
 *
 *  Created on: Mar 13, 2023
 *      Author: local1
 */

#include "DevEventAction.h"

DevEventAction::DevEventAction(DevRunAction* runAction) {
	rAction = runAction;
	manager = G4AnalysisManager::Instance();

}

DevEventAction::~DevEventAction() {
	delete G4AnalysisManager::Instance();
}

void DevEventAction::BeginOfEventAction(const G4Event*) {
	G4cout<<"EVENT START==================================="<<G4endl;

}

G4VHitsCollection* DevEventAction::getHitCollection(const G4Event* anEvent, G4String hcName) {

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

hitContainer DevEventAction::doBothHit(G4VHitsCollection* collection) {
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
			//particle1++;
		} else if (whichParticle == 2) {
			result[1] = 1;
			//particle2++;
		}
	}
	return result;
}

DevEventAction::EventSuccess DevEventAction::classifyParticle(G4int particleID,hitContainer bHits,hitContainer cHits,hitContainer dHits) {
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

void DevEventAction::classifyEvent(G4VHitsCollection* dCol,G4VHitsCollection* bCol,G4VHitsCollection* cCol) {

	hitContainer bHits = doBothHit(bCol);
	hitContainer cHits = doBothHit(cCol);
	hitContainer dHits = doBothHit(dCol);

	DevEventAction::EventSuccess p1Level = classifyParticle(0,bHits,cHits,dHits);
	DevEventAction::EventSuccess p2Level = classifyParticle(1,bHits,cHits,dHits);

	if (p1Level == InvalidEvent || p2Level == InvalidEvent) {
		rAction->addInvalid();
	} else if (p1Level == BadEvent || p2Level == BadEvent) {
		rAction->addBad();
	} else if (p1Level == AlrightEvent || p2Level == AlrightEvent) {
		rAction->addAlright();
	} else if (p1Level == GoodEvent || p2Level == GoodEvent) {
		rAction->addGood();
	}

}

void DevEventAction::sumNumOfHits(G4VHitsCollection* dCol,G4VHitsCollection* bCol,G4VHitsCollection* cCol) {
	G4int dHitCount = dCol->GetSize();
	rAction->addDHits(dHitCount);

	G4int bHitCount = bCol->GetSize();
	rAction->addBHits(bHitCount);

	G4int cHitCount = cCol->GetSize();
	rAction->addCHits(cHitCount);
}

void DevEventAction::fillError(const G4Event* anEvent,
		G4VHitsCollection* collection, G4int tupleID) {


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


void DevEventAction::fillMetrics(const G4Event* anEvent,
		G4VHitsCollection* dCol,G4VHitsCollection* bCol,G4VHitsCollection* cCol) {

	fillError(anEvent,bCol,0);

	fillError(anEvent,cCol,1);

	fillError(anEvent,dCol,2);
}

void DevEventAction::EndOfEventAction(const G4Event* anEvent) {
	//Gets hit collection for each layer
	G4String dName = "StaveDCollection";
	G4VHitsCollection* dHitsCol = getHitCollection(anEvent,dName);

	G4String bName = "StaveBCollection";
	G4VHitsCollection* bHitsCol = getHitCollection(anEvent,bName);

	G4String cName = "StaveCCollection";
	G4VHitsCollection* cHitsCol = getHitCollection(anEvent,cName);

	//Sums the number of hits for each layer
	sumNumOfHits(dHitsCol,bHitsCol,cHitsCol);

	classifyEvent(dHitsCol,bHitsCol,cHitsCol);

	//rAction->printCount();
	//rAction->printType();

	fillMetrics(anEvent,dHitsCol,bHitsCol,cHitsCol);

	//Counter for the number of events
	G4cout<<"Event: "<<anEvent->GetEventID()<<"/100240"<<G4endl;

	G4cout<<"EVENT END==========================================="<<G4endl;


}

