/*
 * Stave.h
 *
 *  Created on: Apr 14, 2023
 *      Author: local1
 */

#ifndef GEANT4_DEVELOPMENT_CLASSES_STAVE_H_
#define GEANT4_DEVELOPMENT_CLASSES_STAVE_H_

#include "HICUnit.h"
#include "parameters.h"

class Stave {
public:
	Stave(G4int numOfHICs,G4LogicalVolume* root);

	inline G4double getWidth() {
		return (unitConstructor.HICWidth*nHICs) + (PADDING*2);
	}

	inline G4double getRadius() {
		return (staveWidth/tan(angle/2.0))+staveThickness/2.0+fleeceThickness;
	}

	G4LogicalVolume* defineMother(G4bool visibility);
	G4LogicalVolume* definePlate();
	G4LogicalVolume* defineFleece();

	void buildPlate();

	void buildStave();

	G4LogicalVolume* pWorldVol;
	G4LogicalVolume* pMother;
	HICUnit unitConstructor;

	G4Material* pEmpty;
	G4Material* pPlateMat;
	G4Material* pFleeceMat;

	G4int nHICs;
	G4double staveWidth;
	G4double staveRadius;

	static constexpr G4double staveLength = 320*mm;
	static constexpr G4double staveThickness = 850*um;
	static constexpr G4double fleeceThickness = 20*um;
	static constexpr G4double angle = M_PI/3.0;
};

#endif /* GEANT4_DEVELOPMENT_CLASSES_STAVE_H_ */
