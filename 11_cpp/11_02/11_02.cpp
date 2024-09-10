#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

typedef std::vector<char> row_t;
typedef std::vector<row_t> map_t;

constexpr long long int expansion_rate = 1000000;

void printMap(map_t map) {
    for (row_t row : map) {
        for (char c : row) {
            std::cout << c;
        }
        std::cout << std::endl;
    }
}

long long int calculateDistances(map_t& map) {
    std::vector<bool> column_containing_galaxy(map[0].size(), false);
    std::vector<bool> row_containing_galaxy(map.size(), false);

    std::vector<std::pair<int, int>> galaxies_coordinates;
    for (size_t i = 0; i < map.size(); i++) {
        for (size_t j = 0; j < map[i].size(); j++) {
            if (map[i][j] == '#') {
                column_containing_galaxy[j] = true;
                row_containing_galaxy[i] = true;
                galaxies_coordinates.push_back(std::make_pair(j, i));
            }
        }
    }

    long long int distance_sum = 0;
    for (size_t i = 0; i < galaxies_coordinates.size(); i++) {
        for (size_t j = i + 1; j < galaxies_coordinates.size(); j++) {
            long long int expansion_count = 0;

            for (size_t k = 0; k < column_containing_galaxy.size(); k++) {
                if (!column_containing_galaxy[k]) {
                    if ((k > galaxies_coordinates[i].first && k < galaxies_coordinates[j].first) ||
                        (k > galaxies_coordinates[j].first && k < galaxies_coordinates[i].first)) {
                        expansion_count++;
                    }
                }
            }
            for (size_t k = 0; k < row_containing_galaxy.size(); k++) {
                if (!row_containing_galaxy[k]) {
                    if ((k > galaxies_coordinates[i].second && k < galaxies_coordinates[j].second) ||
                        (k > galaxies_coordinates[j].second && k < galaxies_coordinates[i].second)) {
                        expansion_count++;
                    }
                }
            }

            long long int distance = std::abs(galaxies_coordinates[i].first - galaxies_coordinates[j].first) +
                                     std::abs(galaxies_coordinates[i].second - galaxies_coordinates[j].second);
            distance_sum += (distance + (expansion_rate - 1LL) * expansion_count);
        }
    }

    return distance_sum;
}

int main() {
    std::ifstream file("input.txt");
    // std::ifstream file("test.txt");
    std::string line;

    map_t map;
    std::vector<std::pair<int, int>> galaxies;

    while (std::getline(file, line)) {
        row_t row;
        for (char c : line) {
            row.push_back(c);
        }

        map.push_back(row);
    }

    long long int result = calculateDistances(map);
    std::cout << result << std::endl;

    file.close();
    return 0;
}
