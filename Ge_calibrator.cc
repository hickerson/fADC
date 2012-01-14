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
#include <unistd.h>

#define NUM_PMTS 1

using namespace std;

// g++ `root-config --cflags` `root-config --libs` spectrum_analyser.cc -o spectrum_analyser

/**
 * Authors: Kevin Peter Hickerson
 * Date: Dec 2010
 */
void usage(const char * arg_name) 
{
	printf("Usage: %s <signal run number> <background run number>\n", arg_name);
}


TTree* gettree(const char *run_number)
{
	TString root_data_dir(getenv("UCNb_PROCESSED_DATA_DIR"));
	TString filename(root_data_dir + "/run" + run_number + ".root");
  	TFile* file = new TFile(filename);
  	if (file->IsZombie())
  	{
		//printf("File "+beta_filename+"not found.\n");
		cout << "File " << filename << "not found." << endl;
		exit(1);
  	}

  	TTree* tree = (TTree*)file->Get("allEvents");
  	if (tree == NULL)
  	{
		cout << "TTree not found in beta file " << filename << endl;
        exit(0);
  	}
	return tree;
}

int main (int arg_c, char **arg_v)
{
	opterr = 0;
	int c;
	while ((c = getopt(arg_c, arg_v, "fbs")) != -1)
	{
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
				printf("unknown option.\n");
				//usage(arg_v[0]);
				//abort();
		}
	}

	int index = optind;
	printf("index %d\n", index);
  	if (arg_c < 2 + index)
  	{
		usage(arg_v[0]);
		exit(1);
  	}

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

	int rootarg_c = 0;
 	TApplication app("Ge Spectrum Analysis", &rootarg_c, 0);

	TString root_data_dir(getenv("UCNb_PROCESSED_DATA_DIR"));
	TString beta_filename(root_data_dir + "/run" + arg_v[1] + ".root");
	TString back_filename(root_data_dir + "/run" + arg_v[2] + ".root");

  // Plot options
  	gStyle->SetOptStat(1);
  	gStyle->SetOptFit(1);

  	// Open beta ntuple
  	TFile* beta_file = new TFile(beta_filename);
  	if (beta_file->IsZombie())
  	{
		//printf("File "+beta_filename+"not found.\n");
		cout << "File " << beta_filename << "not found." << endl;
		exit(1);
  	}

  	TTree* beta_tree = (TTree*)beta_file->Get("allEvents");
  	if (beta_tree == NULL)
  	{
		cout << "TTree not found in beta file " << beta_filename << endl;
        exit(0);
  	}

  	// Open background ntuple
  	TFile* back_file = new TFile(back_filename);
  	if (back_file->IsZombie())
  	{
		cout << "File " << back_filename << " not found." << endl;
		exit(1);
  	}

  	TTree* back_tree = (TTree*)back_file->Get("allEvents");
  	if (back_tree == NULL)
  	{
		cout << "TTree not found in background file " << back_filename << endl;
        	exit(0);
  	}

  	gStyle->SetPalette(1);
  	gStyle->SetOptStat("");

	int bin_count = 1024;
	//int max_area = 4096*200;
	int max_area = 4096;

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
	TCanvas* canvas = new TCanvas("canvas", "Beta spectrum and background", 1920/2,1080/2);
	beta_tree->Draw(beta_draw_cmd, *beta_cut);
	beta_tree->Draw(diff_draw_cmd, *beta_cut);
	back_tree->Draw(back_draw_cmd, *beta_cut);
	
	diff_hist->Add(back_hist, -1.0);

	beta_hist->Draw();
	back_hist->Draw("same");
	diff_hist->Draw("same");
	//canvas.Print(

  app.Run();

  return 0;
}
