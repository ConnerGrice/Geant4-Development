/*
 * HCIParameterisation.h
 *
 *  Created on: Mar 6, 2023
 *      Author: local1
 */

#ifndef CLASSES_HCIPARAMETERISATION_H_
#define CLASSES_HCIPARAMETERISATION_H_

#include <G4VPVParameterisation.hh>
#include <G4ThreeVector.hh>
class HCIParameterisation: public G4VPVParameterisation {
public:
	HCIParameterisation();
	virtual ~HCIParameterisation();

public:
	void ComputeTransformation(const G4int copyNo, G4VPhysicalVolume* phyVol) const;

private:
	G4double ComputeZ(G4int copyNo);
};

#endif /* CLASSES_HCIPARAMETERISATION_H_ */
