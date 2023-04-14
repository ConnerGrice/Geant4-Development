/*
 * Stave.cpp
 *
 *  Created on: Apr 14, 2023
 *      Author: local1
 */

#include "Stave.h"

Stave::Stave(G4int numOfHICs,G4LogicalVolume* root): nHICs(numOfHICs), pWorldVol(root), unitConstructor(root) {
	//Gets material definitions
	G4NistManager* pNist = G4NistManager::Instance();
	G4Material* space = pNist->FindOrBuildMaterial("G4_Galactic");
	G4Material* C = pNist->FindOrBuildMaterial("G4_C");

	pEmpty = space;
	pPlateMat = C;
	pFleeceMat = C;

	staveWidth = getWidth();
	staveRadius = getRadius();

	pMother = defineMother(true);

}

G4LogicalVolume* Stave::defineMother(G4bool visibility) {
	G4Box* motherSolid = new G4Box("StaveS",staveWidth/2.0,staveThickness/2.0,staveLength/2.0);
	G4LogicalVolume* motherLogical = new G4LogicalVolume(motherSolid,pEmpty,"StaveL");

	motherLogical->SetVisAttributes(new G4VisAttributes(visibility));
	return motherLogical;


}

void Stave::buildStave() {
	//unitConstructor.placeUnit(G4ThreeVector(0,0,0));
	new G4PVPlacement(0,G4ThreeVector(0,0,0),pMother,"Stave",pWorldVol,false,0);
}

