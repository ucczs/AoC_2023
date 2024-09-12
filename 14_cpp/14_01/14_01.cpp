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

class SouthNorthLines {
   private:
    std::vector<char> rockLine;

   public:
    SouthNorthLines() {};
    void addPiece(char c) { rockLine.push_back(c); };
    void tiltLine() {
        for (int i = 0; i < rockLine.size(); i++) {
            if (rockLine[i] == 'O' && i - 1 >= 0) {
                for (int j = i - 1; j >= 0; j--) {
                    if (rockLine[j] == '.') {
                        rockLine[j + 1] = '.';
                        rockLine[j] = 'O';
                    } else if (rockLine[j] == '#' || rockLine[j] == 'O') {
                        break;
                    }
                }
            }
        }
    };
    int getLoad() {
        int load_total = 0;
        int caused_load = rockLine.size();
        for (char c : rockLine) {
            if (c == 'O') {
                load_total += caused_load;
            }
            caused_load--;
        }
        return load_total;
    };
};

int main() {
    std::ifstream file("input.txt");
    // std::ifstream file("test.txt");
    std::string line;

    std::vector<SouthNorthLines> platform;

    while (std::getline(file, line)) {
        if (platform.size() != line.size()) {
            platform.resize(line.size());
        }

        for (size_t i = 0; i < line.size(); i++) {
            platform[i].addPiece(line[i]);
        }
    }

    int result = 0;
    for (SouthNorthLines& line : platform) {
        line.tiltLine();
        result += line.getLoad();
    }

    std::cout << result << std::endl;

    file.close();
    return 0;
}
