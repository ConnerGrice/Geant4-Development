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
	inline G4int GetCopyNo() { return copyNo; };
	inline void SetCopyNo(G4int num) { copyNo = num; };

	//Get-Set trackID
	inline G4int GetTrackID() { return trkID; };
	inline void SetTrackID(G4int id) { trkID = id; };
private:
	G4int copyNo;
	G4int trkID;
};

typedef G4THitsCollection<DevHit> DevHitsCollection;

#endif /* CLASSES_DEVHIT_H_ */
