#include "RunGroup.hh"

void RunGroup::init() 
{
	name = "";
	type = "";
	date = "";
	file = 0;
	tree = 0;
	for (int i = 0; i < NUM_CHANNELS; i++)
		spectra[i] = 0;
	sync = 0;
	start_time = 0;
	stop_time = 0;
	cycle_start_time = 0;
	cycle_stop_time = 0;
	live_time = 0;
	dead_time = 0;
}		


//void RunGroup::addRun(const Run &run)
void RunGroup::addRun(Run *run)
{
	runs.push_back(run); // copies run
}

/*
Spectrum& RunGroup::getSpectrum(string foreground_name, string background_name)
{
}

Spectrum& RunGroup::getSpectrum(string foreground_name, string background_name, int channel)
{}
*/

void RunGroup::load() 
{
	using boost::property_tree::ptree;
	ptree pt;
	read_json(filename, pt);	
	cout << "data:" ;
	cout << endl;
	BOOST_FOREACH(const ptree::value_type& child, pt.get_child("data")) {
		cout
			<< "date: "
			<< child.second.get<string>("date")
			<< "\n"
			<< "run: "
			<< child.second.get<int>("run")
			<< "\n";
		int run_number = child.second.get<int>("run");
		string root_data_dir(getenv("UCNb_PROCESSED_DATA_DIR"));
		char run_number_string[200];
		sprintf(run_number_string, "%d", run_number);
		string _filename = root_data_dir + "/run" + run_number_string  + ".root";

		Run* run = new Run();

		run->name = _filename;
		run->date = child.second.get<string>("date");
		run->number = child.second.get<int>("run");
		run->type = child.second.get<string>("type");
		run->real_time = child.second.get<NanoSeconds>("runtime");

		// Open ntuple
		//TFile* file = new TFile(TSTRING(run->name));
		run->file = new TFile(TSTRING(run->name));
		if (run->file->IsZombie())
		{
			//printf("File "+run->name+"not found.\n");
			cout << "File " << run->name << " not found." << endl;
			exit(1); // TODO Be more graceful
		}
		else
			cout << "File " << run->name << " loaded." << endl;

		//TTree* tree = (TTree*)run->file->Get("allEvents");
		run->tree = (TTree*)run->file->Get("allEvents");
		if (run->tree == NULL)
		{
			cout << "TTree not found in file " << run->name << endl;
			exit(0);
		}
		addRun(run);
	}
}


TH1F* RunGroup::getEnergyHistogram(int channel) 
{
	if (not spectra[channel]->foreground)
		spectra[channel]->foreground = new TH1F(TSTRING(name+"_energy_hist"), "Counts per time", bin_count, min_area, max_area);

	for (vector<Run*>::size_type i = 0; i < runs.size(); i++) {
		TH1F* _hist = runs[i]->getEnergyHistogram(channel, bin_count, min_area, max_area);
		spectra[channel]->foreground->Add(_hist, 1.0);
		delete _hist;
	}

	return spectra[channel]->foreground;
}
