#include "platform.h"

#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <map>

using namespace std;

using adjacency_matrix = vector<std::vector<size_t>>;

// global variables :D
int train_id_counter = 0;
int total_trains;
size_t global_tick = 0;
size_t ticks_to_simulate;
vector<Train*> trains;
vector<Station> stations;
vector<Platform> platforms;
map<string, int> station_name_to_id;
vector<int> green_line;
vector<int> yellow_line;
vector<int> blue_line;
vector<vector<bool>> platform_initialized;
vector<vector<bool>> link_occupied;

void initialise_platforms(vector<int>& line) {
    for (unsigned long int i = 0; i < line.size() - 1; i++) {
        int station_id = line[i];
        int next_station_id = line[i + 1];
        Station *station = &stations[station_id];
        Station *next_station = &stations[next_station_id];

        if (platform_initialized[station_id][next_station_id]) {
            continue;
        }

        station->platforms[next_station_id] = new Platform(station->popularity, station_id, next_station_id);
        next_station->platforms[station_id] = new Platform(next_station->popularity, next_station_id, station_id);
        printf("Initialised platform between %d and %d\n", station_id, next_station_id);

        platform_initialized[station_id][next_station_id] = true;
        platform_initialized[next_station_id][station_id] = true;
    }
}

void print_status(int tick) {
    printf("%d: ", tick);
    // print blue troons first
    for (auto train : trains) {
        if (train->colour == BLUE) {
            printf("b%d-", train->id);
            printf("%s ", stations[train->current_station_id()].station_name.c_str());
        }
    }
    // print green troons
    for (auto train : trains) {
        if (train->colour == GREEN) {
            printf("g%d-", train->id);
            printf("%s ", stations[train->current_station_id()].station_name.c_str());
        }
    }
    // print yellow troons
    for (auto train : trains) {
        if (train->colour == YELLOW) {
            printf("y%d-", train->id);
            printf("%s ", stations[train->current_station_id()].station_name.c_str());
        }
    }
    printf("\n");
}

void simulate(size_t num_stations, const vector<string>& station_names,
              const std::vector<size_t>& popularities,
              const adjacency_matrix& mat,
              const vector<string>& green_station_names,
              const vector<string>& yellow_station_names,
              const vector<string>& blue_station_names, size_t ticks,
              size_t num_green_trains, size_t num_yellow_trains,
              size_t num_blue_trains, size_t num_lines) {
    // Silence the compiler
    (void)mat;
    (void)num_blue_trains;
    (void)num_yellow_trains;
    (void)num_green_trains;
    (void)num_lines;
    

    // Initialise global variables
    ticks_to_simulate = ticks;
    platform_initialized = vector<vector<bool>>(num_stations, vector<bool>(num_stations, false));
    link_occupied = vector<vector<bool>>(num_stations, vector<bool>(num_stations, false));

    // Initialise train stations, links and platforms
    for (long unsigned int i = 0; i < station_names.size(); i++) {
        station_name_to_id[station_names[i]] = i;
        string s = station_names[i];
        stations.push_back(Station(i, popularities[i], s));
    }

    // Convert station names to station ids
    for (long unsigned int i = 0; i < green_station_names.size(); i++) {
        green_line.push_back(station_name_to_id[green_station_names[i]]);
    }
    for (long unsigned int i = 0; i < yellow_station_names.size(); i++) {
        yellow_line.push_back(station_name_to_id[yellow_station_names[i]]);
    }
    for (long unsigned int i = 0; i < blue_station_names.size(); i++) {
        blue_line.push_back(station_name_to_id[blue_station_names[i]]);
    }

    // ======== Initialise starting platform of green line ========
    printf("Initialising green line\n");
    initialise_platforms(green_line);

    // ======== Initialise starting platform of blue line ========
    printf("Initialising blue line\n");
    initialise_platforms(blue_line);

    // ======== Initialise starting platform of yellow line ========
    printf("Initialising yellow line\n");
    initialise_platforms(yellow_line);

    for (unsigned long int i = 0; i < num_stations; i++) {
        Station station = stations[i];
        printf("Station %ld has %ld platforms\n", i, station.platforms.size());
        // for (unsigned long int j = 0; j < station.platforms.size(); j++) {
        //     Platform platform = *station.platforms[j];
        //     printf("%d ", platform.destination_station_id);
        // }
        for(auto it = station.platforms.cbegin(); it != station.platforms.cend(); ++it)
        {
            std::cout << it->first << " ";
        }
        printf("\n");
    }

    Train* t;
    for (size_t current_tick = 0; current_tick < ticks; current_tick++) {
        // SPAWN TRAIN
        // Green
        if (num_green_trains >= 1) {
            t = new Train(train_id_counter, 0, &green_line, true, GREEN);
            printf("Spawned green train at %d and queued it to next station %d\n", t->current_station_id(), t->next_station_id());
            stations[t->current_station_id()].platforms.at(t->next_station_id())->queue(t, current_tick);
            train_id_counter++;
            num_green_trains--;
            trains.push_back(t);
            if (num_green_trains >= 1) {
                t = new Train(train_id_counter, green_line.size() - 1, &green_line, false, GREEN);
                printf("Spawned green train at %d and queued it to next station %d\n", t->current_station_id(), t->next_station_id());
                stations[t->current_station_id()].platforms.at(t->next_station_id())->queue(t, current_tick);
                train_id_counter++;
                num_green_trains--;
                trains.push_back(t);
            }
        }
        // Yellow
        if (num_yellow_trains >= 1) {
            t = new Train(train_id_counter, 0, &yellow_line, true, YELLOW);
            printf("Spawned yellow train at %d and queued it to next station %d\n", t->current_station_id(), t->next_station_id());
            stations[t->current_station_id()].platforms[t->next_station_id()]->queue(t, current_tick);
            train_id_counter++;
            num_yellow_trains--;
            trains.push_back(t);
            if (num_yellow_trains >= 1) {
                t = new Train(train_id_counter, yellow_line.size() - 1, &yellow_line, false, YELLOW);
                printf("Spawned yellow train at %d and queued it to next station %d\n", t->current_station_id(), t->next_station_id());
                stations[t->current_station_id()].platforms[t->next_station_id()]->queue(t, current_tick);
                train_id_counter++;
                num_yellow_trains--;
                trains.push_back(t);
            }
        }
        // Blue
        if (num_blue_trains >= 1) {
            t = new Train(train_id_counter, 0, &blue_line, true, BLUE);
            printf("Spawned blue train at %d and queued it to next station %d\n", t->current_station_id(), t->next_station_id());
            stations[t->current_station_id()].platforms[t->next_station_id()]->queue(t, current_tick);
            train_id_counter++;
            num_blue_trains--;
            trains.push_back(t);
            if (num_blue_trains >= 1) {
                t = new Train(train_id_counter, blue_line.size() - 1, &blue_line, false, BLUE);
                printf("Spawned blue train at %d and queued it to next station %d\n", t->current_station_id(), t->next_station_id());
                stations[t->current_station_id()].platforms[t->next_station_id()]->queue(t, current_tick);
                train_id_counter++;
                num_blue_trains--;
                trains.push_back(t);
            }
        }
        // PROGRESS TRANSIT & INSERT INTO QUEUE

        // EMPTY PLATFORM

        // DEQUEUE & OPEN DOOR

        // PRINT STATUS OF ALL PLATFORMS
        print_status(current_tick);
    }

    // ======== Clean up code ========
    // for (unsigned long int i = 0; i < num_stations; i++) {
    //     for (unsigned long int j = 0; j < stations[i].platforms.size(); j++) {
    //         delete stations[i].platforms.at(j);
    //     }
    //     // delete stations[i].platforms;
    // }
}

vector<string> extract_station_names(string& line) {
    constexpr char space_delimiter = ' ';
    vector<string> stations{};
    line += ' ';
    size_t pos;
    while ((pos = line.find(space_delimiter)) != string::npos) {
        stations.push_back(line.substr(0, pos));
        line.erase(0, pos + 1);
    }
    return stations;
}

int main(int argc, char const* argv[]) {
    using std::cout;

    if (argc < 2) {
        std::cerr << argv[0] << " <input_file>\n";
        std::exit(1);
    }

    std::ifstream ifs(argv[1], std::ios_base::in);
    if (!ifs.is_open()) {
        std::cerr << "Failed to open " << argv[1] << '\n';
        std::exit(2);
    }

    // Read S
    size_t S;
    ifs >> S;

    // Read station names.
    string station;
    std::vector<string> station_names{};
    station_names.reserve(S);
    for (size_t i = 0; i < S; ++i) {
        ifs >> station;
        station_names.emplace_back(station);
    }

    // Read P popularity
    size_t p;
    std::vector<size_t> popularities{};
    popularities.reserve(S);
    for (size_t i = 0; i < S; ++i) {
        ifs >> p;
        popularities.emplace_back(p);
    }

    // Form adjacency mat
    adjacency_matrix mat(S, std::vector<size_t>(S));
    for (size_t src{}; src < S; ++src) {
        for (size_t dst{}; dst < S; ++dst) {
            ifs >> mat[src][dst];
        }
    }

    ifs.ignore();

    string stations_buf;

    std::getline(ifs, stations_buf);
    auto green_station_names = extract_station_names(stations_buf);

    std::getline(ifs, stations_buf);
    auto yellow_station_names = extract_station_names(stations_buf);

    std::getline(ifs, stations_buf);
    auto blue_station_names = extract_station_names(stations_buf);

    // N time ticks
    size_t N;
    ifs >> N;

    // g,y,b number of trains per line
    size_t g, y, b;
    ifs >> g;
    ifs >> y;
    ifs >> b;

    size_t num_lines;
    ifs >> num_lines;

    simulate(S, station_names, popularities, mat, green_station_names,
             yellow_station_names, blue_station_names, N, g, y, b, num_lines);

    return 0;
}
