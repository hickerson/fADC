// ROOT INCLUDES
#include <TFile.h>
#include <TTree.h>
#include <TChain.h>
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
#include <unistd.h>

// STL INCLUDES
#include <vector>

// INCLUDES
#include "RunGroup.hh"

#define NUM_PMTS 1

using namespace std;

/**
 * Authors: Kevin Peter Hickerson
 * Date: Oct 2011
 */
void usage(const char * arg_name) 
{
	printf("Usage: %s <json runlog file>\n", arg_name);
}

int main (int arg_c, char **arg_v)
{
	opterr = 0; int c;
	while ((c = getopt(arg_c, arg_v, "f:b:s:")) != -1)
	{
		int run = 0;
		if (optarg)
			run = atoi(optarg);
		if (run == 0)
		{
			printf("Need a valid beta run number.\n");
			usage(arg_v[0]);
			exit(1);
		}
		TString root_data_dir(getenv("UCNb_PROCESSED_DATA_DIR"));
		TString filename(root_data_dir + "/run" + optarg + ".root");

		switch (c)
		{
			case 'f':
				printf("option -f\n");
				break;

			case 'b':
				printf("option -b\n");
				break;

			case 's':
				printf("option -s\n");
				break;

			default:
				printf("unknown option or missing argument.\n");
				usage(arg_v[0]);
				abort();
		}

	}

	int rootarg_c = 0;
 	//TApplication app("Mega Spectrum Analysis", &rootarg_c, 0);

	//RunGroup runGroup("Xe.runlog.json");
	RunGroup runGroup(arg_v[1]);
	runGroup.load();
	
	Run* run = runGroup.runs[0];
	
    // Plot options
  	gStyle->SetOptStat(0);
  	gStyle->SetOptFit(1);
  	gStyle->SetPalette(1);

	int bin_count = 1024;
	int max_area = 4096;
	int channel = 17;

	TString beta_hist_name("beta_spectrum_hist");
	TString back_hist_name("back_spectrum_hist");
	TString diff_hist_name("diff_spectrum_hist");
	char cut_str[1024];
 	sprintf(cut_str, "(channel==%d) && ((maxSample-pedestal) < 2750) && (area > 0)", 21);
 	TCut *beta_cut = new TCut(cut_str);

	TString beta_draw_cmd("(maxInterp) >> "+beta_hist_name);
	TString back_draw_cmd("(maxInterp) >> "+back_hist_name);
	TString diff_draw_cmd("(maxInterp) >> "+diff_hist_name);
	TH1F* beta_hist = new TH1F(beta_hist_name, "Ge events", bin_count, 0, max_area);
	TH1F* back_hist = new TH1F(back_hist_name, "Background Events", bin_count, 0, max_area);
	TH1F* diff_hist = new TH1F(diff_hist_name, "On - Off Events", bin_count, 0, max_area);

	TH1F* time_hist = run->getTimeHistogram(20);
	TH1F* energy_hist = run->getEnergyHistogram(20);
	
	beta_hist->SetLineColor(2);
	back_hist->SetLineColor(4);
	diff_hist->SetLineColor(1);
	//run->tree->Draw(beta_draw_cmd, *beta_cut);
	//beta_tree->Draw(diff_draw_cmd, *beta_cut);
	//back_tree->Draw(back_draw_cmd, *beta_cut);
	
	diff_hist->Add(back_hist, -1.0);


	{
		TCanvas* canvas = new TCanvas("canvas", "Beta spectrum and background", 1920/2, 1080/2);
		time_hist->Draw();
		TString data_pdf_filename = "Xe.time.pdf";
		canvas->SaveAs(data_pdf_filename);
	}

	{
		TCanvas* canvas = new TCanvas("canvas", "Beta spectrum and background", 1920/2, 1080/2);
		energy_hist->Draw();
		TString data_pdf_filename = "Xe.energy.pdf";
		canvas->SaveAs(data_pdf_filename);
	}
	//beta_hist->Draw();
	//back_hist->Draw("same");
	//diff_hist->Draw("same");

    //app.Run();
    return 0;
}
