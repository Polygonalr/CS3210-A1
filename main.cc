#include "platform.h"
#include "traintickpair.h"
#include "station.h"
#include "link.h"

#include <fstream>
#include <iostream>
#include <string>
#include <cstring>
#include <vector>
#include <map>
#include <sstream>
#include <algorithm>

using namespace std;

using adjacency_matrix = vector<std::vector<int>>;

// sorts integers lexigraphically
bool lexi_compare (int lhs, int rhs) {
    return strcmp(to_string(lhs).c_str(), to_string(rhs).c_str()) < 0;
}

// global variables :D
int train_id_counter = 0;
int total_stations;
int ticks_to_simulate;
vector<int> link_weights;
vector<Train*> trains;
vector<int> blue_trains_indexes, green_trains_indexes, yellow_trains_indexes;
bool blue_sorted = false, green_sorted = false, yellow_sorted = false;
vector<Station> stations;
vector<Platform*> platforms;
map<string, int> station_name_to_id;
vector<int> green_line;
vector<int> yellow_line;
vector<int> blue_line;

// 2d vector declared as a 1d vector for optimisation
// tracks the current status of a link between two stations
// link_occupancies[source][destination] = 
// * -2 means unintialised
// * -1 means unoccupied
// * >=0 means occupied by train with id = link_occupancies[source][destination]
vector<int> link_occupancies;
vector<Link> links;

// this function converts the 2d index to a 1d index for the link_occupancies vector
int index_2d_to_1d(int row, int col) {
    return (total_stations * row) + col;
}

void initialise_platforms(vector<int>& line) {
    for (unsigned long int i = 0; i < line.size() - 1; i++) {
        int station_id = line[i];
        int next_station_id = line[i + 1];
        Station *station = &stations[station_id];
        Station *next_station = &stations[next_station_id];

        if (link_occupancies[index_2d_to_1d(station_id, next_station_id)] == -1) {
            continue;
        }

        station->platforms[next_station_id] = new Platform(station->popularity, station_id, next_station_id);
        next_station->platforms[station_id] = new Platform(next_station->popularity, next_station_id, station_id);
        platforms.push_back(station->platforms[next_station_id]);
        platforms.push_back(next_station->platforms[station_id]);
        // printf("Initialised platform between %d and %d\n", station_id, next_station_id);

        link_occupancies[index_2d_to_1d(station_id, next_station_id)] = -1;
        link_occupancies[index_2d_to_1d(next_station_id, station_id)] = -1;
        links.push_back(Link(station_id, next_station_id));
        links.push_back(Link(next_station_id, station_id));
    }
}

void print_status(int tick) {
    stringstream ss, blue_ss, yellow_ss, green_ss;
    ss << tick << ": ";
    #pragma omp parallel sections
    {
        #pragma omp section
        {
            // Sort print orders if not sorted
            if (!blue_sorted) {
                sort(blue_trains_indexes.begin(), blue_trains_indexes.end(), lexi_compare);
                blue_sorted = true;
            }
            for (int b = 0; b < (int)blue_trains_indexes.size(); b++) {
                int train_id = blue_trains_indexes[b];
                blue_ss << "b" << trains[train_id]->id << "-" << stations[trains[train_id]->current_station_id()].station_name.c_str();
                if(trains[train_id]->status == TRANSIT) {
                    blue_ss << "->" << stations[trains[train_id]->next_station_id()].station_name.c_str();
                } else if (trains[train_id]->status == PLATFORM || trains[train_id]->status == DEPARTING) {
                    blue_ss << "%";
                } else if (trains[train_id]->status == QUEUED) {
                    blue_ss << "#";
                }
                blue_ss << " ";
            }
        }
        #pragma omp section
        {
            // Sort print orders if not sorted
            if (!green_sorted) {
                sort(green_trains_indexes.begin(), green_trains_indexes.end(), lexi_compare);
                green_sorted = true;
            }
            for (int g = 0; g < (int)green_trains_indexes.size(); g++) {
                int train_id = green_trains_indexes[g];
                green_ss << "g" << trains[train_id]->id << "-" << stations[trains[train_id]->current_station_id()].station_name.c_str();
                if(trains[train_id]->status == TRANSIT) {
                    green_ss << "->" << stations[trains[train_id]->next_station_id()].station_name.c_str();
                } else if (trains[train_id]->status == PLATFORM || trains[train_id]->status == DEPARTING) {
                    green_ss << "%";
                } else if (trains[train_id]->status == QUEUED) {
                    green_ss << "#";
                }
                green_ss << " ";
            }
        }
        #pragma omp section
        {
            // Sort print orders if not sorted
            if (!yellow_sorted) {
                sort(yellow_trains_indexes.begin(), yellow_trains_indexes.end(), lexi_compare);
                yellow_sorted = true;
            }
            for (int y = 0; y < (int)yellow_trains_indexes.size(); y++) {
                int train_id = yellow_trains_indexes[y];
                yellow_ss << "y" << trains[train_id]->id << "-" << stations[trains[train_id]->current_station_id()].station_name.c_str();
                if(trains[train_id]->status == TRANSIT) {
                    yellow_ss << "->" << stations[trains[train_id]->next_station_id()].station_name.c_str();
                } else if (trains[train_id]->status == PLATFORM || trains[train_id]->status == DEPARTING) {
                    yellow_ss << "%";
                } else if (trains[train_id]->status == QUEUED) {
                    yellow_ss << "#";
                }
                yellow_ss << " ";
            }
        }
    }
    ss << blue_ss.str() << green_ss.str() << yellow_ss.str();
    cout << ss.str() << endl;
}

void simulate(int num_stations, const vector<string>& station_names,
              const std::vector<int>& popularities,
              const adjacency_matrix& mat,
              const vector<string>& green_station_names,
              const vector<string>& yellow_station_names,
              const vector<string>& blue_station_names, int ticks,
              int num_green_trains, int num_yellow_trains,
              int num_blue_trains, int num_lines) {
    // Initialise global variables
    ticks_to_simulate = ticks;
    total_stations = num_stations;
    link_weights = vector<int>(num_stations * num_stations, 0);
    for (int i = 0; i < num_stations; i++) {
        for (int j = 0; j < num_stations; j++) {
            link_weights[index_2d_to_1d(i, j)] = mat[i][j];
        }
    }

    link_occupancies = vector<int>(num_stations * num_stations, -2);

    // stations.resize(num_stations); // doesn't work due to station name being string (which is dynamically allocated...)
    trains.resize(num_green_trains + num_yellow_trains + num_blue_trains);

    // Initialise train stations, links and platforms
    for (int i = 0; i < (int)station_names.size(); i++) {
        station_name_to_id[station_names[i]] = i;
        string s = station_names[i];
        stations.push_back(Station(i, popularities[i], s, num_stations));
    }

    // Convert station names to station ids
    for (int i = 0; i < (int)green_station_names.size(); i++) {
        green_line.push_back(station_name_to_id[green_station_names[i]]);
    }
    for (int i = 0; i < (int)yellow_station_names.size(); i++) {
        yellow_line.push_back(station_name_to_id[yellow_station_names[i]]);
    }
    for (int i = 0; i < (int)blue_station_names.size(); i++) {
        blue_line.push_back(station_name_to_id[blue_station_names[i]]);
    }

    // ======== Initialise starting platform of green line ========
    initialise_platforms(green_line);

    // ======== Initialise starting platform of blue line ========
    initialise_platforms(blue_line);

    // ======== Initialise starting platform of yellow line ========
    initialise_platforms(yellow_line);

    Train* t;
    for (int current_tick = 0; current_tick < ticks_to_simulate; current_tick++) {
        // SPAWN TRAIN
        // Green
        if (num_green_trains >= 1) {
            t = new Train(train_id_counter, 0, &green_line, true, GREEN);
            stations[t->current_station_id()].platforms[t->next_station_id()]->queue(t, current_tick);
            trains[train_id_counter] = t;
            green_trains_indexes.push_back(train_id_counter);
            green_sorted = false;
            train_id_counter++;
            num_green_trains--;
            if (num_green_trains >= 1) {
                t = new Train(train_id_counter, green_line.size() - 1, &green_line, false, GREEN);
                stations[t->current_station_id()].platforms[t->next_station_id()]->queue(t, current_tick);
                trains[train_id_counter] = t;
                green_trains_indexes.push_back(train_id_counter);
                green_sorted = false;
                train_id_counter++;
                num_green_trains--;
            }
        }
        // Yellow
        if (num_yellow_trains >= 1) {
            t = new Train(train_id_counter, 0, &yellow_line, true, YELLOW);
            stations[t->current_station_id()].platforms[t->next_station_id()]->queue(t, current_tick);
            trains[train_id_counter] = t;
            yellow_trains_indexes.push_back(train_id_counter);
            yellow_sorted = false;
            train_id_counter++;
            num_yellow_trains--;
            if (num_yellow_trains >= 1) {
                t = new Train(train_id_counter, yellow_line.size() - 1, &yellow_line, false, YELLOW);
                stations[t->current_station_id()].platforms[t->next_station_id()]->queue(t, current_tick);
                trains[train_id_counter] = t;
                yellow_trains_indexes.push_back(train_id_counter);
                yellow_sorted = false;
                train_id_counter++;
                num_yellow_trains--;
            }
        }
        // Blue
        if (num_blue_trains >= 1) {
            t = new Train(train_id_counter, 0, &blue_line, true, BLUE);
            stations[t->current_station_id()].platforms[t->next_station_id()]->queue(t, current_tick);
            trains[train_id_counter] = t;
            blue_trains_indexes.push_back(train_id_counter);
            blue_sorted = false;
            train_id_counter++;
            num_blue_trains--;
            if (num_blue_trains >= 1) {
                t = new Train(train_id_counter, blue_line.size() - 1, &blue_line, false, BLUE);
                stations[t->current_station_id()].platforms[t->next_station_id()]->queue(t, current_tick);
                trains[train_id_counter] = t;
                blue_trains_indexes.push_back(train_id_counter);
                blue_sorted = false;
                train_id_counter++;
                num_blue_trains--;
            }
        }

        // PROGRESS TRANSIT & INSERT INTO QUEUE
        #pragma omp parallel for
        for (Link link : links) {
            int link_occupied_by_id = link_occupancies[index_2d_to_1d(link.source, link.destination)];
            if (link_occupied_by_id >= 0) {
                Train *train = trains.at(link_occupied_by_id);
                if (train->has_completed(current_tick)) {
                    link_occupancies[index_2d_to_1d(link.source, link.destination)] = -1; // Set link to unoccupied
                    train->move_to_next_station(); // Move train into next station
                    train->status = QUEUED; // Change status of the train
                    stations[train->current_station_id()].platforms[train->next_station_id()]->queue(train, current_tick); // Queue train into holding area of specified platform
                }
            }
        }

        // PROCESS PLATFORMS AND WAITING AREA QUEUES
        #pragma omp parallel for
        for (Platform *platform : platforms) {
            // EMPTY PLATFORMS
            if (link_occupancies[index_2d_to_1d(platform->source_station_id, platform->destination_station_id)] == -1) {
                int train_id = platform->empty_platform(current_tick);
                if (train_id != -1) {
                    // Platform is emptied, push train to link
                    // it->second here is Platform
                    link_occupancies[index_2d_to_1d(platform->source_station_id, platform->destination_station_id)] = train_id;
                    platform->current_train->status = TRANSIT;
                    platform->current_train->completion_tick = current_tick + link_weights[index_2d_to_1d(platform->source_station_id, platform->destination_station_id)];
                    platform->current_train = NULL;
                    platform->has_train = false;
                }
            }

            // DEQUEUE & OPEN DOOR
            if (platform->current_train == NULL) {
                platform->dequeue(current_tick);
            }
        }

        // PRINT STATUS OF ALL PLATFORMS
        if (current_tick >= ticks_to_simulate - num_lines) print_status(current_tick);
    }

    // ======== Clean up code ========
    for (int i = 0; i < (int)platforms.size(); i++) {
        delete platforms[i];
    }
    for (int i = 0; i < (int)stations.size(); i++) {
        free(stations[i].platforms);
    }
    for (int i = 0; i < (int)trains.size(); i++) {
        delete trains[i];
    }
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
    int S;
    ifs >> S;

    // Read station names.
    string station;
    std::vector<string> station_names{};
    station_names.reserve(S);
    for (int i = 0; i < S; ++i) {
        ifs >> station;
        station_names.emplace_back(station);
    }

    // Read P popularity
    int p;
    std::vector<int> popularities{};
    popularities.reserve(S);
    for (int i = 0; i < S; ++i) {
        ifs >> p;
        popularities.emplace_back(p);
    }

    // Form adjacency mat
    adjacency_matrix mat(S, std::vector<int>(S));
    for (int src{}; src < S; ++src) {
        for (int dst{}; dst < S; ++dst) {
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
    int N;
    ifs >> N;

    // g,y,b number of trains per line
    int g, y, b;
    ifs >> g;
    ifs >> y;
    ifs >> b;

    int num_lines;
    ifs >> num_lines;

    simulate(S, station_names, popularities, mat, green_station_names,
             yellow_station_names, blue_station_names, N, g, y, b, num_lines);

    return 0;
}
