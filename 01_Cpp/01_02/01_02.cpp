#include <fstream>
#include <iostream>
#include <string>
#include <cctype>
#include <array>
#include <tuple>
#include <vector>
#include <algorithm>

std::array<std::tuple<std::string, std::string>, 9> number_letters = {
    std::make_tuple("one", "1"),
    std::make_tuple("two", "2"),
    std::make_tuple("three", "3"),
    std::make_tuple("four", "4"),
    std::make_tuple("five", "5"),
    std::make_tuple("six", "6"),
    std::make_tuple("seven", "7"),
    std::make_tuple("eight", "8"),
    std::make_tuple("nine", "9")
};


int main(){
    std::ifstream file("input.txt");
    // std::ifstream file("test.txt");
    std::string line;

    int sum = 0;
    while (std::getline(file, line)) {
        std::cout << line << std::endl;

        std::vector<std::tuple<size_t, std::string, std::string>> found;
        for(const std::tuple<std::string, std::string>& combination : number_letters) {
            size_t start_pos = 0;

            while (start_pos != std::string::npos)
            {
                start_pos = line.find(std::get<0>(combination), start_pos);
                if (start_pos != std::string::npos) {
                    found.push_back(std::make_tuple(start_pos, std::get<0>(combination), std::get<1>(combination)));
                    start_pos++;
                }
            }
        }
        std::sort(found.begin(), found.end(), [](const auto& a, const auto& b){
            return std::get<0>(a) < std::get<0>(b);
        });

        if (!found.empty())
        {
            line.replace(std::get<0>(found.front()), std::get<1>(found.front()).length(), std::get<2>(found.front())+std::get<1>(found.front()));
            line.replace(std::get<0>(found.back())+1, std::get<1>(found.back()).length(), std::get<1>(found.back())+std::get<2>(found.back()));
        }

        std::cout << line << std::endl;

        int first_number = -1;
        int last_number = -1;

        for (char c : line)
        {
            if (std::isdigit(c)) {
                last_number = int(c)-'0';

                if (first_number<0) {
                    first_number = int(c)-'0';
                }
            }
        }

        std::cout << first_number << last_number << std::endl;
        sum += ((first_number)*10 + (last_number));
        std::cout << sum << std::endl;
    }

    std::cout << sum << std::endl;

    file.close();
    return 0;
}