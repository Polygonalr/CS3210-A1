#include <string>
#include <vector>

using std::string;
using std::vector;

class Train {
public:
    int id;
    string destination;
    vector<string> schedule;
    vector<string> next_schedule;

    Train(int id, string& destination,
          vector<string>& schedule, vector<string>& next_schedule);
};
