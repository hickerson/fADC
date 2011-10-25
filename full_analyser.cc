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
#include "PeriodicSpectrum.hh"

#define NUM_PMTS 1

using namespace std;

/**
 * Authors: Kevin Peter Hickerson
 * Date: Oct 2011
 */
void usage(const char * arg_name) 
{
	printf("Usage: %s <signal run number> <background run number>\n", arg_name);
}

int main (int arg_c, char **arg_v)
{
	PeriodicSpectrum foreground("foreground_hist", "foreground");
	PeriodicSpectrum background("background_hist", "background");
	PeriodicSpectrum difference("difference_hist", "difference");
	PeriodicSpectrum* spectra[3] = { &foreground, &background, &difference };

	opterr = 0;
	int c;
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
				foreground.addToChain(filename);
				break;

			case 'b':
				printf("option -b\n");
				background.addToChain(filename);
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

	int index = optind;
  	if (arg_c < index)
  	{
		puts("Not enough arguments.");
		usage(arg_v[0]);
		exit(1);
  	}

/*
  	int foreground_run = atoi(arg_v[index]);
  	if (foreground_run == 0)
  	{
		printf("Need a valid beta run number.\n");
		usage(arg_v[0]);
		exit(1);
  	}

	int background_run = atoi(arg_v[index]);
  	if (background_run == 0)
  	{
		printf("Need a valid background run number.\n");
		usage(arg_v[0]);
		exit(1);
  	}
*/
	int rootarg_c = 0;
 	//TApplication app("Ge Spectrum Analysis", &rootarg_c, 0);

/*
	TString root_data_dir(getenv("UCNb_PROCESSED_DATA_DIR"));
	TString beta_filename(root_data_dir + "/run" + arg_v[1] + ".root");
	TString back_filename(root_data_dir + "/run" + arg_v[2] + ".root");
*/

   // Plot options
  	gStyle->SetOptStat(1);
  	gStyle->SetOptFit(1);
  	gStyle->SetPalette(1);

	int bin_count = 1024;
	//int max_area = 4096*200;
	int max_area = 4096;

/*
	if ( beta_tree->GetEntries() == (long)beta_tree->GetEntries())
  		printf("Number of entries in the tree.%li\n", (long) beta_tree->GetEntries());
	else
  		printf("Number of entries in the tree.%e\n", (double) beta_tree->GetEntries());

	// This will be in a loop soon -->
	TString beta_hist_name("beta_spectrum_hist");
	TString back_hist_name("back_spectrum_hist");
	TString diff_hist_name("diff_spectrum_hist");
 	TCut *beta_cut = new TCut("(channel==17) && ((maxSample-pedestal) < 2750) && (area > 0)");

	TString beta_draw_cmd("(maxInterp) >> "+beta_hist_name);
	TString back_draw_cmd("(maxInterp) >> "+back_hist_name);
	TString diff_draw_cmd("(maxInterp) >> "+diff_hist_name);
	TH1F* beta_hist = new TH1F(beta_hist_name, "Ge events", bin_count, 0, max_area);
	TH1F* back_hist = new TH1F(back_hist_name, "Background Events", bin_count, 0, max_area);
	TH1F* diff_hist = new TH1F(diff_hist_name, "On - Off Events", bin_count, 0, max_area);
	
	beta_hist->SetLineColor(2);
	back_hist->SetLineColor(4);
	diff_hist->SetLineColor(1);
	TCanvas* canvas = new TCanvas("canvas", "Beta spectrum and background", 1920/2, 1080/2);
	beta_tree->Draw(beta_draw_cmd, *beta_cut);
	beta_tree->Draw(diff_draw_cmd, *beta_cut);
	back_tree->Draw(back_draw_cmd, *beta_cut);
	
	diff_hist->Add(back_hist, -1.0);

	beta_hist->Draw();
	back_hist->Draw("same");
	diff_hist->Draw("same");
	//canvas.Print(

    app.Run();
*/
    return 0;
}
