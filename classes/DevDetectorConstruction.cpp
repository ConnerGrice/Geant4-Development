/*
 * DevDetectorConstruction.cpp
 *
 *  Created on: Feb 24, 2023
 *      Author: local1
 */

#include "DevDetectorConstruction.h"

DevDetectorConstruction::DevDetectorConstruction():HCIWidth(15*mm),
staveLength(320*mm),HCILength(271.2*mm),HCIUnitThickness(285*um),angle(M_PI/3.0),sides(6){
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
	G4double thickness = 565*um;

	G4Box* staveS = new G4Box("StaveS",width,thickness/2,staveLength/2);
	G4LogicalVolume* staveL = new G4LogicalVolume(staveS,pWorldMat,"StaveL");

	return staveL;
}

G4LogicalVolume* DevDetectorConstruction::coldPlate(G4double width) {
	G4double thickness = 240*um;
	G4Box* plateS = new G4Box("PlateS",width,thickness/2,staveLength/2);
	G4LogicalVolume* plateL = new G4LogicalVolume(plateS,pPlateMat,"PlateL");

	return plateL;
}

G4LogicalVolume* DevDetectorConstruction::fleece(G4double width) {
	G4double thickness = 20*um;

	G4Box* fleeceS = new G4Box("FleeceS",width,thickness/2,staveLength/2);
	G4LogicalVolume* fleeceL = new G4LogicalVolume(fleeceS,pFleeceMat,"FleeceL");

	return fleeceL;
}

G4LogicalVolume* DevDetectorConstruction::HCIUnitMother() {
	G4Box* HCIS = new G4Box("HCIUnitS",HCIWidth/2,HCIUnitThickness/2,HCILength/2);
	G4LogicalVolume* HCIL = new G4LogicalVolume(HCIS,pWorldMat,"HCIUnitL");

	return HCIL;
}

G4LogicalVolume* DevDetectorConstruction::HCILayerMother(G4double thickness,G4String name) {

	G4Box* stripS = new G4Box(name+"S",HCIWidth/2.0,thickness/2.0,HCILength/2.0);
	G4LogicalVolume* stripL = new G4LogicalVolume(stripS,pWorldMat,name+"L");

	return stripL;
}

G4LogicalVolume* DevDetectorConstruction::HCISegment(G4double thickness,G4String name,G4Material* material) {
	G4double length = HCILength/9.0;

	G4Box* segmentS = new G4Box(name+"SegS",HCIWidth/2.0,thickness/2.0,length/2.0);
	G4LogicalVolume* segmentL = new G4LogicalVolume(segmentS,material,name+"SegL");

	return segmentL;
}

void DevDetectorConstruction::buildHCILayer(G4String name, G4double thickness, G4Material* mat, G4ThreeVector pos) {
	G4LogicalVolume* layerStripL = HCILayerMother(thickness,name);
	G4LogicalVolume* layerSegmentL = HCISegment(thickness,name,mat);

	new G4PVReplica(name+"Replica",layerSegmentL,layerStripL,kZAxis,9,(HCILength/9));
	new G4PVPlacement(0,pos+G4ThreeVector(0,thickness/2,0),layerStripL,name+"StripP",HCIUnitL,false,100);
}

void DevDetectorConstruction::buildHCI(G4String name) {
	G4LogicalVolume* unitL = HCIUnitMother();
	G4ThreeVector height= G4ThreeVector(0,-HCIUnitThickness/2,0);

	std::vector<G4double> thicknesses {40*um,50*um,40*um,30*um,10*um,75*um,10*um,30*um};
	std::vector<G4String> names {"GlueA","Chips","GlueB","SolderA","ConductingA","Substrate","ConductingB","SolderB"};
	std::vector<G4Material*> materials {pGlue,pChips,pGlue,pSolder,pConducting,pSubstrate,pConducting,pSolder};

	for (int i=0; i<thicknesses.size();i++) {
		buildHCILayer(names[i],thicknesses[i],materials[i],height);
		height += G4ThreeVector(0,thicknesses[i],0);
	}

}

void DevDetectorConstruction::ConstructStaves(G4int numOfHCIs,G4String name) {
/*
	//Constants for all staves
	const G4double HCIWidth = 15.0*mm;
	const G4double plateLength = 160.0*mm;

	const G4double angle = M_PI/3.0;
	const G4int sides = 6;
	*/
	const G4double plateThickness = 120*um;
	//Parameter calculations
	const G4double staveWidth = getPlateWidth(numOfHCIs);
	const G4double plateRadius = getPlateRadius(staveWidth,plateThickness);

	G4ThreeVector staveBottom = G4ThreeVector(0,-282.5*um,0);

	G4LogicalVolume* staveL = staveMother(staveWidth);
	G4LogicalVolume* plateL = coldPlate(staveWidth);
	G4LogicalVolume* fleeceL = fleece(staveWidth);
	HCIUnitL = HCIUnitMother();
	buildHCI("HCI1");




	G4VPhysicalVolume* fleecePBot = new G4PVPlacement(0,staveBottom+G4ThreeVector(0,10*um,0),fleeceL,"FleecePBot",staveL,false,2);
	G4VPhysicalVolume* plateP = new G4PVPlacement(0,staveBottom+G4ThreeVector(0,140*um,0),plateL,"PlateP",staveL,false,1);
	G4VPhysicalVolume* fleecePTot = new G4PVPlacement(0,staveBottom+G4ThreeVector(0,270*um,0),fleeceL,"FleecePTot",staveL,false,3);
	G4VPhysicalVolume* HCIUnit1 = new G4PVPlacement(0,G4ThreeVector(0,140*um,0),HCIUnitL,"HCI1",staveL,false,4);

	G4VPhysicalVolume* staveP = new G4PVPlacement(0,G4ThreeVector(0,0,0),staveL,"StaveP",pLogicalWorld,false,5);

/*
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

	G4RotationMatrix* rotateHCI = new G4RotationMatrix(0,0,M_PI/2);
	G4ThreeVector translateHCI = G4ThreeVector(plateRadius,0,0);

	//Moves each cold plate to the correct position
	for (int i = 0; i < sides; i++) {
		//MOve and paste assembly
		pStave->MakeImprint(pLogicalWorld,translateAssembly,rotateAssembly);
		//Rotate assembly
		rotateAssembly->rotateZ(angle);
		//NOTE: Assembly is centered at the center of the world so it does not need to be translated

	}
*/
}

G4VPhysicalVolume* DevDetectorConstruction::Construct(){
	//Defining world volume
	G4Box* pSolidWorld = new G4Box("WorldS",500*cm,500*cm,500*cm);
	pLogicalWorld = new G4LogicalVolume(pSolidWorld,pWorldMat,"WorldL");
	G4PVPlacement* pPhysicalWorld = new G4PVPlacement(0,G4ThreeVector(0,0,0),pLogicalWorld,"WorldP",0,false,0,true);

	//Generate staves
	//ConstructStaves(3,"StaveC");
	ConstructStaves(4,"StaveD");
	//ConstructStaves(2,"StaveB");

	return pPhysicalWorld;
}

