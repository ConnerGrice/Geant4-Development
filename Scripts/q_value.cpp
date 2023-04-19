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

void q_value() {

	TH1F* hist = new TH1F("QHist","Q Value",1000,-13000,-11700);
	hist->GetXaxis()->SetTitle("Q Value (keV/c^2)");
	hist->GetYaxis()->SetTitle("Frequency");

	TFile tree("../Results/data.root");

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

	TFile quasi("../quasi.root");
	TTree* generator = nullptr;
	quasi.GetObject("Particles",generator);
	int nGenerator = generator->GetEntries();

	mergedTree->AddFriend(staveC);
	mergedTree->AddFriend(staveD);
	mergedTree->AddFriend(califa);


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

	eventContainer bData;
	eventContainer cData;
	eventContainer dData;
	std::map<int,TVector3> fragData;
	std::map<int,std::vector<double>> ergyData;

	int c = 0;
	while (reader.Next()) {
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

	std::map<int,std::pair<std::vector<double>,std::vector<TVector3>>> events;

	for (int i = 0; i < nGenerator; i++) {
		std::vector<TVector3> particle;
		std::vector<double> energy;

		bool bHit = false;
		bool cHit = false;
		bool dHit = false;

		auto iter = bData.find(i);
		if (iter != bData.end()){
			bHit = true;
			particle.push_back(iter->second[0]);
			particle.push_back(iter->second[1]);
		}

		iter = cData.find(i);
		if (iter != cData.end()) {
			cHit = true;
			particle.push_back(iter->second[0]);
			particle.push_back(iter->second[1]);
		}

		iter = dData.find(i);
		if (iter != dData.end()) {
			dHit = true;
			particle.push_back(iter->second[0]);
			particle.push_back(iter->second[1]);
		}

		auto eIter = ergyData.find(i);
		if (eIter != ergyData.end()) {
			energy = eIter->second;
		}

		if (bHit + cHit + dHit >= 2) {
			events[i] = std::make_pair(energy,particle);
		}
	}

	std::cout<<events.size()<<std::endl;

	gRandom = new TRandom3();
	gRandom->SetSeed(0);
	TRandom3 rand3;
	rand3.SetSeed(0);

	const double protonMass = Ma;
	const double fragMass = MB;
	const double FWHW = 0.01/2.35;

	const double beamMass = MA;
	const double beamEnergy = ENERGY;
	const double beamMomentum = momentum(beamEnergy,beamMass);
	TLorentzVector beamLMomentum = TLorentzVector(0,0,beamMomentum,beamEnergy);

	const double targetMass = protonMass;
	TLorentzVector targetLMomentum = TLorentzVector(0,0,0,targetMass);

	TLorentzVector momentumIn = beamLMomentum + targetLMomentum;


	for (const auto& event : events) {
		std::vector<TVector3> points = event.second.second;
		std::vector<double> energies = event.second.first;

		TVector3 p1Direction = (points[2] - points[0]).Unit();
		TVector3 p2Direction = (points[3] - points[1]).Unit();

		double p1MomMag = momentum(energies[0],protonMass);
		double p2MomMag = momentum(energies[1],protonMass);
		double p1MomMagRand = rand3.Gaus(p1MomMag,p1MomMag*FWHW);
		double p2MomMagRand = rand3.Gaus(p2MomMag,p2MomMag*FWHW);

		p1Direction = p1MomMagRand*p1Direction;
		p2Direction = p2MomMagRand*p2Direction;

		TLorentzVector p1LMomentum = TLorentzVector(p1Direction,energies[0]);
		TLorentzVector p2LMomentum = TLorentzVector(p2Direction,energies[1]);

		TLorentzVector momentumOut = p1LMomentum + p2LMomentum;
		TLorentzVector missingLMomentum = momentumIn - momentumOut;
		double qValue = missingLMomentum.M()-fragMass;

		std::cout<<qValue<<std::endl;
		hist->Fill(qValue);

	}

	TCanvas* cv = new TCanvas();
	hist->Draw();
	hist->Write();
	cv->SaveAs("../Results/QValue.root");



	tree.Close();
	quasi.Close();
}

