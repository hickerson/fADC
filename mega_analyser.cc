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
//#include "PeriodicSpectrum.hh"
#include "RunGroup.hh"

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

	RunGroup runGroup("test.json");

   // Plot options
  	gStyle->SetOptStat(1);
  	gStyle->SetOptFit(1);
  	gStyle->SetPalette(1);

    //app.Run();
    return 0;
}
