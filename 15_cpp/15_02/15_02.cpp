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

enum class operation_type { ADD, REMOVE };

class Lense {
   private:
    int focal_length;
    std::string name;
    int box_number;
    void determine_box_number() {
        box_number = 0;
        for (char ascii_char : name) {
            box_number += int(ascii_char);
            box_number *= 17;
            box_number = box_number % 256;
        }
    };

   public:
    Lense() = default;
    Lense(std::string lense_string) {
        if (lense_string.find('=') != std::string::npos) {
            std::vector<std::string> input = split_string(lense_string, '=');
            focal_length = std::stoi(input[1]);
            name = input[0];
        } else {
            focal_length = -1;
            name = lense_string;
            name.pop_back();
        }
        determine_box_number();
    };
    int getBoxNumber() { return box_number; };
    int getFocalLenght() { return focal_length; };
    bool operator==(const Lense& rhs) const { return name == rhs.name; };
    void updateFocalLength(int new_focal_length) { focal_length = new_focal_length; };
};

class Operation {
   private:
    operation_type type;
    Lense lense;

   public:
    Operation(std::string operation_string) {
        lense = Lense(operation_string);
        if (operation_string.find('=') != std::string::npos) {
            type = operation_type::ADD;
        } else {
            type = operation_type::REMOVE;
        };
    };

    void performOp(std::vector<std::vector<Lense>>& boxes) {
        std::vector<Lense>& involved_box = boxes[lense.getBoxNumber()];
        auto existing_lense = std::find(involved_box.begin(), involved_box.end(), lense);
        if (existing_lense != involved_box.end()) {
            if (type == operation_type::ADD) {
                existing_lense->updateFocalLength(lense.getFocalLenght());
            } else {
                involved_box.erase(existing_lense);
            }
        } else if (type == operation_type::ADD) {
            boxes[lense.getBoxNumber()].push_back(lense);
        }
    };
};

int main() {
    std::ifstream file("input.txt");
    std::string line;

    std::vector<std::vector<Lense>> boxes(256);

    while (std::getline(file, line)) {
        std::cout << line << std::endl;
        std::vector<std::string> instructions = split_string(line, ',');

        for (std::string instruction : instructions) {
            Operation nextOp = Operation(instruction);
            nextOp.performOp(boxes);
        }
    }

    int result = 0;
    for (size_t i = 0; i < boxes.size(); i++) {
        for (size_t j = 0; j < boxes[i].size(); j++) {
            int value = (1 + i) * (j + 1) * boxes[i][j].getFocalLenght();
            result += value;
        }
    }

    std::cout << result << std::endl;

    file.close();
    return 0;
}
