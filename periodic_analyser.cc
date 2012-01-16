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
#include <TLatex.h>

// STL includes
#include <vector>
#include <string>

// C INCLUDES
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

// UCNb INCLUDES
#include "NGammaEvent.h"
#include "PeriodicSpectrum.hh"

#define NUM_PMTS 1

using namespace std;

// TODO move to own file...
class Spectrum
{
	TString filename;
	
};

// g++ `root-config --cflags` `root-config --libs` spectrum_analyser.cc -o spectrum_analyser

/**
 * Authors: Kevin Peter Hickerson
 * Date: Oct 2010
 *
 */

float lifetime1_start_time;
float lifetime1_stop_time;
float lifetime2_start_time;
float lifetime2_stop_time;

void usage(const char * arg_name) 
{
	printf("Usage: %s <signal run number> <background run number> [start time(s)] [scan time(s)] [max time(s)] [background start time(s)]\n", arg_name);
}


int main (int arg_c, char **arg_v)
{
 	TApplication app("PMT Spectrum Analysis", &arg_c, arg_v);
	vector<PeriodicSpectrum*> spectra;

	TString filename;
	PeriodicPattern foreground_pattern;
	PeriodicPattern background_pattern;
	PeriodicCuts cuts;

	foreground_pattern.start_time = Long64_t(0);       // ns
	foreground_pattern.scan_time = Long64_t(5.2E9); 	// ns
	foreground_pattern.max_time = Long64_t(520E9); 	// ns

	cuts.time_bin_count = 52;
	cuts.area_bin_count = 100;		
	cuts.time_fine_ratio = 1;
	cuts.area_fine_ratio = 4;

	//max_area = 4096;
	cuts.max_area = 4096*16;              // in multiples 78 fC
	cuts.lower_area_cut = 0;
	cuts.upper_area_cut = cuts.max_area;
	cuts.lower_time_cut = foreground_pattern.scan_time * 0.1;
	cuts.upper_time_cut = foreground_pattern.scan_time * 0.9;

	lifetime1_start_time = 156;
	lifetime1_stop_time = 300;
	//lifetime2_start_time = 100;
	//lifetime2_stop_time = 200;

  	if (arg_c < 3)
  	{
		usage(arg_v[0]);
		exit(1);
  	}

  	int foreground_run = atoi(arg_v[1]);
  	if (foreground_run == 0)
  	{
		printf("Need a valid beta run number.\n");
		usage(arg_v[0]);
		exit(1);
  	}

	int background_run = atoi(arg_v[2]);
  	if (background_run == 0)
  	{
		printf("no background run number found.\n");
		//printf("Need a valid background run number.\n");
		//usage(arg_v[0]);
		//exit(1);
  	}
	if (background_run == foreground_run)
	{
		printf("Using same run as background");
		background_run = 0;
	}
	
  	if (arg_c > 3) 
  	{
		foreground_pattern.start_time = Long64_t(atof(arg_v[3]) * 1E9);
	}
	background_pattern.start_time = foreground_pattern.start_time;

  	if (arg_c > 4) 
  	{
		foreground_pattern.scan_time = Long64_t(atof(arg_v[4]) * 1E9);
		cuts.time_bin_count = int(atof(arg_v[4])*10);
		printf("time bin count %d\n", cuts.time_bin_count);
	}
	background_pattern.scan_time = foreground_pattern.scan_time;
	
  	if (arg_c > 5) 
  	{
		foreground_pattern.max_time = Long64_t(atof(arg_v[5]) * 1E9);
		cuts.time_fine_ratio = atof(arg_v[5]) / cuts.time_bin_count;
		printf("time bin count multiplier %f\n", cuts.time_fine_ratio);
	}
	background_pattern.max_time = foreground_pattern.max_time;

	if (background_run)
	{
  		if (arg_c > 6) 
  		{
			background_pattern.start_time = Long64_t(atof(arg_v[6]) * 1E9);
		}
	}

	for (int i = 3; i < arg_c; i++)
	{
		// ...
	}

  	// Construct run filename
	TString root_data_dir(getenv("UCNb_PROCESSED_DATA_DIR"));
	TString foreground_filename(root_data_dir + "/run" + TString(arg_v[1]) + ".root");
	TString background_filename(root_data_dir + "/run" + TString(arg_v[2]) + ".root");

	// new way...
	PeriodicSpectrum* foreground = 0;
	PeriodicSpectrum* background = 0;

	//foreground = new PeriodicSpectrum(fore_filename, "foreground", start_time, scan_time, max_time, +1);
	foreground = new PeriodicSpectrum(foreground_filename, "foreground", foreground_pattern, cuts, +1);
	if (foreground)
	{
		foreground->loadFile();
		foreground->getTriggerPattern();
		foreground->makeHistogram();
	}

	if (background_run)
	{
		//background = new PeriodicSpectrum(back_filename, "background", background_start_time, scan_time, max_time, -1/10);
		background = new PeriodicSpectrum(background_filename, "background", background_pattern, cuts, -1);
		if (background)
		{
			background->loadFile();
			background->getTriggerPattern();
			background->makeHistogram();
		}
	}

	TH2F* diff_area_time_hist;
	TH1F* diff_time_hist;
	TH1F* diff_area_hist;

	diff_area_time_hist = new TH2F(*(foreground->area_time_hist));
	diff_time_hist = new TH1F(*(foreground->time_hist));
	diff_area_hist = new TH1F(*(foreground->area_hist));

	// Create and fill ntuples
/*
	cout << "start_time " << start_time/1e9 << endl;
	MakeHistogram(fore_filename, "beta", &fore_area_time_hist, &fore_time_hist, &fore_area_hist);
	MakeHistogram(fore_filename, "diff", &diff_area_time_hist, &diff_time_hist, &diff_area_hist);
	start_time = background_start_time;
	cout << "background start_time " << start_time/1e9 << endl;
	MakeHistogram(back_filename, "back", &back_area_time_hist, &back_time_hist, &back_area_hist);
*/
	// Do background subtraction
	if (background)
	{
		diff_area_time_hist->Add(background->area_time_hist, background->multiplier);
		diff_time_hist->Add(background->time_hist, background->multiplier);
		diff_area_hist->Add(background->area_hist, background->multiplier);
	}

	// Fit lifetimes to time plot
	TF1 *lifetime1_fit = new TF1("lifetime1_fit", "[0]*exp(-x/[1])", lifetime1_start_time, lifetime1_stop_time);
	TF1 *lifetime2_fit = new TF1("lifetime2_fit", "[0]*exp(-x/[1])", lifetime2_start_time, lifetime2_stop_time);


	lifetime1_fit->SetParameter(0,160);
	lifetime1_fit->SetParameter(1,3);

	lifetime2_fit->SetParameter(0,20);
	lifetime2_fit->SetParameter(1,4);

	diff_time_hist->Fit(lifetime1_fit, "R");
	diff_time_hist->Fit(lifetime2_fit, "+R");

  	// Plot options
  	gStyle->SetOptStat(1);
  	gStyle->SetOptFit(1);
  	gStyle->SetPalette(1);
  	gStyle->SetOptStat("");

	// Draw 2D histogram
	TCanvas* canvas2D = new TCanvas("time_area_hist_canvas", "Beta spectrum and background", 1920/2, 1080/2);
	diff_area_time_hist->Draw("colz");

	// Draw 1D time histogram 
	TCanvas* time_canvas1D = new TCanvas("time_hist_canvas", "Time sequence", 1920/2, 1080/2);
	foreground->time_hist->SetLineColor(2);
	//foreground->time_hist->SetAxisRange(0,1000,"Y");
	foreground->time_hist->Draw("");

	if (background)
	{
		background->time_hist->SetLineColor(4);
		diff_time_hist->SetLineColor(1);
		background->time_hist->Draw("same");
		diff_time_hist->Draw("same");
	}

	TLatex latex;
	latex.DrawLatex(200,200, "#tau = ");
/*
	TCanvas* time_canvas1D = new TCanvas("time_hist_canvas", "Time sequence", 1920/2, 1080/2);
	fore_time_hist->SetLineColor(2);
	back_time_hist->SetLineColor(4);
	diff_time_hist->SetLineColor(1);
	fore_time_hist->SetAxisRange(0,1000,"Y");
	fore_time_hist->Draw("");
	back_time_hist->Draw("same");
	diff_time_hist->Draw("same");
	TLatex latex;
	latex.DrawLatex(200,200, "#tau = ");

	// Draw 1D area histogram
	TCanvas* area_canvas1D = new TCanvas("area_hist_canvas", "Visible energy spectrum", 1920/2, 1080/2);
	fore_area_hist->SetLineColor(2);
	back_area_hist->SetLineColor(4);
	diff_area_hist->SetLineColor(1);
	fore_area_hist->Draw("");
	back_area_hist->Draw("same");
	diff_area_hist->Draw("same");
*/	
	// Draw 1D area histogram
	TCanvas* area_canvas1D = new TCanvas("area_hist_canvas", "Visible energy spectrum", 1920/2, 1080/2);
	foreground->area_hist->SetLineColor(2);
	foreground->area_hist->Draw("");
	if (background)
	{
		background->area_hist->SetLineColor(4);
		diff_area_hist->SetLineColor(1);
		background->area_hist->Draw("same");
		diff_area_hist->Draw("same");
	}

	int cycles = foreground_pattern.max_time / foreground_pattern.scan_time;
	Long64_t time_window = cuts.upper_time_cut - cuts.lower_time_cut;
	Long64_t live_time = time_window * cycles;
	double back_counts = diff_area_hist->Integral();
	double fore_counts = diff_area_hist->Integral();
	double diff_counts = diff_area_hist->Integral();
	cout << "Number of cycles is " << cycles << endl;
	cout << "Time window is " << time_window / 1E9 << " sec" << endl;
	cout << "live time is " << live_time / 1E9 << " sec" << endl;
	cout << "Differance count is " << diff_counts << endl;
	cout << "Difference rate is " << 1E9 * diff_counts / live_time << " Hz" << endl;

    app.Run();

    return 0;
}
