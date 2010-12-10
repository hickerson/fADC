#define _FILE_OFFSET_BITS 64

#include "NGammaEvent.h"
#include "PacketFormat.h"
#include "RollingWindow.cxx"

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include <vector>
#include <deque>
#include <algorithm>
#include <cassert>
#include <iostream>

#include "TFile.h"
#include "TGraph.h"
#include "TSpline.h"
#include "TString.h"
#include "TCanvas.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TF1.h"
#include "TLatex.h"

ClassImp(NGammaEvent)

#define NBOARD 4 
#define CHANNELS_PER_BOARD 8	// canonical channels/board
#define TIME_CHUNK 10			// number of seconds of data to process at a time
#define PEDESTAL_START 5 // starting point for averaging
#define PEDESTAL_SAMPLE_COUNT 15 // number of samples used for pedestal averaging

// Boards 1/2 16ns/clock tick, 
// Board 3: 10x slower 
// later changed to variable on all boards

const float k_clockMul[NBOARD][CHANNELS_PER_BOARD] = { {4,4,4,4,4,4,4,4}, {4,4,4,4,4,4,4,4}, {4,4,4,4,4,4,4,4}, {120,120,120,120,120,120,120,120}};

/// time-sorting comparison
bool compareEventsByTime(NGammaEvent p1, NGammaEvent p2) { return p1.peakTime < p2.peakTime; }

/// pulse shape fit functions
double fit_func(double *x, double *par)
{
  if (x[0] < par[0]) return par[2];
  else if (x[0] < par[1]) return par[2] + (x[0]-par[0])/(par[1]-par[0])*(par[3]-par[2]);
  else return par[3] + (x[0]-par[1])*par[4];
}

/// class for handling board readout
class BoardData {
public:
	/// constructor
	BoardData(int basenum, const float *cmul, unsigned int nchan = CHANNELS_PER_BOARD):
	  baseNumber(basenum), nChannels(nchan), prevtimestamp(-666), timewrap(1<<28), nEventDraw(100), extractOption(1) { 
		data.resize(nChannels);
		printf("board basenum : %d \n", basenum);
		for (unsigned int i=0; i<nChannels; i++) {
			clockMul[i] = cmul[i];
			printf("cmul[%d] = %f \n", i, clockMul[i]);
			timeoffset[i] = 0;
			iEventDraw[i] = 0;
		}
                f1 = new TF1("f1", fit_func, 0.0, 1e3, 5);

	}
	/// destructor
	~BoardData() {}
	/// allocate buffer and return pointer for adding new data
	unsigned char* getWriteBuffer(unsigned int channel, unsigned int size) {
		assert(channel < nChannels);
		int old_size = data[channel].size();	
		data[channel].resize(old_size+size);
		return &data[channel][old_size];
	}
	/// clear data
	void clearData() {
		for(unsigned int i=0; i<data.size(); i++)
			data[i].clear();
	}

	/// process data and append to events vector
	void process_events(std::deque<NGammaEvent>& events) {
		
		// TODO handle this more cleanly across channels
		//Long64_t origOffset = timeoffset;
		//Long64_t origTimestamp = prevtimestamp;
		
		for(unsigned int c=0; c<nChannels; c++) {
			
			if(data[c].size() == 0) continue;
			unsigned int bank_size = data[c].size() / 10;
			//timeoffset = origOffset;
			//prevtimestamp = origTimestamp;
			
			std::vector<int> islandSamples;	// samples in current island
			Long64_t islandT0 = -1;			// start time for current island
			
			// loop through words to build up "islands"
			for (int j = 0; j < bank_size; j++) {
				
				// data format:
				// 
				//  bits 
				//  79-79       ?? 		[1]
				//  78-52       timestamp [27]
				//  51-48       overflow [4]
				//  47-36       sampleB0 [12]
				//  35-24       sampleA0 [12]
				//  23-12       sampleB1 [12]
				//  11-0        sampleA1 [12]
				Long64_t timestamp = (data[c][j * 10 + 0] << 20) | (data[c][j * 10 + 1] << 12) | (data[c][j * 10 + 2] << 4) | (data[c][j * 10 + 3] >> 4);
				bool overflowB0 = ((data[c][j * 10 + 3] & 0x08) != 0);
				bool overflowA0 = ((data[c][j * 10 + 3] & 0x04) != 0);
				bool overflowB1 = ((data[c][j * 10 + 3] & 0x02) != 0);
				bool overflowA1 = ((data[c][j * 10 + 3] & 0x01) != 0);
				int sampleB0 = (overflowB0 << 12) | (data[c][j * 10 + 4] << 4) | (data[c][j * 10 + 5] >> 4);
				int sampleA0 = (overflowA0 << 12) | ((data[c][j * 10 + 5] & 0xf) << 8) | (data[c][j * 10 + 6]);
				int sampleB1 = (overflowB1 << 12) | (data[c][j * 10 + 7] << 4) | (data[c][j * 10 + 8] >> 4);
				int sampleA1 = (overflowA1 << 12) | ((data[c][j * 10 + 8] & 0xf) << 8) | (data[c][j * 10 + 9]);
				
				// startup case
				if(prevtimestamp == -666)
					prevtimestamp = timestamp;
				
				// timestamp wraparound fix
				while(timestamp+timeoffset[c]-prevtimestamp < -timewrap/2 && j > 2)
					{ printf("b %i c %i timestamp %lld timeoffset %lld prevtimestamp %lld\n", baseNumber, c, timestamp, timeoffset[c], prevtimestamp);
					  timeoffset[c] += timewrap;
					  printf("timeoffset %lld\n", timeoffset[c]);
					}
				while(timestamp+timeoffset[c]-prevtimestamp > timewrap/2 && j > 2)
					{ printf("b %i c %i timestamp %lld timeoffset %lld\n", baseNumber, c, timestamp, timeoffset[c]);
					  timeoffset[c] -= timewrap; 
					  printf("timeoffset %lld\n", timeoffset[c]);
					}
				//printf("timeoffset %lld\n", timeoffset[c]);
				timestamp += timeoffset[c];
				
				if(0) {
					if(!c)
						printf("%i %i: %lld\t%i\t%i\t%i\t%i\n",c,j,timestamp,sampleA1,sampleB1,sampleA0,sampleB0);
					if (timestamp < prevtimestamp && !c)
						printf("Time ordering error! %i %lld - %lld = %lld\n",j,timestamp,prevtimestamp,timestamp-prevtimestamp);
				}
				
				// check if previous "island" is completed by timestamp discontinuity
				if (timestamp != prevtimestamp + 1) {
					if(islandSamples.size() > 0)
						processIsland(islandT0 * 4, islandSamples, baseNumber + c, events);
					islandT0 = timestamp;
					islandSamples.clear();
				}
				
				// fill new data
				prevtimestamp = timestamp;
				islandSamples.push_back(sampleA1);
				islandSamples.push_back(sampleB1);
				islandSamples.push_back(sampleA0);
				islandSamples.push_back(sampleB0);
			}
			
			if (islandSamples.size() > 0)
				processIsland(islandT0 * 4, islandSamples, baseNumber + c, events);
		}
	}

	TSpline3 *makeSpline(std::vector<int> &rawSamples, int numRawSamples)
	{
	    double sRaw[numRawSamples];
	    double tRaw[numRawSamples];
	    int nPoints = 0;
	    double rawBinWidth = 1;
	    for (int i = 0; i < numRawSamples; i++) {
	        sRaw[nPoints] = rawSamples[i];
	        tRaw[nPoints] = i;
	        if (sRaw[nPoints] >= 0 && sRaw[nPoints] <= 4095) {
	            nPoints++;
	        }
	    }

	    if (nPoints < 4) {
	        return NULL;
	    }

	    TSpline3 *spl = new TSpline3("spline", tRaw, sRaw, nPoints, "b1e2", 0, 0);
	    return spl;
	}

        TGraph *makeGraph(std::vector<int> &rawSamples, int numRawSamples)
        {
            double sRaw[numRawSamples];
            double tRaw[numRawSamples];
            int nPoints = 0;
            double rawBinWidth = 1;
            for (int i = 0; i < numRawSamples; i++) {
                sRaw[nPoints] = rawSamples[i];
                tRaw[nPoints] = i;
                if (sRaw[nPoints] >= 0 && sRaw[nPoints] <= 4095) {
                    nPoints++;
                }
            }

            if (nPoints < 4) {
                return NULL;
            }

            TGraph *graph = new TGraph(nPoints, tRaw, sRaw);
            return graph;
        }

	/// process one 'island' of events
	int processIsland(Long64_t t0, std::vector<int>& samples, int channel, std::deque<NGammaEvent>& events) {
		
		if (samples.size() < 100)
			return 0;
		
		// find min/max value sample values
		std::vector<int>::iterator minSample = std::min_element(samples.begin(),samples.end());
		std::vector<int>::iterator maxSample = std::max_element(samples.begin(),samples.end());
		
		// find the pedestal by averaging the first four samples
		// find the pedestal by averaging the five samples close to the peak
		double pedestal = 0;
		for (int i = 0; i < PEDESTAL_SAMPLE_COUNT; i++)
			pedestal += samples[PEDESTAL_START + i];
		pedestal /= PEDESTAL_SAMPLE_COUNT;
		
		// Add up the total area
		double area = 0;
		//for(std::vector<int>::iterator it = std::max(maxSample +20,samples.begin()); it < std::min(maxSample + 81,samples.end()); it++)
		for(std::vector<int>::iterator it = samples.begin(); it < samples.end(); it++)
			area += (*it - pedestal)*clockMul[channel-baseNumber];
		
		// find interpolated peak around max using 3-point parabola
		double maxInterp = double(*maxSample);
		double centerInterp = (double)int(maxSample-samples.begin());
		double centerDelta = 0;
		if(samples.begin() < maxSample && maxSample+1 < samples.end()) {
			double y1 = (*(maxSample-1));
			double y2 = (*maxSample);
			double y3 = (*(maxSample+1));
			if(y1 != y2 || y2 != y3) {
				centerDelta = (y3-y1)/(4.0*y2-2.0*(y1+y3));
				maxInterp = y2+(y3-y1)*centerDelta/4.0;
			}
		//	printf("y1 %i %e y2 %e y3 %e \n", int(maxSample-samples.begin()), y1, y2, y3);
		}

		TGraph *graph = makeGraph(samples, samples.size());
		//TH1F *h0 = 0;
		TH1F *h0_smooth = 0;
		TH1F *h0_slope = 0;
		double dEdx_min = 0;
		double dEdx_min_t = 0;
		double t1 = 0;
		double t2 = 0;
		double p1 = 0;
		double p2 = 0;

		if (!graph) 
		{
			printf("graph does not exist!\n");
        }
		else
		{
			if (extractOption == 0)
			{
				double xmax = samples.size();
		                f1->SetParLimits(0, 0.0*xmax, 0.7*xmax);
        		        f1->SetParLimits(1, 0.3*xmax, 1.0*xmax);
	        	        f1->SetParLimits(2, *minSample + 0.8*(*maxSample - *minSample), *maxSample);
	                	f1->SetParLimits(3, *minSample - 0.0*(*maxSample - *minSample), *minSample + 0.3*(*maxSample - *minSample));
		                f1->SetParLimits(4, 0, 10.0);
        		        graph->Fit("f1", "QR", "", 0, samples.size());
			}
			else if (extractOption == 1)
			{
				//graph->Print();
				TH1F *h0 = graph->GetHistogram();
				std::vector<double> vec_pulse_tmp;
				for (int i = 0; i < h0->GetNbinsX(); i++)
                                {
					double x = 0;
					double y = 0;
					graph->GetPoint(i, x, y);
					h0->SetBinContent(i+1, y);
					//std::cout << h0->GetBinContent(i+1) << std::endl;
					vec_pulse_tmp.push_back(y);
                                }

				h0_smooth = new TH1F("h0_smooth", "h0_smooth", h0->GetNbinsX(), 0, h0->GetNbinsX());
				int npoint_smooth = 3;
				for (int i = 0; i < h0->GetNbinsX(); i++)
				{
				  int beg = i - npoint_smooth; if (beg < 0) beg = 0;
				  int end = i + npoint_smooth; if (end >= h0->GetNbinsX()) end = h0->GetNbinsX()-1;
				  double y_smooth = 0;
				  for (int j = beg; j <= end; j++) y_smooth += vec_pulse_tmp[j];
				  y_smooth /= (end-beg+1);
				  h0_smooth->SetBinContent(i+1, y_smooth);
				}
				
				h0_smooth->Smooth(3);
				h0_slope = new TH1F("h0_slope", "h0_slope", h0->GetNbinsX(), 0, h0->GetNbinsX());
				//std::cout << h0->GetNbinsX() << std::endl;
				for (int i = 1; i < h0->GetNbinsX(); i++)
				{
					h0_slope->SetBinContent(i, h0_smooth->GetBinContent(i+1)-h0_smooth->GetBinContent(i));
					//std::cout << h0_slope->GetBinContent(i) << std::endl;
				}
				h0_slope->Smooth(3);

				dEdx_min = 0;
				dEdx_min_t = 0;
				double fraction = 0.6;
				for (int i = 0; i < fraction*h0_slope->GetNbinsX(); i++)
				{
					if (h0_slope->GetBinContent(i+1) < dEdx_min) 
					{
						dEdx_min = h0_slope->GetBinContent(i+1);
						dEdx_min_t = i;
					}
				}

				double threshold = 0.4;
				for (int i = (int)dEdx_min_t; i >= 0; i--)
                                {
                                        if ( h0_slope->GetBinContent(i+1) > threshold * dEdx_min )
                                        {
						t1 = i;
						p1 = h0->GetBinContent(i+1);
						break;
                                        }
                                }
                                for (int i = (int)dEdx_min_t; i < h0_slope->GetNbinsX(); i++)
                                {
                                        if ( h0_slope->GetBinContent(i+1) > threshold * dEdx_min )
                                        {
                                                t2 = i;
                                                p2 = h0->GetBinContent(i+1);
						break;
                                        }
                                }
				//std::cout << "dEdx_min " << dEdx_min << " t " << dEdx_min_t << " t1 " << t1 << " t2 " << t2 << " p1 " << p1 << " p2 " << p2 << std::endl;
			}
		}

                // save the first nEventDraw events in output file
                if (iEventDraw[channel-baseNumber] < nEventDraw && dEdx_min < -4)
                {
			TString c_name("c_"); c_name += channel; c_name += "_"; c_name += iEventDraw[channel-baseNumber];
			std::cout << c_name << std::endl;
			TCanvas *c = new TCanvas(c_name, c_name, 1200, 800);
			TH1F *h1 = new TH1F("h1", "h1", samples.size(), 0, samples.size());
			TH2F *h2 = new TH2F("h2", "h2", 100, *minSample, *maxSample, 100, -50, 50);

			iEventDraw[channel-baseNumber]++;

			int k_npoint = 3;	
			for(std::vector<int>::iterator it = samples.begin(); it < samples.end(); it++)
			{
				int y0 = *it;

				double slope_l = 0.0;
				std::vector<int>::iterator it_l = std::max(it-k_npoint, samples.begin());
				int npoint_l = it - it_l;
				//  printf("npoint_l %d y0 %d ", npoint_l, y0);
				for(it_l = std::max(it-k_npoint, samples.begin()); it_l < it; it_l++)
				{
					//printf("left %d : %d ", it-it_l, *it_l);
					slope_l += double(y0 - *it_l) / (it-it_l);
				}
				if (npoint_l > 0) slope_l /= npoint_l;
				//printf("slope_l %f ", slope_l);

				double slope_r = 0.0;
				std::vector<int>::iterator it_r = std::min(it+k_npoint, samples.end());
				int npoint_r = it_r - it;
				//printf("npoint_r %d y0 %d ", npoint_r, y0);
				for(it_r = std::min(it+k_npoint, samples.end()); it_r > it; it_r--)
				{
					//printf("right %d : %d ", it_r-it, *it_r);
					slope_r += double(y0 - *it_r) / (it-it_r);
				}
				if (npoint_r > 0) slope_r /= npoint_r;
				//printf("slope_r %f ", slope_r);
				//printf("slope_delta %f \n", slope_r - slope_l);

				h1->SetBinContent( it-samples.begin(), slope_l );
				h2->Fill( *it, slope_l );
			}

		  	c->Divide(2, 2);
	 	  	c->cd(1);
		  	if (graph) graph->Draw("AGL");
			if (h0_smooth) { h0_smooth->SetLineColor(2); h0_smooth->Draw("same"); }
		  	c->cd(2);
		  	h1->Draw();
			c->cd(3);
			h2->SetMarkerStyle(2);
			h2->Draw();
			c->cd(4);
			if (h0_slope) { 
			  h0_slope->Draw();
			  TLatex l;
			  TString note("dt_");  note += (t2-t1);
			  note += " t1 "; note += t1;
			  note += " t2 "; note += t2;
			  l.DrawLatex(0,0, note);
			}
			c->Write();

			if (h1) delete h1;
			if (h2) delete h2;
			if (c) delete c;
		}
		//if (h0) delete h0;
		if (h0_smooth) delete h0_smooth;
		if (h0_slope) delete h0_slope;
		if (graph) delete graph;

/*
		TGraph *graph = makeGraph(samples, samples.size());
		TSpline3 *spl = 0;
		//sple = makeSpline(samples, samples.size(), graph);		

		// save the first nEventDraw events in output file
		if (iEventDraw[channel-baseNumber] < nEventDraw && (*minSample) < 3000)
		{
		  TString c_name("c_"); c_name += channel; c_name += "_"; c_name += iEventDraw[channel-baseNumber];
		  TCanvas *c = new TCanvas(c_name, c_name);
		  if (graph)
		  {
		    std::cout << "graph " <<  graph->GetName() << std::endl;
 		    graph->SetMarkerStyle(2);
		    graph->Draw("AGL");
		  }
		  if (spl)
		  {
		    spl->Print();
		    spl->Draw("same");
		  }
		  c->Write();
		  delete c;
		  iEventDraw[channel-baseNumber]++;
		}
		if(spl) {
		  delete spl;
		}
		if(graph) {
		  delete graph;
		}
*/
		// fill event data
		NGammaEvent ev;
		ev.channel = channel;
		ev.triggerTime = Long64_t(t0 * clockMul[channel-baseNumber]);
		ev.peakTime = Long64_t((t0 + int(maxSample-samples.begin())) * clockMul[channel-baseNumber]);
		ev.peakTimeInterp = (t0 + centerInterp) * clockMul[channel-baseNumber];
		ev.pedestal = pedestal;
		ev.minSample = *minSample;
		ev.maxSample = *maxSample;
		ev.maxInterp = maxInterp;
		ev.centerDelta = centerDelta;
		ev.area = area;
                if (extractOption == 0)
                {
			ev.t1 = f1->GetParameter(0) * clockMul[channel-baseNumber]; 
			ev.dt = (f1->GetParameter(1) - f1->GetParameter(0)) *clockMul[channel-baseNumber];
			ev.p1 = f1->GetParameter(2);
			ev.p2 = f1->GetParameter(3);
			ev.dEdx = -999;
			if (ev.dt != 0) ev.dEdx = (ev.p1 - ev.p2)/ev.dt; 
			ev.dEdx_tail = f1->GetParameter(4);		
			ev.chi2 = f1->GetChisquare() / (samples.size()-5);
		}
		else if (extractOption == 1)
                {
			ev.t1 = t1 * clockMul[channel-baseNumber];
			ev.dt = (t2 - t1) * clockMul[channel-baseNumber];
			ev.p1 = p1;
			ev.p2 = p2;
			ev.dEdx = -999;
                        if (ev.dt != 0) ev.dEdx = (ev.p1 - ev.p2)/ev.dt;
			ev.dEdx_min = dEdx_min;
		}

		events.push_back(ev);

            if( (t0*clockMul[channel-baseNumber] > 2.79e9 && t0*clockMul[channel-baseNumber] < 3e9) ) 
            {
                std::cout << "t0 " << t0*clockMul[channel-baseNumber] << " size " << samples.size() << std::endl;
                std::vector<double> slope_l;
                std::vector<double> slope_r;
                std::vector<double> slope_delta;
                for(std::vector<int>::iterator it = samples.begin()+1; it < samples.end()-1; it++)
                {
                  double y1 = (*(it-1));
                  double y2 = (*it);
                  double y3 = (*(it+1));
                  slope_l.push_back( y2-y1 );
                  slope_r.push_back( y3-y2 );
                  slope_delta.push_back( (y3-y2)-(y2-y1) );
                  printf("y %e slope_l %e r %e delta %e \n", y2, y2-y1, y3-y2, (y3-y2)-(y2-y1));
                }
                }
                //for(std::vector<double>::iterator it = slope_delta.begin(); it < slope_delta.end(); it++)
                //{
                //printf("slop %e \n", (*it));
                //}             

	}
	
	
protected:
	float clockMul[CHANNELS_PER_BOARD];					//< clock multiplier
	const int baseNumber;							//< base number for numbering channels
	const unsigned int nChannels;					//< number of channels
	std::vector< std::vector<unsigned char> > data;	//< raw data per channel
	Long64_t timeoffset[CHANNELS_PER_BOARD];							//< timestamp wraparound correction offset
	Long64_t prevtimestamp;							//< timestamp of previous event
	const Long64_t timewrap;						//< timestamp wraparound time
	int iEventDraw[CHANNELS_PER_BOARD];
	const int nEventDraw;
	TF1 *f1;
	int extractOption;

};


void process_file(char* input_filename, RollingWindow* W)
{
    // open input file
    //TString dir_name("/home/kevinh/Data/UCN/UCNb2010/raw/");
    //TString raw_dir_name(getenv("UCNb_RAW_DATA_DIR"));

    FILE* input_fp = fopen(input_filename, "rb");    // try full path name or base directory first
/*
    if (!input_fp) {
    	input_fp = fopen(raw_dir_name+"/"+input_filename, "rb");
    }
*/
    if (!input_fp) {
		fprintf(stderr, "Unable to open %s: \n",  (raw_dir_name+"/"+input_filename).Data());
/*
    	if(raw_dir_name.CompareTo(""))
			perror("The environmental variable UCNb_RAW_DATA_DIR does not seem to be set.");
*/
		exit(1);
    }
	
	// active boards to read out
	std::map<unsigned int,BoardData> active_boards;
	active_boards.insert(std::pair<unsigned int,BoardData>(1<<4,BoardData(0, k_clockMul[0])));
	active_boards.insert(std::pair<unsigned int,BoardData>(1<<5,BoardData(CHANNELS_PER_BOARD, k_clockMul[1])));
	active_boards.insert(std::pair<unsigned int,BoardData>(1<<6,BoardData(2*CHANNELS_PER_BOARD, k_clockMul[2])));
	active_boards.insert(std::pair<unsigned int,BoardData>(1<<7,BoardData(3*CHANNELS_PER_BOARD, k_clockMul[3])));
	
	// read file until end
	printf("Reading from file '%s'...\n",input_filename);
	int event_number = 0;
	std::deque<NGammaEvent> events;
    	while (!feof(input_fp)) {
		
		double blockStartTime = -1;		// start time [s] of current data block
		double packetTime = -1;			// time [s] of most recently read packet
		double blockWidth = TIME_CHUNK;	// width [s] of each data block to read

		// read block file packets until time limit or EOF reached
		int npackets = 0;
		while (packetTime - blockStartTime < blockWidth) {
			// read packet header from file
			output_header header;
			if (fread(&header, sizeof(struct output_header), 1, input_fp) != 1)
				break;
			npackets++;
			if(header.data_size == 0)
				continue;
			
			// locate the board number for this packet, check if active
			std::map<unsigned int,BoardData>::iterator boardit = active_boards.find(header.board_number);
			if(boardit == active_boards.end()) {
				printf("Received packet from unexpected board %d\n",header.board_number);
				continue;
			}
			
			// read in the data
			fread(boardit->second.getWriteBuffer(header.fadc_number, header.data_size), 1, header.data_size, input_fp);
			
			packetTime = header.tv_usec / 1e6 + header.tv_sec;
			if (blockStartTime < 0)
				blockStartTime = packetTime;
		}
		printf("--- %i packets read (%g s).\n",npackets,packetTime-blockStartTime);
	
	printf("blockStartTime %f \n ", blockStartTime);
	//if(blockStartTime > 1286398640) break;
	
		// process and clear out data
		for (std::map<unsigned int,BoardData>::iterator it = active_boards.begin(); it != active_boards.end(); it++) {
			it->second.process_events(events);
			it->second.clearData();
		}
		
		// sort, feed 3/4 of events to coincidence window processor (leave last quarter to merge with next round results)
		std::sort(events.begin(), events.end(), compareEventsByTime);
		int ntoleave = events.size()/4;
		while(events.size() > ntoleave) {
			events.front().evtnum = event_number++;
			W->push_event(events.front());
			events.pop_front();
		}
		
		printf("--- %i events ---\n",event_number);
		W->printCounts();
		
	}
	
	// process remaining events
	while(events.size()) {
		events.front().evtnum = event_number++;
		W->push_event(events.front());
		events.pop_front();
	}
	printf("--- %i events ---\n",event_number);
	W->printCounts();
	
	fclose(input_fp);
}

int main(int argc, char *argv[])
{
    if (argc < 2) {
		printf("Usage: <raw data filename> \n");
		exit(1);
    }

    for (int i = 1; i < argc; i++)
    {
	// open output file
	//TString dir_save("Z:/UCNb/Processed/");
	//TString dir_save("/home/kevinh/Data/UCN/UCNb2010/processed/");
	//TString dir_save(getenv("UCNb_PROCESSED_DATA_DIR"));

	//TFile* output_file = new TFile(dir_save+"/"+TString(argv[1]).ReplaceAll(".fat","")+".root", "RECREATE");
	TFile* output_file = new TFile(TString(argv[1]).ReplaceAll(".fat","")+".root", "RECREATE");
	std::cout << output_file->GetName() << std::endl;
	
	// rolling window for coincidences within 100 clock cycles (?? what are these units)
	RollingWindow W(6000);
	
	// recorder for all events
	RecordAllEvents RA;
	W.addProcessor(&RA);
/*
	// 2-way coincidences betweem channels 1 and 2
	MultiCoincidence MC("chan_16_17");
	MC.addCoincidentChannel(16);
	MC.addCoincidentChannel(17);
	W.addProcessor(&MC);
	
 
	// 3-way coincidences betweem channels 0, 1, and 2
	MultiCoincidence MC2("chan_6_22_25");
	MC2.addCoincidentChannel(6);
	MC2.addCoincidentChannel(22);
	MC2.addCoincidentChannel(25);
	W.addProcessor(&MC2);

        // 4-way
        MultiCoincidence *MC3[1000];
        int mc_count = 0;
        for (int i_channel_3 = 0; i_channel_3 < 27; i_channel_3++)
        {
            if (i_channel_3 >= 6 && i_channel_3 < 16 || i_channel_3 == 22) continue;
            for (int i_channel_4 = 0; i_channel_4 < 27; i_channel_4++)
            {
              if (i_channel_4 >= 6 && i_channel_4 < 16 || i_channel_4 == 22) continue;
              if (i_channel_4 <= i_channel_3) continue;
              int i_channel_1 = 6;
              int i_channel_2 = 22;
              char mc3_name[128];
              sprintf(mc3_name,"chan_%i_%i_%i_%i",i_channel_1, i_channel_2, i_channel_3, i_channel_4);
              MC3[mc_count] = new MultiCoincidence(mc3_name);
              MC3[mc_count]->addCoincidentChannel(i_channel_1);
              MC3[mc_count]->addCoincidentChannel(i_channel_2);
              MC3[mc_count]->addCoincidentChannel(i_channel_3);
              MC3[mc_count]->addCoincidentChannel(i_channel_4);
              W.addProcessor( &(*MC3[mc_count]) );
              mc_count++;
            }
        }
*/
        // scan events in input file
        process_file(argv[1], &W);
	
	// close output file
        output_file->Write();
        output_file->Close();
    }
    return 0;
}
