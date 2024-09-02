#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <array>
#include <sstream>

enum field_type{
    EMPTY,
    NUMBER,
    SYMBOL
};

class field
{
private:
    field_type type;
    char value;
    bool part_number_flag;
    int touching_numbers_count;
    int gear_ratio;
    std::array<int,2> gear_coordinates;
public:
    field();
    field(field_type type, char value);
    ~field();
    void setValues(field_type type, char value);
    void printField();
    int get_value();
    bool is_symbol();
    bool is_empty();
    bool is_partnumber();
    bool is_number();
    void set_part_number_flag();
    void increase_touching_count();
    bool is_gear();
    void set_gear_ratio(int gear_ratio);
    void set_gear_coordinates(int x_pos, int y_pos);
    std::array<int,2> get_gear_coordinates();
};

std::array<int,2> field::get_gear_coordinates(){
    return gear_coordinates;
}

void field::set_gear_coordinates(int x_pos, int y_pos){
    gear_coordinates = {x_pos, y_pos};
}

void field::set_gear_ratio(int gear_ratio){
    gear_ratio = gear_ratio;
}

bool field::is_gear(){
    return touching_numbers_count == 2;
}

int field::get_value(){
    if (type == field_type::NUMBER)
    {
        return int(value)-'0';
    }
    return 0;
}

void field::increase_touching_count(){
    touching_numbers_count++;
}

bool field::is_empty(){
    return type == field_type::EMPTY;
}

bool field::is_number(){
    return type == field_type::NUMBER;
}

bool field::is_partnumber(){
    return part_number_flag;
}

void field::set_part_number_flag(){
    part_number_flag = true;
}

field::field(field_type type, char value){
    this->setValues(type, value);
    part_number_flag = false;
    touching_numbers_count = 0;
}

field::field(){
    part_number_flag = false;
    touching_numbers_count = 0;
}

void field::setValues(field_type type, char value){
    this->type = type;
    this->value = value;
}

void field::printField(){
    std::cout << value;
}

bool field::is_symbol(){
    return type == field_type::SYMBOL;
}

void print_map(std::vector<std::vector<field*>> map_2d){
    for (std::vector<field*> row : map_2d){
        for (field* field : row){
            if (field->is_partnumber()){
                std::cout << "X";
            }
            else{
                field->printField();
            }
        }
        std::cout << std::endl;
    }
}

void check_surrounding(int x_pos, int y_pos, std::vector<std::vector<field*>> map_2d){
    int move_list[3] = {-1, 0, 1};
    for (int move_x : move_list){
        for (int move_y : move_list){
            if (move_x == 0 && move_y == 0){
                continue;
            }
            if (y_pos+move_y < map_2d.size() && y_pos+move_y >= 0 && x_pos+move_x >= 0 && x_pos+move_x < map_2d[0].size())
            {
                if(map_2d[y_pos+move_y][x_pos+move_x]->is_gear() && map_2d[y_pos][x_pos]->is_number()){
                    map_2d[y_pos][x_pos]->set_part_number_flag();
                    map_2d[y_pos][x_pos]->set_gear_coordinates(x_pos+move_x, y_pos+move_y);
                }
            }
        }
    }
}

int getFullPartnumber(int y_pos, int x_pos, std::vector<std::vector<field*>> map_2d){
    int val = map_2d[y_pos][x_pos]->get_value();

    if (x_pos-1 >= 0 && map_2d[y_pos][x_pos-1]->is_number()){
        val += map_2d[y_pos][x_pos-1]->get_value() * 10;
        if (x_pos-2 >= 0 && map_2d[y_pos][x_pos-2]->is_number()){
            val += map_2d[y_pos][x_pos-2]->get_value() * 100;
        }
    }
    return val;
}

void complete_partnumber_flag(std::vector<std::vector<field*>> map_2d){
    for (int y_pos = 0; y_pos < map_2d.size(); y_pos++){
        for (int x_pos = 0; x_pos < map_2d[0].size(); x_pos++){
            std::array<int, 2> null_array = {0,0};

            // checking to the right
            if (map_2d[y_pos][x_pos]->is_partnumber() && (x_pos+1) < map_2d[0].size() && map_2d[y_pos][x_pos+1]->is_number()){
                if (map_2d[y_pos][x_pos]->get_gear_coordinates() != null_array){
                    std::array<int, 2> coordinates = map_2d[y_pos][x_pos]->get_gear_coordinates();
                    map_2d[y_pos][x_pos+1]->set_gear_coordinates(coordinates[0], coordinates[1]);
                }
                
                map_2d[y_pos][x_pos+1]->set_part_number_flag();

                if ((x_pos+2) < map_2d[0].size() && map_2d[y_pos][x_pos+2]->is_number()){
                    if (map_2d[y_pos][x_pos]->get_gear_coordinates() != null_array){
                        std::array<int, 2> coordinates = map_2d[y_pos][x_pos]->get_gear_coordinates();
                        map_2d[y_pos][x_pos+2]->set_gear_coordinates(coordinates[0], coordinates[1]);
                    }
                    map_2d[y_pos][x_pos+2]->set_part_number_flag();
                }
            }

            // checking to the left
            if (map_2d[y_pos][x_pos]->is_partnumber() && (x_pos-1) >= 0 && map_2d[y_pos][x_pos-1]->is_number()){
                if (map_2d[y_pos][x_pos]->get_gear_coordinates() != null_array){
                    std::array<int, 2> coordinates = map_2d[y_pos][x_pos]->get_gear_coordinates();
                    map_2d[y_pos][x_pos-1]->set_gear_coordinates(coordinates[0], coordinates[1]);
                }

                map_2d[y_pos][x_pos-1]->set_part_number_flag();
                if ((x_pos-2) >= 0){
                    if(map_2d[y_pos][x_pos-2]->is_number()){
                        if (map_2d[y_pos][x_pos]->get_gear_coordinates() != null_array){
                            std::array<int, 2> coordinates = map_2d[y_pos][x_pos]->get_gear_coordinates();
                            map_2d[y_pos][x_pos-2]->set_gear_coordinates(coordinates[0], coordinates[1]);
                        }
                        map_2d[y_pos][x_pos-2]->set_part_number_flag();
                    }
                }
            }
        }
    }
}

int calculate_partnumber_sum(std::vector<std::vector<field*>> map_2d){
    int sum = 0;
    std::map<std::string, std::array<int, 2>> gear_numbers_map;
    for (int y_pos = 0; y_pos < map_2d.size(); y_pos++){
        for (int x_pos = 0; x_pos < map_2d[0].size(); x_pos++){
            int size = map_2d[0].size();
            if (map_2d[y_pos][x_pos]->is_partnumber() && ((x_pos+1) >= map_2d[0].size() || map_2d[y_pos][x_pos+1]->is_empty() || map_2d[y_pos][x_pos+1]->is_symbol())){
                int value = getFullPartnumber(y_pos, x_pos, map_2d);

                std::array<int,2> gear_coordinates = map_2d[y_pos][x_pos]->get_gear_coordinates();
                std::stringstream key_ss;
                key_ss << gear_coordinates[0] << "," << gear_coordinates[1];
                const std::string key = key_ss.str();
                if (gear_numbers_map.find(key) == gear_numbers_map.end()){
                    std::array<int, 2> gear_values = {-1, value};
                    gear_numbers_map[key] = gear_values;
                } else{
                    std::array<int, 2> gear_values = gear_numbers_map[key];
                    if (gear_values[0] == -1){
                        gear_values[0] = value;
                        int ratio = value * gear_values[1];
                        map_2d[y_pos][x_pos]->set_gear_ratio(ratio);
                        sum += ratio;
                    }
                    else{
                        std::cout << "ERROR!!!!!!!!" << std::endl;
                        return -1;
                    }

                }

            }
        }
    }
    return sum;
}

void count_touching_numbers(std::vector<std::vector<field*>> map_2d){
    for (int y_pos = 0; y_pos < map_2d.size(); y_pos++){
        for (int x_pos = 0; x_pos < map_2d[0].size(); x_pos++){
            if(map_2d[y_pos][x_pos]->is_symbol()){
                if(x_pos-1 >= 0 && map_2d[y_pos][x_pos-1]->is_number()){
                    map_2d[y_pos][x_pos]->increase_touching_count();
                }
                if(x_pos+1 < map_2d[0].size() && map_2d[y_pos][x_pos+1]->is_number()){
                    map_2d[y_pos][x_pos]->increase_touching_count();
                }

                if(x_pos-1 >= 0 && y_pos-1 >= 0 && map_2d[y_pos-1][x_pos-1]->is_number()){
                    map_2d[y_pos][x_pos]->increase_touching_count();
                    if(x_pos+1 < map_2d[0].size() && map_2d[y_pos-1][x_pos+1]->is_number()){
                        if(!map_2d[y_pos-1][x_pos]->is_number()){
                            map_2d[y_pos][x_pos]->increase_touching_count();
                        }

                    }
                }
                else if(x_pos+1 < map_2d[0].size() && y_pos-1 >= 0 && map_2d[y_pos-1][x_pos+1]->is_number()){
                    map_2d[y_pos][x_pos]->increase_touching_count();
                }
                else if(y_pos-1 >= 0 && map_2d[y_pos-1][x_pos]->is_number()){
                    map_2d[y_pos][x_pos]->increase_touching_count();
                }

                if(x_pos-1 >= 0 && y_pos+1 < map_2d.size() && map_2d[y_pos+1][x_pos-1]->is_number()){
                    map_2d[y_pos][x_pos]->increase_touching_count();
                    if(x_pos+1 < map_2d[0].size() && map_2d[y_pos+1][x_pos+1]->is_number()){
                        if(!map_2d[y_pos+1][x_pos]->is_number()){
                            map_2d[y_pos][x_pos]->increase_touching_count();
                        }
                    }
                }
                else if(x_pos+1 < map_2d[0].size() && y_pos+1 < map_2d.size() && map_2d[y_pos+1][x_pos+1]->is_number()){
                    map_2d[y_pos][x_pos]->increase_touching_count();
                }
                else if(x_pos+1 < map_2d[0].size() && map_2d[y_pos+1][x_pos]->is_number()){
                    map_2d[y_pos][x_pos]->increase_touching_count();
                }
            }
        }
    }
}

std::vector<std::vector<field*>> map_2d;

int main(){
    // std::ifstream file("test.txt");
    // std::ifstream file("test_2.txt");
    std::ifstream file("input.txt");
    std::string line;
    int y_pos = 0;

    while (std::getline(file, line)) {
        // std::cout << line << std::endl;

        std::vector<field*> line_objects;

        int x_pos = 0;

        for (char c : line){
            if (std::isdigit(c)){
                field* number_field = new(field);
                number_field->setValues(field_type::NUMBER, c);
                line_objects.push_back(number_field);
            }
            else if (c == '*'){
                field* symbol_field = new(field);
                symbol_field->setValues(field_type::SYMBOL, c);
                line_objects.push_back(symbol_field);
            }
            else{
                field* empty_field = new(field);
                empty_field->setValues(field_type::EMPTY, '.');
                line_objects.push_back(empty_field);
            }
            x_pos++;
        }
        map_2d.push_back(line_objects);
        y_pos++;
    }

    count_touching_numbers(map_2d);

    for (int y_pos = 0; y_pos < map_2d.size(); y_pos++){
        for (int x_pos = 0; x_pos < map_2d[0].size(); x_pos++){
            check_surrounding(x_pos, y_pos, map_2d);
        }
    }

    complete_partnumber_flag(map_2d);

    int result = calculate_partnumber_sum(map_2d);

    print_map(map_2d);

    std::cout << result << std::endl;

    file.close();
    return 0;
}