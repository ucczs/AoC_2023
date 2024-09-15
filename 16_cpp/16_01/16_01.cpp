#include <algorithm>
#include <chrono>
#include <fstream>
#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <thread>
#include <vector>

enum class direction_t { NORTH, EAST, SOUTH, WEST };

class Space {
   private:
    std::map<direction_t, bool> direction_passed{
        {direction_t::NORTH, false}, {direction_t::EAST, false}, {direction_t::SOUTH, false}, {direction_t::WEST, false}};
    char type;

   public:
    Space(char space_type) : type(space_type) {};
    char getType() const { return type; };
    void passSpace(const direction_t dir) { direction_passed[dir] = true; };
    bool inDirectionAlreadyPassed(const direction_t dir) { return direction_passed[dir]; }

    bool operator==(const char& c) { return c == type; };
};

typedef std::vector<Space> row_t;
typedef std::vector<row_t> map_t;
typedef std::vector<std::vector<char>> trace_map_t;

void printMap(map_t map) {}

class Beam {
   private:
    std::pair<int, int> position;
    direction_t direction;
    bool beam_active;

   public:
    Beam(const std::pair<int, int> pos, direction_t dir) : position(pos), direction(dir), beam_active(true) {};
    std::pair<int, int> getNextPosition() {
        if (beam_active) {
            switch (direction) {
                case direction_t::NORTH:
                    position.second--;
                    break;
                case direction_t::EAST:
                    position.first++;
                    break;
                case direction_t::SOUTH:
                    position.second++;
                    break;
                case direction_t::WEST:
                    position.first--;
                    break;
                default:
                    break;
            }
        }
        return position;
    };
    std::pair<int, int> getPosition() const { return position; };
    void setDirection(direction_t dir) { direction = dir; };
    direction_t getDirection() const { return direction; };
    void deactivateBeam() { beam_active = false; };
    bool isActive() const { return beam_active; };
};

class MirrorSpace {
   private:
    int stepCount{0};
    trace_map_t trace_map;
    map_t mirror_map;
    std::vector<Beam> beams;
    bool deactivateBeamIfNecessary(Beam& beam) {
        std::pair<int, int> beam_pos = beam.getPosition();
        direction_t beam_dir = beam.getDirection();
        bool beam_deactivated = false;
        if (beam_dir == direction_t::NORTH && beam_pos.second == 0 &&
            (mirror_map[beam_pos.second][beam_pos.first] == '.' || mirror_map[beam_pos.second][beam_pos.first] == '|')) {
            beam.deactivateBeam();
            beam_deactivated = true;
        }
        if (beam_dir == direction_t::WEST && beam_pos.first == 0 &&
            (mirror_map[beam_pos.second][beam_pos.first] == '.' || mirror_map[beam_pos.second][beam_pos.first] == '-')) {
            beam.deactivateBeam();
            beam_deactivated = true;
        }
        if (beam_dir == direction_t::SOUTH && beam_pos.second == mirror_map.size() - 1 &&
            (mirror_map[beam_pos.second][beam_pos.first] == '.' || mirror_map[beam_pos.second][beam_pos.first] == '|')) {
            beam.deactivateBeam();
            beam_deactivated = true;
        }
        if (beam_dir == direction_t::EAST && beam_pos.first == mirror_map[0].size() - 1 &&
            (mirror_map[beam_pos.second][beam_pos.first] == '.' || mirror_map[beam_pos.second][beam_pos.first] == '-')) {
            beam.deactivateBeam();
            beam_deactivated = true;
        }
        return beam_deactivated;
    };
    bool posOutOfBounds(std::pair<int, int> pos) {
        return (pos.first < 0 || pos.second < 0 || pos.first >= mirror_map[0].size() || pos.second >= mirror_map.size());
    }

   public:
    MirrorSpace(const map_t mirror_map, const trace_map_t trace_map, std::pair<int, int> start_pos, direction_t start_dir)
        : mirror_map(mirror_map), trace_map(trace_map) {
        if (mirror_map[start_pos.second][start_pos.first].getType() != '.') {
            if (mirror_map[start_pos.second][start_pos.first].getType() == '\\') {
                start_dir = direction_t::SOUTH;
            }
        }

        Beam initial_beam = Beam(start_pos, start_dir);
        beams.push_back(initial_beam);
    };
    MirrorSpace(MirrorSpace& rhs) = delete;
    MirrorSpace(MirrorSpace&& rhs) = delete;
    MirrorSpace& operator=(const MirrorSpace&) = delete;

    int countEnergizedSpaces() const {
        int result = 0;
        for (std::vector<char> row : trace_map) {
            for (char c : row) {
                if (c == '#') {
                    result++;
                }
            }
        }
        return result;
    }

    int activeBeamCount() const {
        int activeBeams = 0;
        for (Beam beam : beams) {
            if (beam.isActive()) {
                activeBeams++;
            }
        }
        return activeBeams;
    };

    void printTraceMap() const {
        for (std::vector<char> row : trace_map) {
            for (char c : row) {
                std::cout << c;
            }
            std::cout << std::endl;
        }
        std::cout << std::endl;
        std::cout << stepCount << std::endl;
    }

    void printMap(bool withTrace = true) const {
        for (size_t i = 0; i < mirror_map.size(); i++) {
            std::cout << i << "\t";
            for (size_t j = 0; j < mirror_map[i].size(); j++) {
                char space_type = mirror_map[i][j].getType();
                if (withTrace && space_type == '.') {
                    std::cout << trace_map[i][j];
                } else {
                    std::cout << space_type;
                }
            }
            std::cout << std::endl;
        }
        std::cout << std::endl;
        std::cout << stepCount << std::endl;
    }

    void proceedBeams() {
        stepCount++;

        int beams_count = beams.size();
        for (size_t i = 0; i < beams_count; i++)
            if (beams[i].isActive()) {
                std::pair<int, int> beam_pos = beams[i].getNextPosition();
                direction_t beam_dir = beams[i].getDirection();

                if (posOutOfBounds(beam_pos) || mirror_map[beam_pos.second][beam_pos.first].inDirectionAlreadyPassed(beam_dir)) {
                    beams[i].deactivateBeam();
                    continue;
                } else {
                    trace_map[beam_pos.second][beam_pos.first] = '#';
                    mirror_map[beam_pos.second][beam_pos.first].passSpace(beam_dir);
                }

                if (deactivateBeamIfNecessary(beams[i])) {
                    continue;
                }
                if (mirror_map[beam_pos.second][beam_pos.first] == '/') {
                    switch (beam_dir) {
                        case direction_t::NORTH:
                            beams[i].setDirection(direction_t::EAST);
                            break;
                        case direction_t::EAST:
                            beams[i].setDirection(direction_t::NORTH);
                            break;
                        case direction_t::SOUTH:
                            beams[i].setDirection(direction_t::WEST);
                            break;
                        case direction_t::WEST:
                            beams[i].setDirection(direction_t::SOUTH);
                            break;
                    }
                } else if (mirror_map[beam_pos.second][beam_pos.first] == '\\') {
                    switch (beam_dir) {
                        case direction_t::NORTH:
                            beams[i].setDirection(direction_t::WEST);
                            break;
                        case direction_t::EAST:
                            beams[i].setDirection(direction_t::SOUTH);
                            break;
                        case direction_t::SOUTH:
                            beams[i].setDirection(direction_t::EAST);
                            break;
                        case direction_t::WEST:
                            beams[i].setDirection(direction_t::NORTH);
                            break;
                    }
                } else if (mirror_map[beam_pos.second][beam_pos.first] == '|') {
                    direction_t newDir1;
                    direction_t newDir2;
                    switch (beam_dir) {
                        case direction_t::NORTH:
                        case direction_t::SOUTH:
                            break;
                        case direction_t::EAST:
                        case direction_t::WEST:
                            beams[i].deactivateBeam();
                            beams.push_back(Beam(beam_pos, direction_t::NORTH));
                            beams.push_back(Beam(beam_pos, direction_t::SOUTH));
                            break;
                    }
                } else if (mirror_map[beam_pos.second][beam_pos.first] == '-') {
                    switch (beam_dir) {
                        case direction_t::EAST:
                        case direction_t::WEST:
                            break;
                        case direction_t::SOUTH:
                        case direction_t::NORTH:
                            beams[i].deactivateBeam();
                            beams.push_back(Beam(beam_pos, direction_t::EAST));
                            beams.push_back(Beam(beam_pos, direction_t::WEST));
                            break;
                    }
                }
            }
    }
};

int main() {
    std::ifstream file("input.txt");
    // std::ifstream file("test.txt");
    // std::ifstream file("test_01.txt");
    // std::ifstream file("test_02.txt");
    std::string line;

    map_t map;
    int row_counter = 0;
    while (std::getline(file, line)) {
        row_t row;
        for (char c : line) {
            row.push_back(Space(c));
        }
        map.push_back(row);
    }

    std::vector<char> trace_row(map[0].size(), '.');
    trace_map_t trace_map(map.size(), trace_row);
    int start_pos_x = 0;
    int start_pos_y = 0;
    direction_t start_dir = direction_t::EAST;

    trace_map[start_pos_y][start_pos_x] = '#';
    map[start_pos_y][start_pos_x].passSpace(start_dir);

    MirrorSpace mirrorSpace = MirrorSpace(map, trace_map, std::make_pair(start_pos_y, start_pos_x), start_dir);

    while (mirrorSpace.activeBeamCount() > 0) {
        mirrorSpace.proceedBeams();
        // mirrorSpace.printMap();
        // std::this_thread::sleep_for(std::chrono::milliseconds(250));
    }

    mirrorSpace.printMap();
    mirrorSpace.printTraceMap();

    int result = mirrorSpace.countEnergizedSpaces();
    std::cout << result << std::endl;

    file.close();
    return 0;
}
