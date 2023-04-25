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
#include <TCanvas.h>
#include <TAxis.h>

void energyLoss() {

	TFile tree("../Results/data.root");

	TTree* califa = nullptr;
	tree.GetObject("CALIFA",califa);

	TTreeReader reader(califa);

	TTreeReaderValue<double> e1 = {reader,"p1Energy"};
	TTreeReaderValue<double> e2 = {reader,"p2Energy"};
	TTreeReaderValue<double> e1Gen = {reader,"p1Gen"};
	TTreeReaderValue<double> e2Gen = {reader,"p2Gen"};
	TTreeReaderValue<double> t1Gen = {reader,"p1Theta"};
	TTreeReaderValue<double> t2Gen = {reader,"p2Theta"};
	TTreeReaderValue<int> event = {reader,"Event"};

	int n = califa->GetEntries();
	int c = 0;
	double e1Diff[n];
	double p1Theta[n];
	double e2Diff[n];
	double p2Theta[n];

	while (reader.Next()) {
		//double diff = *e1Gen - *e1;
		e1Diff[c] = *e1Gen;
		p1Theta[c] = *t1Gen;
		c++;
	}

	auto c1 = new TCanvas();

	auto plot = new TGraph(n,p1Theta,e1Diff);

	plot->GetYaxis()->SetLimits(0,100);
	plot->Draw("ap");

	tree.Close();

}


