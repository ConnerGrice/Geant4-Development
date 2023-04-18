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
#include <g4root.hh>

#include <fstream>

#include "parameters.h"
#include "DevRun.h"

class DevRunAction: public G4UserRunAction {
public:
	DevRunAction();
	virtual ~DevRunAction();

public:
	G4Run* GenerateRun() override;
	void BeginOfRunAction(const G4Run*) override;
	void EndOfRunAction(const G4Run*) override;

public:
	//Creates tables where data will be stored
	void metricsTable(G4String name, G4int tupleID, G4AnalysisManager* manager);
	void energyTable(G4int tupleID,G4AnalysisManager* manager);

	//Prints efficiency values
	void printTypeEff(const G4Run* aRun);

private:
	G4int nGood,nAlright,nBad,nInvalid;

};

#endif /* CLASSES_DEVRUNACTION_H_ */
