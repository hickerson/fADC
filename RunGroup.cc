#include "RunGroup.hh"
#include <TMath.h>

void RunGroup::init() 
{
	name = "";
	type = "";
	date = "";
	file = 0;
	tree = 0;

	//for (int i = 0; i < NUM_CHANNELS; i++)
	//	pmt[i] = 0;

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
		run->run_time = child.second.get<double>("runtime");
		run->real_time = 1E9 * run->run_time;

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




//TH1F* RunGroup::getEnergyHistogram(int channel) 
TH1F* RunGroup::getEnergyHistogram(int channel, string type) 
{
	run_time = 0;

	TH1F* h = new TH1F(TSTRING(name+"_energy_hist"), "Counts per time", bin_count, min_area, max_area);

	cout << "number of runs " << runs.size() << endl;
	for (vector<Run*>::size_type i = 0; i < runs.size(); i++) {
		if (runs[i]->type == type || type == "") {
			TH1F* _hist = runs[i]->getEnergyHistogram(channel, bin_count, min_area, max_area);
			h->Add(_hist, 1.0);
			run_time += runs[i]->run_time;
			printf("Partial run time for run %d is %f\n", (int)i, run_time);
			delete _hist;
		}
		else
			cout << "Skipped run "<<i<<" because type "<<runs[i]->type<<" did not match type "<<type<<endl; 
	}

	for (int i = 0; i < bin_count; i++)
		h->SetBinError(i, TMath::Sqrt(h->GetBinContent(i)));

	printf("total run time %f\n", run_time);

	h->Scale(1/(run_time));
	h->GetXaxis()->SetTitle("keV");
	h->GetYaxis()->SetTitle("rate");

	return h;
}



TH1F* RunGroup::getEnergyHistogram(vector<int> channels, string type)
{
	run_time = 0;

	TH1F* h = new TH1F(TSTRING(name+"_energy_hist"), "Counts per time", bin_count, min_area, max_area);

	cout << "number of runs " << runs.size() << endl;
	for (vector<Run*>::size_type i = 0; i < runs.size(); i++) {
		if (runs[i]->type == type || type == "") {
			TH1F* _hist = runs[i]->getEnergyHistogram(channels, bin_count, min_area, max_area);
			h->Add(_hist, 1.0);
			run_time += runs[i]->run_time;
			printf("Partial run time for run %d is %f\n", (int)i, run_time);
			delete _hist;
		}
		else
			cout << "Skipped run "<<i<<" because type "<<runs[i]->type<<" did not match type "<<type<<endl; 
	}

	for (int i = 0; i < bin_count; i++)
		h->SetBinError(i, TMath::Sqrt(h->GetBinContent(i)));

	printf("total run time %f\n", run_time);

	h->Scale(1/(run_time));
	h->GetXaxis()->SetTitle("keV");
	h->GetYaxis()->SetTitle("rate");

	return h;
}




/*
TH1F* RunGroup::setForegroundHistogram(int channel, string type)
{
	if (not pmt[channel]->foreground)
		pmt[channel]->foreground = getEnergyHistogram(channel, type);

	return pmt[channel]->foreground;
}
*/
