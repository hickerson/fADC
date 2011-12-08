#ifndef __class_RunGroup_hh__
#define __class_RunGroup_hh__

#include <string>
#include <iostream>
#include <vector>

#include "Run.hh"
#include "Spectrum.hh"

#include <boost/foreach.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>


using namespace std;

/**
 * RunGroup
 *
 * Author: Kevin Peter Hickerson
 * Created: Sun Nov  6 13:46:20 PST 2011
 */
struct RunGroup : public Run
{
	string filename; /// name of the run log file
	vector<Run*> runs; /// a list of runs as specified in the run log file

public:
	RunGroup()
	{
		puts("Empty default constructor for RunGroup");
	};

	RunGroup(string _filename) :
		filename(_filename)
	{}

	RunGroup(const RunGroup & copy)
	{
		puts("Empty copy constructor for RunGroup");
	}

	~RunGroup()
	{
		puts("Empty destructor for RunGroup");
	}
	
	RunGroup & operator = (const RunGroup & other)
    {
        if (this != &other) // protect against invalid self-assignment
        {
			runs = other.runs;
            // 1: allocate new memory and copy the data
            // 2: deallocate old memory
            // 3: assign the new memory to the object
        }
        return *this;
    }

	//void addRun(const Run &run);
	void addRun(Run *run);

	//Spectrum& getSpectrum(string foreground_name, string background_name);
	//Spectrum& getSpectrum(string foreground_name, string background_name, int channel);
	void load(); 
};

#endif
