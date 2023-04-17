/*
 * DevDetectorConstruction.cpp
 *
 *  Created on: Feb 24, 2023
 *      Author: local1
 */

#include "DevDetectorConstruction.h"

DevDetectorConstruction::DevDetectorConstruction(){
	pWorldMat = Materials::space;
	pTargetMat = Materials::lHydrogen;
	HICConstructor = HICUnit();
}

DevDetectorConstruction::~DevDetectorConstruction() {
}

void DevDetectorConstruction::buildLayer(G4String name, G4int numOfHICs) {
	HICConstructor = HICUnit(pLogicalWorld,name);
	Stave stave(numOfHICs,HICConstructor,pLogicalWorld);
	stave.placeStaves();
}

G4VPhysicalVolume* DevDetectorConstruction::Construct() {
	//Defining world volume
	G4Box* pSolidWorld = new G4Box("WorldS",500*cm,500*cm,500*cm);
	pLogicalWorld = new G4LogicalVolume(pSolidWorld,pWorldMat,"WorldL");
	G4PVPlacement* pPhysicalWorld = new G4PVPlacement(0,G4ThreeVector(0,0,0),pLogicalWorld,"WorldP",0,false,0,true);

	//Place each layer
	buildLayer("B",2);
	buildLayer("C",3);
	buildLayer("D",4);

	//Place liquid H2 target
	G4double targetDiam = 3*cm;
	G4double targetLength = 1*cm;
	G4ThreeVector targetPosition = G4ThreeVector(0,0,-(HICConstructor.getLength()/2)+HCIZ+(targetLength/2));
	G4Tubs* targetS = new G4Tubs("TargetL",0,targetDiam/2,targetLength/2,0,2*M_PI);
	G4LogicalVolume* targetL = new G4LogicalVolume(targetS,pTargetMat,"TargetL");
	targetL->SetVisAttributes(G4VisAttributes(false));
	new G4PVPlacement(0,targetPosition,targetL,"TargetP",pLogicalWorld,false,6,true);

	return pPhysicalWorld;
}

void DevDetectorConstruction::ConstructSDandField() {

	//Defines chips layers in each stave as the sensitive detector
	auto CSensDet = new DevSensitiveDetector("StaveC","StaveCCollection");
	G4SDManager::GetSDMpointer()->AddNewDetector(CSensDet);
	SetSensitiveDetector("CChipsPixelL",CSensDet);

	auto DSensDet = new DevSensitiveDetector("StaveD","StaveDCollection");
	G4SDManager::GetSDMpointer()->AddNewDetector(DSensDet);
	SetSensitiveDetector("DChipsPixelL",DSensDet);

	auto BSensDet = new DevSensitiveDetector("StaveB","StaveBCollection");
	G4SDManager::GetSDMpointer()->AddNewDetector(BSensDet);
	SetSensitiveDetector("BChipsPixelL",BSensDet);

}

