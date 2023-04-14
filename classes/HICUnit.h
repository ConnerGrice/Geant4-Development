/*
 * HICUnit.h
 *
 *  Created on: Apr 13, 2023
 *      Author: local1
 */

#ifndef GEANT4_DEVELOPMENT_CLASSES_HICUNIT_H_
#define GEANT4_DEVELOPMENT_CLASSES_HICUNIT_H_

#include <G4LogicalVolume.hh>
#include <G4NistManager.hh>
#include <G4Box.hh>
#include <G4VisAttributes.hh>
#include <G4SystemOfUnits.hh>
#include <G4PVPlacement.hh>
#include <G4PVReplica.hh>

#include <map>
#include <vector>

class HICUnit {
public:
	static constexpr G4double         HICWidth = 15*mm;
	static constexpr G4double        HICLength = 271.6*mm;
	static constexpr G4double     HICThickness = 285*um;
	static constexpr G4double 	   pixelLength = 30*um;
	static constexpr G4double HICSectionLength = 30*mm;
	static constexpr G4double     passiveWidth = 1.2*mm;
	static constexpr G4double       sectionGap = 0.2*mm;

	//Constructor
	HICUnit(G4LogicalVolume* root);

	//Places mother volume containing all layers
	inline void placeUnit(G4ThreeVector position) {
		placeLayers();
		new G4PVPlacement(0,position,pMother,"Testing",pWorldVol,false,1,true);
	}

private:
	//Defines mother logical volume
	G4LogicalVolume* defineMother(G4bool visibility);

	//Defines a single layer mother volume
	G4LogicalVolume* defineLayer(G4bool visibility,
			G4String name,G4double layer);

	//Defines a single section mother volume
	G4LogicalVolume* defineSection(G4bool visibility,
			G4String name,G4double layer,G4Material* mat,G4Colour col);

	//Defines a single strip mother volume
	G4LogicalVolume* definePixelStrip(G4bool visibility,
			G4String name,G4double layer,G4Colour col);

	//Defines a single pizel mother volume
	G4LogicalVolume* definePixel(G4bool visibility,
			G4String name,G4double layer,G4Material* mat,G4Colour col);

	//generates physical volumes related to passive layers
	void buildLayer(G4bool visibility,
			G4String name,G4double layer,G4double yShift,G4Material* mat,G4Colour col);

	//Generates physical volumes related to active chip layer
	void buildActiveLayer(G4bool visibility,
			G4String name,G4double layer,G4double yShift,G4Material* mat,G4Colour col);

	//Generates physical volumes related to passive chip layer
	void buildPassiveLayer(G4bool visibility,
			G4String name,G4double layer,G4double yShift,G4Material* mat,G4Colour col);

	//Places layers for a single unit
	void placeLayers();

private:
	G4LogicalVolume* pWorldVol;
	G4LogicalVolume* pMother;


	std::vector<G4bool> visibleLayers {
		true,	//GlueA
		true,	//Chips
		true,	//Passive
		true,	//GLueB
		true, 	//SolderA
		true,	//ConductingA
		true,	//Substrate
		true,	//ConductingB
		true	//SolderB
	};

	G4Material* pEmpty;
	G4Material* pGlue;
	G4Material* pChips;
	G4Material* pSolder;
	G4Material* pConducting;
	G4Material* pSubstrate;

};

#endif /* GEANT4_DEVELOPMENT_CLASSES_HICUNIT_H_ */
