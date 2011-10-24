ROOTCONFIG      := root-config
ROOTLIBS        := $(shell $(ROOTCONFIG) --libs)
ROOTGLIBS       := $(shell $(ROOTCONFIG) --glibs)
ROOTINC         := $(shell $(ROOTCONFIG) --incdir)
ROOTCFLAGS      := $(shell $(ROOTCONFIG) --cflags)
CXXFLAGS        = -O0 -g -Wall -fPIC -I$(ROOTINC) $(ROOTCFLAGS)
CXX             = g++
LD              = g++

# g++ -O0 -g `root-config --cflags` -c PeriodicSpectrum.cc
# g++ -O0 -g `root-config --cflags` analysisUCNb.cxx Dictionary.cxx -o analysisUCNb `root-config --libs` 
# g++ `root-config --cflags` `root-config --libs` spectrum_analyser.cc Dictionary.cxx -o spectrum_analyser
#g++ `root-config --cflags` `root-config --libs` Ge_analyser.cc Dictionary.cxx -o Ge_analyser
#g++ `root-config --cflags` `root-config --libs` periodic_analyser.cc Dictionary.cxx -o periodic_analyser PeriodicSpectrum.o
#g++ `root-config --cflags` `root-config --libs` calibration_analyser.cc Dictionary.cxx -o calibration_analyser PeriodicSpectrum.o
#git add *.cc *.hh *.h *.cxx buildUCNb

analysisUCNb: analysisUCNb.o Dictionary.o
	$(LD) -O0 -g $(LDFLAGS) $^ $(ROOTLIBS) -o $@

spectrum_analyser: spectrum_analyser.o Dictionary.o
	$(LD) -O0 -g $(LDFLAGS) $^ $(ROOTLIBS) -o $@

%.o:: %.cc
	$(CXX) $(CXXFLAGS) -Wno-deprecated -c $< -o $@

Dictionary.cc: NGammaEvent.h LinkDef.h
	@echo "Generating dictionary $@..."
	@rootcint -f $@ -c $^

CalibratedEventDictionary.cxx: CalibratedEvent.hh LinkDef.h
	@echo "Generating dictionary $@..."
	@rootcint -f $@ -c $^

# rootcint -f Dictionary.cxx -c NGammaEvent.h LinkDef.h
# rootcint -f CalibratedEventDictionary.cxx -c CalibratedEvent.hh LinkDef.h



