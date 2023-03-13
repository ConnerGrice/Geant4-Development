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

#include "DevRunAction.h"
#include "DevHit.h"

class DevEventAction: public G4UserEventAction {
public:
	DevEventAction(DevRunAction* runAction);
	virtual ~DevEventAction();

public:
	void BeginOfEventAction(G4Event* anEvent) override;
	void EndOfEventAction(G4Event* anEvent) override;

private:
	DevRunAction* rAction;
};

#endif /* CLASSES_DEVEVENTACTION_H_ */
