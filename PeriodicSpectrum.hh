#ifndef __class_PeriodicSpectrum_hh__
#define __class_PeriodicSpectrum_hh__

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

using namespace std;

/**
 * Authors: Kevin Peter Hickerson
 * Date: Oct 2010
 *
 */
struct PeriodicPattern
{
	Long64_t start_time;
	Long64_t stop_time;
	Long64_t scan_time;
	Long64_t max_time;

};


struct PeriodicCuts
{
	int time_bin_count;
	int area_bin_count;
	float time_fine_ratio;
	float area_fine_ratio;

	double max_area;
	double lower_area_cut;
	double upper_area_cut;
	Long64_t lower_time_cut;
	Long64_t upper_time_cut;
};


/**
 * PeriodicSpectrum
 *
 * Author: Kevin Peter Hickerson
 * Created: Fri Apr  8 14:04:10 PDT 2011
 */
struct PeriodicSpectrum
{
	TString filename;
	TString hist_name;
	PeriodicPattern pattern;
	PeriodicCuts cuts;
	double multiplier;

  	TFile* file;
  	TTree* tree;
	TH1F* trigger_hist; 
	TH1F* time_hist; 
	TH1F* area_hist;
	TH2F* area_time_hist;

	bool verbose;

public:
	//PeriodicSpectrum(TString _filename, TString _hist_name, Long64_t _start_time, Long64_t _scan_time, Long64_t _max_time, double _multiplier);
	PeriodicSpectrum(TString _filename, TString _hist_name, PeriodicPattern _pattern, PeriodicCuts _cuts, double _multiplier);
	int MakeHistogram();
	int LoadFile();
	PeriodicPattern GetTriggerPattern();
};

#endif
