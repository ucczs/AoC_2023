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

class Instruction {
   private:
    std::string part{""};
    std::string operation{""};
    int value_to_compare{0};
    std::string result{""};

   public:
    Instruction(std::string result) : result(result) {};
    Instruction(std::string result, std::string part, std::string operation, int value_to_compare)
        : result(result), part(part), operation(operation), value_to_compare(value_to_compare) {};

    std::string processInstruction(int x, int m, int a, int s) const {
        std::string instrResult = "";
        if (operation.empty()) {
            instrResult = result;
        } else {
            int partValue = 0;
            if (part == "x") {
                partValue = x;
            } else if (part == "m") {
                partValue = m;
            } else if (part == "a") {
                partValue = a;
            } else if (part == "s") {
                partValue = s;
            }

            if (operation == "<") {
                if (partValue < value_to_compare) {
                    instrResult = result;
                } else {
                    instrResult = "";
                }
            } else if (operation == ">") {
                if (partValue > value_to_compare) {
                    instrResult = result;
                } else {
                    instrResult = "";
                }
            }
        }
        return instrResult;
    };
};

class Workflow {
   private:
    std::string name{""};
    std::vector<Instruction> instructions;

   public:
    explicit Workflow(std::string name) : name(name) {};
    // Workflow(const Workflow&) = delete;
    // Workflow(Workflow&& other) : name(std::move(other.name)), instructions(std::move(other.instructions)) {};

    const std::string& getName() const { return name; };
    void addInstruction(const Instruction instruction) { instructions.push_back(instruction); };
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

    std::string processInstructions(int x, int m, int a, int s) const {
        std::string instruction_result = "";
        for (Instruction instruction : instructions) {
            instruction_result = instruction.processInstruction(x, m, a, s);
            if (instruction_result != "") {
                break;
            }
        }
        return instruction_result;
    }

    const std::vector<Instruction>& getInstructions() const { return instructions; };
};

class Part {
   private:
    int x{0};
    int m{0};
    int a{0};
    int s{0};

   public:
    Part(int x, int m, int a, int s) : x(x), m(m), a(a), s(s) {};
    // Part(const Part&) = delete;
    // Part(Part&& other) : x(other.x), m(other.m), a(other.a), s(other.s) {};
    // Part& operator=(const Part&) = delete;

    int getX() const { return x; };
    int getM() const { return m; };
    int getA() const { return a; };
    int getS() const { return s; };
    int getSum() const { return x + m + a + s; };
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
    std::vector<Part> parts;

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
        } else {
            std::regex regex_pattern(R"(\{x=(\d+),m=(\d+),a=(\d+),s=(\d+)\})");
            std::smatch matches;

            if (std::regex_search(line, matches, regex_pattern)) {
                if (matches.size() == 5) {
                    int x = std::stoi(matches[1].str());
                    int m = std::stoi(matches[2].str());
                    int a = std::stoi(matches[3].str());
                    int s = std::stoi(matches[4].str());
                    parts.emplace_back(x, m, a, s);
                }
            } else {
                std::cout << "READ IN ERROR!" << std::endl;
            }
        }
    }

    std::vector<Part> accepted_parts;
    for (const Part& part : parts) {
        Workflow current_workflow = getWorkflowByName("in", workflows);
        std::string instruction_result = "";
        while (instruction_result != "A" && instruction_result != "R") {
            for (const Instruction& instruction : current_workflow.getInstructions()) {
                instruction_result = "";
                instruction_result = instruction.processInstruction(part.getX(), part.getM(), part.getA(), part.getS());
                if (instruction_result == "R") {
                    break;
                } else if (instruction_result == "A") {
                    accepted_parts.push_back(part);
                    break;
                } else if (instruction_result == "") {
                    continue;
                } else {
                    current_workflow = getWorkflowByName(instruction_result, workflows);
                    break;
                }
            }
        }
    }

    int result = 0;
    for (const Part& accepted_part : accepted_parts) {
        result += accepted_part.getSum();
    }

    std::cout << result << std::endl;

    file.close();
    return 0;
}