#include "train.h"

#include <string>
#include <vector>

using std::string;
using std::vector;

Train::Train(int id, long unsigned int position, vector<int>* line, bool forward, LineColour colour) {
    this->id = id;
    this->position = position;
    this->line = line;
    this->forward = forward;
    this->colour = colour;
    this->completion_tick = -1;
    this->status = QUEUED;
}

void Train::move_to_next_station() {
    if (forward) {
        if (position == line->size() - 1) {
            forward = false;
            position--;
        } else {
            position++;
        }
    } else {
        if (position == 0) {
            forward = true;
            position++;
        } else {
            position--;
        }
    }
}

int Train::current_station_id() {
    return line->at(position);
}

int Train::next_station_id() {
    if (forward) {
        if (position == line->size() - 1) {
            return line->at(position - 1);
        } else {
            return line->at(position + 1);
        }
    } else {
        if (position == 0) {
            return line->at(position + 1);
        } else {
            return line->at(position - 1);
        }
    }
}

bool Train::has_completed(size_t current_tick) {
    return completion_tick <= current_tick;
}
