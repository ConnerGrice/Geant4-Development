/*
 * DevPrimaryGeneratorAction.cpp
 *
 *  Created on: Feb 24, 2023
 *      Author: local1
 */

#include "DevPrimaryGeneratorAction.h"

DevPrimaryGeneratorAction::DevPrimaryGeneratorAction() {
	G4int numOfParticles = 1;
	pParticleGun = new G4ParticleGun(numOfParticles);
}

DevPrimaryGeneratorAction::~DevPrimaryGeneratorAction() {
	delete pParticleGun;
}

void DevPrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent){
	pParticleGun->SetParticleDefinition(G4Proton::Definition());

	G4ThreeVector origin = G4ThreeVector(0,0,0);
	pParticleGun->SetParticlePosition(origin);

	G4double e = 1000*MeV;
	pParticleGun->SetParticleEnergy(e);

	G4ParticleMomentum direction = G4ParticleMomentum(1,0.3,0);
	pParticleGun->SetParticleMomentumDirection(direction);

	pParticleGun->GeneratePrimaryVertex(anEvent);
}

