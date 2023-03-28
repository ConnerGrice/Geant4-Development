/*
 * HICParameterisation.cpp
 *
 *  Created on: Mar 27, 2023
 *      Author: local1
 */

#include "HICParameterisation.h"

HICParameterisation::HICParameterisation() {
}

HICParameterisation::~HICParameterisation() {
}

void HICParameterisation::ComputeTransformation(const G4int copyNo,G4VPhysicalVolume* physVol) const {
	ChipDigitiserMap* map = ChipDigitiserMap::get();
	G4int xPixels = 460;
	G4int zPixels = 1000;

	if (copyNo < xPixels + zPixels) {
		physVol->SetTranslation(map->get_position(copyNo));
	} else {

	}
	physVol->SetRotation(0);

}

