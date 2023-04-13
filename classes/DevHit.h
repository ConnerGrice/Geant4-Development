/*
 * DevHit.h
 *
 *  Created on: Mar 10, 2023
 *      Author: local1
 */

#ifndef CLASSES_DEVHIT_H_
#define CLASSES_DEVHIT_H_

#include <G4VHit.hh>

#include <G4THitsCollection.hh>
#include <G4ThreeVector.hh>
class DevHit: public G4VHit {
public:
	DevHit(): evntID(0),trkID(0),exactPos(G4ThreeVector()),digitPos(G4ThreeVector()) {};
	virtual ~DevHit();

public:
	inline G4ThreeVector GetExactPosition() { return exactPos; };
	inline void SetExactPosition(G4ThreeVector num) { exactPos = num; };

	inline G4ThreeVector GetDigitisedPosition() { return digitPos; };
	inline void SetDigitisedPosition(G4ThreeVector num) { digitPos = num; };

	inline G4int GetTrackID() { return trkID; };
	inline void SetTrackID(G4int id) { trkID = id; };
private:
	G4int evntID;
	G4int trkID;

	G4ThreeVector exactPos;
	G4ThreeVector digitPos;
};

typedef G4THitsCollection<DevHit> DevHitsCollection;

#endif /* CLASSES_DEVHIT_H_ */
