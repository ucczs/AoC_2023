#include <algorithm>
#include <array>
#include <cmath>
#include <fstream>
#include <iostream>
#include <regex>
#include <sstream>
#include <string>
#include <vector>

class Workflow;
Workflow& getWorkflowByName(const std::string& name, std::vector<Workflow>& workflows);

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

class Instruction {
   private:
    std::string part{""};
    std::string operation{""};
    int value_to_compare{0};
    std::string result{""};
    float probability_for_result{0.f};

   public:
    Instruction(std::string result) : result(result) {};
    Instruction(std::string result, std::string part, std::string operation, int value_to_compare)
        : result(result), part(part), operation(operation), value_to_compare(value_to_compare) {};

    std::string getResult() const { return result; };
    std::string getOperation() const { return operation; };
    int getCompareValue() const { return value_to_compare; };
    std::string getPart() const { return part; };
};

class Workflow {
   private:
    std::string name{""};
    std::vector<Instruction> instructions;
    long long int calculateCombinationsWithLimits(const std::map<std::string, std::pair<long long, long long>>& limits) const {
        constexpr int adder = 1;
        return (std::get<1>(limits.at("x")) - std::get<0>(limits.at("x")) + adder) *
               (std::get<1>(limits.at("m")) - std::get<0>(limits.at("m")) + adder) *
               (std::get<1>(limits.at("a")) - std::get<0>(limits.at("a")) + adder) *
               (std::get<1>(limits.at("s")) - std::get<0>(limits.at("s")) + adder);
    }

   public:
    explicit Workflow(std::string name) : name(name) {};

    const std::string& getName() const { return name; };
    void addInstructionsViaFullString(const std::string& instructions_string) {
        std::vector<std::string> instructions_string_vec = split_string(instructions_string, ',');
        for (std::string instruction_str : instructions_string_vec) {
            if (instruction_str.find('<') == std::string::npos && instruction_str.find('>') == std::string::npos) {
                instructions.emplace_back(instruction_str);
            } else if (instruction_str.find('<') != std::string::npos) {
                std::vector<std::string> instruction_details = split_string(instruction_str, '<');
                std::vector<std::string> comparison_details = split_string(instruction_details[1], ':');
                instructions.emplace_back(comparison_details[1], instruction_details[0], "<", std::stoi(comparison_details[0]));
            } else if (instruction_str.find('>') != std::string::npos) {
                std::vector<std::string> instruction_details = split_string(instruction_str, '>');
                std::vector<std::string> comparison_details = split_string(instruction_details[1], ':');
                instructions.emplace_back(comparison_details[1], instruction_details[0], ">", std::stoi(comparison_details[0]));
            }
        }
    };
    std::string& getName() { return name; };

    const std::vector<Instruction>& getInstructions() const { return instructions; };

    long long int getCombinationsCount(const std::map<std::string, std::pair<long long, long long>>& prev_limits,
                                       std::vector<Workflow>& workflows) {
        long long result = 0;
        std::map<std::string, std::pair<long long, long long>> current_limits = prev_limits;
        for (const Instruction& instruction : instructions) {
            if (instruction.getResult() == "A" && instruction.getOperation() == "") {
                result += calculateCombinationsWithLimits(current_limits);
            } else if (instruction.getResult() == "R" && instruction.getOperation() == "") {
                continue;
            } else if (instruction.getOperation() == "") {
                std::string nextWorkflowName = instruction.getResult();
                Workflow nextWorkflow = getWorkflowByName(nextWorkflowName, workflows);
                result += nextWorkflow.getCombinationsCount(current_limits, workflows);
            } else {
                int value = instruction.getCompareValue();
                std::string part = instruction.getPart();
                std::string nextWorkflowName = instruction.getResult();
                std::string comparison_type = instruction.getOperation();

                std::array<bool, 2> comparison_results{true, false};

                for (bool comparison_true : comparison_results) {
                    if (comparison_type == ">") {
                        if (comparison_true) {
                            std::map<std::string, std::pair<long long, long long>> current_limits_bigger_than = current_limits;
                            current_limits_bigger_than[part].first = value + 1;

                            if (instruction.getResult() == "A") {
                                result += calculateCombinationsWithLimits(current_limits_bigger_than);
                            } else if (instruction.getResult() == "R") {
                                continue;
                            } else {
                                Workflow nextWorkflow = getWorkflowByName(nextWorkflowName, workflows);
                                result += nextWorkflow.getCombinationsCount(current_limits_bigger_than, workflows);
                            }
                        } else {
                            current_limits[part].second = value;
                            continue;
                        }

                    } else if (comparison_type == "<") {
                        if (comparison_true) {
                            std::map<std::string, std::pair<long long, long long>> current_limits_smaller_than = current_limits;
                            current_limits_smaller_than[part].second = value - 1;

                            if (instruction.getResult() == "A") {
                                result += calculateCombinationsWithLimits(current_limits_smaller_than);
                            } else if (instruction.getResult() == "R") {
                                continue;
                            } else {
                                Workflow nextWorkflow = getWorkflowByName(nextWorkflowName, workflows);
                                result += nextWorkflow.getCombinationsCount(current_limits_smaller_than, workflows);
                            }
                        } else {
                            current_limits[part].first = value;
                            continue;
                        }
                    }
                }
            }
        }
        return result;
    };
};

Workflow& getWorkflowByName(const std::string& name, std::vector<Workflow>& workflows) {
    for (Workflow& workflow : workflows) {
        if (workflow.getName() == name) {
            return workflow;
        }
    }
    throw std::runtime_error("ERROR: WORKFLOW NOT FOUND!");
}

int main() {
    std::ifstream file("input.txt");
    // std::ifstream file("test.txt");
    std::string line;

    bool instruction_flag = true;

    std::vector<Workflow> workflows;

    while (std::getline(file, line)) {
        if (line == "") {
            instruction_flag = false;
            continue;
        }

        if (instruction_flag) {
            std::vector<std::string> input = split_string(line, '{');
            workflows.emplace_back(input[0]);
            std::string instructions_str = std::regex_replace(input[1], std::regex("}"), "");
            workflows.back().addInstructionsViaFullString(instructions_str);
        }
    }

    std::map<std::string, std::pair<long long, long long>> limits{
        {"x", std::make_pair(1, 4000)},
        {"m", std::make_pair(1, 4000)},
        {"a", std::make_pair(1, 4000)},
        {"s", std::make_pair(1, 4000)},
    };
    Workflow first_workflow = getWorkflowByName("in", workflows);
    long long int result = first_workflow.getCombinationsCount(limits, workflows);

    std::cout << result << std::endl;

    file.close();
    return 0;
}