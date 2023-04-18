/*
 * DevDetectorConstruction.h
 *
 *  Created on: Feb 24, 2023
 *      Author: local1
 */

#ifndef INCLUDE_DEVDETECTORCONSTRUCTION_H_
#define INCLUDE_DEVDETECTORCONSTRUCTION_H_

#include <G4VUserDetectorConstruction.hh>

#include <G4LogicalVolume.hh>
#include <G4PVPlacement.hh>
#include <G4Material.hh>
#include <G4Box.hh>
#include <G4Tubs.hh>
#include <G4Sphere.hh>
#include <G4SystemOfUnits.hh>
#include <G4VisAttributes.hh>

#include "DevSensitiveDetector.h"
#include "Stave.h"
#include "Materials.h"
#include "parameters.h"

class DevDetectorConstruction: public G4VUserDetectorConstruction {
public:
	DevDetectorConstruction();
	virtual ~DevDetectorConstruction();

public:
	G4VPhysicalVolume* Construct() override;
	void ConstructSDandField() override;

private:
	//Build a complete layer for the detector
	void buildLayer(G4String name, G4int numOfHICs);

	//Build the particle source and its mylar jacket
	void buildSource(G4bool visibility);

	void buildCALIFA(G4bool visibility);

	G4LogicalVolume* pLogicalWorld;
	HICUnit HICConstructor;

	G4Material* pWorldMat;
	G4Material* pTargetMat;
	G4Material* pMylarMat;
	G4Material* pCalifaMat;

};
#endif /* INCLUDE_DEVDETECTORCONSTRUCTION_H_ */
