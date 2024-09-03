#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <algorithm>

const int invalid = 0;

auto split_string(std::string full_string, char seperator) -> std::vector<std::string> 
{
    std::stringstream full_string_ss(full_string);
    std::vector<std::string> splitted_string;
    std::string split;

    while(std::getline(full_string_ss, split, seperator)){
        if (split[0] == ' '){
            split = split.substr(1, split.length()-1);
        }
        splitted_string.push_back(split);
    }

    return splitted_string;
}

int main(){
    std::ifstream file("input.txt");
    // std::ifstream file("test.txt");
    std::string line;
    int sum = 0;

    while (std::getline(file, line)) {
        int min_red = invalid;
        int min_green = invalid;
        int min_blue = invalid;

        std::string games = line.substr(line.find(":")+2, line.length()-(line.find(":")+2));
        std::cout << games << std::endl;

        std::vector<std::string> split_games = split_string(games, ';');

        for(std::string gm : split_games){
            std::vector<std::string> colors_seperate = split_string(gm, ',');
            for(std::string color : colors_seperate){
                std::vector<std::string> number_color_seperated = split_string(color, ' ');

                int number = std::stoi(number_color_seperated[0]);
                std::string current_color = number_color_seperated[1];

                if (current_color == "red"){
                    min_red = std::max(min_red, number);
                } else if (current_color == "green"){
                    min_green = std::max(min_green, number);
                } else if (current_color == "blue"){
                    min_blue = std::max(min_blue, number);
                } else{
                    std::cout << "Something is wrong!" << std::endl;
                    return 0;
                }
            }
        }

        std::cout << "Red: " << min_red << ", Green: " << min_green << ", Blue: " << min_blue << std::endl;
        int power = min_red * min_green * min_blue;

        sum += power;
    }

    std::cout << sum << std::endl;

    file.close();
    return 0;
}