#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

string trim(string word) {
    size_t left = word.find_first_not_of(" ");
    size_t right = word.find_last_not_of(" ");
    return word.substr(left, right - left + 1);
}

pair<string, string> split(string line, string delimeter) {
    size_t pos = line.find(delimeter);
    if (pos == string::npos) {
        return make_pair(line, "");
    }
    string left = line.substr(0, pos);
    string right = line.substr(pos + delimeter.length(), line.length() - pos - delimeter.length());
    return make_pair(trim(left), trim(right));
}

pair<vector<int>, long> parse_numbers(string line) {
    vector<int> numbers;
    string long_number;
    bool started_reading_number = false;
    int number_start;

    for (int i = 0; i < line.length(); i++) {
        if (isdigit(line[i])) {
            long_number += line[i];
        }
        if (!started_reading_number && isdigit(line[i])) {
            started_reading_number = true;
            number_start = i;
        }
        if (started_reading_number && !isdigit(line[i])) {
            started_reading_number = false;
            numbers.push_back(stoi(line.substr(number_start, i - number_start)));
        }
    }
    if (started_reading_number) {
        numbers.push_back(stoi(line.substr(number_start, line.length() - number_start)));
    }

    return make_pair(numbers, stol(long_number));
}

pair<pair<vector<int>, vector<int>>, pair<long, long>> read_input(ifstream &input) {
    string line;
    getline(input, line); // skip top line
    pair<vector<int>, long> time_line = parse_numbers(line);
    getline(input, line); // skip top line
    pair<vector<int>, long> distance_line = parse_numbers(line);
    return make_pair(
        make_pair(time_line.first, distance_line.first), 
        make_pair(time_line.second, distance_line.second)
    );
}

long ways_of_winning(long time, long distance) {
    long first_time = 0;
    long last_time = 0;
    for (long i = 0; i < time; i++) {
        if ((time - i) * i > distance) {
            first_time = i;
            last_time = time-i;
            break;
        }
    }
    return last_time - first_time + 1;
}

int main() {
    ifstream input("input.txt");
    // ifstream input("short_input.txt");

    pair<pair<vector<int>, vector<int>>, pair<long, long>> pair_input = read_input(input);
    vector<int> time = pair_input.first.first;
    vector<int> distance = pair_input.first.second;
    int mult = 1;
    for (int i = 0; i < time.size(); i++) {
        mult *= ways_of_winning(time[i], distance[i]);
    }
    cout << mult << endl;

    long time2 = pair_input.second.first;
    long distance2 = pair_input.second.second;
    cout << ways_of_winning(time2, distance2) << endl;
}