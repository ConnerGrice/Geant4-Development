/*
 * DevDetectorConstruction.cpp
 *
 *  Created on: Feb 24, 2023
 *      Author: local1
 */

#include "DevDetectorConstruction.h"

DevDetectorConstruction::DevDetectorConstruction(){

	DefineMaterials();
}

DevDetectorConstruction::~DevDetectorConstruction() {
	// TODO Auto-generated destructor stub
}

void DevDetectorConstruction::DefineMaterials(){
	//Defining materials
	G4NistManager* pNist = G4NistManager::Instance();
	pWorldMat = pNist->FindOrBuildMaterial("G4_Galactic");
	pPlateMat = pNist->FindOrBuildMaterial("G4_Si");
}

void DevDetectorConstruction::ConstructStave(G4int numOfHCIs,G4String name) {
	const G4double HCIWidth = 15.0*mm;
	const G4double plateLength = 160.0*mm;
	const G4double plateThickness = 3*mm;
	const G4double angle = M_PI/3.0;
	const G4int sides = 6;

	const G4double plateWidth = getPlateWidth(numOfHCIs,HCIWidth);
	const G4double plateRadius = getPlateRadius(plateWidth,angle,plateThickness);

	//Initial position of Stave section
	G4ThreeVector platePosition = G4ThreeVector(0*mm,plateRadius,0*mm);
	G4RotationMatrix* rotation = new G4RotationMatrix(0,0,0);

	//Initialise assembly
	G4AssemblyVolume* pStave = new G4AssemblyVolume();
	G4ThreeVector translation;
	G4double x;
	G4double y;

	//Defining a single cold plate.
	G4Box* pSolidPlate = new G4Box(name+"S",plateWidth,plateThickness,plateLength);
	G4LogicalVolume* pLogicalPlate = new G4LogicalVolume(pSolidPlate,pPlateMat,name+"L");

	//Add geometery to assembly
	pStave->AddPlacedVolume(pLogicalPlate,platePosition,rotation);

	//Moves each cold plate to the correct position
	for (int i = 0; i < sides; i++) {
		pStave->MakeImprint(pLogicalWorld,translation,rotation);
		rotation->rotateZ(angle);
		x = platePosition.getX()*sin(i*angle);
		y = platePosition.getX()*cos(i*angle);
		translation = G4ThreeVector(x,y,0);
	}

}

G4VPhysicalVolume* DevDetectorConstruction::Construct(){
	//Defining world volume
	G4Box* pSolidWorld = new G4Box("WorldS",500*cm,500*cm,500*cm);
	pLogicalWorld = new G4LogicalVolume(pSolidWorld,pWorldMat,"WorldL");
	G4PVPlacement* pPhysicalWorld = new G4PVPlacement(0,G4ThreeVector(0,0,0),pLogicalWorld,"WorldP",0,false,0,true);

	//Generate staves
	ConstructStave(3,"StaveC");
	ConstructStave(4,"StaveD");

	return pPhysicalWorld;
}

