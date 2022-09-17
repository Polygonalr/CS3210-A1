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

class Train {
public:
    int id;
    vector<int>* line;
    long unsigned int position; // 0 is at the start, line.size()-1 means at the end
    LineColour colour;
    bool forward;

    Train(int id, long unsigned int position, vector<int>* line, bool forward, LineColour colour);
    void move();
    int current_station_id();
    int next_station_id();
};
#endif