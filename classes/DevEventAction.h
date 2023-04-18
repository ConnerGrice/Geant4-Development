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
};

#endif /* CLASSES_DEVEVENTACTION_H_ */
