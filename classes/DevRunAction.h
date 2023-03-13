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
	void EndOfRunAction(const G4Run* aRun) override;
};

#endif /* CLASSES_DEVRUNACTION_H_ */
