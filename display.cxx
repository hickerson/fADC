// Written in standard C++; requires the ROOT framework for histogram display

#include <iostream>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "TH1.h"
#include "TH2.h"
#include "TFile.h"
#include "TTree.h"
#include "TCanvas.h"
#include "TString.h"
#include <vector>

using namespace std;

//const Long64_t k_timestamp_offset = 1<<28;
const Long64_t k_timestamp_offset = 1<<28;
const int k_nboard = 3;
const int k_nchannel = 8;
const int k_timemulti=30;

struct output_header {
    int board_number;
    int packet_serial;
    int fadc_number;
    int data_size;
    unsigned int tv_sec;
    unsigned int tv_usec;
};

int get_board_index(int board)
{
  if (board == 16) return 0;
  else if (board == 64) return 1;
  else if (board == 128) return 2;
  else cout << "board error " << endl;

  return 3;
}

void display(char *filename, int channel_1 = -1)
{
  //TString dir_name("//Pcprad121/PRADUsers/ProtonInterrogation/WNR2010/June30/");
  //TString dir_name("H:/June30/");
  // TString dir_name("//Pcprad121/PRADUsers/WangPRAD/Data/FADC/He4/Sept2010/");

  //TString dir_name("/home/ucnbdaq/Data/UCNb/raw/");
  TString dir_name(getenv("UCNb_RAW_DATA_DIR"));
  TString dir_file_name = dir_name + filename;
  FILE *fp = fopen(dir_file_name.Data(), "rb");
  if (not fp)
	cout << "can't find file " << dir_file_name << endl;

  unsigned char raw[2048];

  ofstream of_file("run.log");

  std::cout << "k_timestamp_offset = " << k_timestamp_offset << std::endl;
  Long64_t timestamp_shift[k_nboard][k_nchannel];
  Long64_t timestamp_offset[k_nboard][k_nchannel];
  Long64_t timestamp_pre_entry[k_nboard][k_nchannel];
  int timestamp_reset_count[k_nboard][k_nchannel];
  int timestamp_entry_count[k_nboard][k_nchannel];
  for (int i = 0; i < k_nboard; i++) {
	for (int j = 0; j < k_nchannel; j++) {
      timestamp_shift[i][j] = 0;
	  timestamp_offset[i][j] = 0;
      timestamp_pre_entry[i][j] = 0;
	  timestamp_reset_count[i][j] = 0;
	  timestamp_entry_count[i][j] = 0;
	}
  }

  TCanvas *c1 = new TCanvas();
 
  vector<int> con_timestamp;
  vector<int> con_data;
  int count = 0;

  TString dir_save("/home/ucnbdaq/Data/UCNb/processed/");
  //TString dir_save(getenv("UCNb_PROCESSED_DATA_DIR"));
  //TString dir_save("H:/He4/");
  //TString dir_save("//Pcprad121/pradusers/WangPRAD/Data/FADC/He4/Sept2010/");
  TFile *f_out = new TFile( dir_save+TString(filename).ReplaceAll(".fat","")+".root", "RECREATE");
  TTree *t = new TTree("t", "data tree");
  Long64_t time;
  int data;
  int channel;
  int board;
  t->Branch("time", &time, "time/L");
  t->Branch("data", &data, "data/I");
  t->Branch("channel", &channel, "channel/I");
  t->Branch("board", &board, "board/I");


  TH2D *pulseHist = new TH2D("pulseHist", "Digitized pulse", 
      1000, 0, 5e9, 4096, 0, 4096);

  while(!feof(fp)) {
    output_header o;
 
    // read header
    fread(&o, sizeof(o), 1, fp);
    //printf("Board %d Serial %d FADC %d Size %d Time %d.%06d\n",
    //  o.board_number, o.packet_serial, o.fadc_number, o.data_size, o.tv_sec, o.tv_usec);
    
    if(o.data_size <= 0) {
      //printf("Start/stop packet, skipping...\n");
      continue;
    }
 
    // read data
    fread(raw, o.data_size, 1, fp);

    if(channel_1 != -1 && channel_1 != o.fadc_number)
      continue;

    // interpret data
    int num_words = o.data_size / 10;

 
    //int ii = 0;

    for(int j = 0; j < num_words; j++) {

      //  bits
      //  78-52       timestamp
      //  51-48       overflow
      //  47-36       sampleB0
      //  35-24       sampleA0
      //  23-12       sampleB1
      //  11-0        sampleA1
      Long64_t timestamp =  (raw[j*10+0] << 20) |
                       (raw[j*10+1] << 12) |
                       (raw[j*10+2] << 4) |
                       (raw[j*10+3] >> 4);
      bool overflowB0 = ((raw[j*10+3] & 0x08) != 0);
      bool overflowA0 = ((raw[j*10+3] & 0x04) != 0);
      bool overflowB1 = ((raw[j*10+3] & 0x02) != 0);
      bool overflowA1 = ((raw[j*10+3] & 0x01) != 0);
      int sampleB0 = (overflowB0 << 12) |
                     (raw[j*10+4] << 4) |
                     (raw[j*10+5] >> 4);
      int sampleA0 = (overflowA0 << 12) |
                     ((raw[j*10+5] & 0xf) << 8) |
                     (raw[j*10+6]);
      int sampleB1 = (overflowB1 << 12) |
                     (raw[j*10+7] << 4) |
                     (raw[j*10+8] >> 4);
      int sampleA1 = (overflowA1 << 12) |
                     ((raw[j*10+8] & 0xf) << 8) |
                     (raw[j*10+9]);

      //printf("t=%d %d %d %d %d\n", timestamp,
      //       sampleA1, sampleB1, sampleA0,sampleB0);

	  //if(o.fadc_number == k_channel_num &&  o.board_number == k_board_num)


/*
	  con_timestamp.push_back(timestamp*16);
	  con_data.push_back(sampleA1);
	  con_timestamp.push_back(timestamp*16+4);
	  con_data.push_back(sampleB1);
	  con_timestamp.push_back(timestamp*16+8);
	  con_data.push_back(sampleA0);
	  con_timestamp.push_back(timestamp*16+12);
	  con_data.push_back(sampleB0);
  

      pulseHist->Fill(timestamp*16, sampleA1);
      pulseHist->Fill(timestamp*16+4, sampleB1);
      pulseHist->Fill(timestamp*16+8,  sampleA0);
      pulseHist->Fill(timestamp*16+12, sampleB0);
*/

	  channel = o.fadc_number;
	  board = o.board_number;
      int board_index = get_board_index(board);

      timestamp_entry_count[board_index][channel]++;

 	  if (board_index == 0 && channel == 2) of_file << timestamp << "  " << timestamp_pre_entry[board_index][channel] << endl;
	  if (timestamp < timestamp_pre_entry[board_index][channel] && 
		  fabs(timestamp - timestamp_pre_entry[board_index][channel]) > 0.95*k_timestamp_offset &&
		  timestamp_entry_count[board_index][channel] > 300 ) {
	    timestamp_entry_count[board_index][channel] = 0;
		timestamp_shift[board_index][channel] += k_timestamp_offset;
		timestamp_reset_count[board_index][channel]++;
 	    
		if (board_index == 0 && channel == 2) {
		  of_file << " * " << timestamp << "  " << timestamp_pre_entry[board_index][channel] 
		       << "  " << fabs(timestamp - timestamp_pre_entry[board_index][channel])*1.0/k_timestamp_offset
			   << endl;
          printf("timestamp for board %i channel %i reset ... \n", board, channel);
          of_file << "timestamp for board " << board << " channel " << channel << " reset ... " << endl;
		}
	  }
      timestamp += timestamp_shift[board_index][channel];

	  time = timestamp*16;
      if (board == 128) time *=k_timemulti;
	  data = sampleA1;
      t->Fill();

      time = timestamp*16+4;
	  if (board == 128) time *= k_timemulti;
	  data = sampleB1;
	  t->Fill();

	  time = timestamp*16+8;
	  if (board == 128) time *= k_timemulti;
	  data = sampleA0;
	  t->Fill();

	  time = timestamp*16+12;
	  if (board == 128) time *= k_timemulti;
	  data = sampleB0;
	  t->Fill();

      timestamp_pre_entry[board_index][channel] = timestamp - timestamp_shift[board_index][channel];

	  
	  count++;
    }

	//if (count > 10000) break;
	if (count%10000 == 0) printf("count %d \n", count);

    // Wait for the user to press a key
    // getchar();

  }
  printf("timestamp num %d data num %d \n", (int)con_timestamp.size(), (int)con_data.size());

  for(int i = 0; i < k_nboard; i++) {
	for (int j = 0; j < k_nchannel; j++) {
      cout << " board " << i << " channel " << j << " reset " << timestamp_reset_count[i][j] << endl;
	}
  }

  t->Write();
  //t->Draw("data:time","board==64","pl");

  //Draw the histogram
  pulseHist->Draw();
  c1->Update();
  c1->Modified();

  f_out->Close();
}
