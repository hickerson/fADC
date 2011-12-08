#ifndef __class_Run_hh__
#define __class_Run_hh__

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
#include <TLatex.h>

// STL INCLUDES
#include <vector>
#include <string>
#include <iostream>

// C INCLUDES
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

// UCNb INCLUDES
#include "NGammaEvent.h"

using namespace std;
typedef Long64_t NanoSeconds;

#define NUM_CHANNELS 8
#define TSTRING(s) (TString(s))

/**
 * Run
 *
 * Author: Kevin Peter Hickerson
 * Created: Sat Nov  5 18:06:55 PDT 2011
 */
struct Run
{
	string name;
	string type;
	string date;
	//Spectrum* spectrum[NUM_CH];
	TFile* file;
	TTree* tree;
	TH1F* histogram[NUM_CHANNELS];
	TH1F* sync;
	NanoSeconds start_time;
	NanoSeconds stop_time;
	NanoSeconds cycle_start_time;
	NanoSeconds cycle_stop_time;
	NanoSeconds cycle_time;
	NanoSeconds live_time;
	NanoSeconds dead_time;
	NanoSeconds real_time;
	NanoSeconds scan_time;

public:
	Run()
	{
		puts("Empty constructor for Run");
	}
	
/*
	Run(const Run & copy);
	Run(TString fileline);
*/
	~Run()
	{
		puts("Empty destructor for Run");
	}
	
	Run & operator = (const Run & other)
    {
		puts("Empty copy operator for Run");
        if (this != &other) // protect against invalid self-assignment
        {
            // 1: allocate new memory and copy the data
            // 2: deallocate old memory
            // 3: assign the new memory to the object
        }
        return *this;
    }

	//void syncronize(int channel);

	NanoSeconds setTrigger();
};

#endif
