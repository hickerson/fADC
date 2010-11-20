// ROOT INCLUDES
#include <TFile.h>
#include <TTree.h>
#include <TString.h>
#include <TCut.h>
#include <TProfile.h>
#include <TCanvas.h>
#include <TGraph.h>
#include <TGraphErrors.h>
#include <TH2F.h>
#include <TStyle.h>
#include <TApplication.h>
#include <TF1.h>

// C INCLUDES
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#define NUM_PMTS 1

// g++ `root-config --cflags` `root-config --libs` spectrum_analyser.cc -o spectrum_analyser

/**
 * Authors: Kevin Peter Hickerson
 * Date: Oct 2010
 */
void usage(const char * arg_name) 
{
	printf("Usage: %s <signal run number> <background run number>\n", arg_name);
}

/*
TF1* FitPedestal(const char *name, TTree *tree, TCut* cut)
{
	char pedestal_name[1024];
	sprintf(pedestal_name, "pedestal_histogram_%s", name);
	char pedestal_draw[1024];
	sprintf(pedestal_draw, "%s >> %s", name, pedestal_name);
	TH1F* pedestal_histogram = new TH1F(pedestal_name, "Pedestal Events", 2000,0,2000);
	tree->Draw(pedestal_draw, *cut);
	int max_bin = pedestal_histogram->GetMaximumBin();
	float max_bin_x = pedestal_histogram->GetBinCenter(max_bin);
  	TF1 *fit = new TF1("gauss_fit", "gaus", max_bin_x-12, max_bin_x+12);
	if (!pedestal_histogram->Fit(fit, "R"))
	{
		printf("Pedestal fit success: mu = %g, sigma = %g\n",fit->GetParameter(1),fit->GetParameter(2));
		return fit;
	} 
	else 
	{
		printf("Couldn't fit pedestal to %s.\n", name);
		return 0;
	}
}
*/

int main (int arg_c, char **arg_v)
{
 	TApplication app("PMT Spectrum Analysis", &arg_c, arg_v);

  	if (arg_c < 3)
  	{
		usage(arg_v[0]);
		exit(1);
  	}

  	int beta_run = atoi(arg_v[1]);
  	if (beta_run == 0)
  	{
		printf("Need a valid beta run number.\n");
		usage(arg_v[0]);
		exit(1);
  	}

  	int background_run = atoi(arg_v[2]);
  	if (background_run == 0)
  	{
		printf("Need a valid background run number.\n");
		usage(arg_v[0]);
		exit(1);
  	}

/* we'll see if this is nessesary...
	float scan_time = 15.0; // in minutes
  	float scan_start_time = 1.0; // in minutes
  	if (arg_c > 2) 
  	{
		scan_start_time = atof(arg_v[2]);
	}
		
  	if (arg_c > 3) 
  	{
		scan_time = atof(arg_v[3]);
	}
*/
		
  	// Construct run filename
  	//char beta_filename[1024];
  	//sprintf(beta_filename, "/home/kevinh/Data/UCN/UCNb2010/processed/run%s_2D.root", arg_v[1]);
	// TODO check envs
  	//char background_filename[1024];
  	//sprintf(background_filename, "/home/kevinh/Data/UCN/UCNb2010/processed/run%s_2D.root", arg_v[2]);
  	//sprintf(background_filename, "/home/kevinh/Data/UCN/UCNb2010/processed/run%s_2D.root", arg_v[2]);
 
	TString root_data_dir(getenv("UCNb_PROCESSED_DATA_DIR"));
	TString beta_filename(root_data_dir + "run" + arg_v[1] + "_2D.root");
	TString back_filename(root_data_dir + "run" + arg_v[2] + "_2D.root");

  	// Plot options
  	gStyle->SetOptStat(1);
  	gStyle->SetOptFit(1);

  	// Open beta ntuple
  	TFile* beta_file = new TFile(beta_filename);
  	if (beta_file->IsZombie())
  	{
		printf("File "+beta_filename+"not found.\n");
		exit(1);
  	}

  	TTree* beta_tree = (TTree*)beta_file->Get("allEvents");
  	if (beta_tree == NULL)
  	{
		printf("TTree not found in beta file "+beta_filename+".\n");
        exit(0);
  	}

  	// Open background ntuple
  	TFile* back_file = new TFile(back_filename);
  	if (back_file->IsZombie())
  	{
		printf("File "+back_filename+" not found.\n");
		exit(1);
  	}

  	TTree* back_tree = (TTree*)back_file->Get("allEvents");
  	if (back_tree == NULL)
  	{
		printf("TTree not found in background file "+back_filename+".\n");
        exit(0);
  	}

  	// Define cuts
  	TH2F* his2D[NUM_PMTS];
  	TProfile* p[NUM_PMTS];
  	TCanvas* c[NUM_PMTS];
  	TGraph* g[NUM_PMTS];
  	TGraphErrors* resg[NUM_PMTS];

  	gStyle->SetPalette(1);
  	gStyle->SetOptStat("");

	int bin_count = 256;
	int max_area = 4096*5;

	if ( beta_tree->GetEntries() == (long)beta_tree->GetEntries())
  		printf("Number of entries in the tree.%li\n", (long) beta_tree->GetEntries());
	else
  		printf("Number of entries in the tree.%e\n", (double) beta_tree->GetEntries());

	// This will be in a loop soon -->
	TString beta_hist_name("beta_spectrum_hist");
	TString back_hist_name("back_spectrum_hist");
	TString diff_hist_name("diff_spectrum_hist");
 	TCut *beta_cut = new TCut("(channel==16) && ((maxSample-pedestal) < 4095) && (area > 0)");

	TString beta_draw_cmd("(area) >> "+beta_hist_name);
	TString back_draw_cmd("(area) >> "+back_hist_name);
	TString diff_draw_cmd("(area) >> "+diff_hist_name);
	TH1F* beta_hist = new TH1F(beta_hist_name, "UCN Events", bin_count, 0, max_area);
	TH1F* back_hist = new TH1F(back_hist_name, "Background Events", bin_count, 0, max_area);
	TH1F* diff_hist = new TH1F(diff_hist_name, "On - Off Events", bin_count, 0, max_area);
	
	beta_hist->SetLineColor(2);
	back_hist->SetLineColor(4);
	diff_hist->SetLineColor(1);
	//TCanvas canvas = new TCanvas("Beta spectrum and background", 1920/2,1080/2);
	beta_tree->Draw(beta_draw_cmd, *beta_cut);
	beta_tree->Draw(diff_draw_cmd, *beta_cut);
	back_tree->Draw(back_draw_cmd, *beta_cut);
	
	diff_hist->Add(back_hist, -1);

	beta_hist->Draw();
	back_hist->Draw("same");
	diff_hist->Draw("same");
	//canvas.Print(

#if 0
  	for (unsigned i = 0; i < NUM_PMTS; i++) 
  	{
  	c[i] = new TCanvas(Cname[i], title[i]);
 	c[i]->Divide(2,1);
	c[i]->cd(1);


	// find Pedestal
	TF1 *ped_fit = FitPedestal(Qadc[i], tree, pedestal_cut);
    float pedestal = 0;
    if (ped_fit)
        pedestal = ped_fit->GetParameter(1);

  	// Define histograms
  	his2D[i] = new TH2F(H2Fname[i], "", 
		(int)(1.2*(scan_time+scan_start_time)*6), 0, 1.2*(scan_time+scan_start_time)*60, 
		1<<8, -pedestal, 4096-pedestal);
	char draw_cmd[1024];
	sprintf(draw_cmd, "%s-%f:S83028/1e6 >> %s", Qadc[i], pedestal, H2Fname[i]);
  	tree->Draw(draw_cmd, *led_cut);
	his2D[i]->SetTitle(title[i]);
 
	int nled = (int)his2D[i]->GetEntries();

	if(!nled) {
		printf("Empty histogram for %i; skipping.\n",i);
 		continue;
	}
	printf("Found %i LED events.\n",nled);

	printf("Building TProfile...\n");	
	p[i] = his2D[i]->ProfileX(Pname[i]);
	p[i]->SetErrorOption("s");
	
	printf("Building nPE plot...\n");	
	g[i] = new TGraph(p[i]->GetNbinsX());
	const float max_npe = 1000;
        for (unsigned j = 0; j < p[i]->GetNbinsX(); j++)
	{
                float x = p[i]->GetBinCenter(j);
		float w = p[i]->GetBinError(j);
		float y = (w<1.0)?0:pow(p[i]->GetBinContent(j)/w,2);
                y = (y>max_npe)?max_npe:y;
		g[i]->SetPoint(j, x, y);
	}

	printf("Re-bin and fit...\n");	
	//p[i]->Rebin(4);
  	TF1 *fit = new TF1("polyfit", "pol1", scan_start_time*60, (scan_time + scan_start_time)*60);
	if (p[i]->Fit(fit, "R"))
		continue;
	printf("Plotting LED intensity...\n");	
	p[i]->SetMarkerColor(2);
	p[i]->SetLineColor(2);
	p[i]->SetMarkerStyle(8);
	p[i]->SetMarkerSize(0.75);
  	//his1.Fit("gaus");

  	his2D[i]->Draw("colz");
  	p[i]->Draw("Same");
	
	p[i]->SetErrorOption(); // actual fit errors, not Poisson error bars for residual plot
	resg[i] = new TGraphErrors(p[i]->GetNbinsX());
        for (unsigned j = 0; j < p[i]->GetNbinsX(); j++)
	{
                float x = p[i]->GetBinCenter(j);
		
		float b = p[i]->GetBinContent(j);
		float y=0;
		if (b != 0)
			y = (b-fit->Eval(x))/b;
		if (y > 0.1)
			y = 0.1;
		if (y < -0.1)
			y = -0.1;
	
		if(60*scan_start_time <= x) { // && x <= 60*(scan_start_time+scan_time)) {	
			resg[i]->SetPoint(j, x, y);
			if(fabs(b) > 10) 
				resg[i]->SetPointError(j, 0, p[i]->GetBinError(j)/b);
		} else {
			resg[i]->SetPoint(j,0,0);
		}
	}
	p[i]->SetErrorOption("s");

 	c[i]->GetPad(2)->Divide(1,2);
	
	printf("Plotting nPE...\n");	
	c[i]->GetPad(2)->cd(1);
	if(g[i]->Fit(fit,"R"))
		continue;
	g[i]->SetTitle("Number of Photoelectrons");
	g[i]->Draw("AL");

	printf("Plotting residuals...\n");	
	c[i]->GetPad(2)->cd(2);
	resg[i]->SetTitle("PMT Linearity Residual");
	resg[i]->Draw("AP");
  }

  TCanvas *ew_canvas = new TCanvas("PMT_linerarity_canvas", "PMT linearity scans for all tubes");
  ew_canvas->Divide(2,1);
  for (int ew = 0; ew < 2; ew++)
  {
 		ew_canvas->GetPad(ew+1)->Divide(2,2);
		for (int tx = 0; tx < 2; tx++) {
			for (int ty = 0; ty < 2; ty++) {
				ew_canvas->GetPad(ew+1)->cd(tx+2*ty+1);
				p[4*ew+tx+2*ty]->Draw("");
			}
		}
  }
#endif
  app.Run();

  return 0;
}
