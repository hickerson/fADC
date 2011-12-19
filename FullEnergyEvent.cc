#include "FullEnergyEvent.hh"


bool FullEnergyEvent::addNearEvent(const NGammaEvent & event, NanoSeconds windowTime)
{
	if ( TMath::Abs(event.triggerTime - triggerTime) < windowTime) {
		puts("found event in window.");
		return true;
	}
	else return false;
}
