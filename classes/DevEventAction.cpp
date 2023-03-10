/*
 * DevEventAction.cpp
 *
 *  Created on: Mar 13, 2023
 *      Author: local1
 */

#include "DevEventAction.h"

DevEventAction::DevEventAction(DevRunAction* runAction) {
	rAction = runAction;

}

DevEventAction::~DevEventAction() {
}

void DevEventAction::BeginOfEventAction(const G4Event*) {

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

G4bool DevEventAction::doBothHit(G4VHitsCollection* collection) {
	G4int hitCount = collection->GetSize();

	DevHit* hit = new DevHit();
	G4int particle1 = 0;
	G4int particle2 = 0;

	//Get how many times each particle hit the layer
	for (int i = 0;i < hitCount;i++) {
		hit = (collection*)[i];
		G4int whichParticle = hit->GetTrackID();
		if (whichParticle == 1) {
			particle1++;
		} else if (whichParticle == 2) {
			particle2++;
		}
	}

	//Checks both particles only hit the layer once
	if (particle1 == 1 && particle2 == 1) {
		return true;
	}

	return false;
}

void DevEventAction::classifyEvent(G4VHitsCollection* dCol,G4VHitsCollection* bCol,G4VHitsCollection* cCol) {
	G4bool dGood = doBothHit(dCol);
	G4bool bGood = doBothHit(bCol);
	G4bool cGood = doBothHit(cCol);

	if (cGood && bGood) {
		rAction->addGood();
	} else if (cGood && dGood) {
		rAction->addAlright();
	} else if (bGood && dGood) {
		rAction->addBad();
	} else {
		rAction->addInvalid();
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

void DevEventAction::EndOfEventAction(const G4Event* anEvent) {
	// TODO Decide if an event is good, alright,bad or invalid

	//Counter for the number of events
	G4cout<<"========================================="<<G4endl;
	G4cout<<"Event: "<<anEvent->GetEventID()<<"/100240"<<G4endl;

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


	rAction->printCount();


}

