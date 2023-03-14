/*
 * DevStackingAction.cpp
 *
 *  Created on: Mar 14, 2023
 *      Author: local1
 */

#include "DevStackingAction.h"

DevStackingAction::DevStackingAction() {
}

DevStackingAction::~DevStackingAction() {
}

G4ClassificationOfNewTrack DevStackingAction::ClassifyNewTrack(const G4Track* aTrack) {
	//deletes secondary particles
	G4int parentID = aTrack->GetParentID();
	if (parentID > 0) {
		return fKill;
	}
	return fUrgent;
}

void DevStackingAction::NewStage() {
}

void DevStackingAction::PrepareNewEvent() {
}
