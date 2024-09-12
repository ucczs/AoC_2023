#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

typedef std::vector<char> row_t;
typedef std::vector<row_t> map_t;

constexpr int cycle_count = 1000000000;

class Platform {
   private:
    map_t platformMap;

    std::size_t hash_vector(row_t v) {
        std::size_t seed = 0;
        for (char c : v) {
            seed ^= std::hash<char>{}(c) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
        }
        return seed;
    }

   public:
    Platform() = default;
    map_t getMap() { return platformMap; };
    void addLine(row_t line) { platformMap.push_back(line); };
    void tiltLineWest() {
        for (row_t& rockLine : platformMap) {
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
        }
    };

    void tiltLineNorth() {
        for (int column = 0; column < platformMap[0].size(); column++) {
            for (int i = 0; i < platformMap.size(); i++) {
                if (platformMap[i][column] == 'O' && i - 1 >= 0) {
                    for (int j = i - 1; j >= 0; j--) {
                        if (platformMap[j][column] == '.') {
                            platformMap[j + 1][column] = '.';
                            platformMap[j][column] = 'O';
                        } else if (platformMap[j][column] == '#' || platformMap[j][column] == 'O') {
                            break;
                        }
                    }
                }
            }
        }
    };

    void tiltLineSouth() {
        for (int column = 0; column < platformMap[0].size(); column++) {
            for (int i = platformMap.size() - 1; i >= 0; i--) {
                if (platformMap[i][column] == 'O' && i + 1 < platformMap.size()) {
                    for (int j = i + 1; j < platformMap.size(); j++) {
                        if (platformMap[j][column] == '.') {
                            platformMap[j - 1][column] = '.';
                            platformMap[j][column] = 'O';
                        } else if (platformMap[j][column] == '#' || platformMap[j][column] == 'O') {
                            break;
                        }
                    }
                }
            }
        }
    }

    void tiltLineEast() {
        for (row_t& rockLine : platformMap) {
            std::reverse(rockLine.begin(), rockLine.end());
        }

        tiltLineWest();

        for (row_t& rockLine : platformMap) {
            std::reverse(rockLine.begin(), rockLine.end());
        }
    };

    int getLoad() {
        int load_total = 0;
        int caused_load = platformMap.size();
        for (row_t rockLine : platformMap) {
            for (char c : rockLine) {
                if (c == 'O') {
                    load_total += caused_load;
                }
            }
            caused_load--;
        }
        return load_total;
    };

    std::size_t getHash() {
        std::size_t seed = 0;
        for (const auto& v : platformMap) {
            seed ^= hash_vector(v) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
        }
        return seed;
    }
};

void printMap(map_t map) {
    for (row_t row : map) {
        for (char c : row) {
            std::cout << c;
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}

int main() {
    std::ifstream file("input.txt");
    // std::ifstream file("test.txt");
    std::string line;

    Platform platformMap;

    while (std::getline(file, line)) {
        row_t platformMapLine;
        for (char c : line) {
            platformMapLine.push_back(c);
        }
        platformMap.addLine(platformMapLine);
    }

    Platform prevPlatformMap;

    std::vector<size_t> hashes;
    std::vector<int> loads;
    for (int i = 0; i < cycle_count; i++) {
        prevPlatformMap = platformMap;
        platformMap.tiltLineNorth();
        platformMap.tiltLineWest();
        platformMap.tiltLineSouth();
        platformMap.tiltLineEast();

        int result = platformMap.getLoad();
        std::cout << result << "\t" << i << std::endl;
        loads.push_back(result);

        size_t hash = platformMap.getHash();
        if (std::find(hashes.begin(), hashes.end(), hash) != hashes.end()) {
            std::cout << "Hash already there, cycle starts: " << i << std::endl;
            break;
        } else {
            hashes.push_back(hash);
        }
    }

    file.close();
    return 0;
}
