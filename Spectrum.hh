#ifndef __class_Spectrum_hh__
#define __class_Spectrum_hh__

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

// C INCLUDES
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

// UCNb INCLUDES
#include "NGammaEvent.h"

using namespace std;

/**
 * Author: Kevin Peter Hickerson
 * Date: Nov 2011
 *
 */



/**
 * Spectrum
 *
 * Author: Kevin Peter Hickerson
 * Created:  Nov 5 2011
 */
struct Spectrum
{
	TString name;
	TString type;
  	//TTree* tree;
	TH1F* foreground;
	TH1F* background;
	TH1F* signal; 
	//bool verbose;

	Spectrum() {
		puts("Empty default constructor");
	}

	Spectrum(TString _name, TString _type) :
		name(_name),
		type(_type),
		foreground(0),
		background(0),
		signal(0)
	{};
/*
	Spectrum(TString _filename, TString _hist_name, TTree* _tree);
	Spectrum(TString _filename, TString _hist_name, const Spectrum& _copy);

	int makeHistogram();
	int loadFile();
	void add(const Spectrum &spectrum);
	void add(const Spectrum &spectrum, double s);
    int scale(double scale);
	*/
};

#endif
