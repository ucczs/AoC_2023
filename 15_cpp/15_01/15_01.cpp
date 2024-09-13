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

int main() {
    std::ifstream file("input.txt");
    // std::ifstream file("test.txt");
    // std::ifstream file("test_02.txt");
    std::string line;

    int result = 0;
    while (std::getline(file, line)) {
        std::cout << line << std::endl;
        std::vector<std::string> steps = split_string(line, ',');

        for (std::string step : steps) {
            int step_result = 0;
            for (char ascii_char : step) {
                step_result += int(ascii_char);
                step_result *= 17;
                step_result = step_result % 256;
            }
            result += step_result;
        }
    }

    std::cout << result << std::endl;

    file.close();
    return 0;
}
