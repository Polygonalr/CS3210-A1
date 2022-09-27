#include "platform.h"

class Station {
public:
    int id;
    int popularity;
    string station_name;
    Platform** platforms;

    Station(int id, int popularity, string& station_name, int num_stations);
    void goto_next_tick();
};
