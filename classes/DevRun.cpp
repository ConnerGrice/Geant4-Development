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

}

void DevRun::RecordEvent(const G4Event* anEvent) {
	//Gets hit collection for each layer
	G4String dName = "StaveDCollection";
	G4VHitsCollection* dHitsCol = getHitCollection(anEvent,dName);

	G4String bName = "StaveBCollection";
	G4VHitsCollection* bHitsCol = getHitCollection(anEvent,bName);

	G4String cName = "StaveCCollection";
	G4VHitsCollection* cHitsCol = getHitCollection(anEvent,cName);

	classifyEvent(dHitsCol,bHitsCol,cHitsCol);

	G4Run::RecordEvent(anEvent);
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

void DevRun::classifyEvent(G4VHitsCollection* dCol,G4VHitsCollection* bCol,G4VHitsCollection* cCol) {

	hitContainer bHits = doBothHit(bCol);
	hitContainer cHits = doBothHit(cCol);
	hitContainer dHits = doBothHit(dCol);

	DevRun::EventSuccess p1Level = classifyParticle(0,bHits,cHits,dHits);
	DevRun::EventSuccess p2Level = classifyParticle(1,bHits,cHits,dHits);

	if (p1Level == InvalidEvent || p2Level == InvalidEvent)
		nInvalid++;
	else if (p1Level == BadEvent || p2Level == BadEvent)
		nBad++;
	else if (p1Level == AlrightEvent || p2Level == AlrightEvent)
		nAlright++;
	else if (p1Level == GoodEvent || p2Level == GoodEvent)
		nGood++;


}

void DevRun::Merge(const G4Run* aRun) {
	const DevRun* localRun = static_cast<const DevRun*>(aRun);
	nGood += localRun->nGood;
	nAlright += localRun->nAlright;
	nBad += localRun->nBad;
	nInvalid += localRun->nInvalid;
	G4Run::Merge(aRun);
}



