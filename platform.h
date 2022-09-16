#include "link.h"

#include <string>
#include <vector>
#include <queue>

using std::string;
using std::vector;
using std::priority_queue;

class TrainTickPair {
public:
    Train train;
    int entry_tick;

    TrainTickPair(Train& train, int entry_tick);
};


struct TrainCompare {
    bool operator()(const TrainTickPair& left_pair, const TrainTickPair& right_pair);
};

class Platform {
public:
    int popularity;
    priority_queue<TrainTickPair, vector<TrainTickPair>, TrainCompare> holding_area;
    Link* link; // destination platform is specified in here

    Platform(int popularity, Link* link);

    void queue(Train& train, int entry_tick);
    void goto_next_tick();
};

class Station {
public:
    int id;
    int popularity;
    string station_name;
    vector<Platform*>* platforms;

    Station(int id, int popularity, string& station_name);
    void goto_next_tick();
};
