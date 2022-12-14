#include <string>
#include <vector>

using std::string;
using std::vector;

#ifndef TRAIN_H
#define TRAIN_H
typedef enum LineColour {
    BLUE = 0,
    GREEN,
    YELLOW,
} LineColour;

typedef enum TrainStatus {
    TRANSIT,
    QUEUED,
    PLATFORM,
    DEPARTING,
} TrainStatus;

class Train {
public:
    int id;
    vector<int>* line; // pointer to the vector containing the station ids of the line which the train is on
    long unsigned int position; // 0 is at the start, line.size()-1 means at the end
    LineColour colour;
    bool forward;
    size_t completion_tick; // tick which the train will complete its current action
    TrainStatus status;

    Train(int id, long unsigned int position, vector<int>* line, bool forward, LineColour colour);
    void move_to_next_station();
    int current_station_id();
    int next_station_id();
    bool has_completed(size_t current_tick);
};
#endif