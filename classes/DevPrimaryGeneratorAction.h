/*
 * DevPrimaryGeneratorAction.h
 *
 *  Created on: Feb 24, 2023
 *      Author: local1
 */

#ifndef INCLUDE_DEVPRIMARYGENERATORACTION_H_
#define INCLUDE_DEVPRIMARYGENERATORACTION_H_

#include <G4VUserPrimaryGeneratorAction.hh>

#include <G4ParticleGun.hh>
#include <G4Proton.hh>
#include <G4SystemOfUnits.hh>
//#include <G4RootAnalysisReader.hh>
#include <g4root.hh>

#include "parameters.h"

class DevPrimaryGeneratorAction: public G4VUserPrimaryGeneratorAction {
public:
	DevPrimaryGeneratorAction();
	virtual ~DevPrimaryGeneratorAction();

public:
	void GeneratePrimaries(G4Event* anEvent) override;

	G4double getE1() const { return e1; };
	G4double getE2() const { return e2; };
	G4double getT1() const { return t1; };
	G4double getT2() const { return t2; };

private:
	G4ParticleGun* pParticleGun;
	G4double e1,e2,t1,t2,ph2,ph1;
};

#endif /* INCLUDE_DEVPRIMARYGENERATORACTION_H_ */
