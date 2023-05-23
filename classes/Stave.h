/*
 * Stave.h
 *
 *  Created on: Apr 14, 2023
 *      Author: local1
 */

#ifndef GEANT4_DEVELOPMENT_CLASSES_STAVE_H_
#define GEANT4_DEVELOPMENT_CLASSES_STAVE_H_

#include <G4PVPlacement.hh>
#include <G4SystemOfUnits.hh>
#include <G4Box.hh>
#include <G4Material.hh>
#include <G4LogicalVolume.hh>
#include <G4VisAttributes.hh>
#include <G4ThreeVector.hh>
#include <G4RotationMatrix.hh>

#include <string>

#include "HICUnit.h"
#include "Materials.h"
#include "parameters.h"

class Stave {
public:
	//Constructor
	Stave(G4int numOfHICs,HICUnit unitConstructor,G4LogicalVolume* root);

	//Generates the physical volumes for each stave in the detector layer.
	void placeStaves(G4bool show);

private:

	//Calculates width of stave given by number of HIC units
	inline G4double getWidth() {
		return (HIC.getWidth()*nHICs) - (nHICs*HIC.getPassiveWidth()) + (PADDING*2);
	}

	//Calculates distance stave is from centre
	inline G4double getRadius() {
		return (staveWidth/(2*tan(angle/2.0)))+plateThickness/2.0+fleeceThickness;
	}

	//Defines the mother stave volume
	G4LogicalVolume* defineMother(G4bool visibility);

	//Defines the plate volume
	G4LogicalVolume* definePlate(G4bool visibility);

	//Defines the fleece mother volume
	G4LogicalVolume* defineFleece(G4bool visibility);

	//Generates physical volumes for the plate and fleece
	void buildPlate(G4bool coldVis, G4bool fleeceVis);

	//Generates the physical volume for the stave and the HIC units
	void buildStave(G4bool show);

	//Stave parameter variables
	G4int nHICs;
	G4double staveWidth;
	G4double staveRadius;

	//Volumes
	G4LogicalVolume* pWorldVol;
	G4LogicalVolume* pMother;

	//HIC builder
	HICUnit HIC;

	//Materials
	G4Material* pEmpty;
	G4Material* pPlateMat;
	G4Material* pFleeceMat;

	//Constant stave parameters
	static constexpr G4double staveLength = 320*mm;
	static constexpr G4double staveThickness = 850*um;
	static constexpr G4double fleeceThickness = 20*um;
	static constexpr G4double plateThickness = 240*um;
	static constexpr G4int nSides = 8;
	static constexpr G4double angle = (2*M_PI)/nSides;
	static constexpr G4double offsetAngle = M_PI/8;
};

#endif /* GEANT4_DEVELOPMENT_CLASSES_STAVE_H_ */
