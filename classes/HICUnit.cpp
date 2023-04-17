/*
 * HICUnit.cpp
 *
 *  Created on: Apr 13, 2023
 *      Author: local1
 */

#include "HICUnit.h"

HICUnit::HICUnit(G4LogicalVolume* root,
		G4String staveName): pWorldVol(root), userName(staveName) {
	//Gets material definitions
	G4NistManager* pNist = G4NistManager::Instance();
	G4Material* space = pNist->FindOrBuildMaterial("G4_Galactic");
	G4Material* C = pNist->FindOrBuildMaterial("G4_C");
	G4Material* Si = pNist->FindOrBuildMaterial("G4_Si");
	G4Material* Al = pNist->FindOrBuildMaterial("G4_Al");
	G4Material* kapton = pNist->FindOrBuildMaterial("G4_KAPTON");

	pEmpty = space;
	pGlue = C;
	pChips = Si;
	pSolder = C;
	pConducting = Al;
	pSubstrate = kapton;

	pMother = defineMother(false);
	placeLayers();
}

G4LogicalVolume* HICUnit::defineMother(G4bool visibility) {
	G4Box* motherSolid =
			new G4Box("HICMotherS",HICWidth/2.0,HICThickness/2.0,HICLength/2.0);

	G4LogicalVolume* motherLogical =
			new G4LogicalVolume(motherSolid,pEmpty,"HICMotherL");

	G4VisAttributes* visAtt = new G4VisAttributes(visibility);
	motherLogical->SetVisAttributes(visAtt);
	return motherLogical;
}

G4LogicalVolume* HICUnit::defineLayer(G4bool visibility,G4String name,G4double layer) {
	G4Box* layerSolid;
	G4double width;

	if (name == "Chips")
		width = HICWidth-passiveWidth;
	else if (name == "Passive")
		width = passiveWidth;
	else
		width = HICWidth;

	layerSolid = new G4Box(name+"S",width/2.0,layer/2.0,HICLength/2.0);

	G4LogicalVolume* layerLogical =
			new G4LogicalVolume(layerSolid,pEmpty,name+"L");

	G4VisAttributes* visAtt = new G4VisAttributes(visibility);
	layerLogical->SetVisAttributes(visAtt);

	return layerLogical;
}

G4LogicalVolume* HICUnit::defineSection(G4bool visibility,
		G4String name,G4double layer,G4Material* mat,G4Colour col) {
	G4Box* sectionSolid;
	G4double width;

	if (name == "Chips")
		width = HICWidth-passiveWidth;
	else if (name == "Passive")
		width = passiveWidth;
	else
		width = HICWidth;

	sectionSolid = new G4Box(name+"SecS",width/2.0,layer/2.0,HICSectionLength/2.0);

	G4LogicalVolume* sectionLogical =
			new G4LogicalVolume(sectionSolid,mat,name+"SecL");

	sectionLogical->SetVisAttributes(new G4VisAttributes(visibility,col));
	return sectionLogical;
}

G4LogicalVolume* HICUnit::definePixelStrip(G4bool visibility,
		G4String name,G4double layer,G4Colour col) {
	G4Box* stripSolid = new G4Box(name+"PStripS",pixelLength/2.0,layer/2.0,HICSectionLength/2.0);
	G4LogicalVolume* stripLogical = new G4LogicalVolume(stripSolid,pEmpty,name+"pStripL");

	stripLogical->SetVisAttributes(new G4VisAttributes(visibility,col));
	return stripLogical;
}

G4LogicalVolume* HICUnit::definePixel(G4bool visibility,
		G4String name,G4double layer,G4Material* mat,G4Colour col) {
	G4Box* pixelSolid = new G4Box(name+"PixelS",pixelLength/2.0,layer/2.0,pixelLength/2.0);
	G4LogicalVolume* pixelLogical = new G4LogicalVolume(pixelSolid,mat,name+"PixelL");

	pixelLogical->SetVisAttributes(new G4VisAttributes(visibility,col));
	return pixelLogical;
}

void HICUnit::buildLayer(G4bool visibility,
		G4String name,G4double layer,G4double yShift,G4Material* mat, G4Colour col) {
	G4LogicalVolume* layerUnit = defineLayer(false,name,layer);
	G4LogicalVolume* sectionUnit = defineSection(visibility,name,layer,mat,col);

	for (int i = 0;i < 9;i++) {
		G4double zShift = (-HICLength/2.0)+(HICSectionLength/2.0)+(i*(HICSectionLength+sectionGap));
		new G4PVPlacement(0,G4ThreeVector(0,0,zShift),sectionUnit,name+"Section",layerUnit,false,0);
	}

	new G4PVPlacement(0,G4ThreeVector(0,yShift+(layer/2.0),0),layerUnit,name+"Layer",pMother,false,1);
}

void HICUnit::buildActiveLayer(G4bool visibility,
		G4String volName,G4double layer,G4double yShift,G4Material* mat,G4Colour col) {

	G4String name = userName + volName;
	G4LogicalVolume* layerUnit = defineLayer(false,volName,layer);
	G4LogicalVolume* sectionUnit = defineSection(visibility,volName,layer,pEmpty,col);
	G4LogicalVolume* pixelStrip = definePixelStrip(false,volName,layer,col);
	G4LogicalVolume* pixel = definePixel(false,name,layer,mat,col);

	new G4PVReplica(name+"PixelStrip",pixelStrip,sectionUnit,kXAxis,460,pixelLength);
	new G4PVReplica(name+"Pixel",pixel,pixelStrip,kZAxis,1000,pixelLength);

	for (int i = 0; i < 9; i++) {
		G4double zShift = (-HICLength/2.0)+(HICSectionLength/2.0)+(i*(HICSectionLength+sectionGap));
		new G4PVPlacement(0,G4ThreeVector(0,0,zShift),sectionUnit,name+"Section",layerUnit,false,0);
	}
	new G4PVPlacement(0,G4ThreeVector(-passiveWidth/2.0,yShift+(layer/2.0),0),layerUnit,name+"Layer",pMother,false,1);
}

void HICUnit::buildPassiveLayer(G4bool visibility,
		G4String name,G4double layer,G4double yShift,G4Material* mat,G4Colour col) {

	G4LogicalVolume* layerUnit = defineLayer(false,name,layer);
	G4LogicalVolume* sectionUnit = defineSection(visibility,name,layer,mat,col);

	for (int i = 0; i < 9; i++) {
		G4double zShift = (-HICLength/2.0)+(HICSectionLength/2.0)+(i*(HICSectionLength+sectionGap));
		new G4PVPlacement(0,G4ThreeVector(0,0,zShift),sectionUnit,name+"Section",layerUnit,false,0);
	}
	new G4PVPlacement(0,G4ThreeVector(HICWidth/2.0-passiveWidth/2.0,yShift+(layer/2.0),0),layerUnit,name+"Layer",pMother,false,1);
}

void HICUnit::placeLayers() {

	std::vector<G4String> layerNames {
		"GlueA",
		"Chips",
		"Passive",
		"GlueB",
		"SolderA",
		"ConductingA",
		"Substrate",
		"ConductingB",
		"SolderB"
	};

	std::vector<G4Material*> layerMats {
		pGlue,
		pChips,
		pChips,
		pGlue,
		pSolder,
		pConducting,
		pSubstrate,
		pConducting,
		pSolder
	};

	G4Colour glueCol = G4Colour(0.0,0.0,1.0);
	G4Colour chipsCol = G4Colour(0.0,1.0,0.0);
	G4Colour solderCol = G4Colour(1.0,0.0,1.0);
	G4Colour conductingCol = G4Colour(0.0,1.0,1.0);
	G4Colour substrateCol = G4Colour(1.0,0.0,0.0);

	std::vector<G4Colour> layerCols {
		glueCol,
		chipsCol,
		chipsCol,
		glueCol,
		solderCol,
		conductingCol,
		substrateCol,
		conductingCol,
		solderCol
	};

	std::vector<G4double> layerThicks {
		40*um,
		50*um,
		50*um,
		40*um,
		30*um,
		10*um,
		75*um,
		10*um,
		30*um
	};

	G4double height = -HICThickness/2.0;
	for (size_t i = 0; i<9;i++) {
		G4String layerName = layerNames[i];
		G4double layerThick = layerThicks[i];
		G4Material* layerMat = layerMats[i];
		G4Colour layerCol = layerCols[i];

		if (layerName == "Chips")
			buildActiveLayer(visibleLayers[i],layerName,layerThick,height,layerMat,layerCol);
		else if (layerName == "Passive") {
			height -= layerThick;
			buildPassiveLayer(visibleLayers[i],layerName,layerThick,height,layerMat,layerCol);
		} else
			buildLayer(visibleLayers[i],layerName,layerThick,height,layerMat,layerCol);

		height += layerThick;
	}
}



