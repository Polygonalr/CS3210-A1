#include "platform.h"

#include <string>
#include <queue>

using std::string;
using std::priority_queue;

TrainTickPair::TrainTickPair(Train& train, int entry_tick) : train(train) {
    this->entry_tick = entry_tick;
}

bool TrainCompare::operator()(const TrainTickPair& left_pair, const TrainTickPair& right_pair) {
    if (left_pair.entry_tick != right_pair.entry_tick) {
        return left_pair.entry_tick > right_pair.entry_tick;
    } else {
        return left_pair.train.id > right_pair.train.id;
    }
}

Platform::Platform(int id, string& platform_name) {
    this->id = id;
    this->platform_name = platform_name;
}

void Platform::queue(Train& train, int entry_tick) {
    TrainTickPair ttp(train, entry_tick);
    holding_area.push(ttp);
}
