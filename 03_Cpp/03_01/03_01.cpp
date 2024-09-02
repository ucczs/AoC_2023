#include <fstream>
#include <iostream>
#include <string>
#include <vector>

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
    int x_pos;
    int y_pos;
    bool part_number_flag;
public:
    field();
    field(field_type type, char value, int x_pos, int y_pos);
    ~field();
    void setValues(field_type type, char value, int x_pos, int y_pos);
    void printField();
    int get_value();
    bool is_symbol();
    bool is_empty();
    bool is_partnumber();
    bool is_number();
    void set_part_number_flag();
};

int field::get_value(){
    if (type == field_type::NUMBER)
    {
        return int(value)-'0';
    }
    return 0;
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

field::field(field_type type, char value, int x_pos, int y_pos){
    this->setValues(type, value, x_pos, y_pos);
    part_number_flag = false;
}

field::field(){
    part_number_flag = false;
}

void field::setValues(field_type type, char value, int x_pos, int y_pos){
    this->type = type;
    this->value = value;
    this->x_pos = x_pos;
    this->y_pos = y_pos;
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
                if(map_2d[y_pos+move_y][x_pos+move_x]->is_symbol() && map_2d[y_pos][x_pos]->is_number()){
                    map_2d[y_pos][x_pos]->set_part_number_flag();
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

int calculate_partnumber_sum(std::vector<std::vector<field*>> map_2d){
    int sum = 0;
    for (size_t y_pos = 0; y_pos < map_2d.size(); y_pos++){
        for (size_t x_pos = 0; x_pos < map_2d[0].size(); x_pos++){
            if (map_2d[y_pos][x_pos]->is_partnumber() && (x_pos+1) < map_2d[0].size() && map_2d[y_pos][x_pos+1]->is_number()){
                map_2d[y_pos][x_pos+1]->set_part_number_flag();
                if ((x_pos+2) < map_2d[0].size() && map_2d[y_pos][x_pos+2]->is_number()){
                    map_2d[y_pos][x_pos+2]->set_part_number_flag();
                }
            }
            int size = map_2d[0].size();
            if (map_2d[y_pos][x_pos]->is_partnumber() && ((x_pos+1) >= map_2d[0].size() || map_2d[y_pos][x_pos+1]->is_empty() || map_2d[y_pos][x_pos+1]->is_symbol())){
                sum += getFullPartnumber(y_pos, x_pos, map_2d);
            }
        }
    }
    return sum;
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
            if (c == '.'){
                field* empty_field = new(field);
                empty_field->setValues(field_type::EMPTY, c, x_pos, y_pos);
                line_objects.push_back(empty_field);
            } else if (std::isdigit(c)){
                field* number_field = new(field);
                number_field->setValues(field_type::NUMBER, c, x_pos, y_pos);
                line_objects.push_back(number_field);
            }
            else{
                field* symbol_field = new(field);
                symbol_field->setValues(field_type::SYMBOL, c, x_pos, y_pos);
                line_objects.push_back(symbol_field);
            }
            x_pos++;
        }
        map_2d.push_back(line_objects);
        y_pos++;
    }

    for (size_t y_pos = 0; y_pos < map_2d.size(); y_pos++){
        for (size_t x_pos = 0; x_pos < map_2d[0].size(); x_pos++){
            check_surrounding(x_pos, y_pos, map_2d);
        }
    }

    int result = calculate_partnumber_sum(map_2d);

    print_map(map_2d);

    std::cout << result << std::endl;

    file.close();
    return 0;
}