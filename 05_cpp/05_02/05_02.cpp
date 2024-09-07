#include <algorithm>
#include <array>
#include <climits>
#include <fstream>
#include <iostream>
#include <numeric>
#include <sstream>
#include <string>
#include <tuple>
#include <utility>
#include <vector>

typedef long long int llint;
typedef std::tuple<llint, llint, llint> conversion_type;
typedef std::pair<llint, llint> range_type;
typedef std::vector<conversion_type> conversion_list_type;
typedef std::vector<range_type> range_list_type;

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

std::vector<llint> get_numbers_from_line(std::string line) {
    std::vector<std::string> numbers_str = split_string(line, ' ');
    std::vector<llint> numbers;

    std::transform(numbers_str.begin(), numbers_str.end(), std::back_inserter(numbers),
                   [](const std::string& str) { return std::stoll(str); });

    return numbers;
}

class TypeMapping {
   private:
    std::vector<std::vector<llint>> mapping_rules;
    std::string map_name;
    conversion_list_type conversion_ranges;

   public:
    TypeMapping(std::string name);
    void add_mapping_rule(std::vector<llint> mapping_rule) { mapping_rules.push_back(mapping_rule); }
    range_list_type mapRanges(range_list_type input_ranges);
};

range_list_type TypeMapping::mapRanges(range_list_type input_ranges) {
    range_list_type out_ranges;
    for (std::vector<llint> mapping_rule : mapping_rules) {
        conversion_type range = {mapping_rule[1], mapping_rule[1] + mapping_rule[2] - 1, mapping_rule[0] - mapping_rule[1]};
        conversion_ranges.push_back(range);
    }

    int converted_ranges = 0;
    range_list_type untouched_ranges = input_ranges;
    bool first_run = true;
    do {
        range_list_type untouched_ranges_next;
        converted_ranges = 0;
        for (range_type current_mapped_range : untouched_ranges) {
            bool range_mapped = false;
            for (conversion_type c_range : conversion_ranges) {
                if (current_mapped_range.first >= std::get<0>(c_range) && current_mapped_range.second <= std::get<1>(c_range)) {
                    // input fully in conversion range -> one new output range
                    range_type out_range = {current_mapped_range.first + std::get<2>(c_range),
                                            current_mapped_range.second + std::get<2>(c_range)};
                    out_ranges.push_back(out_range);
                    converted_ranges++;
                    range_mapped = true;
                    break;
                } else if (current_mapped_range.first < std::get<0>(c_range) && current_mapped_range.second > std::get<1>(c_range)) {
                    // conversion fully in
                    range_type out_range = {std::get<0>(c_range) + std::get<2>(c_range), std::get<1>(c_range) + std::get<2>(c_range)};
                    out_ranges.push_back(out_range);

                    range_type untouched_front = {current_mapped_range.first, std::get<0>(c_range)};
                    untouched_ranges_next.push_back(untouched_front);
                    range_type untouched_end = {std::get<1>(c_range), current_mapped_range.second};
                    untouched_ranges_next.push_back(untouched_end);

                    converted_ranges++;
                    range_mapped = true;
                    break;
                } else if (current_mapped_range.first >= std::get<0>(c_range) && current_mapped_range.first <= std::get<1>(c_range) &&
                           current_mapped_range.second > std::get<1>(c_range)) {
                    // front overlap
                    range_type out_range = {current_mapped_range.first + std::get<2>(c_range), std::get<1>(c_range) + std::get<2>(c_range)};
                    out_ranges.push_back(out_range);

                    range_type untouched = {std::get<1>(c_range) + 1, current_mapped_range.second};
                    untouched_ranges_next.push_back(untouched);
                    converted_ranges++;
                    range_mapped = true;
                    break;
                } else if (current_mapped_range.first <= std::get<0>(c_range) && current_mapped_range.second >= std::get<0>(c_range) &&
                           current_mapped_range.second <= std::get<1>(c_range)) {
                    // end overlap
                    range_type out_range = {std::get<0>(c_range) + std::get<2>(c_range),
                                            current_mapped_range.second + std::get<2>(c_range)};
                    out_ranges.push_back(out_range);

                    range_type untouched = {current_mapped_range.first, std::get<0>(c_range) - 1};
                    untouched_ranges_next.push_back(untouched);
                    converted_ranges++;
                    range_mapped = true;
                    break;
                }
            }
            if (!range_mapped) {
                untouched_ranges_next.push_back(current_mapped_range);
            }
        }
        if (converted_ranges == 0) {
            untouched_ranges_next = untouched_ranges;
        }

        untouched_ranges = untouched_ranges_next;
        first_run = false;
    } while (converted_ranges > 0 && !untouched_ranges.empty());

    out_ranges.insert(out_ranges.end(), untouched_ranges.begin(), untouched_ranges.end());

    std::sort(out_ranges.begin(), out_ranges.end());
    return out_ranges;
}

TypeMapping::TypeMapping(std::string name) { map_name = name; }

int main() {
    std::ifstream file("input.txt");
    std::string line;

    std::vector<llint> seeds;

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
                std::vector<llint> mapping_s2s = get_numbers_from_line(line);
                s2s.add_mapping_rule(mapping_s2s);
            } else if (soil_to_fertilizer_flag) {
                std::vector<llint> mapping_s2f = get_numbers_from_line(line);
                s2f.add_mapping_rule(mapping_s2f);
            } else if (fertilizer_to_water_flag) {
                std::vector<llint> mapping_f2w = get_numbers_from_line(line);
                f2w.add_mapping_rule(mapping_f2w);
            } else if (water_to_light_flag) {
                std::vector<llint> mapping_w2l = get_numbers_from_line(line);
                w2l.add_mapping_rule(mapping_w2l);
            } else if (light_to_temperature_flag) {
                std::vector<llint> mapping_l2t = get_numbers_from_line(line);
                l2t.add_mapping_rule(mapping_l2t);
            } else if (temperature_to_humidity_flag) {
                std::vector<llint> mapping_t2h = get_numbers_from_line(line);
                t2h.add_mapping_rule(mapping_t2h);
            } else if (humidity_to_location_flag) {
                std::vector<llint> mapping_h2l = get_numbers_from_line(line);
                h2l.add_mapping_rule(mapping_h2l);
            } else {
                std::cout << "READ IN ERROR!" << std::endl;
            }
        }
    }

    range_list_type rois;
    llint start_seed_range = 0;
    llint range_length = 0;

    for (size_t i = 0; i < seeds.size(); i++) {
        if (i % 2 == 0) {
            start_seed_range = seeds[i];
        } else {
            range_length = seeds[i];
            range_type roi = {start_seed_range, start_seed_range + range_length - 1};
            rois.push_back(roi);
        }
    }

    range_list_type s = s2s.mapRanges(rois);
    range_list_type f = s2f.mapRanges(s);
    range_list_type w = f2w.mapRanges(f);
    range_list_type l = w2l.mapRanges(w);
    range_list_type t = l2t.mapRanges(l);
    range_list_type h = t2h.mapRanges(t);
    range_list_type lo = h2l.mapRanges(h);

    llint minimum = LLONG_MAX;
    for (range_type range : lo) {
        if (range.first < minimum) {
            minimum = range.first;
        }
    }

    std::cout << minimum << std::endl;

    file.close();
    return 0;
}
