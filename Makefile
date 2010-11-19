analyze-waveform: TabSepParser.o analyze-waveform.o
	g++ -o analyze-waveform TabSepParser.o analyze-waveform.o

TabSepParser.o: TabSepParser.cc TabSepParser.hh
	g++ -c TabSepParser.cc

analyze-waveform.o: analyze-waveform.cc TabSepParser.hh
	g++ -c analyze-waveform.cc 

clean:
	rm analyze-waveform analyze-waveform.o TabSepParser.o


