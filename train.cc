#include "train.h"

#include <string>
#include <vector>

using std::string;
using std::vector;

Train::Train(int id, string& destination, vector<string>& schedule,
             vector<string>& next_schedule)
    : schedule(schedule), next_schedule(next_schedule) {
    this->id = id;
    this->destination = destination;
}
