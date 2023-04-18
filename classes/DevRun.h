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
#include <g4root.hh>

#include "DevHit.h"

//Will hold the number of layers each particle hits
typedef std::vector<G4int> hitContainer;

class DevRun: public G4Run {
public:
	DevRun();
	~DevRun();

	//Options for success level
	enum EventSuccess {
		GoodEvent,
		AlrightEvent,
		BadEvent,
		InvalidEvent
	};

	//Processes information from an event and iterates to the next
	void RecordEvent(const G4Event* anEvent) override;

	//Combines runs from different worker threads
	void Merge(const G4Run* aRun) override;

	//Event classifier counters
	inline void addGood() { nGood++; };
	inline G4int getGood() const { return nGood; };

	inline void addAlright() { nAlright++; };
	inline G4int getAlright() const { return nAlright; };

	inline void addBad() { nBad++; };
	inline G4int getBad() const { return nBad; };

	inline void addInvalid() { nInvalid++; };
	inline G4int getInvalid() const { return nInvalid; };

	void setp1Energy(G4double e) { p1Energy = e; };
	void setp2Energy(G4double e) { p2Energy = e; };

private:
	//Fetches a hit collection by name
	G4VHitsCollection* getHitCollection(const G4Event* anEvent,G4String hcName);

	//Records if each particle came into contact with a stave layer
	hitContainer doBothHit(G4VHitsCollection* collection);

	//Determines the level of success of individual particle
	EventSuccess classifyParticle(G4int particleID,hitContainer& bHits,hitContainer& cHits,hitContainer& dHits);

	//Determines the level of success of the entire event
	EventSuccess classifyEvent(G4VHitsCollection* dCol,
			G4VHitsCollection* bCol,
			G4VHitsCollection* cCol);

	//Records data from a layer
	void recordStaveData(const G4Event* anEvent,
			G4VHitsCollection* collection, G4int tupleID);

	void recordCalifaData(const G4Event* anEvent,G4int tupleID);

	//Records data for all layers
	void recordData(const G4Event* anEvent,
			G4VHitsCollection* dCol,
			G4VHitsCollection* bCol,
			G4VHitsCollection* cCol);


private:
	G4int nGood = 0;
	G4int nAlright = 0;
	G4int nBad = 0;
	G4int nInvalid = 0;

	G4double p1Energy = 0;
	G4double p2Energy = 0;
};

#endif /* GEANT4_DEVELOPMENT_CLASSES_DEVRUN_H_ */
