#include "Run.hh"

NanoSeconds Run::setTrigger() {
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

	int time_bin_count = scan_time / 1E6;

	TH1F* time_hist = new TH1F(TSTRING(name+"_time_hist"), "Counts per time", int(time_bin_count), 0, scan_time);
	
	NanoSeconds num = 0;
	NGammaEvent* event = new NGammaEvent();
    tree->SetBranchAddress("evt", &event);
	
	int N = tree->GetEntries();
	tree->GetEntry(0);
	NanoSeconds first_time = event->peakTime;
	NanoSeconds last_time = first_time;

	for (int i = 0; i < N; i++)
	{
		if (tree->GetEntry(i) > 0)
		{
			NanoSeconds this_time = event->peakTime;
			NanoSeconds sample_time = this_time - first_time;
			NanoSeconds cycle_time = ((sample_time + scan_time - start_time) % scan_time); // seconds
			double area = event->area;  
			double pulse_height = event->maxSample;  
			double charge = area;

			if (event->channel == 21 && sample_time < stop_time)
			{
				if (pulse_height < 4096) {
					//if (charge > cuts.lower_area_cut && charge < cuts.upper_area_cut)
						time_hist->Fill(double(cycle_time/1E9));
					num++;
				}
			}

            last_time = this_time;
		}
		else
			cout << "error getting entry" << i << endl;
	}

    real_time += last_time - first_time;
    dead_time += 0; // TODO ...
    live_time += live_time - real_time;

	cout << "Number of entries filled is " << num << "." << endl;
	return num;
}
