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
    int row;
    int col;
};

Gear find_first_gear(vector<string> schematic, int row, int col) {
    Gear gear = {-1, -1};
    for (int i = col; i < schematic[row].length(); i++) {
        if (schematic[row][i] == '*') {
            gear = {row, i};
            return gear;
        }
    }
    return gear;
}

Number find_digit_smart(vector<string> schematic, int row, int col) {
    int start_position = col;
    int end_position = col;
    Number number = {0, 0, 0, 0};

    if (!isdigit(schematic[row][col])) {
        return number;
    }

    while (start_position >= 0 && isdigit(schematic[row][start_position])) {
        start_position--;
    }
    while (end_position < schematic[row].length() && isdigit(schematic[row][end_position])) {
        end_position++;
    }

    number = {
        stoi(schematic[row].substr(start_position + 1, end_position - start_position - 1)), 
        row, 
        start_position + 1, 
        end_position - start_position - 1
    };
    
    return number;
}

int find_gear_ratio(vector<string> schematic, Gear gear) {
    int num_count = 0;
    int gear_ratio = 1;
    int left = max(gear.col - 1, 0);
    int right = min(gear.col + 1, (int)schematic[0].length()-1);
    int top = max(gear.row - 1, 0);
    int bottom = min(gear.row + 1, (int)schematic.size() - 1);

    // left
    if (isdigit(schematic[gear.row][left])) {
        num_count++;
        gear_ratio *= find_digit_smart(schematic, gear.row, left).value;
    }
    // right
    if (isdigit(schematic[gear.row][right])) {
        num_count++;
        gear_ratio *= find_digit_smart(schematic, gear.row, right).value;
    }
    // top
    if (top != gear.row) {
        // If there is a gear in the top center, there's only one number.
        if (isdigit(schematic[top][gear.col])) {
            num_count++;
            gear_ratio *= find_digit_smart(schematic, top, gear.col).value;
            // cout << "found in top center" << find_digit_smart(schematic, top, gear.col).value << endl;
        }
        else {
            // If there is no gear in the top center, there can be two numbers.
            if (isdigit(schematic[top][left])) {
                num_count++;
                Number number = find_digit_smart(schematic, top, left);
                gear_ratio *= number.value;
                // cout << "Found in top left " << number.value << endl;
                // if there is a gear in the top left, there could be another number in the right
                if (isdigit(schematic[top][right]) && number.start != find_digit_smart(schematic, top, right).start) {
                    num_count++;
                    gear_ratio *= find_digit_smart(schematic, top, right).value;
                    // cout << "Found in top right after top left" << find_digit_smart(schematic, top, right).value << endl;
                }
            }
            else if (isdigit(schematic[top][right])) {
                num_count++;
                gear_ratio *= find_digit_smart(schematic, top, right).value;
                // cout << find_digit_smart(schematic, top, gear.col).value << endl;
                // cout << "Found in top right" << find_digit_smart(schematic, top, right).value << endl;
            }
        }
    }

    // bottom
    if (bottom != gear.row) {
        // If there is a gear in the bottom center, there's only one number.
        if (isdigit(schematic[bottom][gear.col])) {
            num_count++;
            gear_ratio *= find_digit_smart(schematic, bottom, gear.col).value;
            // cout << "found in bottom center" << find_digit_smart(schematic, bottom, gear.col).value << endl;
        }
        else {
            // If there is no gear in the bottom center, there can be two numbers.
            if (isdigit(schematic[bottom][left])) {
                num_count++;
                Number number = find_digit_smart(schematic, bottom, left);
                gear_ratio *= number.value;
                // cout << "Found in bottom left " << number.value << endl;
                // if there is a gear in the bottom left, there could be another number in the right
                if (isdigit(schematic[bottom][right]) && number.start != find_digit_smart(schematic, bottom, right).start) {
                    num_count++;
                    gear_ratio *= find_digit_smart(schematic, bottom, right).value;
                    // cout << "Found in bottom right after bottom left" << find_digit_smart(schematic, bottom, right).value << endl;
                }
            }
            else if (isdigit(schematic[bottom][right])) {
                num_count++;
                gear_ratio *= find_digit_smart(schematic, bottom, right).value;
                // cout << "Found in top right" << find_digit_smart(schematic, bottom, right).value << endl;
            }
        }
    }
    if (num_count == 2) {
        return gear_ratio;
    }
    return 0;
}

int main() {
    // vector<string> schematic = parse_schematic("short_input.txt");
    vector<string> schematic = parse_schematic("input.txt");
    Number number;
    Gear gear;
    int part_sum = 0;
    int gear_ratio_sum = 0;

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

    for (int row = 0; row < schematic.size(); row++) {
        int col = 0;
        while((gear = find_first_gear(schematic, row, col)).row != -1) {
            int gear_ratio = find_gear_ratio(schematic, gear);
            // cout << "Gear found at: " << gear.row << ", " << gear.col << " with ratio: " << gear_ratio << endl;
            gear_ratio_sum += gear_ratio;
            col = gear.col + 1;
        }
    }
    cout << "Gear ratio sum: " << gear_ratio_sum << endl;
    return 0;
}