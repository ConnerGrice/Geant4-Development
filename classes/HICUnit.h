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

#include <tuple>

	enum Layers {
		GlueA = 40,
		Chips = 50,
		Passive = 50,
		GlueB = 40,
		SolderA = 30,
		ConductingA = 10,
		Substrate = 75,
		ConductingB = 10,
		SolderB = 30
	};

namespace layerDefinitions {
	template<Layers L> G4LogicalVolume* defineLayer(G4bool,G4String);
	template<> G4LogicalVolume* defineLayer<Chips>(G4bool,G4String);
	template<> G4LogicalVolume* defineLayer<Passive>(G4bool,G4String);
}

namespace sectionDefinitions {
	template<Layers L> G4LogicalVolume* defineSection(G4bool,G4String);
	template<> G4LogicalVolume* defineSection<Chips>(G4bool,G4String);
	template<> G4LogicalVolume* defineSection<Passive>(G4bool,G4String);
}


class HICUnit {
public:

	HICUnit(G4LogicalVolume* motherLogical);


	G4LogicalVolume* defineMother(G4bool visibility);

	template<Layers L> G4LogicalVolume* defineLayer(G4bool visibility,G4String name) {
		return layerDefinitions::defineLayer<L>(visibility,name);
		//template<> G4LogicalVolume* defineLayer<Passive>(G4bool visibility,G4String name);
	}


	template<Layers L> G4LogicalVolume* defineSection(G4bool visibility,G4String name) {
		return sectionDefinitions::defineSection<L>(visibility,name);
	}


	G4LogicalVolume* definePassiveChipLayer(G4bool visibility,
								G4double thickness);


	G4LogicalVolume* definePassiveChipSection();

	G4LogicalVolume* defineActiveChipStrip();
	G4LogicalVolume* defineActiveChipPixel();

	void placeLayer();
	void placeUnit();

	G4LogicalVolume* pGrandmother;
	G4LogicalVolume* pMother;

	static G4Material* pEmpty;
	static G4Material* pGlue;
	static G4Material* pChips;
	static G4Material* pSolder;
	static G4Material* pConducting;
	static G4Material* pSubstrate;

	static constexpr G4double HICWidth = 15*mm;
	static constexpr G4double HICLength = 271.6*mm;
	static constexpr  G4double HICThickness = 285*mm;
	static constexpr  G4double pixelLength = 30*um;
	static constexpr G4double HICSectionLength = 30*mm;
	static constexpr G4double passiveWidth = 1.2*mm;
};

#endif /* GEANT4_DEVELOPMENT_CLASSES_HICUNIT_H_ */
