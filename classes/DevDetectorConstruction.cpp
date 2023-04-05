/*
 * DevDetectorConstruction.cpp
 *
 *  Created on: Feb 24, 2023
 *      Author: local1
 */

#include "DevDetectorConstruction.h"

DevDetectorConstruction::DevDetectorConstruction():HCIWidth(15*mm),
staveLength(320*mm),HCILength(271.6*mm),HCIUnitThickness(285*um),
HCIPixelSide(30*um),sides(6),angle(M_PI/3.0),plateThickness(240*um),fleeceThickness(20*um){
	//Defines that maertial variables
	DefineMaterials();
}

DevDetectorConstruction::~DevDetectorConstruction() {
}

void DevDetectorConstruction::DefineMaterials(){
	//Defining materials
	G4NistManager* pNist = G4NistManager::Instance();
	G4Material* C = pNist->FindOrBuildMaterial("G4_C");
	G4Material* Si = pNist->FindOrBuildMaterial("G4_Si");
	G4Material* Al = pNist->FindOrBuildMaterial("G4_Al");
	G4Material* kapton = pNist->FindOrBuildMaterial("G4_KAPTON");
	G4Material* liquid_H2 = pNist->FindOrBuildMaterial("G4_lH2");

	//Defining volume materials
	pWorldMat = pNist->FindOrBuildMaterial("G4_Galactic");
	pPlateMat = C;
	pFleeceMat = C;
	pGlue = C;
	pChips = Si;
	pSolder = C;
	pConducting = Al;
	pSubstrate = kapton;
	pTarget = liquid_H2;
	//TODO: Add Mylar target cover
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
	G4Box* fleeceS = new G4Box("FleeceS",width,fleeceThickness/2,staveLength/2);
	G4LogicalVolume* fleeceL = new G4LogicalVolume(fleeceS,pFleeceMat,"FleeceL");

	//Visual parameters
	G4VisAttributes* visAtt = new G4VisAttributes(true,G4Colour(0.5,0.5,0.5));
	fleeceL->SetVisAttributes(visAtt);

	return fleeceL;
}

void DevDetectorConstruction::buildPlate(G4double width) {
	//Padding of plate and fleece
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
	G4Box* stripS;
	G4VisAttributes* visAtt;
	if (name == "BChips" || name == "CChips" || name == "DChips") {
		stripS = new G4Box(name+"S",HCIWidth/2.0-0.6*mm,thickness/2.0,HCILength/2.0);
		visAtt = new G4VisAttributes(false);
	} else {
		visAtt = new G4VisAttributes(false);
		stripS = new G4Box(name+"S",HCIWidth/2.0,thickness/2.0,HCILength/2.0);
	}

	G4LogicalVolume* stripL = new G4LogicalVolume(stripS,pWorldMat,name+"L");

	//Visual parameters
	stripL->SetVisAttributes(visAtt);
	return stripL;
}

G4LogicalVolume* DevDetectorConstruction::HCISegment(G4double thickness,G4String name,G4Material* material, G4VisAttributes* visual) {
	//Volume definition
	G4double length = HCILength/9;

	G4Box* segmentS;

	if (name == "BChips" || name == "CChips" || name == "DChips") {
		segmentS = new G4Box(name+"SegS",HCIWidth/2.0-0.6*mm,thickness/2.0,30*mm/2.0);
	} else {
		segmentS = new G4Box(name+"SegS",HCIWidth/2.0,thickness/2.0,30*mm/2.0);
	}

	G4LogicalVolume* segmentL = new G4LogicalVolume(segmentS,material,name+"SegL");

	//Visual parameters
	segmentL->SetVisAttributes(visual);

	return segmentL;
}

G4LogicalVolume* DevDetectorConstruction::HICChipLayerMother(G4double thickness) {
	G4Box* chipS = new G4Box("PassiveS",0.6*mm,thickness/2.0,HCILength/2.0);
	G4LogicalVolume* chipL = new G4LogicalVolume(chipS,pWorldMat,"PassiveL");

	chipL->SetVisAttributes(false);
	return chipL;
}

G4LogicalVolume* DevDetectorConstruction::HICChipSegment(G4double thickness,G4VisAttributes* visual) {
	G4double length = 30*mm;

	G4Box* chipSegS = new G4Box("PassiveSegS",0.6*mm,thickness/2,length/2);
	G4LogicalVolume* chipSegL = new G4LogicalVolume(chipSegS,pChips,"PassiveSegL");


	G4VisAttributes* visAttr = new G4VisAttributes(visual);
	visAttr->SetColour(G4Colour(0.0,0.5,0.0));

	chipSegL->SetVisAttributes(visAttr);
	return chipSegL;
}

G4LogicalVolume* DevDetectorConstruction::HCIPixelStrip(G4double thickness,G4String name, G4Material* material,G4VisAttributes* visual) {
	G4double length = 30*mm;
	G4Box* pixelStripS = new G4Box(name+"PStripS",HCIPixelSide/2.0,thickness/2.0,length/2.0);
	G4LogicalVolume* pixelStripL = new G4LogicalVolume(pixelStripS,material,name+"PStripS");

	//Visual parameters
	G4VisAttributes* visAtt = new G4VisAttributes(false);
	pixelStripL->SetVisAttributes(visAtt);

	return pixelStripL;
}

G4LogicalVolume* DevDetectorConstruction::HCIPixel(G4double thickness,G4String name, G4Material* material,G4VisAttributes* visual) {
	G4Box* pixelS = new G4Box(name+"PixelS",HCIPixelSide/2.0,thickness/2.0,HCIPixelSide/2.0);
	G4LogicalVolume* pixelL = new G4LogicalVolume(pixelS,material,name+"PixelL");

	//Visual parameters
	G4VisAttributes* visAtt = new G4VisAttributes(false);
	pixelL->SetVisAttributes(visAtt);

	return pixelL;
}

void DevDetectorConstruction::buildHCILayer(G4String name, G4double thickness, G4Material* mat,G4VisAttributes* visual, G4ThreeVector pos) {
	//Get volumes
	G4LogicalVolume* layerStripL = HCILayerMother(thickness,name);
	G4LogicalVolume* layerSegmentL = HCISegment(thickness,name,mat,visual);

	//Constructs chip layer
	if (name == "BChips" || name == "CChips" || name == "DChips") {
		//Pixel array
		G4LogicalVolume* pixelStrip = HCIPixelStrip(thickness,name,mat,visual);
		G4LogicalVolume* pixel = HCIPixel(thickness,name,mat,visual);
		new G4PVReplica(name+"PixelStrip",pixelStrip,layerSegmentL,kXAxis,460,HCIPixelSide);
		new G4PVReplica(name+"Pixel",pixel,pixelStrip,kZAxis,1000,HCIPixelSide);

		//Passive strips
		G4LogicalVolume* chipStrip = HICChipLayerMother(thickness);
		G4LogicalVolume* chipSegment = HICChipSegment(thickness,visual);


		//new G4ReplicatedSlice(name+"ChipStripP",chipSegment,chipStrip,kZAxis,9,30*mm,0.1*mm,0*mm);


		//Full layer

		for (int i = 0;i<9;i++) {
			new G4PVPlacement(0,G4ThreeVector(0,0,(-HCILength/2.0)+15*mm+(i*30.2*mm)),chipSegment,name+"ChipStripP",chipStrip,false,124);
			new G4PVPlacement(0,G4ThreeVector(0,0,(-HCILength/2.0)+15*mm+(i*30.2*mm)),layerSegmentL,name+"Segment",layerStripL,false,123);
		}
		new G4PVPlacement(0,pos+G4ThreeVector(-6.9*mm,thickness/2,0),chipStrip,name+"ChipStripP",HCIUnitL,false,101);

		//new G4ReplicatedSlice(name+"Segment",layerSegmentL,layerStripL,kZAxis,9,30*mm,0.1*mm,0);
		new G4PVPlacement(0,pos+G4ThreeVector(0.6*mm,thickness/2,0),layerStripL,name+"StripP",HCIUnitL,false,1);


	} else {
		for (int i = 0;i<9;i++) {
			new G4PVPlacement(0,G4ThreeVector(0,0,(-HCILength/2.0)+15*mm+(i*30.2*mm)),layerSegmentL,name+"Segment",layerStripL,false,125);
		}
	//Place volumes
	//new G4ReplicatedSlice(name+"Segment",layerSegmentL,layerStripL,kZAxis,9,30*mm,0.1*mm,0);
		new G4PVPlacement(0,pos+G4ThreeVector(0,thickness/2,0),layerStripL,name+"StripP",HCIUnitL,false,1);
	}
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
	G4bool invisible = false;
	G4VisAttributes* visAttr;;

	//Parameters for each layer of the unit
	std::vector<G4double> thicknesses {40*um,50*um,40*um,30*um,10*um,75*um,10*um,30*um};
	std::vector<G4String> names {"GlueA","Chips","GlueB","SolderA","ConductingA","Substrate","ConductingB","SolderB"};
	std::vector<G4Material*> materials {pGlue,pChips,pGlue,pSolder,pConducting,pSubstrate,pConducting,pSolder};
	std::vector<G4Colour> colours {glueCol,chipsCol,glueCol,solderCol,conductingCol,substrateCol,conductingCol,solderCol};

	//Build each layer
	for (unsigned int i=0; i<thicknesses.size();i++) {
		//Visualiser will only show the chips layer
		if (names[i] == "Chips") {
			invisible = true;
		} else {
			invisible = false;
		}
		visAttr = new G4VisAttributes(invisible,colours[i]);
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
	G4ThreeVector HCIPosition = G4ThreeVector(-(HCIWidth*numOfHCIs/2)-HCIWidth/2,0,HCIZ);
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
		new G4PVPlacement(HCIRotation,HCIPosition,HCIUnitL,"HCI"+std::to_string(i),staveL,false,i);
	}

	//Stave location parameters
	const G4double plateRadius = getPlateRadius(staveWidth);
	G4RotationMatrix* staveRotation;
	G4ThreeVector staveTranslation = G4ThreeVector(plateRadius,0,0);

	//Creates a stave for each side of the detector
	for (int i=0; i < sides; i++) {
		staveRotation = new G4RotationMatrix(0,0,(M_PI/2)+(angle*i));

		//Place lowest level stave mother volume
		new G4PVPlacement(staveRotation,staveTranslation,staveL,"Stave"+name+"P",pLogicalWorld,false,i);

		staveTranslation.rotateZ(angle);
	}
}

G4VPhysicalVolume* DevDetectorConstruction::Construct() {
	//Defining world volume
	G4Box* pSolidWorld = new G4Box("WorldS",500*cm,500*cm,500*cm);
	pLogicalWorld = new G4LogicalVolume(pSolidWorld,pWorldMat,"WorldL");
	G4PVPlacement* pPhysicalWorld = new G4PVPlacement(0,G4ThreeVector(0,0,0),pLogicalWorld,"WorldP",0,false,0,true);

	//Place liquid H2 target
	G4double targetDiam = 3*cm;
	G4double targetLength = 1*cm;
	G4ThreeVector targetPosition = G4ThreeVector(0,0,-(HCILength/2)+HCIZ+(targetLength/2));
	G4Tubs* targetS = new G4Tubs("TargetL",0,targetDiam/2,targetLength/2,0,2*M_PI);
	G4LogicalVolume* targetL = new G4LogicalVolume(targetS,pTarget,"TargetL");
	targetL->SetVisAttributes(G4VisAttributes(false));
	new G4PVPlacement(0,targetPosition,targetL,"TargetP",pLogicalWorld,false,6,true);

	//Generate staves
	ConstructStaves(3,"C");
	ConstructStaves(4,"D");
	ConstructStaves(2,"B");

/*
	G4Box* test = new G4Box("test",7.5*mm,25*um,15*mm);
	G4LogicalVolume* testL = new G4LogicalVolume(test,pWorldMat,"testt");
	auto param = new HICParameterisation();
	G4VPhysicalVolume* testFin = new G4PVParameterised("final",testL,pLogicalWorld,kZAxis,1461,param);
*/

/*
	ChipDigitiserMap* map = ChipDigitiserMap::getInstance();
	G4cout<<map->getPosition(0).getX()<<G4endl;
	G4cout<<map->getPosition(0).getY()<<G4endl;
	G4cout<<map->getPosition(0).getZ()<<G4endl;
	G4cout<<"================================"<<G4endl;
	G4cout<<map->getPosition(1).getX()<<G4endl;
	G4cout<<map->getPosition(1).getY()<<G4endl;
	G4cout<<map->getPosition(1).getZ()<<G4endl;
*/
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

