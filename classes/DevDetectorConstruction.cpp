/*
 * DevDetectorConstruction.cpp
 *
 *  Created on: Feb 24, 2023
 *      Author: local1
 */

#include "DevDetectorConstruction.h"

DevDetectorConstruction::DevDetectorConstruction(){
	//Defines that maertial variables
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

void DevDetectorConstruction::ConstructStaves(G4int numOfHCIs,G4String name) {
	//Constants for all staves
	const G4double HCIWidth = 15.0*mm;
	const G4double plateLength = 160.0*mm;
	const G4double plateThickness = 120*um;
	const G4double angle = M_PI/3.0;
	const G4int sides = 6;

	//Parameter calculations
	const G4double plateWidth = getPlateWidth(numOfHCIs,HCIWidth);
	const G4double plateRadius = getPlateRadius(plateWidth,angle,plateThickness);

	//Initialise assembly
	G4AssemblyVolume* pStave = new G4AssemblyVolume();

	//Initial position of Stave section inside assembly
	G4ThreeVector platePosition = G4ThreeVector(plateRadius,0*mm,0*mm);
	G4RotationMatrix* rotation = new G4RotationMatrix(0,0,M_PI/2);

	//Defining a single cold plate.
	G4Box* pSolidPlate = new G4Box(name+"S",plateWidth,plateThickness,plateLength);
	G4LogicalVolume* pLogicalPlate = new G4LogicalVolume(pSolidPlate,pPlateMat,name+"L");

	//Add geometery to assembly
	pStave->AddPlacedVolume(pLogicalPlate,platePosition,rotation);

	//Variables used to move assembly
	G4RotationMatrix* rotateAssembly = new G4RotationMatrix;
	G4ThreeVector translateAssembly;

	//Moves each cold plate to the correct position
	for (int i = 0; i < sides; i++) {
		//MOve and paste assembly
		pStave->MakeImprint(pLogicalWorld,translateAssembly,rotateAssembly);
		//Rotate assembly
		rotateAssembly->rotateZ(angle);
		//NOTE: Assembly is centered at the center of the world so it does not need to be translated
	}

}

G4VPhysicalVolume* DevDetectorConstruction::Construct(){
	//Defining world volume
	G4Box* pSolidWorld = new G4Box("WorldS",500*cm,500*cm,500*cm);
	pLogicalWorld = new G4LogicalVolume(pSolidWorld,pWorldMat,"WorldL");
	G4PVPlacement* pPhysicalWorld = new G4PVPlacement(0,G4ThreeVector(0,0,0),pLogicalWorld,"WorldP",0,false,0,true);

	//Generate staves
	ConstructStaves(3,"StaveC");
	ConstructStaves(4,"StaveD");
	ConstructStaves(2,"StaveB");

	return pPhysicalWorld;
}

