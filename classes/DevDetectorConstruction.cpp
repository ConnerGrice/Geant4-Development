/*
 * DevDetectorConstruction.cpp
 *
 *  Created on: Feb 24, 2023
 *      Author: local1
 */

#include "DevDetectorConstruction.h"

DevDetectorConstruction::DevDetectorConstruction(): pLogicalWorld(0), pScoringVolume(0),
	pWorldMat(Materials::space),
	pTargetMat(Materials::lHydrogen),
	pMylarMat(Materials::mylar),
	pCalifaMat(Materials::ceasiumIodide),
	HICConstructor(HICUnit()) {}

DevDetectorConstruction::~DevDetectorConstruction() {
}

void DevDetectorConstruction::buildLayer(G4String name, G4int numOfHICs,G4bool show) {
	HICConstructor = HICUnit(pLogicalWorld,name,show);
	Stave stave(numOfHICs,HICConstructor,pLogicalWorld);
	stave.placeStaves(show);
}

void DevDetectorConstruction::buildSource(G4bool visibility) {
	//Liquid H parameters
	G4double targetDiam = 3*cm;
	G4double targetLength = 1*cm;
	G4ThreeVector targetPosition = G4ThreeVector(0,0,-(HICConstructor.getLength()/2)+HCIZ+(targetLength/2));

	G4Tubs* targetS = new G4Tubs("TargetS",0,targetDiam/2,targetLength/2,0,2*M_PI);
	G4LogicalVolume* targetL = new G4LogicalVolume(targetS,pTargetMat,"TargetL");

	targetL->SetVisAttributes(G4VisAttributes(false));
	new G4PVPlacement(0,targetPosition,targetL,"TargetP",pLogicalWorld,false,6);

	//Mylar layer parameter
	G4double mylarThickness = 100*um;

	//Mylar barrel
	G4Tubs* mylarS = new G4Tubs("MylarL",
			targetDiam/2.0,(targetDiam+mylarThickness)/2.0,targetLength/2.0,
			0,2*M_PI);
	G4LogicalVolume* mylarL = new G4LogicalVolume(mylarS,pMylarMat,"MylarL");

	mylarL->SetVisAttributes(G4VisAttributes(visibility));
	new G4PVPlacement(0,targetPosition,mylarL,"MylarP",pLogicalWorld,false,6);

	//Mylar caps
	G4Tubs* capS = new G4Tubs("CapS",0,(targetDiam+mylarThickness)/2.0,mylarThickness/2.0,0,2*M_PI);
	G4LogicalVolume* capL = new G4LogicalVolume(capS,pMylarMat,"CapL");

	capL->SetVisAttributes(G4VisAttributes(visibility));
	G4ThreeVector zShift = G4ThreeVector(0,0,(targetLength+mylarThickness)/2.0);
	new G4PVPlacement(0,targetPosition + zShift,capL,"CapA",pLogicalWorld,false,7);
	new G4PVPlacement(0,targetPosition - zShift,capL,"CapA",pLogicalWorld,false,8);
}

void DevDetectorConstruction::buildCALIFA(G4bool visibility) {
	G4double calEmpty = 40*cm;
	G4double calThickness = 44*cm;
	G4double calDiam = calEmpty + calThickness;

	G4Sphere* calSolid = new G4Sphere("CALIFAS",calEmpty/2.0,calDiam/2.0,0,2*M_PI,0,M_PI);
	G4LogicalVolume* calLogical = new G4LogicalVolume(calSolid,pCalifaMat,"CALIFAL");

	pScoringVolume = calLogical;

	calLogical->SetVisAttributes(G4VisAttributes(visibility));
	new G4PVPlacement(0,G4ThreeVector(0,0,0),calLogical,"CALIFA",pLogicalWorld,false,3,true);

}

G4VPhysicalVolume* DevDetectorConstruction::Construct() {
	//Defining world volume
	G4Box* pSolidWorld = new G4Box("WorldS",500*cm,500*cm,500*cm);
	pLogicalWorld = new G4LogicalVolume(pSolidWorld,pWorldMat,"WorldL");
	G4PVPlacement* pPhysicalWorld = new G4PVPlacement(0,G4ThreeVector(0,0,0),pLogicalWorld,"WorldP",0,false,0,true);

	//Place each layer
	buildLayer("B",2,true);
	buildLayer("C",3,false);
	buildLayer("D",4,false);

	buildSource(true);
	buildCALIFA(false);

	return pPhysicalWorld;
}

void DevDetectorConstruction::ConstructSDandField() {
	//Defines chips layers in each stave as the sensitive detector
	auto BSensDet = new DevSensitiveDetector("StaveB","StaveBCollection");
	G4SDManager::GetSDMpointer()->AddNewDetector(BSensDet);
	SetSensitiveDetector("BChipsPixelL",BSensDet);

	auto CSensDet = new DevSensitiveDetector("StaveC","StaveCCollection");
	G4SDManager::GetSDMpointer()->AddNewDetector(CSensDet);
	SetSensitiveDetector("CChipsPixelL",CSensDet);

	auto DSensDet = new DevSensitiveDetector("StaveD","StaveDCollection");
	G4SDManager::GetSDMpointer()->AddNewDetector(DSensDet);
	SetSensitiveDetector("DChipsPixelL",DSensDet);

}

