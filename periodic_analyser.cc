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
void usage(const char * arg_name) 
{
	printf("Usage: %s <signal run number> <background run number>\n", arg_name);
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

	Long64_t scan_time = (Long64_t)5.2E9; 	// in ns
  	Long64_t scan_start_time = (Long64_t)0; // in ns
  	Long64_t max_time = (Long64_t)520E9; 	// in ns

	int time_bin_count = 520;
	int area_bin_count = 32;
	double max_area = 4096*8;

  	if (arg_c > 4) 
  	{
		scan_start_time = Long64_t(atof(arg_v[2]) * 1E9);
	}
  	if (arg_c > 5) 
  	{
		scan_time = Long64_t(atof(arg_v[3]) * 1E9);
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
	/*
  	TH2F* his2D[NUM_PMTS];
  	TProfile* p[NUM_PMTS];
  	TCanvas* c[NUM_PMTS];
  	TGraph* g[NUM_PMTS];
  	TGraphErrors* resg[NUM_PMTS];
	*/

  	gStyle->SetPalette(1);
  	gStyle->SetOptStat("");

	if ( beta_tree->GetEntries() == (long)beta_tree->GetEntries())
  		printf("Number of entries in the tree.%li\n", (long) beta_tree->GetEntries());
	else
  		printf("Number of entries in the tree.%e\n", (double) beta_tree->GetEntries());

	// This will be in a loop soon -->
	//TString beta_hist_name("beta_spectrum_hist");
	const char * beta_hist_name = "beta_spectrum_hist";
	//TString back_hist_name("back_spectrum_hist");
	//TString diff_hist_name("diff_spectrum_hist");
 	TCut *beta_cut = new TCut("(channel==16) && ((maxSample-pedestal) < 4095) && (area > 0)");

	//TString beta_draw_cmd();
	
	//char beta_draw_cmd[1024];
	//sprintf(beta_draw_cmd, "area:(peakTime-(%f*TMath::Floor(peakTime/%f))) >> %s", scan_time, scan_time, beta_hist_name);
	//sprintf(beta_draw_cmd, "area:peakTime >> %s", beta_hist_name);
	//printf("%s\n", beta_draw_cmd);
	//TString beta_draw_cmd("((peakTime-"+scan_time+"*TMath::Floor(peakTime/"+scan_time+")):area) >> "+beta_hist_name);
	//TString back_draw_cmd("(area) >> "+back_hist_name);
	//TString diff_draw_cmd("(area) >> "+diff_hist_name);
	//TH1F* beta_hist = new TH1F(beta_hist_name, "UCN Events", bin_count, 0, 5.25);
	//TH1F* back_hist = new TH1F(back_hist_name, "Background Events", bin_count, 0, max_area);
	//TH1F* diff_hist = new TH1F(diff_hist_name, "On - Off Events", bin_count, 0, max_area);
	TH2F* beta_v_time_hist = new TH2F(beta_hist_name, "time v. Evis", 
					  time_bin_count, 0, scan_time, area_bin_count, 0, max_area);
	
	Long64_t num = 0;
	NGammaEvent* event = new NGammaEvent();
	/*
	double area = 0;	
	Long64_t peakTime = 0;
	int channel = 0;
	//void* area_address = &area;
	//void* peakTime_address = &peakTime;
	//void* channel_address = &channel;
        beta_tree->SetBranchAddress("area", &area);
        beta_tree->SetBranchAddress("peakTime", &peakTime);
        beta_tree->SetBranchAddress("channel", &channel);
	if (beta_tree->GetBranch("area") == NULL)
		std::cout << "branch is null" << std::endl;;
	*/
        beta_tree->SetBranchAddress("evt", &event);
	
	int N = beta_tree->GetEntries();
	beta_tree->GetEntry(0);
	first_time = event->peakTime;
	for (int i = 0; i < N; i++)
	{
		if (beta_tree->GetEntry(i) > 0)
		{
			//std::cout << "event->channel = " << event->channel << std::endl;
			if (event->channel == 16 && )
			{
				beta_v_time_hist->Fill((event->peakTime - start_time) % scan_time, event->area);
				num++;
				//std::cout << event->peakTime << " " <<  event->area << std::endl;
			}
		}
		else
			std::cout << "error!" << std::endl;
	}

	//Long64_t num = beta_tree->Draw(beta_draw_cmd, *beta_cut);
	printf("Number of entries filled is %d.\n", num);
	//beta_tree->Draw(diff_draw_cmd, *beta_cut);
	//back_tree->Draw(back_draw_cmd, *beta_cut);
	//diff_hist->Add(back_hist, -1.0);

	beta_v_time_hist->SetLineColor(2);
	//back_hist->SetLineColor(4);
	//diff_hist->SetLineColor(1);
	//TCanvas canvas = new TCanvas("Beta spectrum and background", 1920/2,1080/2);
	beta_v_time_hist->Draw("colz");
	//back_hist->Draw("same");
	//diff_hist->Draw("same");

    app.Run();

    return 0;
}
