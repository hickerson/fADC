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
	//...
};

// g++ `root-config --cflags` `root-config --libs` spectrum_analyser.cc -o spectrum_analyser

/**
 * Authors: Kevin Peter Hickerson
 * Date: Oct 2010
 *
 */

void usage(const char * arg_name) 
{
	printf("Usage: %s <signal run number> <background run number> [start time(s)] [scan time(s)] [max time(s)] [background start time(s)]\n", arg_name);
}

void addSpectrum(TString filename, TString name, PeriodicPattern pattern, PeriodicCuts cuts, PeriodicSpectrum **sum, double mult)
{
	// construct spectrum
	PeriodicSpectrum spectrum(filename, name, pattern, cuts, mult);
	spectrum.LoadFile();
	spectrum.GetTriggerPattern();
	spectrum.MakeHistogram();

	if (*sum)
		(*sum)->add(spectrum);
	else
	{
		*sum = new PeriodicSpectrum("sum", "sum_hist", spectrum);
		if (not *sum)
		{
			cout << "Error making sum spectrum." << endl;
			exit(1);
		}
	}
}

int main (int arg_c, char **arg_v)
{
 	TApplication app("PMT Spectrum Analysis", &arg_c, arg_v);
	int N = 4;
	//TString name[N] = {"Ce139", "Cd139", "Sn113", "Bi207", "background"};
	TString name[] = {"Bi207", "Bi207", "Bi207", "background"};
	TString run[] = {"3851", "3854", "3855", "3800"};
	//int  run[] = {3814, 3800};
	double mult[] = {+1, +1, +1, -3};
	TString root_data_dir(getenv("UCNb_PROCESSED_DATA_DIR"));
	PeriodicPattern pattern;
	PeriodicCuts cuts;

/*
  	if (arg_c < 3)
  	{
		usage(arg_v[0]);
		exit(1);
  	}

	TString fore_run_str(arg_v[1]);
  	int foreground_run = atoi(fore_run_str);
  	if (foreground_run == 0)
  	{
		printf("Need a valid beta run number.\n");
		usage(arg_v[0]);
		exit(1);
  	}

	if (background_run == foreground_run)
	{
		printf("Using same run as background");
		background_run = 0;
	}
  	if (arg_c > 3) 
  	{
		pattern.start_time = Long64_t(atof(arg_v[3]) * 1E9);
	}

  	if (arg_c > 4) 
  	{
		double d = atof(arg_v[4]);
		pattern.scan_time = Long64_t(d * 1E9);
		cuts.time_bin_count = int(d)*10);
		printf("time bin count %d\n", cuts.time_bin_count);
	}
	
  	if (arg_c > 5) 
  	{
		double d = atof(arg_v[5]);
		pattern.max_time = Long64_t(d * 1E9);
		cuts.time_fine_ratio = d / cuts.time_bin_count;
		printf("time bin count multiplier %f\n", cuts.time_fine_ratio);
	}

	if (background_run)
	{
  		if (arg_c > 6) 
  		{
			pattern.start_time = Long64_t(atof(arg_v[6]) * 1E9);
		}
	}
*/

	pattern.start_time = Long64_t(0);       // ns
	pattern.scan_time = Long64_t(5.2E9); 	// ns
	pattern.max_time = Long64_t(520E9); 	// ns

	cuts.time_bin_count = 52;
	cuts.area_bin_count = 100;		
	cuts.time_fine_ratio = 1;
	cuts.area_fine_ratio = 4;

	//max_area = 4096;
	cuts.max_area = 4096*16;              // in multiples 78 fC
	cuts.lower_area_cut = 0;
	cuts.upper_area_cut = cuts.max_area;
	cuts.lower_time_cut = pattern.scan_time * 0.1;
	cuts.upper_time_cut = pattern.scan_time * 0.9;


	PeriodicSpectrum* sum = 0;
	for (int i = 0; i < N; i++)
	{
  		// construct run filename
		TString filename(root_data_dir + "/run" + run[i] + ".root");

		// add in the new data
		addSpectrum(filename, name[i], pattern, cuts, &sum, mult[i]);
		if (not sum)
		{
			cout << "no sum made" << endl;
			exit(1);
		}
	}

  	// Plot options
  	gStyle->SetOptStat(1);
  	gStyle->SetOptFit(1);
  	gStyle->SetPalette(1);
  	gStyle->SetOptStat("");

	// Draw 2D histogram
	TCanvas* canvas2D = new TCanvas("time_area_hist_canvas", "Beta spectrum and background", 1920/2, 1080/2);
	sum->area_time_hist->Draw("colz");

	// Draw 1D time histogram 
	TCanvas* time_canvas1D = new TCanvas("time_hist_canvas", "Time sequence", 1920/2, 1080/2);
	sum->time_hist->SetLineColor(2);
	//foreground->time_hist->SetAxisRange(0,1000,"Y");
	sum->time_hist->Draw("");

/*
	if (background)
	{
		background->time_hist->SetLineColor(4);
		diff_time_hist->SetLineColor(1);
		background->time_hist->Draw("same");
		diff_time_hist->Draw("same");
	}

	TLatex latex;
	latex.DrawLatex(200,200, "#tau = ");
*/
	// Draw 1D area histogram
	TCanvas* area_canvas1D = new TCanvas("area_hist_canvas", "Visible energy spectrum", 1920/2, 1080/2);
	sum->area_hist->SetLineColor(2);
	sum->area_hist->Draw("");
/*
	if (background)
	{
		background->area_hist->SetLineColor(4);
		diff_area_hist->SetLineColor(1);
		background->area_hist->Draw("same");
		diff_area_hist->Draw("same");
	}
*/

	int cycles = pattern.max_time / pattern.scan_time;
	Long64_t time_window = cuts.upper_time_cut - cuts.lower_time_cut;
	Long64_t live_time = time_window * cycles;
/*
	double back_counts = sum->area_hist->Integral();
	double fore_counts = diff_area_hist->Integral();
*/
	double sum_counts = sum->area_hist->Integral();
	cout << "Number of cycles is " << cycles << endl;
	cout << "Time window is " << time_window / 1E9 << " sec" << endl;
	cout << "live time is " << live_time / 1E9 << " sec" << endl;
	cout << "Differance count is " << sum_counts << endl;
	cout << "Difference rate is " << 1E9 * sum_counts / live_time << " Hz" << endl;

    app.Run();

    return 0;
}
