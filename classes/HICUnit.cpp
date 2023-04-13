/*
 * HICUnit.cpp
 *
 *  Created on: Apr 13, 2023
 *      Author: local1
 */

#include "HICUnit.h"

HICUnit::HICUnit(G4LogicalVolume* motherLogical): pGrandmother(motherLogical) {
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

	new G4PVPlacement(0,G4ThreeVector(0,0,0),pMother,"Testing",motherLogical,false,1,true);
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

template<Layers L>
G4LogicalVolume* layerDefinitions::defineLayer(G4bool visibility,G4String name) {

	G4Box* layerSolid =
			new G4Box(name+"S",HICUnit::HICWidth/2.0,L*um/2.0,HICUnit::HICLength/2.0);

	G4LogicalVolume* layerLogical =
			new G4LogicalVolume(layerSolid,HICUnit::pEmpty,name+"L");

	G4VisAttributes* visAtt = new G4VisAttributes(visibility);
	layerLogical->SetVisAttributes(visAtt);

	return layerLogical;
}

template<>
G4LogicalVolume* HICUnit::defineLayer<Chips>(G4bool visibility,G4String name) {
	G4Box* layerSolid =
			new G4Box(name+"S",HICWidth/2.0-passiveWidth/2.0,Chips*um/2.0,HICLength/2.0);

	G4LogicalVolume* layerLogical =
			new G4LogicalVolume(layerSolid,pEmpty,name+"L");

	G4VisAttributes* visAtt = new G4VisAttributes(visibility);
	layerLogical->SetVisAttributes(visAtt);

	return layerLogical;
}

template<>
G4LogicalVolume* HICUnit::defineLayer<Passive>(G4bool visibility,G4String name) {
	G4Box* layerSolid =
			new G4Box(name+"S",passiveWidth/2.0,Passive*um/2.0,HICLength/2.0);

	G4LogicalVolume* layerLogical =
			new G4LogicalVolume(layerSolid,pEmpty,name+"L");

	G4VisAttributes* visAtt = new G4VisAttributes(visibility);
	layerLogical->SetVisAttributes(visAtt);

	return layerLogical;
}

template<Layers L>
G4LogicalVolume* HICUnit::defineSection(G4bool visibility,G4String name) {

	G4Box* sectionSolid =
			new G4Box(name+"SecS",HICWidth/2.0,L*um/2.0,HICSectionLength/2.0);

	G4LogicalVolume* sectionLogical =
			new G4LogicalVolume(sectionSolid,pEmpty,name+"SecL");

	sectionLogical->SetVisAttributes(new G4VisAttributes(visibility));
	return sectionLogical;
}

template<>
G4LogicalVolume* HICUnit::defineSection<Chips>(G4bool visibility,G4String name,) {

	G4Box* sectionSolid =
			new G4Box(name+"SecS",HICWidth/2.0-passiveWidth,Chips*um/2.0,HICSectionLength/2.0);

	G4LogicalVolume* sectionLogical =
			new G4LogicalVolume(sectionSolid,pEmpty,name+"SecL");

	sectionLogical->SetVisAttributes(new G4VisAttributes(visibility));
	return sectionLogical;
}

template<>
G4LogicalVolume* HICUnit::defineSection<Passive>(G4bool visibility,G4String name,) {

	G4Box* sectionSolid =
			new G4Box(name+"SecS",passiveWidth,Passive*um/2.0,HICSectionLength/2.0);

	G4LogicalVolume* sectionLogical =
			new G4LogicalVolume(sectionSolid,pEmpty,name+"SecL");

	sectionLogical->SetVisAttributes(new G4VisAttributes(visibility));
	return sectionLogical;
}

void HICUnit::placeLayer() {
	new G4PVPlacement(0,G4ThreeVector(0,0,0),pMother,"Testing",pGrandmother,false,1,true);
}




