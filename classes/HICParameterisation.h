/*
 * HICParameterisation.h
 *
 *  Created on: Mar 27, 2023
 *      Author: local1
 */

#ifndef GEANT4_DEVELOPMENT_CLASSES_HICPARAMETERISATION_H_
#define GEANT4_DEVELOPMENT_CLASSES_HICPARAMETERISATION_H_

#include <G4VPVParameterisation.hh>

class HICParameterisation: public G4VPVParameterisation {
public:
	HICParameterisation();
	virtual ~HICParameterisation();

public:
	void ComputeTransformation(const G4int copyNo,G4VPhysicalVolume*) const override;
	void ComputeDimensions(G4Box&,const G4int copyNo,const G4VPhysicalVolume*) const override;
	G4VSolid* ComputeSolid(const G4int copyNo,G4VPhysicalVolume*) override;
	G4Material* ComputeMaterial(const G4int copyNo,G4VPhysicalVolume*,const G4VTouchable* parentTouch=0) override;

};

#endif /* GEANT4_DEVELOPMENT_CLASSES_HICPARAMETERISATION_H_ */
