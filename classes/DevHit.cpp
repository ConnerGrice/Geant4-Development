/*
 * DevHit.cpp
 *
 *  Created on: Mar 10, 2023
 *      Author: local1
 */

#include "DevHit.h"

DevHit::DevHit():zPx(0),yPx(0),segment(0),unit(0),stave(0),trkID(0){
	exactPos = G4ThreeVector(0,0,0);
	digitPos = G4ThreeVector(0,0,0);

}

DevHit::~DevHit() {
}

