/*
 * DevPrimaryGeneratorAction.cpp
 *
 *  Created on: Feb 24, 2023
 *      Author: local1
 */

#include "DevPrimaryGeneratorAction.h"

DevPrimaryGeneratorAction::DevPrimaryGeneratorAction() {
	//Default number of particles shot with each "GeneratePrimaryVertex" call
	G4int numOfParticles = 1;
	pParticleGun = new G4ParticleGun(numOfParticles);
}

DevPrimaryGeneratorAction::~DevPrimaryGeneratorAction() {
	delete pParticleGun;
}

void DevPrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent){
	//Setting the values for the particle gun
	pParticleGun->SetParticleDefinition(G4Proton::Definition());

	//Create analysis reader to read data file
	G4AnalysisReader* reader = G4AnalysisReader::Instance();
	reader->SetVerboseLevel(1);

	//Read an individual row of the "Particles" tree in the "quasi.root" file
	G4int ntupleID = reader->GetNtuple("Particles","../quasi.root");
	//G4cout<<"ID: "<<ntupleID<<G4endl;

	G4double x,y,z,px1,py1,pz1,px2,py2,pz2;

	//Pick out the wanted values in that row
	reader->SetNtupleDColumn(ntupleID,"Xint",x);
	reader->SetNtupleDColumn(ntupleID,"Yint",y);
	reader->SetNtupleDColumn(ntupleID,"Zint",z);
	reader->SetNtupleDColumn(ntupleID,"P1x",px1);
	reader->SetNtupleDColumn(ntupleID,"P1y",py1);
	reader->SetNtupleDColumn(ntupleID,"P1z",pz1);
	reader->SetNtupleDColumn(ntupleID,"P2x",px2);
	reader->SetNtupleDColumn(ntupleID,"P2y",py2);
	reader->SetNtupleDColumn(ntupleID,"P2z",pz2);

	//Get values
	reader->GetNtupleRow();
/*
	//Set reaction origin point
	const G4double HCIHalfLength = 135.6*mm;
	pParticleGun->SetParticlePosition(G4ThreeVector(x,y,(z/10.0)-HCIHalfLength+HCIZ));

	//Define and shoot outgoing particle 1
	pParticleGun->SetParticleMomentum(G4ParticleMomentum(px1,py1,pz1));
	pParticleGun->GeneratePrimaryVertex(anEvent);

	//Define and shoot outgoing particle 2
	pParticleGun->SetParticleMomentum(G4ParticleMomentum(px2,py2,pz2));
	pParticleGun->GeneratePrimaryVertex(anEvent);

*/
	pParticleGun->SetParticleDefinition(G4Proton::Definition());

	G4ThreeVector origin = G4ThreeVector(0,0,0);
	pParticleGun->SetParticlePosition(origin);

	G4double e = 100*GeV;
	pParticleGun->SetParticleEnergy(e);

	G4ParticleMomentum direction = G4ParticleMomentum(1,0,0);
	pParticleGun->SetParticleMomentumDirection(direction);

	pParticleGun->GeneratePrimaryVertex(anEvent);

}

