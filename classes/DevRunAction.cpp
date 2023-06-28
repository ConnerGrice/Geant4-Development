/*
 * DevRunAction.cpp
 *
 *  Created on: Mar 13, 2023
 *      Author: local1
 */

#include "DevRunAction.h"

DevRunAction::DevRunAction(): nGood(0),nAlright(0),nBad(0),nInvalid(0) {
	G4AnalysisManager* manager = G4AnalysisManager::Instance();

	//Combines results from each worker into a single file
	manager->SetNtupleMerging(true);

	//Defines output file
	manager->SetFileName("../Results/data.root");

	//Create a table for each stave layer
	metricsTable("StaveB",0,manager);
	metricsTable("StaveC",1,manager);
	metricsTable("StaveD",2,manager);
	energyTable(3,manager);
}


DevRunAction::~DevRunAction() {
	delete G4AnalysisManager::Instance();
}

G4Run* DevRunAction::GenerateRun() {
	//Use custom Run class instead of standard G4Run
	return new DevRun;
}

void DevRunAction::BeginOfRunAction(const G4Run*) {
	if (IsMaster())
		G4cout<<"RUN MASTER START================================"<<G4endl;
	else
		G4cout<<"RUN LOCAL START ================================"<<G4endl;

	G4AnalysisManager* manager = G4AnalysisManager::Instance();
	manager->OpenFile();
}

void DevRunAction::metricsTable(G4String name, G4int tupleID, G4AnalysisManager* manager) {

	//Defines all columns that will be needed
	manager->CreateNtuple(name,"Metrics");
	manager->CreateNtupleDColumn("xDiff1");
	manager->CreateNtupleDColumn("yDiff1");
	manager->CreateNtupleDColumn("zDiff1");
	manager->CreateNtupleDColumn("xDiff2");
	manager->CreateNtupleDColumn("yDiff2");
	manager->CreateNtupleDColumn("zDiff2");

	manager->CreateNtupleDColumn("xPos1");
	manager->CreateNtupleDColumn("yPos1");
	manager->CreateNtupleDColumn("zPos1");
	manager->CreateNtupleDColumn("xPos2");
	manager->CreateNtupleDColumn("yPos2");
	manager->CreateNtupleDColumn("zPos2");

	manager->CreateNtupleIColumn("Event");

	manager->FinishNtuple(tupleID);
}

void DevRunAction::energyTable(G4int tupleID,G4AnalysisManager* manager) {
	manager->CreateNtuple("CALIFA","Energy");
	manager->CreateNtupleDColumn("p1Energy");
	manager->CreateNtupleDColumn("p2Energy");
	manager->CreateNtupleDColumn("p1Gen");
	manager->CreateNtupleDColumn("p2Gen");
	manager->CreateNtupleDColumn("p1Theta");
	manager->CreateNtupleDColumn("p2Theta");
	manager->CreateNtupleDColumn("p1Phi");
	manager->CreateNtupleDColumn("p2Phi");
	manager->CreateNtupleIColumn("Event");
	manager->FinishNtuple(tupleID);
}

void DevRunAction::printTypeEff(const G4Run* aRun) {
	//Gets current run and converts it to my custom run
	const DevRun* run = static_cast<const DevRun*>(aRun);

	//Gets values for efficiency calculations from run current run
	nGood = run->getGood();
	nAlright = run->getAlright();
	nBad = run->getBad();
	nInvalid = run->getInvalid();

	//Calculates different values
	G4int successful = nGood + nAlright + nBad;
	G4int total = successful + nInvalid;
	G4double succEff = successful/(G4double)total;
	G4double goodEff = nGood/(G4double)successful;
	G4double alrightEff = nAlright/(G4double)successful;
	G4double badEff = nBad/(G4double)successful;

	//Prints results
	if (IsMaster())
		G4cout<<"MASTER RESULTS================================"<<G4endl;
	else
		G4cout<<"LOCAL RESULTS================================"<<G4endl;
	G4cout<<"Valid Events: "<<successful<<"/"<<total<<" = "<<succEff*100<<"%"<<G4endl;
	G4cout<<"---------------Level of Success-------------"<<G4endl;
	G4cout<<"Good	: "<<nGood<<"/"<<successful<<" = "<<goodEff*100<<"%"<<G4endl;
	G4cout<<"Alright: "<<nAlright<<"/"<<successful<<" = "<<alrightEff*100<<"%"<<G4endl;
	G4cout<<"Bad	: "<<nBad<<"/"<<successful<<" = "<<badEff*100<<"%"<<G4endl;
	G4cout<<"--------------------------------------------"<<G4endl;
}

void DevRunAction::EndOfRunAction(const G4Run* aRun) {

	//Print efficiency
	printTypeEff(aRun);

	G4AnalysisManager* manager = G4AnalysisManager::Instance();
	manager->Write();
	manager->CloseFile();

	if (IsMaster()) {
		G4cout<<"RUN MASTER END================================"<<G4endl;
		//Entres data into simple data file for special cases
		std::ofstream eff;
		eff.open("../Results/Eff.dat",std::ios_base::app);
		eff<<PADDING<<" "<<((nGood+nAlright+nBad)/100240.0)<<std::endl;
	} else
		G4cout<<"RUN LOCAL END ================================"<<G4endl;
}


