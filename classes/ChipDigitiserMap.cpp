/*
 * ChipDigitiserMap.cpp
 *
 *  Created on: Mar 27, 2023
 *      Author: local1
 */

#include "ChipDigitiserMap.h"

ChipDigitiserMap::ChipDigitiserMap() {
	//Position of first pixel
	G4ThreeVector init = G4ThreeVector(-7.5*mm+pixelDim/2,0,-135.8*mm+pixelDim/2);

	G4ThreeVector xShift = G4ThreeVector(pixelDim,0,0);
	G4ThreeVector zShift = G4ThreeVector(0,0,pixelDim);

	G4int xPixels = 460;
	G4int zPixels = 1000;

	for (int j=0; j<zPixels;j++) {
		for (int i=0;i<xPixels;i++) {

		}
	}

}

