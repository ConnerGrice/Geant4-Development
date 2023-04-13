/*
 * DevRun.h
 *
 *  Created on: Apr 13, 2023
 *      Author: local1
 */

#ifndef GEANT4_DEVELOPMENT_CLASSES_DEVRUN_H_
#define GEANT4_DEVELOPMENT_CLASSES_DEVRUN_H_

#include <G4Run.hh>

#include <G4VHitsCollection.hh>
#include <G4HCofThisEvent.hh>
#include <G4Event.hh>
#include <G4SDManager.hh>

#include "DevHit.h"

typedef std::vector<G4int> hitContainer;

class DevRun: public G4Run {
public:
	DevRun();
	~DevRun();

	enum EventSuccess {
		GoodEvent,
		AlrightEvent,
		BadEvent,
		InvalidEvent
	};


	void RecordEvent(const G4Event* anEvent) override;
	void Merge(const G4Run* aRun) override;

	inline G4int getGood() const { return nGood; };
	inline G4int getAlright() const { return nAlright; };
	inline G4int getBad() const { return nBad; };
	inline G4int getInvalid() const { return nInvalid; };

	//Level of success classifiers
	inline void addGood() { nGood++; };
	inline void addAlright() { nAlright++; };
	inline void addBad() { nBad++; };
	inline void addInvalid() { nInvalid++; };
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

private:
	G4int nGood = 0;
	G4int nAlright = 0;
	G4int nBad = 0;
	G4int nInvalid = 0;
};

#endif /* GEANT4_DEVELOPMENT_CLASSES_DEVRUN_H_ */
