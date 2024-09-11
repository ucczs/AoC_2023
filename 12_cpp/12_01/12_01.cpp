#include <algorithm>
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
        if (split == "") {
            continue;
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

int cleanUpSetup(std::vector<int>& damaged_counts, std::vector<std::string>& damaged_order) {
    static int counter = 0;
    static int full_removed_counter = 0;
    int combination_count = 0;
    bool still_removing = false;

    do {
        still_removing = false;

        // remove fields which contain a damaged and fit from size perfectly (front and back)
        if (damaged_counts.front() == damaged_order.front().size() && damaged_order.front().find("#") != std::string::npos) {
            damaged_counts.erase(damaged_counts.begin());
            damaged_order.erase(damaged_order.begin());
            still_removing = true;
            counter++;
        }
        if (damaged_counts.back() == damaged_order.back().size() && damaged_order.back().find("#") != std::string::npos) {
            damaged_counts.erase(damaged_counts.end() - 1);
            damaged_order.erase(damaged_order.end() - 1);
            still_removing = true;
            counter++;
        }

        if (damaged_counts.empty()) {
            combination_count = 1;
            full_removed_counter++;
            break;
        }

        // remove fields which contain all necessarry damaged and maximum one "?" (front and back)
        std::string::difference_type damaged_first = std::count(damaged_order.front().begin(), damaged_order.front().end(), '#');
        std::string::difference_type damaged_last = std::count(damaged_order.back().begin(), damaged_order.back().end(), '#');

        if (damaged_counts.front() == damaged_first && damaged_order.front().size() <= damaged_counts.front() + 1) {
            damaged_counts.erase(damaged_counts.begin());
            damaged_order.erase(damaged_order.begin());
            still_removing = true;
            counter++;
        }
        if (damaged_counts.back() == damaged_last && damaged_order.back().size() <= damaged_counts.back() + 1) {
            damaged_counts.erase(damaged_counts.end() - 1);
            damaged_order.erase(damaged_order.end() - 1);
            still_removing = true;
            counter++;
        }

        if (damaged_counts.empty()) {
            combination_count = 1;
            full_removed_counter++;
            break;
        }

        // remove fields which are too small to fit damaged at beginning and end
        if (damaged_counts.front() > damaged_order.front().size()) {
            damaged_order.erase(damaged_order.begin());
            still_removing = true;
            counter++;
        }
        if (damaged_counts.back() > damaged_order.back().size()) {
            damaged_order.erase(damaged_order.end() - 1);
            still_removing = true;
            counter++;
        }

        // remove parts of damaged order and damaged counts in case it is clear that it is at the very beginning or very end
        if (damaged_order.front().front() == '#' && damaged_order.front().size() >= damaged_counts.front()) {
            damaged_order.front().erase(0, damaged_counts.front());
            damaged_counts.erase(damaged_counts.begin());
            still_removing = true;
            counter++;
        }
        if (damaged_order.back().back() == '#' && damaged_order.back().size() >= damaged_counts.back()) {
            damaged_order.back().erase(damaged_order.back().size() - damaged_counts.back(), damaged_counts.back());
            damaged_counts.erase(damaged_counts.end() - 1);
            still_removing = true;
            counter++;
        }

        if (damaged_counts.empty()) {
            combination_count = 1;
            full_removed_counter++;
            break;
        }

        if (damaged_order.front().front() == '?' && *(damaged_order.front().begin() + 1) == '#' &&
            *(damaged_order.front().begin() + damaged_counts.front()) == '#') {
            damaged_order.front().erase(0, damaged_counts.front() + 1);
            damaged_counts.erase(damaged_counts.begin());
            still_removing = true;
            counter++;
        }

        if (damaged_order.back().back() == '?' && *(damaged_order.back().end() - 2) == '#' &&
            *(damaged_order.back().end() - damaged_counts.back() - 1) == '#') {
            int erase_start_pos = damaged_order.back().size() - (damaged_counts.back() + 1);
            int erase_length = damaged_counts.back() + 1;
            damaged_order.back().erase(erase_start_pos, erase_length);
            damaged_counts.erase(damaged_counts.end() - 1);
            still_removing = true;
            counter++;
        }

        if (damaged_counts.empty()) {
            combination_count = 1;
            full_removed_counter++;
            break;
        }
    } while (still_removing);

    std::cout << counter << std::endl;
    std::cout << full_removed_counter << std::endl;

    return combination_count;
}

int main() {
    std::ifstream file("input.txt");
    // std::ifstream file("test.txt");
    // std::ifstream file("test_02.txt");
    std::string line;

    int sum = 0;

    while (std::getline(file, line)) {
        std::cout << line << std::endl;

        while (line.find("..") != std::string::npos) {
            line = std::regex_replace(line, std::regex("\\.\\."), ".");
        }

        std::vector<std::string> inputs = split_string(line, ' ');
        std::vector<std::string> damaged_order = split_string(inputs[0], '.');

        std::vector<std::string> damaged_counts_str = split_string(inputs[1], ',');
        std::vector<int> damaged_counts = stringVec2intVec(&damaged_counts_str);

        int cobination_count = cleanUpSetup(damaged_counts, damaged_order);
        sum += cobination_count;
    }

    file.close();
    return 0;
}
