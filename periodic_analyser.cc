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

// UCNb INCLUDES
#include "NGammaEvent.h"

#define NUM_PMTS 1

// TODO move to own file...
class Spectrum
{
	TString filename;
	
};

// g++ `root-config --cflags` `root-config --libs` spectrum_analyser.cc -o spectrum_analyser

/**
 * Authors: Kevin Peter Hickerson
 * Date: Oct 2010
 */

Long64_t scan_time = Long64_t(5.2E9); 	// ns
Long64_t max_time = Long64_t(520E9); 	// ns
Long64_t start_time = Long64_t(0);      // ns

int time_bin_count = 520/5;
int area_bin_count = 32;		
double max_area = 4096*8;               // in multiples 78 fC
double lower_area_cut = 0;
double upper_area_cut = max_area;

float lifetime1_start_time;
float lifetime1_stop_time;
float lifetime2_start_time;
float lifetime2_stop_time;

void usage(const char * arg_name) 
{
	printf("Usage: %s <signal run number> <background run number> [start time(s)] [scan time(s)] [max time(s)]\n", arg_name);
}

int make_historgram(TString filename, TString hist_name, TH2F** area_time_hist, TH1F** time_hist)
{
  	TFile* file = new TFile(filename);
  	if (file->IsZombie())
  	{
		printf("File "+filename+"not found.\n");
		exit(1);
  	}

  	TTree* tree = (TTree*)file->Get("allEvents");
  	if (tree == NULL)
  	{
		printf("TTree not found in beta file "+filename+".\n");
        	exit(1);
  	}

	if ( tree->GetEntries() == (long)tree->GetEntries())
  		printf("Number of entries in the tree %li.\n", (long) tree->GetEntries());
	else
  		printf("Number of entries in the tree %e.\n", (double) tree->GetEntries());

	*area_time_hist = new TH2F(hist_name+"_area_time_hist", "Counts per time and area", time_bin_count, 
				   0, scan_time/1E9, area_bin_count, 0, max_area);
	*time_hist = new TH1F(hist_name+"_time_hist", "Counts per time", 5*time_bin_count, 0, scan_time/1E9);
	*area_hist = new TH1F(hist_name+"_area_hist", "Visable energy", area_bin_count, max_area);
	
	Long64_t num = 0;
	NGammaEvent* event = new NGammaEvent();
        tree->SetBranchAddress("evt", &event);
	
	int N = tree->GetEntries();
	tree->GetEntry(0);
	Long64_t first_time = event->peakTime;

	for (int i = 0; i < N; i++)
	{
		if (tree->GetEntry(i) > 0)
		{
			Long64_t sample_time = event->peakTime - first_time;
			if (event->channel == 16 || event->channel == 17 && sample_time < max_time)
			{
				double cycle_time = ((sample_time - start_time) % scan_time) / 1E9; // seconds
				(*area_time_hist)->Fill(cycle_time, event->area);
				if (event->area > lower_area_cut && event->area < upper_area_cut)
					(*time_hist)->Fill(cycle_time);
				num++;
			}
		}
		else
			std::cout << "error getting entry" << i << std::endl;
	}

	printf("Number of entries filled is %d.\n", num);
	return num;
}

int main (int arg_c, char **arg_v)
{
 	TApplication app("PMT Spectrum Analysis", &arg_c, arg_v);

  	if (arg_c < 3)
  	{
		usage(arg_v[0]);
		exit(1);
  	}

/*
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
*/

	scan_time = Long64_t(5.2E9); 	// ns
	start_time = Long64_t(0);      // ns
	max_time = Long64_t(520E9); 	// ns

	time_bin_count = 520/10;
	area_bin_count = 32;		
	max_area = 4096*8;               // in multiples 78 fC
	lower_area_cut = 0;
	upper_area_cut = max_area;

	lifetime1_start_time = 2.5;
	lifetime1_stop_time = 5.5;
	lifetime2_start_time = 7;
	lifetime2_stop_time = 25;

  	if (arg_c > 3) 
  	{
		start_time = Long64_t(atof(arg_v[3]) * 1E9);
	}

  	if (arg_c > 4) 
  	{
		scan_time = Long64_t(atof(arg_v[4]) * 1E9);
	}
	
  	if (arg_c > 5) 
  	{
		max_time = Long64_t(atof(arg_v[5]) * 1E9);
	}
	
  	// Construct run filename
  	//char beta_filename[1024];
  	//sprintf(beta_filename, "/home/kevinh/Data/UCN/UCNb2010/processed/run%s_2D.root", arg_v[1]);

	// TODO check envs
  	//char background_filename[1024];
  	//sprintf(background_filename, "/home/kevinh/Data/UCN/UCNb2010/processed/run%s_2D.root", arg_v[2]);
  	//sprintf(background_filename, "/home/kevinh/Data/UCN/UCNb2010/processed/run%s_2D.root", arg_v[2]);
	TString root_data_dir(getenv("UCNb_PROCESSED_DATA_DIR"));
	//TString beta_filename(root_data_dir + "run" + arg_v[1] + "_2D.root");
	TString beta_filename(root_data_dir + arg_v[1]);
	//TString back_filename(root_data_dir + "run" + arg_v[2] + "_2D.root");
	TString back_filename(root_data_dir + arg_v[2]);

  	// Open beta ntuple
/*
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

*/
  	// Define cuts
	/*
  	TH2F* his2D[NUM_PMTS];
  	TProfile* p[NUM_PMTS];
  	TCanvas* c[NUM_PMTS];
  	TGraph* g[NUM_PMTS];
  	TGraphErrors* resg[NUM_PMTS];
	*/

  	// Plot options
  	gStyle->SetOptStat(1);
  	gStyle->SetOptFit(1);
  	gStyle->SetPalette(1);
  	gStyle->SetOptStat("");
/*
	if ( beta_tree->GetEntries() == (long)beta_tree->GetEntries())
  		printf("Number of entries in the tree.%li\n", (long) beta_tree->GetEntries());
	else
  		printf("Number of entries in the tree.%e\n", (double) beta_tree->GetEntries());
*/
	// This will be in a loop soon -->
	//TString beta_hist_name("beta_spectrum_hist");
/*
	const char * beta_hist_name = "beta_spectrum_hist";
	const char * time_hist_name = "time_hist";
	const char * back_hist_name = "back_spectrum_hist";
	const char * diff_hist_name = "diff_spectrum_hist";
*/

	TH2F* beta_area_time_hist;
	TH1F* beta_time_hist;
	TH2F* back_area_time_hist;
	TH1F* back_time_hist;
	TH2F* diff_area_time_hist;
	TH1F* diff_time_hist;

	start_time = Long64_t(6.6E9);
	make_historgram(beta_filename, "beta", &beta_area_time_hist, &beta_time_hist);
	make_historgram(beta_filename, "diff", &diff_area_time_hist, &diff_time_hist);
	start_time = Long64_t(15.75E9);
	make_historgram(back_filename, "back", &back_area_time_hist, &back_time_hist);
/*	
	TH2F* beta_area_v_time_hist = new TH2F(beta_hist_name, "time v. Evis", 
					  time_bin_count, 0, scan_time/1E9, area_bin_count, 0, max_area);
	TH1F* beta_time_hist = new TH1F(beta_time_hist_name, "time v. counts", 10*time_bin_count, 0, scan_time/1E9);
	
	Long64_t num = 0;
	NGammaEvent* event = new NGammaEvent();
        beta_tree->SetBranchAddress("evt", &event);
	
	int N = beta_tree->GetEntries();
	beta_tree->GetEntry(0);
	Long64_t first_time = event->peakTime;

	for (int i = 0; i < N; i++)
	{
		if (beta_tree->GetEntry(i) > 0)
		{
			Long64_t sample_time = event->peakTime - first_time;
			if (event->channel == 16 && sample_time < max_time)
			{
				beta_area_v_time->Fill(((sample_time - start_time) % scan_time, event->area)/1E9);
				if (event->area > lower_area_cut && event->area < upper_area_cut)
					beta_time_hist->Fill(((sample_time - start_time) % scan_time)/1E9);
				num++;
			}
		}
		else
			std::cout << "error getting entry" << i << std::endl;
	}
*/
	diff_time_hist->Add(back_time_hist, -1.0);
	diff_area_time_hist->Add(back_area_time_hist, -1.0);
	
	TF1 *lifetime1_fit = new TF1("lifetime1_fit", "[0]*exp(-x/[1])+30", lifetime1_start_time, lifetime1_stop_time);
	TF1 *lifetime2_fit = new TF1("lifetime2_fit", "[0]*exp(-x/[1])", lifetime2_start_time, lifetime2_stop_time);

	lifetime1_fit->SetParameter(0,160);
	lifetime1_fit->SetParameter(1,3);
	//lifetime1_fit->SetParameter(2,10);

	lifetime2_fit->SetParameter(0,40);
	lifetime2_fit->SetParameter(1,40);
	//lifetime2_fit->SetParameter(2,5);
	diff_time_hist->Fit(lifetime1_fit, "R");
	diff_time_hist->Fit(lifetime2_fit, "+R");

	//back_hist->SetLineColor(4);
	//diff_hist->SetLineColor(1);
	TCanvas* canvas2D = new TCanvas("time_area_hist_canvas", "Beta spectrum and background", 1920/2, 1080/2);
	diff_area_time_hist->Draw("colz");

	TCanvas* canvas1D = new TCanvas("time_hist_canvas", "Time sequence", 1920/2, 1080/2);
	beta_time_hist->SetLineColor(2);
	back_time_hist->SetLineColor(4);
	diff_time_hist->SetLineColor(1);
	beta_time_hist->Draw("");
	back_time_hist->Draw("same");
	diff_time_hist->Draw("same");

    app.Run();

    return 0;
}
