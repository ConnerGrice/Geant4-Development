/*
 * DevPhysicsList.h
 *
 *  Created on: Feb 24, 2023
 *      Author: local1
 */

#ifndef INCLUDE_DEVPHYSICSLIST_H_
#define INCLUDE_DEVPHYSICSLIST_H_

#include <G4VUserPhysicsList.hh>

class DevPhysicsList: public G4VUserPhysicsList {
public:
	DevPhysicsList();
	virtual ~DevPhysicsList();

public:
	void ConstructParticle() override;
	void ConstructProcess() override;
};

#endif /* INCLUDE_DEVPHYSICSLIST_H_ */
