#include <fstream>
#include <iostream>
#include <string>
#include <cctype>

int main(){
    std::ifstream file("input.txt");
    std::string line;

    int sum = 0;
    while (std::getline(file, line)) {
        int first_number = -1;
        int last_number = -1;

        for (char c : line)
        {
            if (std::isdigit(c)) {
                last_number = int(c);

                if (first_number<0) {
                    first_number = int(c);
                }
            }
        }

        std::cout << first_number << last_number << std::endl;
        sum += ((first_number-48)*10 + (last_number-48));
    }

    std::cout << sum << std::endl;

    file.close();
    return 0;
}