CXX=g++
CXXFLAGS:=-Wall -Werror -Wextra -pedantic -std=c++17 -fopenmp
RELEASEFLAGS:=-O3
DEBUGFLAGS:=-g

.PHONY: all clean
all: submission

submission: main.o train.o platform.o
	$(CXX) $(CXXFLAGS) $(RELEASEFLAGS) -o troons $^

main.o: main.cc
	$(CXX) $(CXXFLAGS) $(RELEASEFLAGS) -c $^

train.o: train.cc
	$(CXX) $(CXXFLAGS) $(RELEASEFLAGS) -c train.cc

platform.o: platform.cc
	$(CXX) $(CXXFLAGS) $(RELEASEFLAGS) -c platform.cc

clean:
	$(RM) *.o troons

debug: main.cc
	$(CXX) $(CXXFLAGS) $(DEBUGFLAGS) -D DEBUG -o troons main.cc train.cc platform.cc
