#include "platform.h"

#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <map>

using namespace std;

using adjacency_matrix = vector<std::vector<size_t>>;

// global variables :D
int total_trains;
size_t global_tick = 0;
size_t ticks_to_simulate;
vector<Train> trains;
vector<Station> stations;
vector<Link> links;
vector<Platform> platform;
map<string, int> station_name_to_id;
vector<int> green_line;
vector<int> yellow_line;
vector<int> blue_line;

void simulate(size_t num_stations, const vector<string>& station_names,
              const std::vector<size_t>& popularities,
              const adjacency_matrix& mat,
              const vector<string>& green_station_names,
              const vector<string>& yellow_station_names,
              const vector<string>& blue_station_names, size_t ticks,
              size_t num_green_trains, size_t num_yellow_trains,
              size_t num_blue_trains, size_t num_lines) {

    // Initialise global variables
    ticks_to_simulate = ticks;

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
    Station starting_station = stations[green_line[0]];
    Platform *first_platform = new Platform(starting_station.popularity, NULL);
    starting_station.platforms->push_back(first_platform);

    Platform *previous_platform = first_platform, *current_platform;

    for (long unsigned int i = 1; i < green_line.size(); i++) {
        int station_id = green_line[i];
        int previous_station_id = green_line[i - 1];

        // Create a new platform for current station
        Station current_station = stations[station_id];
        current_platform = new Platform(current_station.popularity, NULL);

        // Initialise link from previous platform to current platform
        Link forward_link = Link(current_platform, mat[previous_station_id][station_id]);
        previous_platform->link = &forward_link;

        // Finally, add the platform to the current station
        current_station.platforms->push_back(current_platform);
        previous_platform = current_platform;
    }

    for (long unsigned int i = green_line.size() - 2; i > 0; i--) {
        int station_id = green_line[i];
        int next_station_id = green_line[i + 1];

        // Create a new platform for current station
        Station current_station = stations[station_id];
        current_platform = new Platform(current_station.popularity, NULL);

        // Initialise link from previous platform to current platform
        Link backward_link = Link(current_platform, mat[next_station_id][station_id]);
        previous_platform->link = &backward_link;

        // Finally, add the platform to the current station
        current_station.platforms->push_back(current_platform);
        previous_platform = current_platform;
    }

    // Link the last platform to the first platform
    Link last_link = Link(first_platform, mat[green_line[1]][green_line[0]]);
    previous_platform->link = &last_link;

    // ======== Initialise starting platform of blue line ========
    starting_station = stations[blue_line[0]];
    first_platform = new Platform(starting_station.popularity, NULL);
    starting_station.platforms->push_back(first_platform);

    previous_platform = first_platform;

    for (long unsigned int i = 1; i < blue_line.size(); i++) {
        int station_id = blue_line[i];
        int previous_station_id = blue_line[i - 1];

        // Create a new platform for current station
        Station current_station = stations[station_id];
        current_platform = new Platform(current_station.popularity, NULL);

        // Initialise link from previous platform to current platform
        Link forward_link = Link(current_platform, mat[previous_station_id][station_id]);
        previous_platform->link = &forward_link;

        // Finally, add the platform to the current station
        current_station.platforms->push_back(current_platform);
        previous_platform = current_platform;
    }

    for (long unsigned int i = blue_line.size() - 2; i > 0; i--) {
        int station_id = blue_line[i];
        int next_station_id = blue_line[i + 1];

        // Create a new platform for current station
        Station current_station = stations[station_id];
        current_platform = new Platform(current_station.popularity, NULL);

        // Initialise link from previous platform to current platform
        Link backward_link = Link(current_platform, mat[next_station_id][station_id]);
        previous_platform->link = &backward_link;

        // Finally, add the platform to the current station
        current_station.platforms->push_back(current_platform);
        previous_platform = current_platform;
    }

    // Link the last platform to the first platform
    last_link = Link(first_platform, mat[blue_line[1]][blue_line[0]]);
    previous_platform->link = &last_link;

    // ======== Initialise starting platform of yellow line ========
    starting_station = stations[yellow_line[0]];
    first_platform = new Platform(starting_station.popularity, NULL);
    starting_station.platforms->push_back(first_platform);

    previous_platform = first_platform;

    for (long unsigned int i = 1; i < yellow_line.size(); i++) {
        int station_id = yellow_line[i];
        int previous_station_id = yellow_line[i - 1];

        // Create a new platform for current station
        Station current_station = stations[station_id];
        current_platform = new Platform(current_station.popularity, NULL);

        // Initialise link from previous platform to current platform
        Link forward_link = Link(current_platform, mat[previous_station_id][station_id]);
        previous_platform->link = &forward_link;

        // Finally, add the platform to the current station
        current_station.platforms->push_back(current_platform);
        previous_platform = current_platform;
    }

    for (long unsigned int i = yellow_line.size() - 2; i > 0; i--) {
        int station_id = yellow_line[i];
        int next_station_id = yellow_line[i + 1];

        // Create a new platform for current station
        Station current_station = stations[station_id];
        current_platform = new Platform(current_station.popularity, NULL);

        // Initialise link from previous platform to current platform
        Link backward_link = Link(current_platform, mat[next_station_id][station_id]);
        previous_platform->link = &backward_link;

        // Finally, add the platform to the current station
        current_station.platforms->push_back(current_platform);
        previous_platform = current_platform;
    }

    // Link the last platform to the first platform
    last_link = Link(first_platform, mat[yellow_line[1]][yellow_line[0]]);
    previous_platform->link = &last_link;
    

    std::cout << num_stations << '\n';

    for (size_t i{}; i < num_stations; ++i) {
        std::cout << station_names[i] << ' ' << popularities[i] << ' ';
    }
    std::cout << '\n';

    for (size_t i{}; i < num_stations; ++i) {
        for (size_t j{}; j < num_stations; ++j) {
            std::cout << mat[i][j] << ' ';
        }
        std::cout << '\n';
    }

    for (const auto& stn : green_station_names) {
        std::cout << stn << ' ';
    }
    std::cout << '\n';

    for (const auto& stn : yellow_station_names) {
        std::cout << stn << ' ';
    }
    std::cout << '\n';

    for (const auto& stn : blue_station_names) {
        std::cout << stn << ' ';
    }
    std::cout << '\n';

    std::cout << ticks << '\n';
    std::cout << num_green_trains << '\n';
    std::cout << num_yellow_trains << '\n';
    std::cout << num_blue_trains << '\n';

    std::cout << num_lines << '\n';

    // ======== Clean up code ========
    for (unsigned long int i = 0; i < num_stations; i++) {
        for (unsigned long int j = 0; j < stations[i].platforms->size(); j++) {
            delete stations[i].platforms->at(j);
        }
        delete stations[i].platforms;
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
