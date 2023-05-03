/*
 * exactQ.cpp
 *
 *  Created on: May 2, 2023
 *      Author: local1
 */
#include <TH1F.h>
#include <TFile.h>
#include <TTree.h>
#include <TTreeReader.h>
#include <TLorentzVector.h>
#include <TVector3.h>

#include <cmath>

#include "info.h"

/*Calculates the relativistic momentum from the particle mass and kinetic energy*/
double momentum(double energy, const double mass){
	return std::sqrt((energy*energy)+(2*energy*mass));
}

/*Calculates a particles total energy from its momentum and mass*/
double totalEnergy(double momentum,const double mass) {
	return std::sqrt((mass*mass) + (momentum*momentum));
}

void printVector(TLorentzVector vec) {
	std::cout<<"("<<vec.X()<<","<<vec.Y()<<","<<vec.Z()<<") : "<<vec.E();
}

void exactQ() {
	auto hist = new TH1F("hist","Exact Q Value",1000,-100,100);
	hist->GetXaxis()->SetTitle("Q Value (MeV/c^2)");
	hist->GetYaxis()->SetTitle("Frequency");

	TFile tree("../quasi.root");

	TTree* data = nullptr;
	tree.GetObject("Particles",data);

	TTreeReader reader(data);

	TTreeReaderValue<double> P1x = {reader,"P1x"};
	TTreeReaderValue<double> P1y = {reader,"P1y"};
	TTreeReaderValue<double> P1z = {reader,"P1z"};
	TTreeReaderValue<double> E1 = {reader,"E1"};

	TTreeReaderValue<double> P2x = {reader,"P2x"};
	TTreeReaderValue<double> P2y = {reader,"P2y"};
	TTreeReaderValue<double> P2z = {reader,"P2z"};
	TTreeReaderValue<double> E2 = {reader,"E2"};

	//Beam 4-momentum
	const double beamM = MA;
	const double beamEk = ENERGY * A;
	const double beamP = momentum(beamEk,beamM);
	const double beamE = totalEnergy(beamP,beamM);
	auto lBeam = TLorentzVector(0,0,beamP,beamE);

	//Target proton 4-momentum
	const double targetM = Ma;
	auto lTarget = TLorentzVector(0,0,0,targetM);

	//Initial system 4-momentum
	auto momIn = lTarget + lBeam;

	//Reads each row of tree
	while (reader.Next()) {
		//Proton 1 4-momentum
		auto p1 = TVector3(*P1x,*P1y,*P1z);
		auto e1 = totalEnergy(p1.Mag(),Ma);
		auto lP1 = TLorentzVector(p1,e1);


		//Proton 2 momentum
		auto p2 = TVector3(*P2x,*P2y,*P2z);
		auto e2 = totalEnergy(p2.Mag(),Ma);
		auto lP2 = TLorentzVector(p2,e2);

		//Final system 4-momentum  (excluding fragment)
		auto momOut = lP1 + lP2;

		//Difference of start and final 4-momentum
		auto missing = momIn - momOut;

		std::cout<<"P1: ";
		printVector(lP1);
		std::cout<<std::endl;

		std::cout<<"P2: ";
		printVector(lP2);
		std::cout<<std::endl;

		std::cout<<"P1 + P2: ";
		printVector(lP1+lP2);
		std::cout<<std::endl;

		std::cout<<"T + B: ";
		printVector(lTarget+lBeam);
		std::cout<<std::endl;

		std::cout<<"Missing: ";
		printVector(missing);
		std::cout<<std::endl;

		std::cout<<"Q: "<<missing.M()<<std::endl;
		std::cout<<std::endl;
		/*
		std::cout<<"P1: "<<lP1.M()<<std::endl;
		std::cout<<"P2: "<<lP2.M()<<std::endl;
		std::cout<<"Frag: "<<MB<<std::endl;
		std::cout<<"Target: "<<lTarget.M()<<std::endl;
		std::cout<<"Beam: "<<lBeam.M()<<std::endl;
		std::cout<<"Missing: "<<missing.M()<<std::endl;
		*/
		//Q value
		auto qValue = missing.M() - MB;
		//std::cout<<qValue<<std::endl;
		hist->Fill(qValue);
	}
	hist->Draw();
}



