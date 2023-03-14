/*
 * DevRunAction.h
 *
 *  Created on: Mar 13, 2023
 *      Author: local1
 */

#ifndef CLASSES_DEVRUNACTION_H_
#define CLASSES_DEVRUNACTION_H_

#include <G4UserRunAction.hh>

#include <G4HCtable.hh>
#include <G4Run.hh>

class DevRunAction: public G4UserRunAction {
public:
	DevRunAction();
	virtual ~DevRunAction();

public:
	void BeginOfRunAction(const G4Run*) override;
	void EndOfRunAction(const G4Run*) override;

public:
	//Prints to hit counts for each layer
	void printCount();

	//Hit counter for each layer
	inline void addBHits(G4int hits) { bTotal += hits; };
	inline void addCHits(G4int hits) { cTotal += hits; };
	inline void addDHits(G4int hits) { dTotal += hits; };

private:
	G4int bTotal,cTotal,dTotal;

};

#endif /* CLASSES_DEVRUNACTION_H_ */
