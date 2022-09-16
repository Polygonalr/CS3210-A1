#include "link.h"
#include "platform.h"

Link::Link(Platform* destination, int transit_time) {
    this->destination = destination;
    this->transit_time = transit_time;
    this->transit_time_remaining = 0;
    this->train = NULL;
}

bool Link::is_empty() {
    return train == NULL;
}

void Link::occupy(Train* train) {
    this->train = train;
    this->transit_time_remaining = this->transit_time;
}

void Link::goto_next_tick() {
    if (transit_time_remaining > 0) {
        transit_time_remaining--;
    } else if (transit_time_remaining == 0) {
        destination->queue(*train, current_tick);
        train = NULL;
    }
    current_tick++;
}