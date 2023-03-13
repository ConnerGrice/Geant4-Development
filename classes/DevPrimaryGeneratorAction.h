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
#include <G4RootAnalysisReader.hh>
#include <g4root.hh>

class DevPrimaryGeneratorAction: public G4VUserPrimaryGeneratorAction {
public:
	DevPrimaryGeneratorAction();
	virtual ~DevPrimaryGeneratorAction();

public:
	void GeneratePrimaries(G4Event* anEvent) override;

private:
	G4ParticleGun* pParticleGun;
};

#endif /* INCLUDE_DEVPRIMARYGENERATORACTION_H_ */
