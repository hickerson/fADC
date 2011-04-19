/**
 * PeriodicSpectrum.cc 
 * 
 * Author: Kevin Peter Hickerson
 * created: <DATE>
 */
#include "PeriodicSpectrum.hh"

PeriodicSpectrum::PeriodicSpectrum(TString _filename, TString _hist_name, 
						PeriodicPattern _pattern, PeriodicCuts _cuts, double _multiplier)
{
	filename = _filename;
	hist_name = _hist_name;

	pattern = _pattern;
	cuts = _cuts;
	multiplier = _multiplier;

  	file = 0;
  	tree = 0;
	time_hist = 0; 
	area_hist = 0;
	area_time_hist = 0;

    time.live = 0;
    time.real = 0;
    time.dead = 0;
}

PeriodicSpectrum::PeriodicSpectrum(TString _filename, TString _hist_name, const PeriodicSpectrum& copy)
{
	filename = _filename;
	hist_name = _hist_name;

	pattern = copy.pattern;
	cuts = copy.cuts;
	multiplier = copy.multiplier;

  	file = 0;
  	tree = 0;
	time_hist = 0; 
	area_hist = 0;
	area_time_hist = 0;

    time.live = copy.time.live;
    time.real = copy.time.real;
    time.dead = copy.time.dead;

	add(copy);
}

PeriodicPattern PeriodicSpectrum::getTriggerPattern()
{
	
}

int PeriodicSpectrum::scaleToRealTime()
{
    if (time.real == 0)
    {   
        cout << "Real time is zero! Can't scale!" << endl;
        return 1;
    }
    double s = 1E9/time.real;
    cout << "Scaling to real time " << 1/s << " s" << endl;
    return scale(s);
}

int PeriodicSpectrum::scale(double s)
{
    if (s == 0)
    {   
        cout << "Scalar is zero! Can't scale!" << endl;
        return 1;
    }

	if (area_hist)
		area_hist->Scale(s);

	if (time_hist)
		time_hist->Scale(s);

	if (area_time_hist)
		area_time_hist->Scale(s);

    return 0;
}

void PeriodicSpectrum::add(const PeriodicSpectrum &spectrum)
{
    add(spectrum, spectrum.multiplier);
}

void PeriodicSpectrum::add(const PeriodicSpectrum &spectrum, double s)
{
	if (area_hist)
		area_hist->Add(spectrum.area_hist, s);
	else
		area_hist = new TH1F(*(spectrum.area_hist));

	if (time_hist)
		time_hist->Add(spectrum.time_hist, s);
	else
		time_hist = new TH1F(*(spectrum.time_hist));

	if (area_time_hist)
		area_time_hist->Add(spectrum.area_time_hist, s);
	else
		area_time_hist = new TH2F(*(spectrum.area_time_hist));

    time.real += spectrum.time.real;
    time.live += spectrum.time.live;
    time.dead += spectrum.time.dead;
}

int PeriodicSpectrum::loadFile()
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

/*
	if ( verbose ) {
		if ( tree->GetEntries() == (long)tree->GetEntries())
  			printf("Number of entries in the tree %li.\n", (long) tree->GetEntries());
		else
  			printf("Number of entries in the tree %e.\n", (double) tree->GetEntries());
	}
*/
}

int PeriodicSpectrum::makeHistogram()
{
	area_time_hist = new TH2F(hist_name+"_area_time_hist", "Counts per time and area", 
				   cuts.time_bin_count, 0, pattern.scan_time/1E9, cuts.area_bin_count, 0, cuts.max_area);
	time_hist = new TH1F(hist_name+"_time_hist", "Counts per time", int(cuts.time_fine_ratio*cuts.time_bin_count), 0, pattern.scan_time/1E9);
	area_hist = new TH1F(hist_name+"_area_hist", "Visible energy", int(cuts.area_fine_ratio*cuts.area_bin_count), 0, cuts.max_area);
	
	Long64_t num = 0;
	NGammaEvent* event = new NGammaEvent();
        tree->SetBranchAddress("evt", &event);
	
	int N = tree->GetEntries();
	tree->GetEntry(0);
	Long64_t first_time = event->peakTime;
	Long64_t last_time = first_time;

	for (int i = 0; i < N; i++)
	{
		if (tree->GetEntry(i) > 0)
		{
			Long64_t this_time = event->peakTime;
			Long64_t sample_time = this_time - first_time;
			Long64_t cycle_time = ((sample_time + pattern.scan_time - pattern.start_time) % pattern.scan_time); // seconds
			double area = event->area;  
			double pulse_height = event->maxSample;  
			double charge = area;

			if (event->channel == 21 && sample_time < pattern.max_time)
			{
				if (pulse_height < 4096) {
					area_time_hist->Fill(cycle_time, charge);

					if (charge > cuts.lower_area_cut && charge < cuts.upper_area_cut)
						time_hist->Fill(double(cycle_time/1E9));

					if (cycle_time > cuts.lower_time_cut && cycle_time < cuts.upper_time_cut)
						area_hist->Fill(charge);

					num++;
				}
			}

            last_time = this_time;
		}
		else
			cout << "error getting entry" << i << endl;
	}

    time.real += last_time - first_time;
    time.dead += 0; // TODO ...
    time.live += time.live - time.real;

	cout << "Number of entries filled is " << num << "." << endl;
	return num;
}

