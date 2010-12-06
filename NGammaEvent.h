#ifndef NGAMMAEVENT_HH
#define NGAMMAEVENT_HH 1

#include "TROOT.h"

class NGammaEvent
{
public:
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

	ClassDef(NGammaEvent, 1)  // Neutron-gamma event
};

#endif
