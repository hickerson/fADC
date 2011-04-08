#ifndef FULLENERGYEVENT_HH
#define FULLENERGYEVENT_HH

#include "TROOT.h"

#define MAX_CHANNELS_PER_EVENT 8;
#define ENERGY_CALIBRATION_DEGREE 3;

class FullEnergyEvent
{
public:
	int events;											//< the number of channel events that make this event
	NGammaEvent events[MAX_CHANNELS_PER_EVENT];         
	//double gmsCoefficients[MAX_CHANNELS_PER_EVENT][ENERGY_CALIBRATION_DEGREE];
	//double visibleEnergy;
	double trueEnergy;
	Long64_t firstEventNum;
	Long64_t visibleTriggerTime;
	Long64_t calibratedTriggerTime;
};


//---------------------------------------------------------------------------------
/*
	int channel;				//< channel on which event occured
	Long64_t evtnum;			//< sequential event number in run
	Long64_t triggerTime;		//< time of event trigger since run start [ns]
	Long64_t peakTime;			//< time of event peak since run start [ns]
	double centerDelta;			//< interpolated tweak to peak center timing
	double peakTimeInterp;		//< peak time interpolated
	double pedestal;			//< pedestal value (based on first 4 samples)
	int minSample;				//< minimum sample value
	int maxSample;				//< maximum sample value
	double maxInterp;			//< maximum value interpolated from peak samples
	double area;				//< integrated area
	double t1;
	double dt;
	double p1;
	double p2;
	double dEdx;
	double dEdx_tail;
	double dEdx_min;
	double chi2;

	//ClassDef(NGammaEvent, 1)  // Neutron-gamma event
*/

#endif
