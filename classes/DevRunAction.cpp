/*
 * DevRunAction.cpp
 *
 *  Created on: Mar 13, 2023
 *      Author: local1
 */

#include "DevRunAction.h"

DevRunAction::DevRunAction(): bTotal(0),cTotal(0),dTotal(0),
nGood(0),nAlright(0),nBad(0),nInvalid(0) {

	G4AnalysisManager* manager = G4AnalysisManager::Instance();
	manager->SetNtupleMerging(true);

	manager->SetFileName("../Results/data.root");

	metricsTable("StaveB",0,manager);
	metricsTable("StaveC",1,manager);
	metricsTable("StaveD",2,manager);
}


DevRunAction::~DevRunAction() {
	delete G4AnalysisManager::Instance();
}

void DevRunAction::BeginOfRunAction(const G4Run*) {
	G4cout<<"RUN START================================"<<G4endl;
	G4AnalysisManager* manager = G4AnalysisManager::Instance();
	manager->OpenFile();
}

void DevRunAction::metricsTable(G4String name, G4int tupleID, G4AnalysisManager* manager) {

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

void DevRunAction::printCount() {
	G4cout<<"StaveB Count: "<<bTotal<<G4endl;
	G4cout<<"StaveD Count: "<<dTotal<<G4endl;
	G4cout<<"StaveC Count: "<<cTotal<<G4endl;

}

void DevRunAction::printCountEff() {
	G4double dEff = dTotal/200480.0;
	G4cout<<"StaveD Eff: "<<dTotal<<"/200480"<<" = "<<dEff<<G4endl;
	G4double bEff = bTotal/200480.0;
	G4cout<<"StaveB Eff: "<<bTotal<<"/200480"<<" = "<<bEff<<G4endl;
	G4double cEff = cTotal/200480.0;
	G4cout<<"StaveC Eff: "<<cTotal<<"/200480"<<" = "<<cEff<<G4endl;
}

void DevRunAction::printType() {
	G4cout<<"Good Count: "<<nGood<<G4endl;
	G4cout<<"Alright Count: "<<nAlright<<G4endl;
	G4cout<<"Bad Count: "<<nBad<<G4endl;
}

void DevRunAction::printTypeEff() {
	G4int successful = nGood + nAlright + nBad;
	G4double succEff = successful/100240.0;
	G4double goodEff = nGood/(G4double)successful;
	G4double alrightEff = nAlright/(G4double)successful;
	G4double badEff = nBad/(G4double)successful;

	G4cout<<"==============EFFICIENCY OVERVIEW==========="<<G4endl;
	G4cout<<"Valid Events: "<<successful<<"/100240 = "<<succEff*100<<"%"<<G4endl;
	G4cout<<"---------------Level of Success-------------"<<G4endl;
	G4cout<<"Good	: "<<nGood<<"/"<<successful<<" = "<<goodEff*100<<"%"<<G4endl;
	G4cout<<"Alright: "<<nAlright<<"/"<<successful<<" = "<<alrightEff*100<<"%"<<G4endl;
	G4cout<<"Bad	: "<<nBad<<"/"<<successful<<" = "<<badEff*100<<"%"<<G4endl;
	G4cout<<"--------------------------------------------"<<G4endl;
}

void DevRunAction::EndOfRunAction(const G4Run*) {
	std::ofstream eff;
	eff.open("../Results/Eff.dat",std::ios_base::app);
	eff<<PADDING<<" "<<((nGood+nAlright+nBad)/100240.0)<<std::endl;

	printTypeEff();

	G4AnalysisManager* manager = G4AnalysisManager::Instance();
	manager->Write();
	manager->CloseFile();

	G4cout<<"RUN END==============================="<<G4endl;

}


