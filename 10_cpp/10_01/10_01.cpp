#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

auto split_string(std::string full_string, char seperator) -> std::vector<std::string> {
    std::stringstream full_string_ss(full_string);
    std::vector<std::string> splitted_string;
    std::string split;

    while (std::getline(full_string_ss, split, seperator)) {
        if (split[0] == ' ') {
            split = split.substr(1, split.length() - 1);
        }
        splitted_string.push_back(split);
    }

    return splitted_string;
}

auto stringVec2intVec(std::vector<std::string>* vec_str) -> std::vector<int> {
    std::vector<int> vec_int;
    std::transform(vec_str->begin(), vec_str->end(), std::back_inserter(vec_int), [](const std::string& str) { return std::stoi(str); });
    return vec_int;
}

enum class direction_t { NORTH, EAST, SOUTH, WEST };

int main() {
    std::ifstream file("input.txt");
    direction_t direction = direction_t::NORTH;

    // std::ifstream file("test_01.txt");
    // std::ifstream file("test_02.txt");
    // direction_t direction = direction_t::SOUTH;

    std::string line;

    std::vector<std::vector<char>> map;
    int y_idx = 0;

    std::pair<int, int> start_pos;

    while (std::getline(file, line)) {
        int x_idx = 0;
        std::vector<char> row;
        for (char c : line) {
            row.push_back(c);
            if (c == 'S') {
                start_pos = std::make_pair(y_idx, x_idx);
            }
            x_idx++;
        }
        map.push_back(row);
        y_idx++;
    }

    std::pair<int, int> current_pos;
    if (direction == direction_t::SOUTH) {
        current_pos = std::make_pair(start_pos.first + 1, start_pos.second);
    } else if (direction == direction_t::NORTH) {
        current_pos = std::make_pair(start_pos.first - 1, start_pos.second);
    }

    int step_count = 0;

    do {
        step_count++;
        switch (map[current_pos.first][current_pos.second]) {
            case '|':
                if (direction == direction_t::SOUTH) {
                    current_pos = std::make_pair(current_pos.first + 1, current_pos.second);
                } else {
                    current_pos = std::make_pair(current_pos.first - 1, current_pos.second);
                }
                break;
            case '-':
                if (direction == direction_t::EAST) {
                    current_pos = std::make_pair(current_pos.first, current_pos.second + 1);
                } else {
                    current_pos = std::make_pair(current_pos.first, current_pos.second - 1);
                }
                break;
            case 'L':
                if (direction == direction_t::SOUTH) {
                    current_pos = std::make_pair(current_pos.first, current_pos.second + 1);
                    direction = direction_t::EAST;
                } else {
                    current_pos = std::make_pair(current_pos.first - 1, current_pos.second);
                    direction = direction_t::NORTH;
                }
                break;
            case 'F':
                if (direction == direction_t::NORTH) {
                    current_pos = std::make_pair(current_pos.first, current_pos.second + 1);
                    direction = direction_t::EAST;
                } else {
                    current_pos = std::make_pair(current_pos.first + 1, current_pos.second);
                    direction = direction_t::SOUTH;
                }
                break;
            case 'J':
                if (direction == direction_t::SOUTH) {
                    current_pos = std::make_pair(current_pos.first, current_pos.second - 1);
                    direction = direction_t::WEST;
                } else {
                    current_pos = std::make_pair(current_pos.first - 1, current_pos.second);
                    direction = direction_t::NORTH;
                }
                break;
            case '7':
                if (direction == direction_t::NORTH) {
                    current_pos = std::make_pair(current_pos.first, current_pos.second - 1);
                    direction = direction_t::WEST;
                } else {
                    current_pos = std::make_pair(current_pos.first + 1, current_pos.second);
                    direction = direction_t::SOUTH;
                }
                break;
            case 'S':
                std::cout << "Start pos reached." << std::endl;
                break;
            default:
                std::cout << "Direction error!" << std::endl;
                break;
        }
    } while (current_pos != start_pos);

    std::cout << "Steps total: " << step_count << std::endl;
    std::cout << (step_count + 1) / 2 << std::endl;

    file.close();
    return 0;
}
