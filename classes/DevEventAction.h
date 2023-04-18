/*
 * DevEventAction.h
 *
 *  Created on: Mar 13, 2023
 *      Author: local1
 */

#ifndef CLASSES_DEVEVENTACTION_H_
#define CLASSES_DEVEVENTACTION_H_

#include <G4UserEventAction.hh>

#include <G4Event.hh>

class DevEventAction: public G4UserEventAction {
public:
	DevEventAction();
	virtual ~DevEventAction();

public:
	void BeginOfEventAction(const G4Event*) override;
	void EndOfEventAction(const G4Event* anEvent) override;

	void setp1Energy(G4double e) { p1Energy = e; };
	void setp2Energy(G4double e) { p2Energy = e; };

private:
	G4double p1Energy;
	G4double p2Energy;
};

#endif /* CLASSES_DEVEVENTACTION_H_ */
