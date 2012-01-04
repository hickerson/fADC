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
	json_filename += ".json";
	RunGroup runGroup(json_filename);
	runGroup.load();
	
	//Run* run = runGroup.runs[0];
	
    // global plot options
  	gStyle->SetOptStat(0);
  	gStyle->SetOptFit(1);
  	gStyle->SetPalette(1);

	runGroup.bin_count = 140;
	runGroup.min_area = 0;
	runGroup.max_area = 1400;
	int channel = 24;

/*
	TString beta_hist_name("beta_spectrum_hist");
	TString back_hist_name("back_spectrum_hist");
	TString diff_hist_name("diff_spectrum_hist");
	char cut_str[1024];
 	sprintf(cut_str, "(channel==%d) && ((maxSample-pedestal) < 2750) && (area > 0)", channel);
 	TCut *beta_cut = new TCut(cut_str);

	TString beta_draw_cmd("(maxInterp) >> "+beta_hist_name);
	TString back_draw_cmd("(maxInterp) >> "+back_hist_name);
	TString diff_draw_cmd("(maxInterp) >> "+diff_hist_name);
	TH1F* beta_hist = new TH1F(beta_hist_name, "Ge events", bin_count, 0, max_area);
	TH1F* back_hist = new TH1F(back_hist_name, "Background Events", bin_count, 0, max_area);
	TH1F* diff_hist = new TH1F(diff_hist_name, "On - Off Events", bin_count, 0, max_area);

	
	beta_hist->SetLineColor(2);
	back_hist->SetLineColor(4);
	diff_hist->SetLineColor(1);
	//run->tree->Draw(beta_draw_cmd, *beta_cut);
	//beta_tree->Draw(diff_draw_cmd, *beta_cut);
	//back_tree->Draw(back_draw_cmd, *beta_cut);
	
	diff_hist->Add(back_hist, -1.0);
*/

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
		channels.push_back(20);
		channels.push_back(21);
		channels.push_back(23);

		// select out the foreground runs
		cout << arg_v[1] << endl;
		//runGroup.spectrum["full_energy"].foreground = runGroup.getEnergyHistogram(channel, arg_v[1]);
		runGroup.spectrum["full_energy"].foreground = runGroup.getEnergyHistogram(channels, arg_v[1]);
		cout << "number of spectra is " << runGroup.spectrum.size() << endl;
		TH1F* h = runGroup.spectrum["full_energy"].foreground;
		h->Draw("G");

		// select out the background runs
		runGroup.spectrum["full_energy"].background = runGroup.getEnergyHistogram(channel, "background");
		//runGroup.spectrum["full_energy"].background->Draw("same");

		// save global canvas to a pdf
	//	string data_pdf_filename = arg_v[1];
	//	data_pdf_filename += ".energy.pdf";
	//	canvas->Print(data_pdf_filename.c_str());
		char filename[1024];
		sprintf(filename, "%s.energy-ch%d.pdf", arg_v[1], channel);
		canvas->Print(filename);
	}



	//beta_hist->Draw();
	//back_hist->Draw("same");
	//diff_hist->Draw("same");

    //app.Run();
    return 0;
}
