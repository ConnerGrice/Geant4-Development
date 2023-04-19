/*
 * q_value.cpp
 *
 *  Created on: Apr 18, 2023
 *      Author: local1
 */
#include <TFile.h>
#include <TCanvas.h>
#include <TH1F.h>
#include <TTree.h>
#include <TTreeReader.h>
#include <TVector3.h>
#include <TRandom3.h>
#include <TSystem.h>
#include <TLorentzVector.h>

#include <iostream>
#include <vector>
#include <map>

#include "info.h"

//Component parameters
const double protonMass = Ma;
const double fragMass = MB;

//Full width at half height at 1%
const double FWHW = 0.01/2.35;

typedef std::map<int,std::vector<TVector3>> eventContainer;

void fillStaveData(eventContainer& map,int event,double x1,double y1,double z1,
		double x2,double y2,double z2) {
	TVector3 p1{x1,y1,z1};
	TVector3 p2{x2,y2,z2};
	std::vector<TVector3> data{p1,p2};
	map[event] = data;
}

double momentum(double energy, const double mass){
	return sqrt((energy*energy)+(2*energy*mass));
}

TLorentzVector calculateLMomentum(int particleID,std::vector<TVector3> points,std::vector<double> energies,TRandom3& rand3) {
	TVector3 unitVector = (points[particleID+1] - points[particleID-1]).Unit();
	double momentumMagnitude = momentum(energies[particleID-1],protonMass);
	double momentumMagnitudeRandom = rand3.Gaus(momentumMagnitude,momentumMagnitude*FWHW);

	unitVector = momentumMagnitudeRandom*unitVector;
	return TLorentzVector(unitVector,energies[particleID-1]);
}

TLorentzVector calculateBeamMomentum() {
	//Beam momentum
	const double beamMass = MA;
	const double beamEnergy = ENERGY;
	const double beamMomentum = momentum(beamEnergy,beamMass);
	return TLorentzVector(0,0,beamMomentum,beamEnergy);
}

TLorentzVector calculateTargetMomentum() {
	//Target momentum
	const double targetMass = protonMass;
	return TLorentzVector(0,0,0,targetMass);
}

bool recordHit(int event,eventContainer& data,std::vector<TVector3>& collection,eventContainer::iterator& iter) {
	iter = data.find(event);
	if (iter != data.end()){
		collection.push_back(iter->second[0]);
		collection.push_back(iter->second[1]);
		return true;
	}
	return false;
}

void recordEnergies(int event, std::map<int,std::vector<double>>& data, std::vector<double>& collection, std::map<int,std::vector<double>>::iterator& iter) {
	iter = data.find(event);
	if (iter != data.end()) {
		collection = iter->second;
	}
}

void q_value() {
	//Declare histogram
	TH1F* hist = new TH1F("QHist","Q Value",1000,-13000,-11700);
	hist->GetXaxis()->SetTitle("Q Value (keV/c^2)");
	hist->GetYaxis()->SetTitle("Frequency");

	//Get results data
	TFile tree("../Results/data.root");

	//Get wanted branches from tree
	TTree* mergedTree= nullptr;
	tree.GetObject("StaveB",mergedTree);
	int nB = mergedTree->GetEntries();

	TTree* staveC = nullptr;
	tree.GetObject("StaveC",staveC);
	int nC = staveC->GetEntries();

	TTree* staveD = nullptr;
	tree.GetObject("StaveD",staveD);
	int nD = staveD->GetEntries();

	TTree* califa = nullptr;
	tree.GetObject("CALIFA",califa);
	int nE = califa->GetEntries();

	//Get original data tree
	TFile quasi("../quasi.root");
	TTree* generator = nullptr;
	quasi.GetObject("Particles",generator);
	int nGenerator = generator->GetEntries();

	//Combine branches together
	mergedTree->AddFriend(staveC);
	mergedTree->AddFriend(staveD);
	mergedTree->AddFriend(califa);

	//Get values from tree using the reader
	TTreeReader reader(mergedTree);

	TTreeReaderValue<double> bX1 = {reader,"xPos1"};
	TTreeReaderValue<double> bY1 = {reader,"yPos1"};
	TTreeReaderValue<double> bZ1 = {reader,"zPos1"};
	TTreeReaderValue<double> bX2 = {reader,"xPos2"};
	TTreeReaderValue<double> bY2 = {reader,"yPos2"};
	TTreeReaderValue<double> bZ2 = {reader,"zPos2"};
	TTreeReaderValue<int> bEvnt = {reader,"Event"};

	TTreeReaderValue<double> cX1 = {reader,"StaveC.xPos1"};
	TTreeReaderValue<double> cY1 = {reader,"StaveC.yPos1"};
	TTreeReaderValue<double> cZ1 = {reader,"StaveC.zPos1"};
	TTreeReaderValue<double> cX2 = {reader,"StaveC.xPos2"};
	TTreeReaderValue<double> cY2 = {reader,"StaveC.yPos2"};
	TTreeReaderValue<double> cZ2 = {reader,"StaveC.zPos2"};
	TTreeReaderValue<int> cEvnt = {reader,"StaveC.Event"};

	TTreeReaderValue<double> dX1 = {reader,"StaveD.xPos1"};
	TTreeReaderValue<double> dY1 = {reader,"StaveD.yPos1"};
	TTreeReaderValue<double> dZ1 = {reader,"StaveD.zPos1"};
	TTreeReaderValue<double> dX2 = {reader,"StaveD.xPos2"};
	TTreeReaderValue<double> dY2 = {reader,"StaveD.yPos2"};
	TTreeReaderValue<double> dZ2 = {reader,"StaveD.zPos2"};
	TTreeReaderValue<int> dEvnt = {reader,"StaveD.Event"};

	TTreeReaderValue<double> e1 = {reader,"CALIFA.p1Energy"};
	TTreeReaderValue<double> e2 = {reader,"CALIFA.p2Energy"};
	TTreeReaderValue<int> eEvnt = {reader,"CALIFA.Event"};

	//Initialise data containers
	eventContainer bData;
	eventContainer cData;
	eventContainer dData;
	std::map<int,TVector3> fragData;
	std::map<int,std::vector<double>> ergyData;

	//Loop through each value inside the tree
	int c = 0;
	while (reader.Next()) {
		//Only fill containers until the data from the branch runs out
		if (c < nB) {
			fillStaveData(bData,*bEvnt,*bX1,*bY1,*bZ1,
					*bX2,*bY2,*bZ2);
		}

		if (c < nC) {
			fillStaveData(cData,*cEvnt,*cX1,*cY1,*cZ1,
					*cX2,*cY2,*cZ2);
		}

		if (c < nD) {
			fillStaveData(dData,*dEvnt,*dX1,*dY1,*dZ1,
					*dX2,*dY2,*dZ2);
		}

		if (c < nE) {
			std::vector<double> energy{*e1,*e2};
			ergyData[*eEvnt] = energy;
		}
		c++;
	}

	//Data containers for each event
	std::map<int,std::pair<std::vector<double>,std::vector<TVector3>>> events;

	//Loop though all values collected from tree
	for (int i = 0; i < nGenerator; i++) {
		std::vector<TVector3> particle;
		std::vector<double> energy;

		eventContainer::iterator iter;
		bool bHit = recordHit(i,bData,particle,iter);
		bool cHit = recordHit(i,cData,particle,iter);
		bool dHit = recordHit(i,dData,particle,iter);

		std::map<int,std::vector<double>>::iterator eIter;
		recordEnergies(i,ergyData,energy,eIter);

		//Only add value events to the data container
		if (bHit + cHit + dHit >= 2) {
			events[i] = std::make_pair(energy,particle);
		}
	}

	//Random generator
	gRandom = new TRandom3();
	gRandom->SetSeed(0);
	TRandom3 rand3;
	rand3.SetSeed(0);

	//Calculate initial momentum
	TLorentzVector beamLMomentum = calculateBeamMomentum();
	TLorentzVector targetLMomentum = calculateTargetMomentum();
	TLorentzVector momentumIn = beamLMomentum + targetLMomentum;

	//Loop through each valid event
	for (const auto& event : events) {
		//Get values from container
		std::vector<TVector3> points = event.second.second;
		std::vector<double> energies = event.second.first;

		//Calculate momentums for each particle
		TLorentzVector p1LMomentum = calculateLMomentum(1,points,energies,rand3);
		TLorentzVector p2LMomentum = calculateLMomentum(2,points,energies,rand3);

		//Calculate momentum after scattering
		TLorentzVector momentumOut = p1LMomentum + p2LMomentum;

		//Calulate Q value
		TLorentzVector missingLMomentum = momentumIn - momentumOut;
		double qValue = missingLMomentum.M()-fragMass;

		std::cout<<qValue<<std::endl;
		hist->Fill(qValue);

	}

	TCanvas* cv = new TCanvas();
	hist->Draw();
	cv->SaveAs("../Results/QValue.root");

	tree.Close();
	quasi.Close();
}

