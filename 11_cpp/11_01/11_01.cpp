#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

typedef std::vector<char> row_t;
typedef std::vector<row_t> map_t;

void printMap(map_t map) {
    for (row_t row : map) {
        for (char c : row) {
            std::cout << c;
        }
        std::cout << std::endl;
    }
}

void expandUniverse(map_t& map) {
    std::vector<bool> column_containing_galaxy(map[0].size(), false);
    std::vector<bool> row_containing_galaxy(map.size(), false);

    printMap(map);

    for (size_t i = 0; i < map.size(); i++) {
        for (size_t j = 0; j < map[i].size(); j++) {
            if (map[i][j] == '#') {
                column_containing_galaxy[j] = true;
                row_containing_galaxy[i] = true;
            }
        }
    }

    for (size_t i = map.size(); i > 0; i--) {
        if (!row_containing_galaxy[i]) {
            map.insert(map.begin() + i, map[i]);
        }
    }

    for (row_t& row : map) {
        for (size_t i = row.size(); i > 0; i--) {
            if (!column_containing_galaxy[i - 1]) {
                row.insert(row.begin() + i - 1, '.');
            }
        }
    }

    std::cout << std::endl << "*************************" << std::endl;
    printMap(map);
}

int calculateDistances(map_t& map) {
    std::vector<std::pair<int, int>> galaxies_coordinates;
    for (size_t row_idx = 0; row_idx < map.size(); row_idx++) {
        for (size_t column_idx = 0; column_idx < map[row_idx].size(); column_idx++) {
            if (map[row_idx][column_idx] == '#') {
                galaxies_coordinates.push_back(std::make_pair(column_idx, row_idx));
            }
        }
    }

    int distance_sum = 0;
    for (size_t i = 0; i < galaxies_coordinates.size(); i++) {
        for (size_t j = i; j < galaxies_coordinates.size(); j++) {
            int distance = std::abs(galaxies_coordinates[i].first - galaxies_coordinates[j].first) +
                           std::abs(galaxies_coordinates[i].second - galaxies_coordinates[j].second);
            distance_sum += distance;
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

    int column_count = 0;
    while (std::getline(file, line)) {
        row_t row;
        int row_count = 0;
        for (char c : line) {
            row.push_back(c);
            if (c == '#') {
                galaxies.push_back(std::make_pair(row_count, column_count));
            }
            row_count++;
        }

        map.push_back(row);
        column_count++;
    }

    expandUniverse(map);

    int result = calculateDistances(map);
    std::cout << result << std::endl;

    file.close();
    return 0;
}
