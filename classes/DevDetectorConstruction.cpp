/*
 * DevDetectorConstruction.cpp
 *
 *  Created on: Feb 24, 2023
 *      Author: local1
 */

#include "DevDetectorConstruction.h"

DevDetectorConstruction::DevDetectorConstruction():HCIWidth(15*mm),
staveLength(320*mm),HCILength(271.2*mm),HCIUnitThickness(285*um),
sides(6),angle(M_PI/3.0),plateThickness(240*um){
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
	pFleeceMat = pNist->FindOrBuildMaterial("G4_C");
	pGlue = pPlateMat;
	pChips = pPlateMat;
	pSolder = pPlateMat;
	pConducting = pPlateMat;
	pSubstrate = pPlateMat;
}

G4LogicalVolume* DevDetectorConstruction::staveMother(G4double width){
	//Volume definition
	G4double thickness = 850*um;
	G4Box* staveS = new G4Box("StaveS",width,thickness/2,staveLength/2);
	G4LogicalVolume* staveOut = new G4LogicalVolume(staveS,pWorldMat,"StaveL");

	//Visual parameters
	G4VisAttributes* visAtt = new G4VisAttributes(false); //Invisible
	staveOut->SetVisAttributes(visAtt);

	return staveOut;
}

G4LogicalVolume* DevDetectorConstruction::coldPlate(G4double width) {
	//Volume definition
	G4Box* plateS = new G4Box("PlateS",width,plateThickness/2,staveLength/2);
	G4LogicalVolume* plateL = new G4LogicalVolume(plateS,pPlateMat,"PlateL");

	//Visual parameters
	G4VisAttributes* visAtt = new G4VisAttributes(true,G4Colour(0.75,0.75,0.75));
	plateL->SetVisAttributes(visAtt);

	return plateL;
}

G4LogicalVolume* DevDetectorConstruction::fleece(G4double width) {
	//Volume definition
	G4double thickness = 20*um;
	G4Box* fleeceS = new G4Box("FleeceS",width,thickness/2,staveLength/2);
	G4LogicalVolume* fleeceL = new G4LogicalVolume(fleeceS,pFleeceMat,"FleeceL");

	//Visual parameters
	G4VisAttributes* visAtt = new G4VisAttributes(true,G4Colour(0.5,0.5,0.5));
	fleeceL->SetVisAttributes(visAtt);

	return fleeceL;
}

void DevDetectorConstruction::buildPlate(G4double width) {
	//Padding of plate anf fleece
	G4double offset = plateThickness/2+10*um;

	//Get volumes
	G4LogicalVolume* plateL = coldPlate(width);
	G4LogicalVolume* fleeceL = fleece(width);

	//PLace volumes
	//Bottom fleece layer
	new G4PVPlacement(0,G4ThreeVector(0,-offset,0),fleeceL,"FleecePBot",staveL,false,2);
	//Cold plate layer
	new G4PVPlacement(0,G4ThreeVector(0,0,0),plateL,"PlateP",staveL,false,1);
	//Top fleece layer
	new G4PVPlacement(0,G4ThreeVector(0,offset,0),fleeceL,"FleecePTot",staveL,false,3);
}

G4LogicalVolume* DevDetectorConstruction::HCIUnitMother() {
	//Volume Definition
	G4Box* HCIS = new G4Box("HCIUnitS",HCIWidth/2,HCIUnitThickness/2,HCILength/2);
	G4LogicalVolume* HCIL = new G4LogicalVolume(HCIS,pWorldMat,"HCIUnitL");

	//Visual parameters
	G4VisAttributes* visAtt = new G4VisAttributes(false);
	HCIL->SetVisAttributes(visAtt);
	return HCIL;
}

G4LogicalVolume* DevDetectorConstruction::HCILayerMother(G4double thickness,G4String name) {
	//Volume definition
	G4Box* stripS = new G4Box(name+"S",HCIWidth/2.0,thickness/2.0,HCILength/2.0);
	G4LogicalVolume* stripL = new G4LogicalVolume(stripS,pWorldMat,name+"L");

	//Visual parameters
	G4VisAttributes* visAtt = new G4VisAttributes(false);
	stripL->SetVisAttributes(visAtt);
	return stripL;
}

G4LogicalVolume* DevDetectorConstruction::HCISegment(G4double thickness,G4String name,G4Material* material, G4VisAttributes* visual) {
	//Volume definition
	G4double length = HCILength/9.0;
	G4Box* segmentS = new G4Box(name+"SegS",HCIWidth/2.0,thickness/2.0,length/2.0);
	G4LogicalVolume* segmentL = new G4LogicalVolume(segmentS,material,name+"SegL");

	//Visual parameters
	segmentL->SetVisAttributes(visual);

	return segmentL;
}

void DevDetectorConstruction::buildHCILayer(G4String name, G4double thickness, G4Material* mat,G4VisAttributes* visual, G4ThreeVector pos) {
	//Get volumes
	G4LogicalVolume* layerStripL = HCILayerMother(thickness,name);
	G4LogicalVolume* layerSegmentL = HCISegment(thickness,name,mat,visual);

	//Place volumes
	new G4PVReplica(name+"Replica",layerSegmentL,layerStripL,kZAxis,9,(HCILength/9));
	new G4PVPlacement(0,pos+G4ThreeVector(0,thickness/2,0),layerStripL,name+"StripP",HCIUnitL,false,100);
}

void DevDetectorConstruction::buildHCI(G4String name) {
	//Position of leftmost unit
	G4ThreeVector height= G4ThreeVector(0,-HCIUnitThickness/2,0);

	//Layer colours
	G4Colour glueCol(0.0,0.0,1.0);
	G4Colour chipsCol(0.0,1.0,0.0);
	G4Colour solderCol(1.0,0.0,1.0);
	G4Colour conductingCol(0.0,1.0,1.0);
	G4Colour substrateCol(1.0,0.0,0.0);
	G4VisAttributes* visAttr;;

	//Parameters for each layer of the unit
	std::vector<G4double> thicknesses {40*um,50*um,40*um,30*um,10*um,75*um,10*um,30*um};
	std::vector<G4String> names {"GlueA","Chips","GlueB","SolderA","ConductingA","Substrate","ConductingB","SolderB"};
	std::vector<G4Material*> materials {pGlue,pChips,pGlue,pSolder,pConducting,pSubstrate,pConducting,pSolder};
	std::vector<G4Colour> colours {glueCol,chipsCol,glueCol,solderCol,conductingCol,substrateCol,conductingCol,solderCol};

	//Build each layer
	for (unsigned int i=0; i<thicknesses.size();i++) {
		visAttr = new G4VisAttributes(true,colours[i]);
		buildHCILayer(name+names[i],thicknesses[i],materials[i],visAttr,height);
		height += G4ThreeVector(0,thicknesses[i],0);
	}

}

void DevDetectorConstruction::ConstructStaves(G4int numOfHCIs,G4String name) {
	//Parameter calculations
	const G4double staveWidth = getPlateWidth(numOfHCIs);

	//Initialise stave mother volume
	staveL = staveMother(staveWidth);

	//Creates and places cold plate and fleece
	buildPlate(staveWidth);

	//Initialise HCI mother volume
	HCIUnitL = HCIUnitMother();

	//Creates and places entire HCI unit
	buildHCI(name);

	//Left-most HCI unit position
	G4ThreeVector HCIPosition = G4ThreeVector(-(HCIWidth*numOfHCIs/2)-HCIWidth/2,0,0);
	G4RotationMatrix* HCIRotation = new G4RotationMatrix;

	//Places each HCI unit on alternating side of the plate
	for (int i=0;i < numOfHCIs;i++){
		//Slide to the right
		HCIPosition += G4ThreeVector(HCIWidth,0,0);
		if (i%2 == 0) {
			//Place on top
			HCIPosition.setY(282.5*um);
			HCIRotation = new G4RotationMatrix(0,0,0);
		} else {
			//Place on bottom and rotate
			HCIPosition.setY(-282.5*um);
			HCIRotation = new G4RotationMatrix(0,0,M_PI);
		}

		//Place HCI Unit volume in simulation
		new G4PVPlacement(HCIRotation,HCIPosition,HCIUnitL,"HCI"+(G4String)i,staveL,false,(10*numOfHCIs)+i);
	}

	//Stave location parameters
	const G4double plateRadius = getPlateRadius(staveWidth);
	G4RotationMatrix* staveRotation;;
	G4ThreeVector staveTranslation = G4ThreeVector(plateRadius,0,0);

	//Creates a stave for each side of the detector
	for (int i=0; i < sides; i++) {
		staveRotation = new G4RotationMatrix(0,0,(M_PI/2)+(angle*i));

		//Place lowest level stave mother volume
		new G4PVPlacement(staveRotation,staveTranslation,staveL,"Stave"+name+"P",pLogicalWorld,false,5+i);

		staveTranslation.rotateZ(angle);
	}
}

G4VPhysicalVolume* DevDetectorConstruction::Construct() {
	//Defining world volume
	G4Box* pSolidWorld = new G4Box("WorldS",500*cm,500*cm,500*cm);
	pLogicalWorld = new G4LogicalVolume(pSolidWorld,pWorldMat,"WorldL");
	G4PVPlacement* pPhysicalWorld = new G4PVPlacement(0,G4ThreeVector(0,0,0),pLogicalWorld,"WorldP",0,false,0,true);

	//Generate staves
	ConstructStaves(3,"C");
	ConstructStaves(4,"D");
	ConstructStaves(2,"B");

	return pPhysicalWorld;
}

void DevDetectorConstruction::ConstructSDandField() {
	auto CSensDet = new DevSensitiveDetector("StaveC","StaveCCollection");
	G4SDManager::GetSDMpointer()->AddNewDetector(CSensDet);
	SetSensitiveDetector("CChipsL",CSensDet);

}

