#include "RunGroup.hh"


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
		/* for reference, members of Run
		string name;
		string type;
		string date;
		TFile* file;
		TTree* tree;
		TH1F* histogram[NUM_CHANNELS];
		TH1F* sync;
		NanoSeconds start_time;
		NanoSeconds stop_time;
		NanoSeconds cycle_start_time;
		NanoSeconds cycle_stop_time;
		NanoSeconds cycle_time;
		NanoSeconds live_time;
		NanoSeconds dead_time;
		*/
		addRun(run);
	}
}


TH1F* RunGroup::getEnergyHistogram(int channel) 
{
	int bin_count = 100;
	int range = 16000;

	if (not spectra[channel]->singnal)
		spectra[channel]->single = new TH1F(TSTRING(name+"_energy_hist"), "Counts per time", bin_count, 0, range);

	for (vector<Run*>::size_type i = 0; i < runs.size(); i++) {
		TH1F* _hist = runs[i]->getEnergyHistogram(channel);
		spectra[channel]->signal->Add(_hist, 1.0);
		delete _hist;
	}
}
