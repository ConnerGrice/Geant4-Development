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

	G4int copyNo = 0;

	for (int i=0;i<zPixels;i++) {
		for (int j=0;j<xPixels;j++){
			table[copyNo] = init + j*xShift;
		}
		init += zShift;
	}

}

static ChipDigitiserMap& ChipDigitiserMap::get() {
	static ChipDigitiserMap output;
	return output;
}
