ROOTCONFIG      := root-config
ROOTLIBS        := $(shell $(ROOTCONFIG) --libs)
ROOTGLIBS       := $(shell $(ROOTCONFIG) --glibs)
ROOTINC         := $(shell $(ROOTCONFIG) --incdir)
ROOTCFLAGS      := $(shell $(ROOTCONFIG) --cflags)
CXXFLAGS        = -O0 -g -Wall -fPIC -I$(ROOTINC) $(ROOTCFLAGS) -I/usr/include
CXX             = g++
LD              = g++


all: mega_analyser 

clean: 
	rm *.o mega_analyser

html:
	doxygen
	sudo cp -r ~/Public/fADC /var/www

commit:
	git add *.cc *.hh *.h *.cxx Makefile
	git commit -a

analysisUCNb: analysisUCNb.o Dictionary.o
	$(LD) -O0 -g $(LDFLAGS) $^ $(ROOTLIBS) -o $@

spectrum_analyser: spectrum_analyser.o Dictionary.o
	$(LD) -O0 -g $(LDFLAGS) $^ $(ROOTLIBS) -o $@

full_analyser: full_analyser.o Dictionary.o PeriodicSpectrum.o
	$(LD) -O0 -g $(LDFLAGS) $^ $(ROOTLIBS) -o $@

mega_analyser: mega_analyser.o Dictionary.o RunGroup.o Spectrum.o Run.o
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



