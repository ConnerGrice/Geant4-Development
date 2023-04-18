/*
 * q_value.cpp
 *
 *  Created on: Apr 18, 2023
 *      Author: local1
 */
#include <TFile.h>
#include <TTree.h>
#include <TTreeReader.h>
#include <TVector3.h>

#include <iostream>
#include <vector>
#include <map>

typedef std::map<int,std::vector<TVector3>> eventContainer;

void fillStaveData(eventContainer& map,
		int event,double x1,double y1,double z1,
		double x2,double y2,double z2);

void q_value() {
	TFile results("../Results/data.root");
	TTree* staveB = nullptr;
	results.GetObject("StaveB",staveB);
	int nB = staveB->GetEntries();
	TTree* staveC = nullptr;
	results.GetObject("StaveC",staveC);
	int nC = staveC->GetEntries();
	TTree* staveD = nullptr;
	results.GetObject("StaveD",staveD);
	int nD = staveD->GetEntries();
	TTree* califa = nullptr;
	results.GetObject("CALIFA",califa);
	int nE = califa->GetEntries();

	TFile quasi("../quasi.root");
	TTree* generator = nullptr;
	quasi.GetObject("Particles",generator);
	int nGenerator = generator->GetEntries();

	staveB->AddFriend(staveC);
	staveB->AddFriend(staveD);
	staveB->AddFriend(califa);
	staveB->AddFriend(generator);

	TTreeReader reader(staveB);

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

	TTreeReaderValue<double> fragX = {reader,"Particles.PBx"};
	TTreeReaderValue<double> fragY = {reader,"Particles.PBy"};
	TTreeReaderValue<double> fragZ = {reader,"Particles.PBz_lab"};

	std::cout<<nB<<":"<<nC<<":"<<nD<<":"<<nE<<":"<<nGenerator<<std::endl;

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

		if (c < nGenerator) {
		TVector3 frag{*fragX,*fragY,*fragZ};
		fragData[c] = frag;
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

		auto fIter = fragData.find(i);
		if (fIter != fragData.end()) {
			particle.push_back(fIter->second);
		}

		auto iter = bData.find(i);
		if (iter != bData.end()){
			bHit = true;
			std::cout<<"B";
			particle.push_back(iter->second[0]);
			particle.push_back(iter->second[1]);
		}

		iter = cData.find(i);
		if (iter != cData.end()) {
			cHit = true;
			std::cout<<" C";
			particle.push_back(iter->second[0]);
			particle.push_back(iter->second[1]);
		}

		iter = dData.find(i);
		if (iter != dData.end()) {
			dHit = true;
			std::cout<<" D";
			particle.push_back(iter->second[0]);
			particle.push_back(iter->second[1]);
		}


		auto eIter = ergyData.find(i);
		if (eIter != ergyData.end()) {
			std::cout<<" E";
			energy = eIter->second;
		}

		if (bHit + cHit + dHit >= 2) {
			std::cout<<" "<<i<<std::endl;
			events[i] = std::make_pair(energy,particle);
		}
	}

	std::cout<<events.size()<<std::endl;

	for (const auto& event : events) {
		std::cout<<event.second.second.size()<<std::endl;
	}

}





void fillStaveData(eventContainer& map,
		int event,double x1,double y1,double z1,
		double x2,double y2,double z2) {

	TVector3 p1{x1,y1,z1};
	TVector3 p2{x2,y2,z2};
	std::vector<TVector3> data{p1,p2};
	map[event] = data;
}

