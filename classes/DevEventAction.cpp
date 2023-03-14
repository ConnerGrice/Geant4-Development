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
	G4SDManager* senDetManager = G4SDManager::GetSDMpointer();

	G4HCofThisEvent* hce = anEvent->GetHCofThisEvent();

	if (!hce) {
		G4ExceptionDescription msg;
		msg<<"No hits collection of this event found."<<G4endl;
		G4Exception("EventAction::EndOfEventAction()",
				"Code001", JustWarning,msg);
		return nullptr;
	}

	G4int hcID = senDetManager->GetCollectionID(hcName);

	auto hc = hce->GetHC(hcID);

	if (!hc) {
		G4ExceptionDescription msg;
		msg<<"Hits collection "<<hcID<<" of This event not found." <<G4endl;
		G4Exception("EventAction::EndOfEventAction()",
				"Code001", JustWarning, msg);
	}
	return hc;
}

G4int DevEventAction::getTotalHits(const G4Event* anEvent,G4String hcName) {
	auto hc = getHitCollection(anEvent,hcName);
	G4int nHits = hc->GetSize();
	return nHits;
}

void DevEventAction::EndOfEventAction(const G4Event* anEvent) {
	G4cout<<"Event: "<<anEvent->GetEventID()<<"/100240"<<G4endl;

	G4String cName = "StaveCCollection";
	G4int cHits = getTotalHits(anEvent,cName);
	rAction->addCHits(cHits);

	G4String dName = "StaveDCollection";
	G4int dHits = getTotalHits(anEvent,dName);
	rAction->addDHits(dHits);

	G4String bName = "StaveBCollection";
	G4int bHits = getTotalHits(anEvent,bName);
	rAction->addBHits(bHits);

	rAction->printCount();


}

