/*
 * q_value.cpp
 *
 *  Created on: Apr 18, 2023
 *      Author: local1
 */
#include <TFile.h>
#include <TCanvas.h>
#include <TH1F.h>
#include <TGraph.h>
#include <TF1.h>
#include <TTree.h>
#include <TTreeReader.h>
#include <TVector3.h>
#include <TRandom3.h>
#include <TSystem.h>
#include <TStyle.h>
#include <TLorentzVector.h>

#include <iostream>
#include <vector>
#include <map>
#include <string>

#include "info.h"

//Component parameters
const double protonMass = Ma;
const double fragMass = MB;

//Full width at half height at 1%
const double FWHW = 0.01/2.35;

/*dict containing the position vectors of both particles*/
typedef std::map<int,std::vector<TVector3>> eventContainer;

/*Puts position values into the event container in vector format*/
void fillStaveData(eventContainer& map,int event,double x1,double y1,double z1,
		double x2,double y2,double z2) {
	TVector3 p1{x1,y1,z1};
	TVector3 p2{x2,y2,z2};
	std::vector<TVector3> data{p1,p2};
	map[event] = data;
}

/*
 * Checks if the particle data contains a given event ID
 * Builds array of particles position vectors for that event
 */
bool recordHit(int event,eventContainer& data,std::vector<TVector3>& collection,eventContainer::iterator& iter) {
	iter = data.find(event);
	if (iter != data.end()){
		collection.push_back(iter->second[0]);
		collection.push_back(iter->second[1]);
		return true;
	}
	return false;
}

/*
 * Checks if the energy data contains a value for a given event ID
 * Builds list of energies of each particle for that event
 */
bool recordEnergies(int event, std::map<int,std::vector<double>>& data, std::vector<double>& collection, std::map<int,std::vector<double>>::iterator& iter) {
	iter = data.find(event);
	if (iter != data.end()) {
		collection = iter->second;
		return true;
	}
	return false;
}

/*
 * Plots energy difference vs emission angle
 * Fits a quadratic and outputs the coefficients
 */
std::vector<double> energyLossParameters(int order,double* diff, double* angle,int size) {
	//Generates plot
	auto plot = new TGraph(size,angle,diff);

	//Fits quadratic curve
	std::string poly = "pol" + std::to_string(order);
	auto formula = poly.c_str();
	auto line = new TF1("gfit",formula);
	plot->Fit("gfit",formula);

	//builds vector containing parameters
	std::vector<double> output;
	for (int i = 0; i < order + 1; i++) {
		output.push_back(line->GetParameter(i));
	}

	return output;
}
/*Calculates the relativistic momentum from the particle mass and kinetic energy*/
double momentum(double energy, const double mass){
	return std::sqrt((energy*energy)+(2*energy*mass));
}

/*Calculates a particles total energy from its momentum and mass*/
double totalEnergy(double momentum,const double mass) {
	return std::sqrt((mass*mass) + (momentum*momentum));
}


/*Quadratic for energy loss correction*/
double func(double x, std::vector<double> coeff) {
	return coeff[0] + coeff[1]*x + coeff[2]*x*x;
}

/*
 * Calculates the 4-momentum of the C12 beam
 */
TLorentzVector calculateBeamMomentum() {
	//Beam momentum
	const double beamMass = MA;	//12 atomic mass units
	const double beamEnergy = ENERGY * A; //Total kinetic energy of beam (energy * 12)

	//Momentum and energy values
	const double beamMomentum = momentum(beamEnergy,beamMass);
	const double totalBeamEnergy = totalEnergy(beamMomentum,beamMass);

	//Beam along z axis
	auto output = TLorentzVector(0,0,beamMomentum,totalBeamEnergy);
	return output;
}

//Calculates 4-momentum of stationary target proton
TLorentzVector calculateTargetMomentum() {
	//Target momentum
	const double targetMass = protonMass;
	return TLorentzVector(0,0,0,targetMass);
}

/*Calculates a particles 4-momentum by:
 * - Calculates unit vector that represents direction of travel
 * - Calculates the energy approximation from energy reading and energy loss correction function
 * - Calculates the relativistic momentum magnitude of the particle
 * - Applies random 1% FWHW to momentum magnitude
 * - Calculates total energy of the particles using this momentum value
 * - Constructs 3-momentum vector using direction of travel vector and momentum magnitude
 * - Outputs 4-momentum vector with the total energy
 */
TLorentzVector calculateLMomentum(
		int particleID,std::vector<TVector3> points,
		std::vector<double> energies,TRandom3& rand3,std::vector<double> coefficients) {

	/*
	 * points = [p1layer1, p2layer1, p1layer2, p2layer2]
	 */
	//Particle direction of travel
	TVector3 unitVector = (points[particleID+1] - points[particleID-1]).Unit();


	/*
	 * energies = [energy1,energy2,emissionAngle1,emissionAngle2]
	 */
	//Energy + correction
	double energyApprox = energies[particleID-1] + func(energies[particleID+1],coefficients);

	//Particle momentum
	double momentumMagnitude = momentum(energyApprox,protonMass);
	double momentumMagnitudeRandom = rand3.Gaus(momentumMagnitude,momentumMagnitude*FWHW);

	//Total energy
	double totalMomentumEnergy = totalEnergy(momentumMagnitudeRandom,protonMass);

	//4-momentum vector
	unitVector = momentumMagnitudeRandom*unitVector;
	auto output = TLorentzVector(unitVector,totalMomentumEnergy);
	return output;
}

/*
 * MAIN SCRIPT
 */
void q_value() {
	//Declare histogram
	TH1F* hist = new TH1F("QHist","Q Value",1000,-20,20);
	hist->GetXaxis()->SetTitle("Q Value (MeV/c^2)");
	hist->GetYaxis()->SetTitle("Frequency");

	//Get results data
	TFile tree("../Results/data.root");

	//Get wanted branches from tree
	//and records the number of entries in each branch
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
	mergedTree->AddFriend(generator);

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
	TTreeReaderValue<double> e1Gen = {reader,"CALIFA.p1Gen"};
	TTreeReaderValue<double> e2Gen = {reader,"CALIFA.p2Gen"};
	TTreeReaderValue<double> t1 = {reader,"CALIFA.p1Theta"};
	TTreeReaderValue<double> t2 = {reader,"CALIFA.p2Theta"};
	TTreeReaderValue<int> eEvnt = {reader,"CALIFA.Event"};

	//Initialise data containers
	//Position vectors for each layer
	eventContainer bData;
	eventContainer cData;
	eventContainer dData;

	//Energy data for each event
	std::map<int,std::vector<double>> ergyData;

	//Arrays for energy correction plot
	double e1Diff[nE];
	double e2Diff[nE];
	double p1Theta[nE];
	double p2Theta[nE];

	//Loop through each value inside the tree
	int c = 0;
	while (reader.Next()) {
		//Only fill containers until the data from the branch runs out
		if (c < nB) {
			//Fill B layer positions
			fillStaveData(bData,*bEvnt,*bX1,*bY1,*bZ1,
					*bX2,*bY2,*bZ2);
		}

		if (c < nC) {
			//Fill C layer positions
			fillStaveData(cData,*cEvnt,*cX1,*cY1,*cZ1,
					*cX2,*cY2,*cZ2);
		}

		if (c < nD) {
			//Fill D layer positions
			fillStaveData(dData,*dEvnt,*dX1,*dY1,*dZ1,
					*dX2,*dY2,*dZ2);
		}

		if (c < nE) {
			//Fill energy values and their emission angle
			std::vector<double> energy{*e1,*e2,*t1,*t2};
			ergyData[*eEvnt] = energy;

			//Fill energy difference arrays
			e1Diff[c] = *e1Gen - *e1;
			e2Diff[c] = *e2Gen - *e2;
			p1Theta[c] = *t1;
			p2Theta[c] = *t2;
		}
		c++;
	}

	//Data containers for each event
	/*
	 *{eventID : ([energies vector],[position vectors vector]), ...}
	 */
	std::map<int,std::pair<std::vector<double>,std::vector<TVector3>>> events;

	//Loop though all values collected from tree
	for (int i = 0; i < nGenerator; i++) {

		//Map element vectors
		std::vector<TVector3> particle;
		std::vector<double> energy;

		//Only add to vector if particle hit layer for given event
		eventContainer::iterator iter;
		bool bHit = recordHit(i,bData,particle,iter);
		bool cHit = recordHit(i,cData,particle,iter);
		bool dHit = recordHit(i,dData,particle,iter);

		//Only add energy vector if energy value was given for given event
		std::map<int,std::vector<double>>::iterator eIter;
		bool calHit = recordEnergies(i,ergyData,energy,eIter);

		//Only add value to event map if the entire event was valid
		if ((bHit + cHit + dHit >= 2) && calHit)
			events[i] = std::make_pair(energy,particle);
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

	//Get energy loss function parameters for each particle
	auto p1Fit = energyLossParameters(2,e1Diff,p1Theta,nE);
	auto p2Fit = energyLossParameters(2,e2Diff,p2Theta,nE);

	//Loop through each valid event
	for (const auto& event : events) {
		//Get values from container
		std::vector<TVector3> points = event.second.second;
		std::vector<double> energies = event.second.first;

		//Calculate momentums for each particle
		TLorentzVector p1LMomentum = calculateLMomentum(1,points,energies,rand3,p1Fit);
		TLorentzVector p2LMomentum = calculateLMomentum(2,points,energies,rand3,p2Fit);

		//Calculate momentum after scattering
		TLorentzVector momentumOut = p1LMomentum + p2LMomentum;
		//std::cout<<"Particle MASS: "<<momentumOut.M()-2*protonMass<<std::endl;
		//std::cout<<"Fragment: "<<fragMass<<std::endl;
		//std::cout<<"Input: "<<momentumIn.M()<<std::endl;

		//Calculate total 4-momentum by finding the difference in initial and final momentum (excluding fragment)
		TLorentzVector missingLMomentum = momentumIn - momentumOut;

		//Remove mass of the fragment to get final q value
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

