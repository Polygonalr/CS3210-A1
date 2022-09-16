#include <vector>
#include <string>

class Station {
public:
    int id;
    int popularity;
    string station_name;
    vector<Platform> platforms;

    Station(int id, int popularity, string& station_name, vector<Platform>* platforms);
    void goto_next_tick();
};