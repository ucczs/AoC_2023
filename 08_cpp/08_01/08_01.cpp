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
        splitted_string.push_back(split);
    }

    return splitted_string;
}

int main() {
    std::ifstream file("input.txt");
    // std::ifstream file("test.txt");
    std::string line;

    std::string instructions;
    std::map<std::string, std::pair<std::string, std::string>> network;

    while (std::getline(file, line)) {
        if (line == "") {
            continue;
        }

        if (line.find('=') != std::string::npos) {
            line = std::regex_replace(line, std::regex("\\ "), "");
            line = std::regex_replace(line, std::regex("\\("), "");
            line = std::regex_replace(line, std::regex("\\)"), "");

            std::vector<std::string> node = split_string(line, '=');

            std::vector<std::string> targets_vec = split_string(node[1], ',');
            std::pair<std::string, std::string> targets = std::make_pair(targets_vec[0], targets_vec[1]);
            network[node[0]] = targets;
        } else {
            instructions = line;
        }
    }

    std::string current_node = network.begin()->first;
    int step_count = 0;
    while (current_node.find("ZZZ") == std::string::npos) {
        char direction = instructions.at(step_count % instructions.size());
        std::pair<std::string, std::string> current_node_options = network[current_node];

        if (direction == 'L') {
            current_node = current_node_options.first;
        } else if (direction == 'R') {
            current_node = current_node_options.second;
        } else {
            std::cout << "Direction error!" << std::endl;
        }
        step_count++;
    }

    std::cout << step_count << std::endl;

    file.close();
    return 0;
}
