#include <TGraph.h>
#include <TCanvas.h>

void padding_plot() {
	TCanvas* c2 = new TCanvas();
	TGraph* plot2 = new TGraph("../Results/Eff_padding.dat");

	plot2->SetTitle("System Efficiency Against Padding on Cold Plate");
	plot2->GetXaxis()->SetTitle("Padding (mm)");
	plot2->GetYaxis()->SetTitle("Efficiency");
	plot2->SetMarkerStyle(7);
	plot2->Draw("acp");

	c2->Print("../Results/Eff_padding.svg");

}
