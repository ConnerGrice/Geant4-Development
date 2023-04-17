/*
 * Stave.cpp
 *
 *  Created on: Apr 14, 2023
 *      Author: local1
 */

#include "Stave.h"

Stave::Stave(G4int numOfHICs,HICUnit unitConstructor,G4LogicalVolume* root):
nHICs(numOfHICs), pWorldVol(root), HIC(unitConstructor) {
	pEmpty = Materials::space;
	pPlateMat = Materials::carbon;
	pFleeceMat = Materials::carbon;

	staveWidth = getWidth();
	staveRadius = getRadius();

	pMother = defineMother(false);

}

G4LogicalVolume* Stave::defineMother(G4bool visibility) {
	G4Box* motherSolid = new G4Box("StaveS",staveWidth/2.0,staveThickness/2.0,staveLength/2.0);
	G4LogicalVolume* motherLogical = new G4LogicalVolume(motherSolid,pEmpty,"StaveL");

	motherLogical->SetVisAttributes(new G4VisAttributes(visibility));
	return motherLogical;


}

G4LogicalVolume* Stave::definePlate(G4bool visibility) {
	G4Box* plateSolid = new G4Box("PlateS",staveWidth/2.0,plateThickness/2.0,staveLength/2.0);
	G4LogicalVolume* plateLogical = new G4LogicalVolume(plateSolid,pPlateMat,"PlateL");

	plateLogical->SetVisAttributes(new G4VisAttributes(visibility));
	return plateLogical;
}

G4LogicalVolume* Stave::defineFleece(G4bool visibility) {
	G4Box* fleeceSolid = new G4Box("FleeceS",staveWidth/2.0,fleeceThickness/2.0,staveLength/2.0);
	G4LogicalVolume* fleeceLogical = new G4LogicalVolume(fleeceSolid,pFleeceMat,"FleeceL");

	fleeceLogical->SetVisAttributes(new G4VisAttributes(visibility));
	return fleeceLogical;
}

void Stave::buildPlate(G4bool coldVis, G4bool fleeceVis) {
	G4LogicalVolume* coldPlate = definePlate(coldVis);
	G4LogicalVolume* fleece = defineFleece(fleeceVis);

	G4double offset = plateThickness/2.0 + fleeceThickness/2.0;
	new G4PVPlacement(0,G4ThreeVector(0,offset,0),fleece,"FleeceTop",pMother,false,0);
	new G4PVPlacement(0,G4ThreeVector(0,0,0),coldPlate,"ColdPlate",pMother,false,1);
	new G4PVPlacement(0,G4ThreeVector(0,-offset,0),fleece,"FleeceBottom",pMother,false,2);
}

void Stave::buildStave() {

	buildPlate(true,true);

	G4double HICWidth = HIC.getWidth();
	//G4ThreeVector HICPos = G4ThreeVector(-(HICWidth*nHICs/2.0) - HICWidth/2.0,0,HCIZ);
	G4ThreeVector HICPos = G4ThreeVector(-staveWidth/2.0 + PADDING + HICWidth/2.0,0,HCIZ);
	G4RotationMatrix* HICRot = new G4RotationMatrix;
	G4double yShift = plateThickness/2.0 + fleeceThickness + (HIC.getThickness()/2.0);

	G4LogicalVolume* HICMother = HIC.getMother();

	for (int i = 0;i < nHICs;i++) {
		if (i%2 == 0) {
			HICPos.setY(yShift);
			HICRot = new G4RotationMatrix(0,0,0);
		} else {
			HICPos.setY(-yShift);
			HICRot = new G4RotationMatrix(0,0,M_PI);
		}

		new G4PVPlacement(HICRot,HICPos,HICMother,"HICUnit",pMother,false, i);
		HICPos += G4ThreeVector(HICWidth,0,0);
	}
}

void Stave::placeStaves() {
	buildStave();

	G4RotationMatrix* staveRot;
	G4ThreeVector stavePos = G4ThreeVector(staveRadius,0,0);

	for (int i = 0; i < nSides; i++) {
		G4cout<<"HER"<<G4endl;
		staveRot = new G4RotationMatrix(0,0,(M_PI/2.0) + (angle*i));
		new G4PVPlacement(staveRot,stavePos,pMother, "Stave" + std::to_string(nHICs),pWorldVol,false,i);
		stavePos.rotateZ(angle);
	}
}

