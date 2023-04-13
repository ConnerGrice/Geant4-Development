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
#include <G4RunManager.hh>
#include <G4Event.hh>
#include <g4root.hh>

#include <vector>

#include "DevRunAction.h"
#include "DevHit.h"
#include "DevRun.h"

typedef std::vector<G4int> hitContainer;

class DevEventAction: public G4UserEventAction {
public:
	DevEventAction(DevRunAction* runAction);
	virtual ~DevEventAction();


	enum EventSuccess {
		GoodEvent,
		AlrightEvent,
		BadEvent,
		InvalidEvent
	};

public:
	void BeginOfEventAction(const G4Event*) override;
	void EndOfEventAction(const G4Event* anEvent) override;

private:


	//Fetches a hit collection by name
	G4VHitsCollection* getHitCollection(const G4Event* anEvent,G4String hcName);

	//Makes sure each particle hits a detector only once
	hitContainer doBothHit(G4VHitsCollection* collection);

	EventSuccess classifyParticle(G4int particleID,hitContainer& bHits,hitContainer& cHits,hitContainer& dHits);

	//Classifies if event was good, alright, or bad
	void classifyEvent(G4VHitsCollection* dCol,
			G4VHitsCollection* bCol,
			G4VHitsCollection* cCol);

	//Sums how many hits are detected by each layer
	void sumNumOfHits(G4VHitsCollection* dCol,
			G4VHitsCollection* bCol,
			G4VHitsCollection* cCol);

	//Fills given table with digitisation error data
	void fillError(const G4Event* anEvent,
			G4VHitsCollection* collection,
			G4int tupleID);


	//Fills table with specified data
	void fillMetrics(const G4Event* anEvent,
			G4VHitsCollection* dCol,
			G4VHitsCollection* bCol,
			G4VHitsCollection* cCol);


	DevRunAction* rAction;
	G4AnalysisManager* manager;
};


#endif /* CLASSES_DEVEVENTACTION_H_ */
