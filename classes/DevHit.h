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
	inline G4int GetCopyNo() { return copyNo; };
	inline void SetCopyNo(G4int num) { copyNo = num; };

private:
	G4int copyNo;
};

typedef G4THitsCollection<DevHit> DevHitsCollection;

#endif /* CLASSES_DEVHIT_H_ */
