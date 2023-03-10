/*
 * DevSensitiveDetector.h
 *
 *  Created on: Mar 10, 2023
 *      Author: local1
 */

#ifndef CLASSES_DEVSENSITIVEDETECTOR_H_
#define CLASSES_DEVSENSITIVEDETECTOR_H_

#include <G4VSensitiveDetector.hh>

#include <G4SDManager.hh>

#include "DevHit.h"

class DevSensitiveDetector: public G4VSensitiveDetector {
public:
	DevSensitiveDetector(G4String name, G4String collection);
	virtual ~DevSensitiveDetector();

public:
	void Initialize(G4HCofThisEvent*) override;
	G4bool ProcessHits(G4Step*,G4TouchableHistory*) override;
	void EndOfEvent(G4HCofThisEvent*) override;



private:
	DevHitsCollection* pHitCollection;
};

#endif /* CLASSES_DEVSENSITIVEDETECTOR_H_ */
