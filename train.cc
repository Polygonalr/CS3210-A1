#include "train.h"

#include <string>
#include <vector>

using std::string;
using std::vector;

Train::Train(int in_id, State in_state, string& in_destination,
      vector<string>& in_schedule, vector<string>& in_next_schedule,
      int in_wait_time) {
    id = in_id;
    state = in_state;
    destination = in_destination;
    schedule = in_schedule;
    next_schedule = in_next_schedule;
    wait_time = in_wait_time;
}

void Train::next() {
    // TODO: CHANGE TRAIN STATE
    switch (state) {
        case HOLDING_AREA:
            Train::move_from_holding_area();
            break;
        case PLATFORM:
            Train::move_from_platform();
            break;
        case LINK:
            Train::move_from_link();
            break;
    }
}

void Train::move_from_holding_area() {
}

void Train::move_from_platform() {
}

void Train::move_from_link() {
}
