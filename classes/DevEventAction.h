/*
 * DevEventAction.h
 *
 *  Created on: Mar 13, 2023
 *      Author: local1
 */

#ifndef CLASSES_DEVEVENTACTION_H_
#define CLASSES_DEVEVENTACTION_H_

#include <G4UserEventAction.hh>

#include <G4SDManager.hh>
#include <G4HCofThisEvent.hh>
#include <G4Event.hh>

#include "DevRunAction.h"
#include "DevHit.h"

class DevEventAction: public G4UserEventAction {
public:
	DevEventAction(DevRunAction* runAction);
	virtual ~DevEventAction();

public:
	void BeginOfEventAction(const G4Event*) override;
	void EndOfEventAction(const G4Event* anEvent) override;

private:
	//Fetches a hit collection by name
	G4VHitsCollection* getHitCollection(const G4Event* anEvent,G4String hcName);

	G4bool doBothHit(G4VHitsCollection* collection);

	void classifyEvent(G4VHitsCollection* dCol,G4VHitsCollection* bCol,G4VHitsCollection* cCol);

	//Sums how many hits are detected by each layer
	void sumNumOfHits(G4VHitsCollection* dCol,G4VHitsCollection* bCol,G4VHitsCollection* cCol);

	DevRunAction* rAction;
};

#endif /* CLASSES_DEVEVENTACTION_H_ */
