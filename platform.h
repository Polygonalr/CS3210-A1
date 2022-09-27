#include "train.h"
#include "traintickpair.h"

#include <string>
#include <vector>
#include <queue>

using std::string;
using std::vector;
using std::priority_queue;

#ifndef PLATFORM_H
#define PLATFORM_H
class Platform {
public:
    int popularity;
    priority_queue<TrainTickPair, vector<TrainTickPair>, TrainCompare> holding_area;
    int source_station_id;
    int destination_station_id;
    Train* current_train;
    bool has_train;
    int trains_in_queue;

    Platform(int popularity, int source_station_id, int destination_station_id);

    void queue(Train* train, size_t entry_tick);
    void dequeue(size_t current_tick);
    bool has_completed(size_t current_tick);
    int empty_platform(size_t current_tick);
};
#endif
