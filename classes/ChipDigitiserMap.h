/*
 * ChipDigitiserMap.h
 *
 *  Created on: Mar 27, 2023
 *      Author: local1
 */

#ifndef GEANT4_DEVELOPMENT_CLASSES_CHIPDIGITISERMAP_H_
#define GEANT4_DEVELOPMENT_CLASSES_CHIPDIGITISERMAP_H_

#include <G4SystemOfUnits.hh>
#include <G4ThreeVector.hh>

#include <DevDetectorConstruction.h>

#include <map>


class ChipDigitiserMap {

	std::map<G4int, G4ThreeVector> table;

private:
	ChipDigitiserMap();

public:

	static ChipDigitiserMap* getInstance() {
		ChipDigitiserMap* inst = new ChipDigitiserMap();
		return inst;
	}
	G4ThreeVector getPosition(const G4int copyNo) { return table[copyNo]; };

	G4double pixelDim = 30*um;
	G4int xPixels = 460;
	G4int zPixels = 1000;

};

#endif /* GEANT4_DEVELOPMENT_CLASSES_CHIPDIGITISERMAP_H_ */
