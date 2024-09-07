#include <algorithm>
#include <array>
#include <climits>
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

std::vector<long long int> get_numbers_from_line(std::string line) {
    std::vector<std::string> numbers_str = split_string(line, ' ');
    std::vector<long long int> numbers;

    std::transform(numbers_str.begin(), numbers_str.end(), std::back_inserter(numbers),
                   [](const std::string& str) { return std::stoll(str); });

    return numbers;
}

class TypeMapping {
   private:
    std::vector<std::vector<long long int>> mapping_rules;
    std::string map_name;

   public:
    TypeMapping(std::string name);
    void add_mapping_rule(std::vector<long long int> mapping_rule) { mapping_rules.push_back(mapping_rule); }
    long long int getMappedVal(long long int index);
};

long long int TypeMapping::getMappedVal(long long int index) {
    for (std::vector<long long int> mapping_rule : mapping_rules) {
        if (index >= mapping_rule[1] && index < mapping_rule[1] + mapping_rule[2]) {
            return (index + mapping_rule[0] - mapping_rule[1]);
        }
    }
    return index;
}

TypeMapping::TypeMapping(std::string name) { map_name = name; }

int main() {
    // std::ifstream file("input.txt");
    // std::ifstream file("test.txt");
    std::ifstream file("test_02.txt");
    std::string line;

    std::vector<long long int> seeds;

    bool seed_to_soil_flag = false;
    bool soil_to_fertilizer_flag = false;
    bool fertilizer_to_water_flag = false;
    bool water_to_light_flag = false;
    bool light_to_temperature_flag = false;
    bool temperature_to_humidity_flag = false;
    bool humidity_to_location_flag = false;

    TypeMapping s2s = TypeMapping("s2s");
    TypeMapping s2f = TypeMapping("s2f");
    TypeMapping f2w = TypeMapping("f2w");
    TypeMapping w2l = TypeMapping("w2l");
    TypeMapping l2t = TypeMapping("l2t");
    TypeMapping t2h = TypeMapping("t2h");
    TypeMapping h2l = TypeMapping("h2l");

    while (std::getline(file, line)) {
        if (line == "") {
            continue;
        }

        if (line.find("seeds:", 0) != std::string::npos) {
            std::vector<std::string> seeds_str = split_string(split_string(line, ':')[1], ' ');

            std::transform(seeds_str.begin(), seeds_str.end(), std::back_inserter(seeds),
                           [](const std::string& str) { return std::stoll(str); });
        } else if (line.find("seed-to-soil") != std::string::npos) {
            seed_to_soil_flag = true;
        } else if (line.find("soil-to-fertilizer") != std::string::npos) {
            seed_to_soil_flag = false;
            soil_to_fertilizer_flag = true;
        } else if (line.find("fertilizer-to-water") != std::string::npos) {
            soil_to_fertilizer_flag = false;
            fertilizer_to_water_flag = true;
        } else if (line.find("water-to-light") != std::string::npos) {
            fertilizer_to_water_flag = false;
            water_to_light_flag = true;
        } else if (line.find("light-to-temperature") != std::string::npos) {
            water_to_light_flag = false;
            light_to_temperature_flag = true;
        } else if (line.find("temperature-to-humidity") != std::string::npos) {
            light_to_temperature_flag = false;
            temperature_to_humidity_flag = true;
        } else if (line.find("humidity-to-location") != std::string::npos) {
            temperature_to_humidity_flag = false;
            humidity_to_location_flag = true;
        } else {
            if (seed_to_soil_flag) {
                std::vector<long long int> mapping_s2s = get_numbers_from_line(line);
                s2s.add_mapping_rule(mapping_s2s);
            } else if (soil_to_fertilizer_flag) {
                std::vector<long long int> mapping_s2f = get_numbers_from_line(line);
                s2f.add_mapping_rule(mapping_s2f);
            } else if (fertilizer_to_water_flag) {
                std::vector<long long int> mapping_f2w = get_numbers_from_line(line);
                f2w.add_mapping_rule(mapping_f2w);
            } else if (water_to_light_flag) {
                std::vector<long long int> mapping_w2l = get_numbers_from_line(line);
                w2l.add_mapping_rule(mapping_w2l);
            } else if (light_to_temperature_flag) {
                std::vector<long long int> mapping_l2t = get_numbers_from_line(line);
                l2t.add_mapping_rule(mapping_l2t);
            } else if (temperature_to_humidity_flag) {
                std::vector<long long int> mapping_t2h = get_numbers_from_line(line);
                t2h.add_mapping_rule(mapping_t2h);
            } else if (humidity_to_location_flag) {
                std::vector<long long int> mapping_h2l = get_numbers_from_line(line);
                h2l.add_mapping_rule(mapping_h2l);
            } else {
                std::cout << "READ IN ERROR!" << std::endl;
            }
        }
    }

    long long int min = LLONG_MAX;
    long long int min_seed = 0;
    long long int start_seed_range = 0;
    long long int range_length = 0;
    for (size_t i = 0; i < seeds.size(); i++) {
        std::cout << i << " of " << seeds.size() << " done." << std::endl;
        if (i % 2 == 0) {
            start_seed_range = seeds[i];
        } else {
            range_length = seeds[i];

            for (long long int seed_no = start_seed_range; seed_no < start_seed_range + range_length; seed_no++) {
                long long int s2s_no = s2s.getMappedVal(seed_no);
                long long int s2f_no = s2f.getMappedVal(s2s_no);
                long long int f2w_no = f2w.getMappedVal(s2f_no);
                long long int w2l_no = w2l.getMappedVal(f2w_no);
                long long int l2t_no = l2t.getMappedVal(w2l_no);
                long long int t2h_no = t2h.getMappedVal(l2t_no);
                long long int loc = h2l.getMappedVal(t2h_no);

                // std::cout << seed_no << " -> " << s2s_no << " -> " << s2f_no << " -> " << f2w_no << " -> " << w2l_no << " -> " << l2t_no
                //           << " -> " << t2h_no << " -> " << loc << std::endl;

                // std::cout << seed_no << " at location " << loc << std::endl;
                // std::cout << "****************" << std::endl;

                if (loc < min) {
                    min = loc;
                    min_seed = seed_no;
                }
            }
        }
    }

    std::cout << min << " with seed " << min_seed << std::endl;
    file.close();
    return 0;
}
