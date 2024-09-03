#include <fstream>
#include <iostream>
#include <string>


int main(){
    std::ifstream file("input.txt");
    std::string line;

    while (std::getline(file, line)) {
        std::cout << line << std::endl;
    }



    file.close();
    return 0;
}

