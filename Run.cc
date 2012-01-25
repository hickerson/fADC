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

	TH1F* time_hist = new TH1F(TSTRING(name+"_time_hist"), name.c_str(), int(time_bin_count), 0, 100);
	
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


TH1F* Run::getEnergyHistogram(int channel, int bin_count, int min, int max) 
{
	TH1F* hist = new TH1F(TSTRING(name+"_energy_hist"), name.c_str(), int(bin_count), min, max);
	
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
		event = new NGammaEvent();
    	tree->SetBranchAddress("evt", &event);

		if (tree->GetEntry(i) > 0)
		{
			NanoSeconds this_time = event->peakTime;
			//NanoSeconds sample_time = this_time - start_time;
			// TODO NanoSeconds cycle_time = ((sample_time + scan_time - start_time) % scan_time); // seconds
			double area = event->area / 32;  
			double pulse_height = event->maxSample - event->pedestal;  
			//double charge = area;

			//if (event->channel == 21 && sample_time < stop_time)
			if (event->channel == channel)
			{
				if (pulse_height < 4096) {
					//if (charge > cuts.lower_area_cut && charge < cuts.upper_area_cut)
						//hist->Fill(double(cycle_time/1E9));
					//hist->Fill(area);
					hist->Fill(pulse_height);
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



TH1F* Run::getEnergyHistogram(vector<int> channels, int bin_count, int min, int max) 
{
	TH1F* hist = new TH1F(TSTRING(name+"_energy_hist"), name.c_str(), int(bin_count), min, max);
	
	int64_t num = 0;
	findFullEnergyEvents(10);
	for (unsigned i = 0; i < energyEvents.size(); i++)
	{
		int coincidence_count = 0;
		double area_sum = 0;
		double pulse_sum = 0;
		FullEnergyEvent* full_event = energyEvents[i];
		full_event->maxSample = 0;
		for (unsigned j = 0; j < channels.size(); j++)
		{
			int channel = channels[j];
			if (full_event->events.find(channel) != full_event->events.end())
			{
				NGammaEvent* event = full_event->events[channel];
				area_sum += event->area; // do stuff...
				pulse_sum += event->maxInterp - event->pedestal;
				//cout << event->area << "(" << event->channel << ", " << channel << ") + ";
				//cout << event->maxInterp - event->pedestal << " + ";
				//cout << event->maxInterp - event->pedestal << " + (" << event->channel << ", " << (event->triggerTime % 1000000000) << ") + ";
				if (event->maxSample > full_event->maxSample);
					full_event->maxSample = event->maxSample;  
				coincidence_count++;
			}
		}
				
		if (full_event->maxSample < 4096 and coincidence_count > 3) {
			//cout << " = " << pulse_sum << endl;
			//hist->Fill(area_sum);
			hist->Fill(pulse_sum);
			num++;
		}
	}

	return hist;
}



void Run::findFullEnergyEvents(NanoSeconds windowTime) 
{
	NGammaEvent* event = new NGammaEvent();
    tree->SetBranchAddress("evt", &event);
	int64_t event_num = 0;
	
	int N = tree->GetEntries();
	tree->GetEntry(0);
	//NanoSeconds first_time = event->peakTime;
	//NanoSeconds last_time = first_time;
	start_time = event->peakTime;
	stop_time = start_time;
	NanoSeconds last_time = start_time;
	FullEnergyEvent* full_event = new FullEnergyEvent();

	for (int i = 0; i < N; i++)
	{
		event = new NGammaEvent();
    	tree->SetBranchAddress("evt", &event);
		if (tree->GetEntry(i) > 0)
		{
			NanoSeconds this_time = event->peakTime;
			if (TMath::Abs(this_time - last_time) < windowTime)
			{
				//cout << event->area << endl;
			}
			else
			{
				event_num++;
				last_time = this_time;
				energyEvents.push_back(full_event);
				full_event = new FullEnergyEvent();
			}

			full_event->events[event->channel] = event; // TODO replace with method

			//cout << "Event "<< event_num <<" channel " << event->channel <<" event time " << this_time << endl;
		}
	}
}
