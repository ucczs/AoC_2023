#include <algorithm>
#include <array>
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

enum class HandType { HighCard, Pair, TwoPairs, ThreeOfAKind, FullHouse, FourOfAKind, FiveOfAKind, Invalid };

enum class CardType { One, Joker, Two, Three, Four, Five, Six, Seven, Eight, Nine, Ten, Queen, King, Ace, Invalid };

class CardHand {
   private:
    HandType handType;
    int bid_amount;
    int rank;
    int jokerCount;
    std::array<CardType, 5> cards;
    void analyzeHand();

   public:
    CardHand(int bid_amount, std::array<CardType, 5> cards);
    void setRank(int rank) { this->rank = rank; };
    int getPoints() { return rank * bid_amount; };
    bool operator<(const CardHand& other) const {
        bool result = handType < other.handType;
        if (handType == other.handType) {
            bool highestCardFound = false;
            int count = 0;
            while (count < cards.size()) {
                if (cards[count] != other.cards[count]) {
                    result = cards[count] < other.cards[count];
                    break;
                }
                count++;
            }
        }
        return result;
    }
};

CardHand::CardHand(int bid_amount, std::array<CardType, 5> cards) {
    this->bid_amount = bid_amount;
    this->cards = cards;
    analyzeHand();
}

void CardHand::analyzeHand() {
    std::array<CardType, 5> sortedCards = cards;
    std::sort(sortedCards.begin(), sortedCards.end());

    CardType prevCard = CardType::Invalid;
    std::vector<std::pair<int, CardType>> sameCardsCounts;
    int sameCounter = 0;
    for (CardType card : sortedCards) {
        if (card == CardType::Joker) {
            jokerCount++;
            continue;
        }

        if (prevCard == card) {
            sameCounter++;
        } else if (sameCounter != 0) {
            sameCardsCounts.push_back(std::make_pair(sameCounter + 1, card));
            sameCounter = 0;
        }
        prevCard = card;
    }
    if (sameCounter != 0) {
        sameCardsCounts.push_back(std::make_pair(sameCounter + 1, prevCard));
    }

    std::sort(sameCardsCounts.begin(), sameCardsCounts.end());

    if (sameCardsCounts.size() == 0 && jokerCount == 0) {
        handType = HandType::HighCard;
    } else if (sameCardsCounts.size() == 1 || sameCardsCounts.size() == 0 && jokerCount > 0) {
        int sameCardsCount = 1;
        if (!sameCardsCounts.empty()) {
            sameCardsCount = sameCardsCounts[0].first;
        }

        if (jokerCount == 5) {
            handType = HandType::FiveOfAKind;
        } else if (sameCardsCount + jokerCount == 2) {
            handType = HandType::Pair;
        } else if (sameCardsCount + jokerCount == 3) {
            handType = HandType::ThreeOfAKind;
        } else if (sameCardsCount + jokerCount == 4) {
            handType = HandType::FourOfAKind;
        } else if (sameCardsCount + jokerCount == 5) {
            handType = HandType::FiveOfAKind;
        } else {
            std::cout << "INVALID handType detected" << std::endl;
            handType = HandType::Invalid;
        }
    } else if (sameCardsCounts.size() == 2) {
        if (sameCardsCounts[0].first == 2 && sameCardsCounts[1].first + jokerCount == 3) {
            handType = HandType::FullHouse;
        } else if (sameCardsCounts[0].first == 2 && sameCardsCounts[1].first == 2) {
            handType = HandType::TwoPairs;
        } else {
            std::cout << "INVALID handType detected" << std::endl;
            handType = HandType::Invalid;
        }
    }
}

int main() {
    std::ifstream file("input.txt");
    // std::ifstream file("test_02.txt");
    // std::ifstream file("test.txt");
    std::string line;
    std::vector<CardHand> allHands;

    while (std::getline(file, line)) {
        std::vector<std::string> input = split_string(line, ' ');
        std::string hand = input[0];
        int bid_amount = std::stoi(input[1]);

        std::array<CardType, 5> cards;
        for (std::string::size_type i = 0; i < hand.size(); ++i) {
            if (std::isdigit(hand[i])) {
                CardType card = static_cast<CardType>(hand[i] - '0');
                cards[i] = card;
            } else {
                CardType card;
                switch (hand[i]) {
                    case 'T':
                        card = CardType::Ten;
                        break;
                    case 'J':
                        card = CardType::Joker;
                        break;
                    case 'Q':
                        card = CardType::Queen;
                        break;
                    case 'K':
                        card = CardType::King;
                        break;
                    case 'A':
                        card = CardType::Ace;
                        break;
                    default:
                        card = CardType::Invalid;
                        std::cout << "READING IN ERROR" << std::endl;
                        break;
                }
                cards[i] = card;
            }
        }
        CardHand cardHand = CardHand(bid_amount, cards);
        allHands.push_back(cardHand);
    }

    std::sort(allHands.begin(), allHands.end());

    long result = 0;
    for (size_t i = 0; i < allHands.size(); i++) {
        allHands[i].setRank(i + 1);
        result += allHands[i].getPoints();
    }

    std::cout << result << std::endl;

    file.close();
    return 0;
}
