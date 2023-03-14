/*
 * DevPhysicsList.cpp
 *
 *  Created on: Feb 24, 2023
 *      Author: local1
 */
#include "DevPhysicsList.h"

#include <G4BaryonConstructor.hh>
#include <G4LeptonConstructor.hh>
#include <G4MesonConstructor.hh>
#include <G4BosonConstructor.hh>
#include <G4ShortLivedConstructor.hh>
#include <G4IonConstructor.hh>

#include <G4VPhysicsConstructor.hh>
#include <G4EmStandardPhysics.hh>


DevPhysicsList::DevPhysicsList() {
}

DevPhysicsList::~DevPhysicsList() {
}

void DevPhysicsList::ConstructParticle(){

	G4BaryonConstructor bConstructor;
	bConstructor.ConstructParticle();

	G4LeptonConstructor lConstructor;
	lConstructor.ConstructParticle();

	G4MesonConstructor mConstructor;
	mConstructor.ConstructParticle();

	G4BosonConstructor bsConstructor;
	bsConstructor.ConstructParticle();

	G4ShortLivedConstructor slConstructor;
	slConstructor.ConstructParticle();

	G4IonConstructor iConstructor;
	iConstructor.ConstructParticle();

}

void DevPhysicsList::ConstructProcess(){
	AddTransportation();

	G4VPhysicsConstructor* emPhys = new G4EmStandardPhysics();
	emPhys->ConstructProcess();

}
