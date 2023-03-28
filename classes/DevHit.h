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

class DevHit: public G4VHit {
public:
	DevHit();
	virtual ~DevHit();

public:
	//Get-Set copy number
	inline G4int GetZPx() { return zPx; };
	inline void SetZPx(G4int num) { zPx = num; };

	inline G4int GetYPx() { return yPx; };
	inline void SetYPx(G4int num) { yPx = num; };

	inline G4int GetSegment() { return segment; };
	inline void SetSegment(G4int num) { segment = num; };

	inline G4int GetUnit() { return unit; };
	inline void SetUnit(G4int num) { unit = num; };

	inline G4int GetStave() { return stave; };
	inline void SetStave(G4int num) { stave = num; };


	//Get-Set trackID
	inline G4int GetTrackID() { return trkID; };
	inline void SetTrackID(G4int id) { trkID = id; };
private:
	G4int zPx;
	G4int yPx;
	G4int segment;
	G4int unit;
	G4int stave;
	G4int trkID;
};

typedef G4THitsCollection<DevHit> DevHitsCollection;

#endif /* CLASSES_DEVHIT_H_ */
