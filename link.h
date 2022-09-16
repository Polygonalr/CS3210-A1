#include "train.h"

class Platform;

#ifndef LINK_H
#define LINK_H
class Link {
    public:
        Train* train;
        Platform* destination;
        int transit_time_remaining;
        int transit_time;
        int current_tick; // not sure whether should put here, but needed to call queue() of destination platform.


        Link(Platform* destination, int transit_time);

        bool is_empty();
        void occupy(Train* train);
        void goto_next_tick();
};
#endif
