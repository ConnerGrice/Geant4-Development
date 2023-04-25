/*
 * DevPrimaryGeneratorAction.cpp
 *
 *  Created on: Feb 24, 2023
 *      Author: local1
 */

#include "DevPrimaryGeneratorAction.h"

DevPrimaryGeneratorAction::DevPrimaryGeneratorAction(): e1(0),e2(0) {
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
	reader->SetNtupleDColumn(ntupleID,"E1",e1);
	reader->SetNtupleDColumn(ntupleID,"E2",e2);
	reader->SetNtupleDColumn(ntupleID,"theta_1",t1);
	reader->SetNtupleDColumn(ntupleID,"theta_2",t2);
	reader->SetNtupleDColumn(ntupleID,"phi_1",ph1);
	reader->SetNtupleDColumn(ntupleID,"phi_2",ph2);

	//Get values
	reader->GetNtupleRow();

	//Set reaction origin point
	const G4double HCIHalfLength = 135.6*mm;
	pParticleGun->SetParticlePosition(G4ThreeVector(x,y,(z/10.0)-HCIHalfLength+HCIZ));

	//Define and shoot outgoing particle 1
	pParticleGun->SetParticleMomentum(G4ParticleMomentum(px1,py1,pz1));
	G4cout<<"P1: "<<pParticleGun->GetParticleEnergy()<<G4endl;
	pParticleGun->GeneratePrimaryVertex(anEvent);

	//Define and shoot outgoing particle 2
	pParticleGun->SetParticleMomentum(G4ParticleMomentum(px2,py2,pz2));
	G4cout<<"P2: "<<pParticleGun->GetParticleEnergy()<<G4endl;
	pParticleGun->GeneratePrimaryVertex(anEvent);

	G4AnalysisManager* manager = G4AnalysisManager::Instance();
	manager->FillNtupleDColumn(3,2,e1);
	manager->FillNtupleDColumn(3,3,e2);
	manager->FillNtupleDColumn(3,4,t1);
	manager->FillNtupleDColumn(3,5,t2);
	manager->FillNtupleDColumn(3,6,ph1);
	manager->FillNtupleDColumn(3,7,ph2);
	//manager->AddNtupleRow(3);
/*
	pParticleGun->SetParticleDefinition(G4Proton::Definition());

	G4ThreeVector origin = G4ThreeVector(0,0,0);
	pParticleGun->SetParticlePosition(origin);

	G4double e = 100*GeV;
	pParticleGun->SetParticleEnergy(e);

	G4ParticleMomentum direction = G4ParticleMomentum(1,-0.3,0);
	//direction.rotateZ(5*M_PI/3);
	pParticleGun->SetParticleMomentumDirection(direction);
	pParticleGun->GeneratePrimaryVertex(anEvent);
*/
	/*
	G4ThreeVector origin = G4ThreeVector(57.1*mm,0,0);
	pParticleGun->SetParticlePosition(origin);

	G4double e = 100*GeV;
	pParticleGun->SetParticleEnergy(e);

	G4ParticleMomentum direction = G4ParticleMomentum(0,0.3,1);
	//direction.rotateZ(5*M_PI/3);
	pParticleGun->SetParticleMomentumDirection(direction);
	pParticleGun->GeneratePrimaryVertex(anEvent);
*/
/*
	direction.rotateZ(2*M_PI/3);
	pParticleGun->SetParticleMomentumDirection(direction);
	pParticleGun->GeneratePrimaryVertex(anEvent);

	direction.rotateZ(M_PI/3);
	pParticleGun->SetParticleMomentumDirection(direction);
	pParticleGun->GeneratePrimaryVertex(anEvent);


	direction.rotateZ(M_PI/3);
	pParticleGun->SetParticleMomentumDirection(direction);
	pParticleGun->GeneratePrimaryVertex(anEvent);

	direction.rotateZ(M_PI/3);
	pParticleGun->SetParticleMomentumDirection(direction);
	pParticleGun->GeneratePrimaryVertex(anEvent);

	direction.rotateZ(M_PI/3);
	pParticleGun->SetParticleMomentumDirection(direction);
	pParticleGun->GeneratePrimaryVertex(anEvent);
*/


}

