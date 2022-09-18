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
}

void Platform::queue(Train* train, size_t entry_tick) {
    TrainTickPair ttp(train, entry_tick);
    holding_area.push(ttp);
}

void Platform::goto_next_tick() {
    // if (!holding_area.empty()) {
    //     TrainTickPair ttp = holding_area.top();
    //     holding_area.pop();
    //     // TODO: move train to link
    // }
}

Station::Station(int id, int popularity, string& station_name) {
    this->id = id;
    this->popularity = popularity;
    this->station_name = station_name;
}