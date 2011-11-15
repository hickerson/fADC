#include "RunGroup.hh"

void RunGroup::addRun(const Run &run)
{}

Spectrum& RunGroup::getSpectrum(string foreground_name, string background_name)
{}

Spectrum& RunGroup::getSpectrum(string foreground_name, string background_name, int channel)
{}


void RunGroup::load() 
{
	using boost::property_tree::ptree;
	ptree pt;
	read_json(filename, pt);	

}

