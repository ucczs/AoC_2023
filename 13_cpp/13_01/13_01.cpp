#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
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

int check_for_symetrie(std::vector<std::vector<char>> patterns) {
    auto equal_neightbours = patterns.begin();
    bool axis_found = false;
    int index_of_mirror_axis = 0;
    int idx_equal_neightbours = -1;

    while ((equal_neightbours = std::adjacent_find(equal_neightbours, patterns.end())) != patterns.end() && !axis_found) {
        idx_equal_neightbours = std::distance(patterns.begin(), equal_neightbours);
        int step_away_from_mirror_axis = 1;
        while ((idx_equal_neightbours != 0) && (idx_equal_neightbours - step_away_from_mirror_axis >= 0) &&
               (idx_equal_neightbours + step_away_from_mirror_axis + 1 < patterns.size())) {
            auto pattern_left = equal_neightbours - step_away_from_mirror_axis;
            auto pattern_right = equal_neightbours + step_away_from_mirror_axis + 1;
            if (std::equal(pattern_left->begin(), pattern_left->end(), pattern_right->begin())) {
                axis_found = true;
                step_away_from_mirror_axis++;
            } else {
                axis_found = false;
                break;
            }
        }
        if (idx_equal_neightbours == 0) {
            break;
        } else {
            ++equal_neightbours;
        }
    }

    if (axis_found || idx_equal_neightbours + 2 == patterns.size() || idx_equal_neightbours == 0) {
        index_of_mirror_axis = idx_equal_neightbours + 1;
    }

    return index_of_mirror_axis;
}

int main() {
    std::ifstream file("input.txt");
    // std::ifstream file("test_02.txt");
    // std::ifstream file("test.txt");
    std::string line;

    std::vector<std::vector<char>> vertical_lines;
    std::vector<std::vector<char>> horizontal_lines;

    int pattern_row_counter = 0;
    bool pattern_initialized = false;

    int vertical_sum = 0;
    int horizontal_sum = 0;

    int pattern_count = 0;

    while (std::getline(file, line)) {
        if (line == "") {
            pattern_count++;
            pattern_initialized = false;
            pattern_row_counter = 0;
            vertical_sum += check_for_symetrie(vertical_lines);
            horizontal_sum += check_for_symetrie(horizontal_lines);
            continue;
        }

        if (!pattern_initialized) {
            pattern_initialized = true;
            vertical_lines.clear();
            vertical_lines.resize(line.size());
            horizontal_lines.clear();
        }

        horizontal_lines.push_back({});
        for (size_t i = 0; i < line.length(); i++) {
            vertical_lines[i].push_back(line[i]);
            horizontal_lines[pattern_row_counter].push_back(line[i]);
        }
        pattern_row_counter++;
    }

    vertical_sum += check_for_symetrie(vertical_lines);
    horizontal_sum += check_for_symetrie(horizontal_lines);

    int result = vertical_sum + horizontal_sum * 100;

    std::cout << result << std::endl;

    file.close();
    return 0;
}
