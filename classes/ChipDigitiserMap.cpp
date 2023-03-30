/*
 * ChipDigitiserMap.cpp
 *
 *  Created on: Mar 27, 2023
 *      Author: local1
 *
 * A Singleton that represents a look-up-table that contains
 * a pixel position based on the copy number it is assigned.
 */

#include "ChipDigitiserMap.h"

ChipDigitiserMap* ChipDigitiserMap::instance = nullptr;

ChipDigitiserMap::ChipDigitiserMap() {
	/*
	 * Frame of reference:
	 * HIC Segment vertical,
	 * pixel (0,0) -> Top back (-15*mm,6.0*mm)
	 * Z direction 0 -> 999, +ve
	 * Y direction 0 -> 459, -ve
	 *
	 * Copy number meanings:
	 * 0,Z-pixels - (0 -> 999)
	 * 1,Y-pixels - (0 -> 459)
	 * 2,Segments - (0 -> 8)
	 * 4,Unit - (0-> 1,2,3) depends on stave B,C, or D
	 * 5,Stave - (0 -> 5)
	 * Layer - (0 -> 2)
	 */
	G4double zPos = -15*mm;
	G4double yPos;

	DevDetectorConstruction* construction;
	G4int numOfHIC = 2;
	G4double BWidth = construction->getPlateWidth(2);
	G4double BRadius = construction->getPlateRadius(BWidth);


	for (int i=0;i<zPixels;i++) {
		zPos += i*pixelDim;
		yPos = 6.9*mm;
		for (int j=0;j<yPixels;j++) {
			yPos += j*pixelDim;
			G4cout<<i<<","<<j<<G4endl;
			table[std::make_pair(i,j)] = G4ThreeVector(0,yPos,zPos);
		}
	}

}
/*
G4ThreeVector ChipDigitiserMap::getPosition(G4String sensName,
		const std::pair<int,int> copyNo) {
	switch(sensName) {
	case("StaveB"):
			return tableB[copyNo];
	case("StaveC"):
			return tableC[copyNo];
	case("StaveD"):
			return tableD[copyNo];
	}
}
*/
