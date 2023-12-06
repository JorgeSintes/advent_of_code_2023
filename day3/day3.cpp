#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;

vector<string> parse_schematic(string filename) {
    ifstream input(filename);
    string line;
    vector<string> schematic;

    while (getline(input, line)) {
        schematic.push_back(line);
    }

    return schematic;
}

struct Number {
    int value;
    int row;
    int start;
    int length;
};

Number find_first_number(vector<string> schematic, int row, int col) {
    bool started_reading_number = false, finished_reading_number = false;
    int number_start = 0;
    int number_length = 0;
    struct Number number = {0, 0, 0, 0};

    int i = col;

    while (!finished_reading_number && i < schematic[row].length()) {
        if (!started_reading_number && isdigit(schematic[row][i]) != 0) {
            started_reading_number = true;
            number_start = i;
        } 
        else if (started_reading_number && isdigit(schematic[row][i]) == 0) {
            finished_reading_number = true;
            number_length = i - number_start;
        }
        i++;
    }
    if (!finished_reading_number && started_reading_number) {
        finished_reading_number = true;
        number_length = i - number_start;
    }

    if (number_length != 0) {
        number = {stoi(schematic[row].substr(number_start, number_length)), row, number_start, number_length};
    }
    return number;
}

bool has_simbol_around(vector<string> schematic, Number number) {
    int left = max(number.start - 1, 0);
    int right = min(number.start + number.length, (int)schematic[0].length()-1);
    int top = max(number.row - 1, 0);
    int bottom = min(number.row + 1, (int)schematic.size() - 1);
    // left
    if (!isdigit(schematic[number.row][left]) && schematic[number.row][left] != '.') {
        // cout << number.value << " left " << schematic[row_number][left] << endl;
        return true;
    }
    // right
    if (!isdigit(schematic[number.row][right]) && schematic[number.row][right] != '.') {
        // cout << number.value << " right " << schematic[row_number][right] << endl;
        return true;
    }
    // top 
    for (int i = left; i <= right; i++) {
        if (!isdigit(schematic[top][i]) && schematic[top][i] != '.') {
            // cout << number.value << " top " << schematic[top][i] << endl;
            return true;
        }
    }
    // bottom
    for (int i = left; i <= right; i++) {
        if (!isdigit(schematic[bottom][i]) && schematic[bottom][i] != '.') {
            // cout << number.value << " bottom " << schematic[bottom][i] << endl;
            return true;
        }
    }
    // cout << number.value << " false" << endl;
    return false;
}

struct Gear {
    int start_index;
    int length;
};

Gear find_first_gear(string line, int start_index) {
    for (int i = start_index; i < line.length(); i++) {
        if (line[i] == '*') {
            return {i, 1};
        }
    }
    return {0, 0};
}

// PartNumber find_digit_smart(string line, int index) {
//     int i = 0;
//     PartNumber number;
//     while ((number = find_first_number(line, i)).length != 0 && i < line.length()) {
//         // cout << "number: " << number.value << endl;
//         if (number.start_index <= index && index < number.start_index + number.length) {
//             return number;
//         }
//         i = number.start_index + number.length;
//     }
//     return 0;
// }

// int find_gear_ratio(Gear gear, int row_number, vector<string> schematic) {
//     int left = max(gear.start_index - 1, 0);
//     int right = min(gear.start_index + gear.length, (int)schematic[0].length()-1);
//     int top = max(row_number - 1, 0);
//     int bottom = min(row_number + 1, (int)schematic.size() - 1);
//     int gear_count = 0;
//     int gear_ratio = 1;
//     // left
//     if (isdigit(schematic[row_number][left])) {
//         gear_count++;
//         gear_ratio *= find_digit_smart(schematic[row_number], left).value;
//         cout << "left " << find_digit_smart(schematic[row_number], left) << endl;
//     }
//     // right
//     if (isdigit(schematic[row_number][right])) {
//         gear_count++;
//         gear_ratio *= find_digit_smart(schematic[row_number], right).value;
//         cout << "right " << find_digit_smart(schematic[row_number], right) << endl;
//     }
//     //top
//     if (top != row_number) {
//         if (isdigit(schematic[top][left])) {
//             PartNumber left_digit = find_digit_smart(schematic[top], left);
//             gear_count++;
//             gear_ratio *= left_digit.value;
//         }
//     }

//     // bottom 
//     if (bottom != row_number) {
//         for (int i = left; i <= right; i++) {
//             if (isdigit(schematic[bottom][i])) {
//                 gear_count++;
//                 gear_ratio *= find_digit_smart(schematic[bottom], i);
//                 cout << "bottom" << find_digit_smart(schematic[bottom], i) << endl;
//             }
//         }
//     }

//     if (gear_count == 2) {
//         return gear_ratio;
//     }

//     return 0;

// }

int main() {
    // vector<string> schematic = parse_schematic("short_input.txt");
    vector<string> schematic = parse_schematic("input.txt");
    Number number;
    int part_sum = 0;

    for (int row = 0; row < schematic.size(); row++) {
        // cout << "\nline " << row_number << endl;
        int col = 0;
        while((number = find_first_number(schematic, row, col)).length != 0) {
            if (has_simbol_around(schematic, number)) {
                part_sum += number.value;
            }
            col = number.start + number.length;
            // cout << "Running number: " << part_sum << endl;
        }
    }

    cout << "Part sum: " << part_sum << endl;

    // Gear gear;
    // int gear_sum = 0;
    // vector<int> gear_numbers;

    // for (int row_number = 0; row_number < schematic.size(); row_number++) {
    //     cout << "\nline " << row_number << endl;
    //     int j = 0;
    //     while((gear = find_first_gear(schematic[row_number], j)).length != 0) {
    //         cout << "Gear found at " <<  gear.start_index << endl;
    //         gear_sum += find_gear_ratio(gear, row_number, schematic);
    //         j = gear.start_index + gear.length;
    //         cout << "Running number: " << gear_sum << endl;
    //     }
    // }

    return 0;
}