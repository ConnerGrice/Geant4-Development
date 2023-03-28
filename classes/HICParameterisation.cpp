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
	ChipDigitiserMap* map = ChipDigitiserMap::getInstance();
	G4ThreeVector origin;
	if (copyNo < xPixels * zPixels) {
		origin = map->getPosition(copyNo);
	} else {
		origin = G4ThreeVector(6.9*mm,0,0);
	}
//G4cout<<copyNo<<":"<<origin.getX()<<" "<<origin.getZ()<<G4endl;
	physVol->SetTranslation(origin);
	physVol->SetRotation(0);
}

void HICParameterisation::ComputeDimensions(G4Box& pixel,const G4int copyNo,const G4VPhysicalVolume*) const {
	if (copyNo < xPixels * zPixels) {
		pixel.SetXHalfLength(30*um);
		pixel.SetZHalfLength(30*um);
	} else {
		pixel.SetXHalfLength(0.6*mm);
		pixel.SetZHalfLength(15*mm);
	}
	pixel.SetYHalfLength(25*um);
}

