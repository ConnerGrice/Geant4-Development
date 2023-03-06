/*
 * DevDetectorConstruction.h
 *
 *  Created on: Feb 24, 2023
 *      Author: local1
 */

#ifndef INCLUDE_DEVDETECTORCONSTRUCTION_H_
#define INCLUDE_DEVDETECTORCONSTRUCTION_H_

#include <G4VPhysicalVolume.hh>
#include <G4LogicalVolume.hh>
#include <G4PVPlacement.hh>
#include <G4PVReplica.hh>
#include <G4NistManager.hh>
#include <G4Box.hh>
#include <G4SystemOfUnits.hh>
#include <G4AssemblyVolume.hh>
#include <G4Transform3D.hh>

#include <cmath>
#include <vector>

#include <G4VUserDetectorConstruction.hh>

class DevDetectorConstruction: public G4VUserDetectorConstruction {
public:
	DevDetectorConstruction();
	virtual ~DevDetectorConstruction();

public:
	G4VPhysicalVolume* Construct() override;

private:
	//Initialises material definitions
	void DefineMaterials();

	//Constructs a single layer of 9 segments in the HCI
	void ConstructHCILayer(G4String name,
							G4double width,
							G4double thickness,
							G4Material* material,
							G4RotationMatrix* rot,
							G4ThreeVector trans,
							G4int copyNo);
	void ConstructHCI(G4RotationMatrix* initRotation,G4ThreeVector initPosition);

	//Methods for constructing a stave
	inline G4double getPlateWidth(G4int numOfHCIs,G4double HCIWidth){
		return (HCIWidth*numOfHCIs)/2.0 + 4;
	}
	inline G4double getPlateRadius(G4double plateWidth, G4double angle, G4double plateThickness){
		return (plateWidth/tan(angle/2.0))+plateThickness;
	}

	void ConstructStaves(G4int numOfHCIs, G4String name);

	G4LogicalVolume* pLogicalWorld;
	G4Material* pWorldMat;
	G4Material* pPlateMat;
};
#endif /* INCLUDE_DEVDETECTORCONSTRUCTION_H_ */
