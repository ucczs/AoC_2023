#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <tuple>
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

int check_for_symetrie(std::vector<std::vector<char>> patterns, int smudge_pos1, int smudge_pos2) {
    auto equal_neightbours = patterns.begin();
    bool axis_found = false;
    int index_of_mirror_axis = 0;
    int idx_equal_neightbours = -1;
    bool smudge_included = true;

    while ((equal_neightbours = std::adjacent_find(equal_neightbours, patterns.end())) != patterns.end() && !axis_found) {
        smudge_included = true;
        idx_equal_neightbours = std::distance(patterns.begin(), equal_neightbours);
        int step_away_from_mirror_axis = 1;
        if (idx_equal_neightbours != (smudge_pos1 + smudge_pos2) / 2) {
            axis_found = false;
            smudge_included = false;
            ++equal_neightbours;
            continue;
        }
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

    if (smudge_included) {
        if (axis_found || idx_equal_neightbours + 2 == patterns.size() || idx_equal_neightbours == 0) {
            index_of_mirror_axis = idx_equal_neightbours + 1;
        }
    }
    return index_of_mirror_axis;
}

void printPattern(std::vector<std::vector<char>>& pattern, bool inverted, int mirror_axis, int smudge_pos1, int smudge_pos2,
                  int smudge_pos3) {
    pattern[smudge_pos1][smudge_pos3] = 'O';
    pattern[smudge_pos2][smudge_pos3] = 'O';
    char axis_symbol = inverted ? '|' : '=';
    std::vector<std::vector<char>> pattern_with_axis = pattern;
    std::vector<char> axis(pattern_with_axis[0].size(), axis_symbol);
    pattern_with_axis.insert(pattern_with_axis.begin() + mirror_axis, axis);

    std::cout << std::endl;
    std::vector<std::vector<char>> pattern_print;
    if (inverted) {
        pattern_print.resize(pattern_with_axis[0].size());

        for (size_t i = 0; i < pattern_with_axis.size(); i++) {
            int row_counter = 0;
            for (char c : pattern_with_axis[i]) {
                pattern_print[row_counter].push_back(c);
                row_counter++;
            }
        }
    } else {
        pattern_print = pattern_with_axis;
    }

    for (std::vector<char> row : pattern_print) {
        for (char c : row) {
            std::cout << c;
        }
        std::cout << std::endl;
    }
}

int check_for_symetrie_with_smudge(std::vector<std::vector<char>>& patterns, bool inverted) {
    int one_diff_counter = 0;
    for (size_t i = 0; i < patterns.size(); i++) {
        for (size_t j = i + 1; j < patterns.size(); j++) {
            std::vector<std::tuple<int, int, int>> diff_pos;
            for (size_t k = 0; k < patterns[0].size(); k++) {
                if (patterns[i][k] != patterns[j][k]) {
                    std::tuple<int, int, int> positions = std::make_tuple(i, j, k);
                    diff_pos.push_back(positions);
                }
            }
            if (diff_pos.size() == 1) {
                one_diff_counter++;
                std::vector<std::vector<char>> patterns_modified = patterns;
                if (patterns_modified[std::get<0>(diff_pos[0])][std::get<2>(diff_pos[0])] == '#') {
                    patterns_modified[std::get<0>(diff_pos[0])][std::get<2>(diff_pos[0])] = '.';
                } else {
                    patterns_modified[std::get<0>(diff_pos[0])][std::get<2>(diff_pos[0])] = '#';
                }

                int possible_count = check_for_symetrie(patterns_modified, std::get<0>(diff_pos[0]), std::get<1>(diff_pos[0]));
                if (possible_count != 0) {
                    printPattern(patterns_modified, inverted, possible_count, std::get<0>(diff_pos[0]), std::get<1>(diff_pos[0]),
                                 std::get<2>(diff_pos[0]));
                    return possible_count;
                }
            }
        }
    }

    return 0;
}

int main() {
    std::ifstream file("input.txt");
    // std::ifstream file("test_02.txt");
    // std::ifstream file("test_03.txt");
    // std::ifstream file("test_04.txt");
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

            vertical_sum += check_for_symetrie_with_smudge(vertical_lines, true);
            horizontal_sum += check_for_symetrie_with_smudge(horizontal_lines, false);
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

    vertical_sum += check_for_symetrie_with_smudge(vertical_lines, true);
    horizontal_sum += check_for_symetrie_with_smudge(horizontal_lines, false);

    int result = vertical_sum + horizontal_sum * 100;

    std::cout << result << std::endl;

    file.close();
    return 0;
}
