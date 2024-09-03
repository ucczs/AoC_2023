#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
#include <vector>

const int red_max = 12;
const int green_max = 13;
const int blue_max = 14;


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
    int id = 0;
    int sum = 0;

    while (std::getline(file, line)) {
        id++;

        bool game_valid = true;

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
                    game_valid = game_valid && (number <= red_max);
                } else if (current_color == "green"){
                    game_valid = game_valid && (number <= green_max);
                } else if (current_color == "blue"){
                    game_valid = game_valid && (number <= blue_max);
                } else{
                    std::cout << "Something is wrong!" << std::endl;
                    return 0;
                }

                // std::cout << "Color: " << current_color << std::endl;
                // std::cout << "Number: " << number << std::endl;
            }
        }


        if (game_valid){
            sum += id;
            std::cout << "Game " << id << " is valid." << std::endl;
        }
        
    }

    std::cout << sum << std::endl;

    file.close();
    return 0;
}