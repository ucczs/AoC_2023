#include <algorithm>
#include <fstream>
#include <iostream>
#include <numeric>
#include <regex>
#include <sstream>
#include <string>
#include <vector>

typedef std::pair<std::string, std::string> node_targets;
typedef std::map<std::string, node_targets> network_type;

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

std::vector<std::string> getAllStartingNodes(network_type network) {
    std::vector<std::string> startingNodeNames;
    for (auto node : network) {
        if (node.first.back() == 'A') {
            startingNodeNames.push_back(node.first);
        }
    }
    return startingNodeNames;
}

bool allNodesEndWithZ(std::vector<std::string> node_names, std::vector<int>& steps_count_reaching_z, int step_count) {
    bool valid = true;
    int count = 0;
    for (std::string node_name : node_names) {
        if (node_name.back() == 'Z') {
            steps_count_reaching_z[count] = step_count;
        }
        count++;
    }
    return std::all_of(steps_count_reaching_z.begin(), steps_count_reaching_z.end(), [](int value) { return value != 0; });
}

std::vector<node_targets> getAllTargets(network_type network, std::vector<std::string> current_nodes) {
    std::vector<node_targets> targets;
    for (std::string current_node : current_nodes) {
        targets.push_back(network[current_node]);
    }
    return targets;
}

std::vector<std::string> getNextNodes(char direction, std::vector<node_targets> targets) {
    std::vector<std::string> next_nodes;
    for (node_targets target : targets) {
        if (direction == 'L') {
            next_nodes.push_back(target.first);
        } else if (direction == 'R') {
            next_nodes.push_back(target.second);
        } else {
            std::cout << "Direction error!" << std::endl;
        }
    }
    return next_nodes;
}

long long int lcm(long long int a, long long int b) { return (a * b) / std::gcd(a, b); }

int main() {
    std::ifstream file("input.txt");
    // std::ifstream file("test.txt");
    std::string line;

    std::string instructions;
    network_type network;

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
            node_targets targets = std::make_pair(targets_vec[0], targets_vec[1]);
            network[node[0]] = targets;
        } else {
            instructions = line;
        }
    }

    std::vector<std::string> current_nodes = getAllStartingNodes(network);
    int step_count = 0;

    std::vector<int> steps_count_reaching_z(current_nodes.size(), 0);
    while (!allNodesEndWithZ(current_nodes, steps_count_reaching_z, step_count)) {
        char direction = instructions.at(step_count % instructions.size());

        std::vector<node_targets> current_nodes_options = getAllTargets(network, current_nodes);

        current_nodes = getNextNodes(direction, current_nodes_options);
        step_count++;

        if (step_count % 100000 == 0) {
            std::cout << step_count << std::endl;
        }
    }

    long long result = std::accumulate(steps_count_reaching_z.begin(), steps_count_reaching_z.end(), 1LL, lcm);

    std::cout << result << std::endl;

    file.close();
    return 0;
}
