/*
 * DevStackingAction.h
 *
 *  Created on: Mar 14, 2023
 *      Author: local1
 */

#ifndef CLASSES_DEVSTACKINGACTION_H_
#define CLASSES_DEVSTACKINGACTION_H_

#include <G4UserStackingAction.hh>

#include <G4ClassificationOfNewTrack.hh>
#include <G4Track.hh>

class DevStackingAction: public G4UserStackingAction {
public:
	DevStackingAction();
	virtual ~DevStackingAction();

public:
	G4ClassificationOfNewTrack ClassifyNewTrack(const G4Track* aTrack) override;
	void NewStage() override;
	void PrepareNewEvent() override;
};

#endif /* CLASSES_DEVSTACKINGACTION_H_ */
