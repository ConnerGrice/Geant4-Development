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

	//Methods for constructing a stave
	inline G4double getPlateWidth(G4int numOfHCIs){
		return (HCIWidth*numOfHCIs)/2.0 + 4;
	}
	inline G4double getPlateRadius(G4double plateWidth, G4double plateThickness){
		return (plateWidth/tan(angle/2.0))+plateThickness;
	}

	G4LogicalVolume* staveMother(G4double width);
	G4LogicalVolume* coldPlate(G4double width);
	G4LogicalVolume* fleece(G4double width);
	G4LogicalVolume* HCIUnitMother();
	G4LogicalVolume* HCILayerMother(G4double thickness,G4String name);
	G4LogicalVolume* HCISegment(G4double thickness,G4String name,G4Material* material);
	void buildHCILayer(G4String name,G4double thickness, G4Material* mat,G4ThreeVector pos);
	void buildHCI(G4String name);

	void ConstructStaves(G4int numOfHCIs, G4String name);

	G4LogicalVolume* pLogicalWorld;
	G4LogicalVolume* HCIUnitL;
	G4Material* pWorldMat;
	G4Material* pPlateMat;
	G4Material* pFleeceMat;
	G4Material* pGlue;
	G4Material* pChips;
	G4Material* pSolder;
	G4Material* pConducting;
	G4Material* pSubstrate;
	//Constants for all staves
	G4double HCIWidth;
	G4double staveLength;
	G4double HCILength;
	G4double HCIUnitThickness;
	G4int sides;
	G4double angle;
};
#endif /* INCLUDE_DEVDETECTORCONSTRUCTION_H_ */
