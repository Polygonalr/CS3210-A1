#include "train.h"

#include <string>
#include <vector>
#include <queue>
#include <map>

using std::string;
using std::vector;
using std::priority_queue;
using std::map;

class TrainTickPair {
public:
    Train* train;
    size_t entry_tick;

    TrainTickPair(Train* train, size_t entry_tick);
};

struct TrainCompare {
    bool operator()(const TrainTickPair& left_pair, const TrainTickPair& right_pair);
};

class Link {
public:
    int source;
    int destination;

    Link(int source, int destination);
};

class Platform {
public:
    int popularity;
    priority_queue<TrainTickPair, vector<TrainTickPair>, TrainCompare> holding_area;
    int source_station_id;
    int destination_station_id;

    Platform(int popularity, int source_station_id, int destination_station_id);

    void queue(Train* train, size_t entry_tick);
    void goto_next_tick();
};

class Station {
public:
    int id;
    int popularity;
    string station_name;
    map<int, Platform*> platforms;

    Station(int id, int popularity, string& station_name);
    void goto_next_tick();
};
