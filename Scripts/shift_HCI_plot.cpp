#include "TGraph.h"
#include "TCanvas.h"

void shift_HCI_plot() {
	TCanvas* c1 = new TCanvas();
	TGraph* plot1 = new TGraph("../Results/Eff_HCI.dat");

	plot1->SetTitle("System Efficiency against HCI position Along Detector");
	plot1->GetXaxis()->SetTitle("Displacement (mm)");
	plot1->GetYaxis()->SetTitle("Efficiency");
	plot1->SetMarkerStyle(7);
	plot1->Draw("acp");

	c1->Print("../Results/Eff_HCI.eps");

	c1->Close();

}
