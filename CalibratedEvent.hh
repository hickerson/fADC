#ifndef __class_CalibratedEvent_hh__
#define __class_CalibratedEvent_hh__

#include "TROOT.h"

#define MAX_CHANNELS_PER_EVENT 8;
#define ENERGY_CALIBRATION_DEGREE 3;

/**
 * CalibratedEvent
 *
 * Author: Kevin Peter Hickerson
 * Created: Mon Jan 31 22:48:59 PST 2011
 */
class CalibratedEvent
{
public:
	int events;						//< the number of channel events that make this event
	//NGammaEvent events[MAX_CHANNELS_PER_EVENT];         
	//double gmsCoefficients[MAX_CHANNELS_PER_EVENT][ENERGY_CALIBRATION_DEGREE];
	//double visibleEnergy;
	double trueEnergy;
	Long64_t firstEventNum;
	Long64_t visibleTriggerTime;
	Long64_t calibratedTriggerTime;

public:
/*
	CalibratedEvent();

	CalibratedEvent(const CalibratedEvent & copy)
	{
	}

	~CalibratedEvent();
	
	CalibratedEvent & operator = (const CalibratedEvent & other)
    {
        if (this != &other) // protect against invalid self-assignment
        {
            // 1: allocate new memory and copy the data
            // 2: deallocate old memory
            // 3: assign the new memory to the object
        }
        return *this;
    }
*/
	ClassDef(CalibratedEvent, 1)  // Neutron-gamma event
};

#endif
