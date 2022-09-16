#include <string>
#include <vector>

using std::string;
using std::vector;

#ifndef TRAIN_H
#define TRAIN_H
class Train {
public:
    int id;
    string destination;
    vector<string> schedule;
    vector<string> next_schedule;

    Train(int id, string& destination,
          vector<string>& schedule, vector<string>& next_schedule);
};
#endif