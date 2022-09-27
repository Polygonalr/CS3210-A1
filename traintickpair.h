#include "train.h"

#ifndef TRAINTICKPAIR_H
#define TRAINTICKPAIR_H
class TrainTickPair {
public:
    Train* train;
    size_t entry_tick;

    TrainTickPair(Train* train, size_t entry_tick);
};

struct TrainCompare {
    bool operator()(const TrainTickPair& left_pair, const TrainTickPair& right_pair);
};
#endif
