#include <string>
#include <vector>

using std::string;
using std::vector;

enum State { HOLDING_AREA, PLATFORM, LINK };

class Train {
private:
    int id;
    State state;
    string destination;
    vector<string> schedule;
    vector<string> next_schedule;
    int wait_time;

public:
    Train(int in_id, State in_state, string& in_destination,
          vector<string>& in_schedule, vector<string>& in_next_schedule,
          int in_wait_time);

    void next();

    void move_from_holding_area();

    void move_from_platform();

    void move_from_link();
};
