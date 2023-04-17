/*
 * Materials.h
 *
 *  Created on: Apr 17, 2023
 *      Author: local1
 */

#ifndef GEANT4_DEVELOPMENT_CLASSES_MATERIALS_H_
#define GEANT4_DEVELOPMENT_CLASSES_MATERIALS_H_

#include <G4NistManager.hh>

namespace {
struct Materials {
	static G4Material* space;
	static G4Material* carbon;
	static G4Material* silicon;
	static G4Material* aluminum;
	static G4Material* kapton;
	static G4Material* lHydrogen;

};

G4NistManager* pNist = G4NistManager::Instance();
G4Material* Materials::space = pNist->FindOrBuildMaterial("G4_Galactic");
G4Material* Materials::carbon = pNist->FindOrBuildMaterial("G4_C");
G4Material* Materials::silicon = pNist->FindOrBuildMaterial("G4_Si");
G4Material* Materials::aluminum = pNist->FindOrBuildMaterial("G4_Al");
G4Material* Materials::kapton = pNist->FindOrBuildMaterial("G4_KAPTON");
G4Material* Materials::lHydrogen = pNist->FindOrBuildMaterial("G4_lH2");
}

#endif /* GEANT4_DEVELOPMENT_CLASSES_MATERIALS_H_ */
