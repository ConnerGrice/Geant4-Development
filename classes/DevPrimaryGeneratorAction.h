/*
 * DevPrimaryGeneratorAction.h
 *
 *  Created on: Feb 24, 2023
 *      Author: local1
 */

#ifndef INCLUDE_DEVPRIMARYGENERATORACTION_H_
#define INCLUDE_DEVPRIMARYGENERATORACTION_H_

#include <G4VUserPrimaryGeneratorAction.hh>

class DevPrimaryGeneratorAction: public G4VUserPrimaryGeneratorAction {
public:
	DevPrimaryGeneratorAction();
	virtual ~DevPrimaryGeneratorAction();

public:
	void GeneratePrimaries(G4Event*) override;
};

#endif /* INCLUDE_DEVPRIMARYGENERATORACTION_H_ */
