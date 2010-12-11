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

// C INCLUDES
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

// UCNb INCLUDES
#include "NGammaEvent.h"

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

Long64_t scan_time;
Long64_t max_time;
TString filename;
Long64_t background_start_time;
Long64_t start_time;

int time_bin_count;
int area_bin_count;
int time_fine_ratio;
int area_fine_ratio;

double max_area;
double lower_area_cut;
double upper_area_cut;
double lower_time_cut;
double upper_time_cut;

float lifetime1_start_time;
float lifetime1_stop_time;
float lifetime2_start_time;
float lifetime2_stop_time;

struct periodic_spectrum 
{
	TString filename;
  	TFile* file;
  	TTree* tree;
	TString hist_name;
	TH1F* time_hist; 
	TH1F* area_hist;
	TH2F* area_time_hist;
	Long64_t start_time;
	Long64_t scan_time;
	Long64_t max_time;
	double multiplier;

public:
	int make_historgram();
};

bool verbose;

void usage(const char * arg_name) 
{
	printf("Usage: %s <signal run number> <background run number> [start time(s)] [scan time(s)] [max time(s)]\n", arg_name);
}

Long64_t trigger(TTree* tree) {
	
}

int periodic_spectrum::make_historgram()
{
/*
  	TFile* file = new TFile(filename);
  	if (file->IsZombie())
  	{
		cout << "File "<<filename<<"not found."<<endl;
		exit(1);
  	}

  	TTree* tree = (TTree*)file->Get("allEvents");
  	if (!tree)
  	{
		cout<<"TTree not found in beta file "<<filename<<endl;
        	exit(1);
  	}

	if ( verbose ) {
		if ( tree->GetEntries() == (long)tree->GetEntries())
  			printf("Number of entries in the tree %li.\n", (long) tree->GetEntries());
		else
  			printf("Number of entries in the tree %e.\n", (double) tree->GetEntries());
	}

	*area_time_hist = new TH2F(hist_name+"_area_time_hist", "Counts per time and area", 
				   time_bin_count, 0, scan_time/1E9, area_bin_count, 0, max_area);
	*time_hist = new TH1F(hist_name+"_time_hist", "Counts per time", time_fine_ratio*time_bin_count, 0, scan_time/1E9);
	*area_hist = new TH1F(hist_name+"_area_hist", "Visible energy", area_fine_ratio*area_bin_count, 0, max_area);
	
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
			if (event->channel == 23 && sample_time < max_time)
			{
				double cycle_time = ((sample_time + scan_time - start_time) % scan_time) / 1E9; // seconds
				double area = event->area;  
				double pulse_height = event->maxSample;  
				double charge = area;

				if (pulse_height < 4096) {
					(*area_time_hist)->Fill(cycle_time, charge);

					if (charge > lower_area_cut && charge < upper_area_cut)
						(*time_hist)->Fill(cycle_time);

					if (cycle_time > lower_time_cut && cycle_time < upper_area_cut)
						(*area_hist)->Fill(charge);

					num++;
				}
			}
		}
		else
			cout << "error getting entry" << i << endl;
	}

	cout << "Number of entries filled is " << num << "." << endl;
	return num;
*/
}

int make_historgram(TString filename, TString hist_name, TH2F** area_time_hist, TH1F** time_hist, TH1F** area_hist)
{
  	TFile* file = new TFile(filename);
  	if (file->IsZombie())
  	{
		cout << "File "<<filename<<"not found."<<endl;
		exit(1);
  	}

  	TTree* tree = (TTree*)file->Get("allEvents");
  	if (!tree)
  	{
		cout<<"TTree not found in beta file "<<filename<<endl;
        	exit(1);
  	}

	if ( verbose ) {
		if ( tree->GetEntries() == (long)tree->GetEntries())
  			printf("Number of entries in the tree %li.\n", (long) tree->GetEntries());
		else
  			printf("Number of entries in the tree %e.\n", (double) tree->GetEntries());
	}

	*area_time_hist = new TH2F(hist_name+"_area_time_hist", "Counts per time and area", 
				   time_bin_count, 0, scan_time/1E9, area_bin_count, 0, max_area);
	*time_hist = new TH1F(hist_name+"_time_hist", "Counts per time", time_fine_ratio*time_bin_count, 0, scan_time/1E9);
	*area_hist = new TH1F(hist_name+"_area_hist", "Visible energy", area_fine_ratio*area_bin_count, 0, max_area);
	
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
			if (event->channel == 23 && sample_time < max_time)
			{
				double cycle_time = ((sample_time + scan_time - start_time) % scan_time) / 1E9; // seconds
				double area = event->area;  
				double pulse_height = event->maxSample;  
				double charge = area;

				if (pulse_height < 4096) {
					(*area_time_hist)->Fill(cycle_time, charge);

					if (charge > lower_area_cut && charge < upper_area_cut)
						(*time_hist)->Fill(cycle_time);

					if (cycle_time > lower_time_cut && cycle_time < upper_area_cut)
						(*area_hist)->Fill(charge);

					num++;
				}
			}
		}
		else
			cout << "error getting entry" << i << endl;
	}

	cout << "Number of entries filled is " << num << "." << endl;
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

	scan_time = Long64_t(5.2E9); 	// ns
	start_time = Long64_t(0);       // ns
	max_time = Long64_t(520E9); 	// ns
	background_start_time = Long64_t(0); 	// ns

	time_bin_count = 40;
	area_bin_count = 100;		
	time_fine_ratio = 2;
	area_fine_ratio = 2;

	//max_area = 4096;
	max_area = 4096*8;              // in multiples 78 fC
	lower_area_cut = 0;
	upper_area_cut = max_area;
	lower_time_cut = 1;
	upper_time_cut = 38;

	lifetime1_start_time = 2.5;
	lifetime1_stop_time = 5.5;
	lifetime2_start_time = 5;
	lifetime2_stop_time = 40;

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

  	if (arg_c > 6) 
  	{
		background_start_time = Long64_t(atof(arg_v[6]) * 1E9);
	}

	for (int i = 3; i < arg_c; i++)
	{
		// ...
	}
	
  	// Construct run filename
	TString root_data_dir(getenv("UCNb_PROCESSED_DATA_DIR"));
	TString beta_filename(root_data_dir + "/run" + TString(arg_v[1]) + ".root");
	//TString beta_filename(root_data_dir + arg_v[1]);
	TString back_filename(root_data_dir + "/run" + TString(arg_v[2]) + ".root");
	//TString back_filename(root_data_dir + arg_v[2]);

  	// Open beta ntuple
	TH2F* beta_area_time_hist;
	TH1F* beta_time_hist;
	TH1F* beta_area_hist;
	TH2F* back_area_time_hist;
	TH1F* back_time_hist;
	TH1F* back_area_hist;
	TH2F* diff_area_time_hist;
	TH1F* diff_time_hist;
	TH1F* diff_area_hist;

	// Create and fill ntuples
	cout << "start_time " << start_time/1e9 << endl;
	make_historgram(beta_filename, "beta", &beta_area_time_hist, &beta_time_hist, &beta_area_hist);
	make_historgram(beta_filename, "diff", &diff_area_time_hist, &diff_time_hist, &diff_area_hist);
	start_time = background_start_time;
	cout << "background start_time " << start_time/1e9 << endl;
	make_historgram(back_filename, "back", &back_area_time_hist, &back_time_hist, &back_area_hist);

	// Do background subtraction
	diff_area_time_hist->Add(back_area_time_hist, -1.0);
	diff_time_hist->Add(back_time_hist, -1.0);
	diff_area_hist->Add(back_area_hist, -1.0);

	// Fit lifetimes to time plot
	//TF1 *lifetime1_fit = new TF1("lifetime1_fit", "[0]*exp(-x/[1])+30", lifetime1_start_time, lifetime1_stop_time);
	TF1 *lifetime2_fit = new TF1("lifetime2_fit", "[0]*exp(-x/[1])", lifetime2_start_time, lifetime2_stop_time);

/*
	lifetime1_fit->SetParameter(0,160);
	lifetime1_fit->SetParameter(1,3);
*/
	lifetime2_fit->SetParameter(0,20);
	lifetime2_fit->SetParameter(1,4);

/*
	diff_time_hist->Fit(lifetime1_fit, "R");
*/
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
	beta_time_hist->SetLineColor(2);
	back_time_hist->SetLineColor(4);
	diff_time_hist->SetLineColor(1);
	beta_time_hist->SetAxisRange(0,1000,"Y");
	beta_time_hist->Draw("");
	back_time_hist->Draw("same");
	diff_time_hist->Draw("same");
	TLatex latex;
	latex.DrawLatex(200,200, "#tau = ");
	
	// Draw 1D area histogram
	TCanvas* area_canvas1D = new TCanvas("area_hist_canvas", "Visible energy spectrum", 1920/2, 1080/2);
	beta_area_hist->SetLineColor(2);
	back_area_hist->SetLineColor(4);
	diff_area_hist->SetLineColor(1);
	beta_area_hist->Draw("");
	back_area_hist->Draw("same");
	diff_area_hist->Draw("same");

	double time_window = upper_time_cut - lower_time_cut;
	double live_time = time_window * max_time / scan_time;
	double back_counts = diff_area_hist->Integral();
	double beta_counts = diff_area_hist->Integral();
	double diff_counts = diff_area_hist->Integral();
	cout << "diff counts: " << diff_counts << endl;
	cout << "time window: " << time_window << " sec" << endl;
	cout << "live time: " << live_time << " sec" << endl;
	cout << "diff rate " << diff_counts / live_time << " Hz" << endl;

    app.Run();

    return 0;
}
