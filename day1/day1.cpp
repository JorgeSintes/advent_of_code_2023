#include <fstream>
#include <iostream>
#include <string>
#include <vector>

int parse_line(std::string line) {
    std::string result;
    int first_number, second_number;
    first_number = -1;
    second_number = -1;
    for (int i = 0; i < line.length(); i++) {
        if (std::isdigit(line[i]) != 0) {
            if (first_number == -1) {
                first_number = line[i] - '0';
                second_number = line[i] - '0';
            } else {
                second_number = line[i] - '0';
            }
        }
    }
    return first_number*10 + second_number;
}

std::string substitute_digits(std::string line) {
    std::string new_line = line;
    std::vector<std::pair<std::string, int>> digits_vector;
    std::size_t idx_found;

    digits_vector.push_back(std::make_pair("oneight", 18));
    digits_vector.push_back(std::make_pair("twone", 21));
    digits_vector.push_back(std::make_pair("threeight", 38));
    digits_vector.push_back(std::make_pair("fiveight", 58));
    digits_vector.push_back(std::make_pair("sevenine", 79));
    digits_vector.push_back(std::make_pair("eightwo", 82));
    digits_vector.push_back(std::make_pair("eighthree", 83));
    digits_vector.push_back(std::make_pair("nineight", 98));
    digits_vector.push_back(std::make_pair("one", 1));
    digits_vector.push_back(std::make_pair("two", 2));
    digits_vector.push_back(std::make_pair("three", 3));
    digits_vector.push_back(std::make_pair("four", 4));
    digits_vector.push_back(std::make_pair("five", 5));
    digits_vector.push_back(std::make_pair("six", 6));
    digits_vector.push_back(std::make_pair("seven", 7));
    digits_vector.push_back(std::make_pair("eight", 8));
    digits_vector.push_back(std::make_pair("nine", 9));

    for (auto it = digits_vector.begin(); it != digits_vector.end(); it++) {
        idx_found = new_line.find(it->first);
        while (idx_found != std::string::npos) {
            // new_line.replace(idx_found, (it->first).size(), std::to_string(it->second));
            new_line.insert(idx_found, std::to_string(it->second));
            idx_found = new_line.find(it->first, idx_found + it->first.size() );
        }
    }

    return new_line;
}

int main(){
    std::ifstream input("input.txt");
    std::string line;
    int first_number, second_number, sum;
    sum = 0;

    if (input.is_open()) {
        while (getline(input, line)) {
            // std::cout << line << std::endl;
            line = substitute_digits(line);
            // std::cout << line << std::endl;
            // std::cout << parse_line(line) << std::endl;
            sum += parse_line(line);
        }
        input.close();
    } else {
        std::cout << "Unable to open file" << std::endl;
    }

    std::cout << "Sum: " << sum << std::endl;

    return 0;
}