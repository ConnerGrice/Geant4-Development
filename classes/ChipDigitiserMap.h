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

#include "DevDetectorConstruction.h"

#include <map>


class ChipDigitiserMap {

	std::map<std::pair<G4int,G4int>, G4ThreeVector> table;

private:
	ChipDigitiserMap();

public:

	static ChipDigitiserMap* getInstance() {
		ChipDigitiserMap* inst = new ChipDigitiserMap();
		return inst;
	}
	G4ThreeVector getPosition(const std::pair<int,int> copyNo) { return table[copyNo]; };

	G4double pixelDim = 30*um;
	G4int yPixels = 460;
	G4int zPixels = 1000;
	G4int segments = 9;

};

#endif /* GEANT4_DEVELOPMENT_CLASSES_CHIPDIGITISERMAP_H_ */
