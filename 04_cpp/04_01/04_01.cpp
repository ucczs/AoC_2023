#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <algorithm>
#include <regex>
#include <cmath>


class ScratchCard{
private:
    std::vector<int> winning_numbers;
    std::vector<int> card_numbers;
public:
    ScratchCard(std::vector<int> winning_numbers, std::vector<int> card_numbers);
    int getScore();
};

ScratchCard::ScratchCard(std::vector<int> winning, std::vector<int> card){
    winning_numbers = winning;
    card_numbers = card;
}

int ScratchCard::getScore(){
    int winning_numbers_count = 0;
    for(int no : card_numbers){
        if(std::find(winning_numbers.begin(), winning_numbers.end(), no) != winning_numbers.end()){
            winning_numbers_count++;
        }
    }
    int score = 0;
    if(winning_numbers_count > 0){
        score = pow(2, winning_numbers_count-1);
    }

    return score;
}

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

    std::vector<ScratchCard*> card_collection;

    while (std::getline(file, line)) {
        line = std::regex_replace(line, std::regex("  "), " ");
        std::vector<std::string> win_and_my = split_string(split_string(line, ':')[1], '|');
        std::cout << win_and_my[0] << " || " << win_and_my[1] << std::endl;

        std::vector<std::string> win_no_str = split_string(win_and_my[0], ' ');
        std::vector<std::string> card_no_str = split_string(win_and_my[1], ' ');

        std::vector<int> win_no;
        std::vector<int> card_no;

        std::transform(win_no_str.begin(), win_no_str.end(), std::back_inserter(win_no),
               [](const std::string& str) { return std::stoi(str); });

        std::transform(card_no_str.begin(), card_no_str.end(), std::back_inserter(card_no),
               [](const std::string& str) { return std::stoi(str); });

        ScratchCard* newCard = new ScratchCard(win_no, card_no);
        card_collection.push_back(newCard);
    }

    int result = 0;
    for(ScratchCard* card : card_collection){
        result += card->getScore();
    }


    std::cout << result << std::endl;

    file.close();
    return 0;
}

