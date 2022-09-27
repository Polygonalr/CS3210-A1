CXX=g++
CXXFLAGS:=-Wall -Werror -Wextra -pedantic -std=c++17 -fopenmp
NOOMPFLAGS:=-Wall -Wextra -pedantic -std=c++17
RELEASEFLAGS:=-O3
DEBUGFLAGS:=-g

.PHONY: all clean
all: submission

submission: main.o train.o platform.o station.o link.o traintickpair.o
	$(CXX) $(CXXFLAGS) $(RELEASEFLAGS) -o troons $^

submissionseq: mainseq.o trainseq.o platformseq.o station.o link.o traintickpair.o 
	$(CXX) $(NOOMPFLAGS) $(RELEASEFLAGS) -o troonsseq $^

mainseq.o: main.cc
	$(CXX) $(NOOMPFLAGS) $(RELEASEFLAGS) -c -o $@ $<

trainseq.o: train.cc
	$(CXX) $(NOOMPFLAGS) $(RELEASEFLAGS) -c -o $@ $<

platformseq.o: platform.cc
	$(CXX) $(NOOMPFLAGS) $(RELEASEFLAGS) -c -o $@ $<

main.o: main.cc
	$(CXX) $(CXXFLAGS) $(RELEASEFLAGS) -c $^

train.o: train.cc
	$(CXX) $(CXXFLAGS) $(RELEASEFLAGS) -c train.cc

platform.o: platform.cc
	$(CXX) $(CXXFLAGS) $(RELEASEFLAGS) -c platform.cc

station.o: station.cc
	$(CXX) $(CXXFLAGS) $(RELEASEFLAGS) -c station.cc

link.o: link.cc
	$(CXX) $(CXXFLAGS) $(RELEASEFLAGS) -c link.cc

traintickpair.o: traintickpair.cc
	$(CXX) $(CXXFLAGS) $(RELEASEFLAGS) -c traintickpair.cc

clean:
	$(RM) *.o troons

debug: main.cc
	$(CXX) $(CXXFLAGS) $(DEBUGFLAGS) -D DEBUG -o troons main.cc train.cc platform.cc

debugseq: main.cc
	$(CXX) $(NOOMPFLAGS) $(DEBUGFLAGS) -D DEBUG -o troonsseq main.cc train.cc platform.cc