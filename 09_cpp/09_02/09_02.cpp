#include <algorithm>
#include <cmath>
#include <fstream>
#include <iostream>
#include <numeric>
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

std::vector<int> getDerivation(std::vector<int> number_set) {
    std::vector<int> derivate;
    int prevValue = number_set[0];
    for (auto iter = std::next(number_set.begin()); iter != number_set.end(); iter++) {
        derivate.push_back(*iter - prevValue);
        prevValue = *iter;
    }
    return derivate;
}

int getNextNumber(std::vector<std::vector<int>> full_tree) {
    int new_first_value = 0;
    for (auto it = full_tree.rbegin(); it != full_tree.rend(); ++it) {
        new_first_value = it->front() - new_first_value;
    }
    return new_first_value;
}

bool allZeros(std::vector<int> numbers) {
    bool valid = true;
    for (int number : numbers) {
        if (number != 0) {
            valid = false;
            break;
        }
    }
    return valid;
}

int main() {
    std::ifstream file("input.txt");
    // std::ifstream file("test.txt");
    std::string line;
    std::vector<int> new_numbers;

    while (std::getline(file, line)) {
        std::vector<std::string> history_str = split_string(line, ' ');
        std::vector<int> history = stringVec2intVec(&history_str);

        std::vector<std::vector<int>> full_tree;
        full_tree.push_back(history);

        while (!allZeros(full_tree.back())) {
            std::vector<int> derivate = getDerivation(full_tree.back());
            full_tree.push_back(derivate);
        }

        new_numbers.push_back(getNextNumber(full_tree));
    }

    long long int result = std::accumulate(new_numbers.begin(), new_numbers.end(), 0LL);

    std::cout << result << std::endl;

    file.close();
    return 0;
}
