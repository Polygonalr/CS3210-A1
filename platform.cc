#include "platform.h"

#include <string>
#include <queue>
#include <map>

using std::string;
using std::priority_queue;
using std::map;

TrainTickPair::TrainTickPair(Train* train, size_t entry_tick) : train(train) {
    this->entry_tick = entry_tick;
}

bool TrainCompare::operator()(const TrainTickPair& left_pair, const TrainTickPair& right_pair) {
    if (left_pair.entry_tick != right_pair.entry_tick) {
        return left_pair.entry_tick > right_pair.entry_tick;
    } else {
        return left_pair.train->id > right_pair.train->id;
    }
}

Link::Link(int source, int destination) {
    this->source = source;
    this->destination = destination;
}

Platform::Platform(int popularity, int source_station_id, int destination_station_id) {
    this->popularity = popularity;
    this->source_station_id = source_station_id;
    this->destination_station_id = destination_station_id;
    this->current_train = NULL;
    this->has_train = false;
}

void Platform::queue(Train* train, size_t entry_tick) {
    TrainTickPair ttp(train, entry_tick);
    // CRITICAL
    #pragma omp critical
    holding_area.push(ttp);
}

void Platform::dequeue(size_t current_tick) {
    if (holding_area.empty()) {
        return;
    }
    Train* train = holding_area.top().train;
    holding_area.pop();
    train->status = PLATFORM;
    train->completion_tick = current_tick + this->popularity + 1;
    this->current_train = train;
    has_train = true;
}

bool Platform::has_completed(size_t current_tick) {
    return current_train->has_completed(current_tick);
}

int Platform::empty_platform(size_t current_tick) {
    if (!has_train) {
        return -1;
    }
    if (current_train->has_completed(current_tick)) {
        if (current_train->status == PLATFORM) {
            current_train->status = DEPARTING;
            current_train->completion_tick++;
        } else if (current_train->status == DEPARTING) {
            return current_train->id;
        }
    }
    return -1;
}

Station::Station(int id, int popularity, string& station_name) {
    this->id = id;
    this->popularity = popularity;
    this->station_name = station_name;
}