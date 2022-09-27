#include "platform.h"
#include "station.h"

Station::Station(int id, int popularity, string& station_name, int num_stations) {
    this->id = id;
    this->popularity = popularity;
    this->station_name = station_name;
    this->platforms = (Platform**)malloc(sizeof(Platform*) * num_stations);
}