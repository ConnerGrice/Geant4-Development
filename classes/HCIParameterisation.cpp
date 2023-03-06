/*
 * HCIParameterisation.cpp
 *
 *  Created on: Mar 6, 2023
 *      Author: local1
 */

#include "HCIParameterisation.h"

HCIParameterisation::HCIParameterisation() {
	// TODO Auto-generated constructor stub

}

HCIParameterisation::~HCIParameterisation() {
	// TODO Auto-generated destructor stub
}



void HCIParameterisation::ComputeTransformation(const G4int copyNo,G4VPhysicalVolume* physVol) const{
	//9 sections a layer
	//8 layers
	//72 copy numbers
	G4int layer = copyNo/9;

	G4ThreeVector pos;
	switch (layer){
	case 0:
		if (copyNo-layer)
	}

}
