/**
 * PeriodicSpectrum.cc 
 * 
 * Author: Kevin Peter Hickerson
 * created: <DATE>
 */
#include "PeriodicSpectrum.hh"

PeriodicSpectrum::PeriodicSpectrum(TString _filename, TString _hist_name, PeriodicPattern _pattern, double _multiplier)
{
	filename = _filename;
	hist_name = _hist_name;

	pattern = _pattern;
	//start_time = _start_time;
	//stop_time = _stop_time;
	//scan_time = _scan_time;
	//max_time = _max_time;
	multiplier = _multiplier;

  	file = 0;
  	tree = 0;
	time_hist = 0; 
	area_hist = 0;
	area_time_hist = 0;
}

PeriodicPattern PeriodicSpectrum::GetTriggerPattern()
{
	
}


int PeriodicSpectrum::LoadFile()
{
  	file = new TFile(filename);
  	if (file->IsZombie())
  	{
		cout << "File "<<filename<<" not found."<<endl;
		exit(1);
  	}

  	tree = (TTree*)file->Get("allEvents");
  	if (!tree)
  	{
		cout<<"TTree not found in beta file "<<filename<<endl;
        	exit(1);
  	}

	if ( verbose ) {
		if ( tree->GetEntries() == (long)tree->GetEntries())
  			printf("Number of entries in the tree %li.\n", (long) tree->GetEntries());
		else
  			printf("Number of entries in the tree %e.\n", (double) tree->GetEntries());
	}
}

int PeriodicSpectrum::MakeHistogram()
{
	area_time_hist = new TH2F(hist_name+"_area_time_hist", "Counts per time and area", 
				   time_bin_count, 0, pattern.scan_time/1E9, area_bin_count, 0, max_area);
	time_hist = new TH1F(hist_name+"_time_hist", "Counts per time", int(time_fine_ratio*time_bin_count), 0, pattern.scan_time/1E9);
	area_hist = new TH1F(hist_name+"_area_hist", "Visible energy", int(area_fine_ratio*area_bin_count), 0, max_area);
	
	Long64_t num = 0;
	NGammaEvent* event = new NGammaEvent();
        tree->SetBranchAddress("evt", &event);
	
	int N = tree->GetEntries();
	tree->GetEntry(0);
	Long64_t first_time = event->peakTime;

	for (int i = 0; i < N; i++)
	{
		if (tree->GetEntry(i) > 0)
		{
			Long64_t sample_time = event->peakTime - first_time;
			if (event->channel == 21 && sample_time < pattern.max_time)
			{
				//double cycle_time = ((sample_time + scan_time - start_time) % scan_time) / 1E9; // seconds
				//double cycle_time = ((sample_time + pattern.scan_time - pattern.start_time) % pattern.scan_time) / 1E9; // seconds
				Long64_t cycle_time = ((sample_time + pattern.scan_time - pattern.start_time) % pattern.scan_time); // seconds
				double area = event->area;  
				double pulse_height = event->maxSample;  
				double charge = area;

				if (pulse_height < 4096) {
					area_time_hist->Fill(cycle_time, charge);

					if (charge > lower_area_cut && charge < upper_area_cut)
						time_hist->Fill(double(cycle_time/1E9));

					if (cycle_time > lower_time_cut && cycle_time < upper_time_cut)
						area_hist->Fill(charge);

					num++;
				}
			}
		}
		else
			cout << "error getting entry" << i << endl;
	}

	cout << "Number of entries filled is " << num << "." << endl;
	return num;
}

