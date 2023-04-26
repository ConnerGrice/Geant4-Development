/*
 * energyLoss.cpp
 *
 *  Created on: Apr 25, 2023
 *      Author: local1
 */
#include <TFile.h>
#include <TTree.h>
#include <TTreeReader.h>
#include <TGraph.h>
#include <TF1.h>
#include <TCanvas.h>
#include <TAxis.h>
#include <TStyle.h>

void energyLoss() {

	//OPtio
	bool theta = true;

	TFile tree("../Results/data.root");

	TTree* califa = nullptr;
	tree.GetObject("CALIFA",califa);

	TTreeReader reader(califa);

	TTreeReaderValue<double> e1 = {reader,"p1Energy"};
	TTreeReaderValue<double> e1Gen = {reader,"p1Gen"};
	TTreeReaderValue<double> t1 = {reader,"p1Theta"};
	TTreeReaderValue<double> ph1 = {reader,"p1Phi"};
	TTreeReaderValue<int> event = {reader,"Event"};

	int n = califa->GetEntries();
	int c = 0;
	double e1Diff[n];
	double p1Theta[n];
	double p1Phi[n];

	while (reader.Next()) {

		double eDiff = *e1Gen - *e1;

		//std::cout<<*event<<": "<<*e1Gen<<" - "<<*e1<<" = "<<eDiff<<std::endl;

		e1Diff[c] = eDiff;
		p1Theta[c] = *t1;
		p1Phi[c] = *ph1;
		c++;
	}

	auto c1 = new TCanvas("c1","Energy Loss by Emission Angle");

	TGraph* plot;

	if (theta) {
		plot = new TGraph(n,p1Theta,e1Diff);
		plot->GetXaxis()->SetTitle("Emission angle theta (about x axis)");
		plot->GetYaxis()->SetTitle("Energy loss (MeV)");

		auto line = new TF1("gfit","pol2");

		plot->Fit("gfit","pol2");

		std::cout<<line->GetParameter(0)<<std::endl;

		gStyle->SetOptFit();
	} else{
		plot = new TGraph(n,p1Phi,e1Diff);
		plot->GetXaxis()->SetTitle("Emission angle phi (about z axis)");

		plot->GetYaxis()->SetTitle("Energy loss (MeV)");
	}

	plot->SetTitle("");
	plot->Draw("ap");
	c1->Print("../Results/Energy_Loss.svg");

	tree.Close();

}


