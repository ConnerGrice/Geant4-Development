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
#include <G4PVDivision.hh>
#include <G4ReplicatedSlice.hh>
#include <G4NistManager.hh>
#include <G4Material.hh>
#include <G4Box.hh>
#include <G4Tubs.hh>
#include <G4SystemOfUnits.hh>
#include <G4AssemblyVolume.hh>
#include <G4Transform3D.hh>
#include <G4Colour.hh>
#include <G4VisAttributes.hh>

#include <cmath>
#include <vector>

#include <G4VUserDetectorConstruction.hh>

#include "DevSensitiveDetector.h"
#include "parameters.h"

class DevDetectorConstruction: public G4VUserDetectorConstruction {
public:
	DevDetectorConstruction();
	virtual ~DevDetectorConstruction();

public:
	G4VPhysicalVolume* Construct() override;
	void ConstructSDandField() override;

private:
	//Initialises material definitions
	void DefineMaterials();

	//Methods for constructing a stave
	//Gets the width of the stave based on the number of HCIs
	inline G4double getPlateWidth(G4int numOfHCIs){
		return (HCIWidth*numOfHCIs)/2.0 + PADDING;
	}

	//Gets the radius of the hexagon in order to make all staves touch
	inline G4double getPlateRadius(G4double plateWidth){
		return (plateWidth/tan(angle/2.0))+plateThickness/2+fleeceThickness;
	}

	//Defines volume containing all stave components
	G4LogicalVolume* staveMother(G4double width);

	//Defines plate and fleece volumes
	G4LogicalVolume* coldPlate(G4double width);
	G4LogicalVolume* fleece(G4double width);

	//Builds the plate the HCI units are attached to
	void buildPlate(G4double width);

	//Defines volume that will contain a HCI unit
	G4LogicalVolume* HCIUnitMother();

	//Defines volume that will contain a layer a segment of that layer
	G4LogicalVolume* HCILayerMother(G4double thickness,G4String name);
	G4LogicalVolume* HCISegment(G4double thickness,G4String name,G4Material* material,G4VisAttributes* visual);

	G4LogicalVolume* HCIPixelStrip(G4double thickness,G4String name, G4Material* material,G4VisAttributes* visual);

	G4LogicalVolume* HCIPixel(G4double thickness,G4String name, G4Material* material,G4VisAttributes* visual);

	//Generates physical volumes for a HCI layer
	void buildHCILayer(G4String name,G4double thickness, G4Material* mat,G4VisAttributes* visual,G4ThreeVector pos);

	//Builds a single HCI unit
	void buildHCI(G4String name);

	//Builds all staves
	void ConstructStaves(G4int numOfHCIs, G4String name);

	G4LogicalVolume* pLogicalWorld;
	G4LogicalVolume* staveL;
	G4LogicalVolume* HCIUnitL;

	G4Material* pWorldMat;
	G4Material* pPlateMat;
	G4Material* pFleeceMat;
	G4Material* pGlue;
	G4Material* pChips;
	G4Material* pSolder;
	G4Material* pConducting;
	G4Material* pSubstrate;
	G4Material* pTarget;

public:
	G4double HCIWidth;
	G4double staveLength;
	G4double HCILength;
	G4double HCIUnitThickness;
	G4double HCIPixelSide;
	G4int sides;
	G4double angle;
	G4double plateThickness;
	G4double fleeceThickness;
};
#endif /* INCLUDE_DEVDETECTORCONSTRUCTION_H_ */
