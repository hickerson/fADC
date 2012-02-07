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
#include <map>

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

	//int rootarg_c = 0;
 	//TApplication app("Mega Spectrum Analysis", &rootarg_c, 0);

	//RunGroup runGroup("Xe.runlog.json");
	string json_filename(arg_v[1]);
	string signal_name(arg_v[2]);
	string background_name;
	if (arg_c > 3)
		background_name = arg_v[3];
	else
		background_name = "background";
	string plot_name = signal_name + " - " + background_name;

	json_filename += ".json";
	RunGroup runGroup(json_filename);
	runGroup.load();
	
	//Run* run = runGroup.runs[0];
	
    // global plot options
  	gStyle->SetOptStat(0);
  	gStyle->SetOptFit(1);
  	gStyle->SetPalette(1);

	runGroup.bin_count = 100;
	runGroup.min_area = 0;
	//runGroup.max_area = 140000;
	runGroup.max_area = 5000;
	runGroup.name = plot_name;
	//int channel = 24;

/*
	{
		TH1F* time_hist = run->getTimeHistogram(channel);
		TCanvas* canvas = new TCanvas("canvas", "Beta spectrum and background", 1920/2, 1080/2);
		time_hist->Draw();
		string data_pdf_filename = arg_v[1];
		data_pdf_filename += ".time.pdf";
		canvas->SaveAs(data_pdf_filename.c_str());
	}

	{
		TH1F* energy_hist = run->getEnergyHistogram(channel, runGroup.bin_count, 0, runGroup.max_area);
		TCanvas* canvas = new TCanvas("canvas", "Beta spectrum and background", 1920/2, 1080/2);
		energy_hist->Draw();

		char filename[1024]; 
		sprintf(filename, "%s.energy.run%d-ch%d.pdf", arg_v[1], run->number, channel);
		canvas->SaveAs(filename);
	}


	{
		TH1F* energy_hist = runGroup.getEnergyHistogram(channel);
		TCanvas* canvas = new TCanvas("canvas", "Energy rate spectrum", 1920/2, 1080/2);
		energy_hist->GetXaxis()->SetTitle("keV");
		energy_hist->GetYaxis()->SetTitle("rate");
		energy_hist->Draw("G");
		string data_pdf_filename = arg_v[1];
		data_pdf_filename += ".energy.pdf";
		canvas->SaveAs(data_pdf_filename.c_str());
	}
*/


	{
		// create the global canvas
		TCanvas* canvas = new TCanvas("canvas", "Energy rate spectrum", 1920/2, 1080/2);
		vector<int> channels;
		channels.push_back(16);
		channels.push_back(17);
		channels.push_back(18);
		channels.push_back(19);

		// select out the foreground runs
		cout << arg_v[1] << endl;
		runGroup.spectrum["full_energy"].foreground = runGroup.getEnergyHistogram(channels, signal_name);
		cout << "number of spectra is " << runGroup.spectrum.size() << endl;
		TH1F* foreground_h = runGroup.spectrum["full_energy"].foreground;

		// select out the background runs
		if (background_name != "none") {
			runGroup.spectrum["full_energy"].background = runGroup.getEnergyHistogram(channels, background_name);
			TH1F* background_h = runGroup.spectrum["full_energy"].background;
			foreground_h->Add(background_h, -1);
		}

		foreground_h->Draw("G");

		//runGroup.spectrum["full_energy"].background->Draw("same");

		// save global canvas to a pdf
		char filename[1024];
		sprintf(filename, "%s-%s-%s.full-energy.pdf", json_filename.c_str(), signal_name.c_str(), background_name.c_str());
		canvas->Print(filename);
		sprintf(filename, "%s-%s-%s.full-energy.gif", json_filename.c_str(), signal_name.c_str(), background_name.c_str());
		canvas->Print(filename);
	}


    //app.Run();
    return 0;
}
