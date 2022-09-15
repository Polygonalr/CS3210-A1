#include "train.h"

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
    int id;
    string platform_name;
    priority_queue<TrainTickPair, vector<TrainTickPair>, TrainCompare> holding_area;

    Platform(int id, string& platform_name);

    void queue(Train &train, int entry_tick);
};
