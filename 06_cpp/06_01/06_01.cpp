#include <fstream>
#include <iostream>
#include <regex>
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

int main() {
    std::ifstream file("input.txt");
    // std::ifstream file("test.txt");
    std::string line;

    std::vector<int> time_list;
    std::vector<int> distance_list;

    while (std::getline(file, line)) {
        while (line.find("  ") != std::string::npos) {
            line = std::regex_replace(line, std::regex("  "), " ");
        }

        if (line.find("Time") != std::string::npos) {
            std::vector<std::string> time_str;
            time_str = split_string(split_string(line, ':')[1], ' ');
            time_list = stringVec2intVec(&time_str);
        }
        if (line.find("Distance") != std::string::npos) {
            std::vector<std::string> distance_str;
            distance_str = split_string(split_string(line, ':')[1], ' ');
            distance_list = stringVec2intVec(&distance_str);
        }
    }

    std::vector<int> new_record_count;
    for (size_t i = 0; i < time_list.size(); i++) {
        int new_record = 0;
        int time = time_list[i];
        int distance = distance_list[i];

        for (size_t hold_button = 0; hold_button <= time; hold_button++) {
            int distance_this_run = hold_button * (time - hold_button);
            if (distance_this_run > distance) {
                new_record++;
            }
        }
        new_record_count.push_back(new_record);
    }

    int result = 1;
    for (int record : new_record_count) {
        result *= record;
    }

    std::cout << result << std::endl;

    file.close();
    return 0;
}
