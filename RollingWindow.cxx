#include <vector>
#include <deque>
#include <map>
#include <algorithm>
#include "NGammaEvent.h"
#include "TTree.h"

ClassImp(NGammaEvent)

class RollingWindow;

void printEvent(const NGammaEvent& e) {
	printf("Event %lld @ %lld [%i]: %i\n",e.evtnum,e.peakTime,e.channel,e.maxSample);
}

/// event number sorting comparison
bool compareEventsByEvtnum(NGammaEvent p1, NGammaEvent p2) { return p1.evtnum < p2.evtnum; }

/// class for writing coincidence events tree --- subclass this to make actual coincidence analyzers
class CoincidenceProcessor {
public:
	/// constructor
	CoincidenceProcessor(const char* treeName) {
		coincTree = new TTree(treeName, "Coincidence tree");
	}
	/// destructor
	virtual ~CoincidenceProcessor() {} // delete(coincTree); }
	/// process events in rolling window
	virtual void processWindow(RollingWindow* W) {}	
	/// print summary of collected events
	virtual void printSummary() const { printf("Tree '%s' located %i events.\n",coincTree->GetName(),coincTree->GetEntries()); }
	TTree* coincTree;	//< tree to contain coincidence data
};


/// class for handling a "rolling window" analysis of events for finding coincidences
/// finds all maximal clusters of events such that including the preceding or following event would exceed time window
class RollingWindow {
public:
	/// constructor
	RollingWindow(float wlength = 100): windowLength(wlength), nWindows(0), timeFaults(0) {}
	
	/// add an event processor for identifying/recording coincidences
	void addProcessor(CoincidenceProcessor* P) { eventProcessors.push_back(P); }
	
	/// push new event, update window, call event analyzers
	void push_event(const NGammaEvent& evt) {
		// check for time-ordering faults
		while(windowEvents.size() && evt.peakTime < windowEvents.back().peakTime) {
			channelEvents[windowEvents.back().channel].pop_back();
			windowEvents.pop_back();
			timeFaults++;
		}		
		// if window already maximal, process events in window
		if(windowEvents.size() && evt.peakTime - windowEvents.front().peakTime > windowLength) {
			nWindows++;
			for(std::vector<CoincidenceProcessor*>::iterator it = eventProcessors.begin(); it != eventProcessors.end(); it++)
				(*it)->processWindow(this);
		}
		// add new event
		windowEvents.push_back(evt);
		channelEvents[evt.channel].push_back(evt);
		channelCounts[evt.channel]++;
		// trim window down to size
		while(evt.peakTime-windowEvents.front().peakTime > windowLength) {
			channelEvents[windowEvents.front().channel].pop_front();
			windowEvents.pop_front();
		}
	}
	
	/// print channel counter summary
	void printCounts() const {
		printf("Channel\tCounts\n");
		int t = 0;
		for(std::map<int, int>::const_iterator it = channelCounts.begin(); it != channelCounts.end(); it++) {
			printf("%i\t%i",it->first,it->second);
			std::map<int, std::deque<NGammaEvent> >::const_iterator z = channelEvents.find(it->first);
			if(z != channelEvents.end())
				printf("\t[%i]",z->second.size());
			printf("\n");
			t += it->second;
		}
		printf("Total: %i in %i windows [%i timing faults]\n",t,nWindows,timeFaults);
		for(std::vector<CoincidenceProcessor*>::const_iterator it = eventProcessors.begin(); it != eventProcessors.end(); it++)
			(*it)->printSummary();
	}
	
	float windowLength;										//< time width of rolling window
	unsigned int nWindows;									//< number of windows that have been processed
	unsigned int timeFaults;								//< counter for time-misordering errors
	std::vector<CoincidenceProcessor*> eventProcessors;		//< coincidence-identifying event processors
	std::deque<NGammaEvent> windowEvents;					//< all events in window
	std::map< int, std::deque<NGammaEvent> > channelEvents;	//< events listed by channel
	std::map<int, int> channelCounts;						//< total counts in each channel
	
};

/// class for recording all events
class RecordAllEvents: public CoincidenceProcessor {
public:
	/// constructor
	RecordAllEvents(): CoincidenceProcessor("allEvents") {
		chandat.evtnum = -1;
		readin = &chandat;
		coincTree->Branch("evt", "NGammaEvent", &readin);
	}

	/// record new events in rolling window
	void processWindow(RollingWindow* W) {
		// find previous stopping point
		std::deque<NGammaEvent>::iterator it = std::upper_bound(W->windowEvents.begin(), W->windowEvents.end(), chandat, &compareEventsByEvtnum);
		for(; it != W->windowEvents.end(); it++) {
			chandat = *it;
			coincTree->Fill();
		}
	}

	NGammaEvent chandat;
	NGammaEvent* readin;
};




/// example multiple-channel coincidence watcher (max. 128 channels)
class MultiCoincidence: public CoincidenceProcessor {
public:
	/// constructor
	MultiCoincidence(const char* treeName): CoincidenceProcessor(treeName) {}
	
	/// add a channel to include in coincidences
	void addCoincidentChannel(unsigned int c, const char* cname = NULL) {
		char tmp[128];
		if(cname)
			sprintf(tmp,"%s",cname);
		else
			sprintf(tmp,"Chan%i",c);
		readins[channelNums.size()] = &channelDat[channelNums.size()];
		coincTree->Branch(tmp, "NGammaEvent", &readins[channelNums.size()]);
		channelNums.push_back(c);
	}
	
	/// process events in rolling window
	void processWindow(RollingWindow* W) {
		// collect data from channels of interest			
		bool someNew = false;
		for(unsigned int i=0; i<channelNums.size(); i++) {
			unsigned int c = channelNums[i];
			if(!W->channelEvents[c].size())
				return;
			someNew = someNew || W->channelEvents[c].front().evtnum != channelDat[i].evtnum;
			channelDat[i] = W->channelEvents[c].front();
		}
		// if at least one channel different from previous event, record data
		if(someNew)
			coincTree->Fill();
	}
	
protected:
	std::vector<unsigned int> channelNums;		//< channel numbers to watch for coincidences
	NGammaEvent* readins[128];					//< pointers to read points
	NGammaEvent channelDat[128];				//< coincident events channel readout
};