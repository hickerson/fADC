#include "Run.hh"

int64_t Run::setTrigger() {
	/* outline...
	double trigger_level = histogram mean();
	event = start event;
	while (i < number of events in histogram)
	{
		if (event(i)->value() > trigger_level) {
			cycle_start_time = current event -> time			
			break;
		}
		event++;
	}
	*/
	abort();
}

TH1F* Run::getTimeHistogram(int channel) {
	int time_bin_count = 100;

	TH1F* time_hist = new TH1F(TSTRING(name+"_time_hist"), "Counts per time", int(time_bin_count), 0, 100);
	
	int64_t num = 0;
	NGammaEvent* event = new NGammaEvent();
    tree->SetBranchAddress("evt", &event);
	
	int N = tree->GetEntries();
	tree->GetEntry(0);
	//NanoSeconds first_time = event->peakTime;
	//NanoSeconds last_time = first_time;
	start_time = event->peakTime;
	stop_time = start_time;

	for (int i = 0; i < N; i++)
	{
		if (tree->GetEntry(i) > 0)
		{
			NanoSeconds this_time = event->peakTime;
			NanoSeconds sample_time = this_time - start_time;
			// TODO NanoSeconds cycle_time = ((sample_time + scan_time - start_time) % scan_time); // seconds
			//double area = event->area;  
			double pulse_height = event->maxSample;  
			//double charge = area;

			//if (event->channel == 21 && sample_time < stop_time)
			if (event->channel == channel)
			{
				if (pulse_height < 4096) {
					//if (charge > cuts.lower_area_cut && charge < cuts.upper_area_cut)
						//time_hist->Fill(double(cycle_time/1E9));
					time_hist->Fill(double(sample_time/1E9));
					num++;
				}
			}

            stop_time = this_time;
		}
		else
			cout << "error getting entry" << i << endl;
	}

/*
    real_time += last_time - first_time;
    dead_time += 0; // TODO ...
    live_time += real_time; // TODO ...
*/
	cout << "Number of entries filled is " << num << "." << endl;
	return time_hist;
}


TH1F* Run::getEnergyHistogram(int channel) {
	int bin_count = 1000;
	int range = 16000;

	TH1F* hist = new TH1F(TSTRING(name+"_energy_hist"), "Counts per time", int(bin_count), 0, range);
	
	int64_t num = 0;
	NGammaEvent* event = new NGammaEvent();
    tree->SetBranchAddress("evt", &event);
	
	int N = tree->GetEntries();
	tree->GetEntry(0);
	//NanoSeconds first_time = event->peakTime;
	//NanoSeconds last_time = first_time;
	start_time = event->peakTime;
	stop_time = start_time;

	for (int i = 0; i < N; i++)
	{
		if (tree->GetEntry(i) > 0)
		{
			NanoSeconds this_time = event->peakTime;
			//NanoSeconds sample_time = this_time - start_time;
			// TODO NanoSeconds cycle_time = ((sample_time + scan_time - start_time) % scan_time); // seconds
			double area = event->area;  
			double pulse_height = event->maxSample;  
			//double charge = area;

			//if (event->channel == 21 && sample_time < stop_time)
			if (event->channel == channel)
			{
				if (pulse_height < 4096) {
					//if (charge > cuts.lower_area_cut && charge < cuts.upper_area_cut)
						//hist->Fill(double(cycle_time/1E9));
					hist->Fill(area);
					num++;
				}
			}

            stop_time = this_time;
		}
		else
			cout << "error getting entry" << i << endl;
	}

/*
    real_time += last_time - first_time;
    dead_time += 0; // TODO ...
    live_time += real_time; // TODO ...
*/
	cout << "Number of entries filled is " << num << "." << endl;
	return hist;
	
}


TH1F* Run::getEnergyHistogram(vector<int> channels) {
	puts("tbd...");
}
