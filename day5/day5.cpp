#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
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

vector<long> parse_seeds(string line) {
    vector<long> seeds;
    bool started_reading_number = false;
    int number_start;

    for (int i = 0; i < line.length(); i++) {
        if (!started_reading_number && isdigit(line[i])) {
            started_reading_number = true;
            number_start = i;
        }
        if (started_reading_number && !isdigit(line[i])) {
            started_reading_number = false;
            seeds.push_back(stol(line.substr(number_start, i - number_start)));
        }
    }
    if (started_reading_number) {
        seeds.push_back(stol(line.substr(number_start, line.length() - number_start)));
    }

    return seeds;
}

vector<long> parse_map_x_to_x(ifstream &input, vector<long> numbers) {
    string line;
    getline(input, line); // skip top line
    vector<bool> changed;
    for (int i = 0; i < numbers.size(); i++) {
        changed.push_back(false);
    }

    while (getline(input, line)) {
        if (line.length() == 0) {
            break;
        }
        vector<long> conversions = parse_seeds(line);
        long dest_start = conversions[0];
        long source_start = conversions[1];
        long length = conversions[2];

        for (int i = 0; i < numbers.size(); i++) {
            if (numbers[i] >= source_start && numbers[i] < source_start + length && !changed[i]) {
                numbers[i] = dest_start + (numbers[i] - source_start);
                changed[i] = true;
            }
        }
    }
    return numbers;
}

vector<long> parse_map_x_to_x_2(ifstream &input, vector<long> numbers) {
    string line;
    getline(input, line); // skip top line
    vector<bool> changed;
    for (int i = 0; i < numbers.size(); i+=2) {
        changed.push_back(false);
    }
    int numbers_size = numbers.size();

    while (getline(input, line)) {
        if (line.length() == 0) {
            break;
        }
        vector<long> conversions = parse_seeds(line);
        long dest_start = conversions[0];
        long source_start = conversions[1];
        long length = conversions[2];
        long source_end = source_start + length;
        // cout << endl << "line " << source_start << "-" << source_end << " to " << dest_start << "-" << dest_start + length << endl;

        for (int i = 0; i < numbers_size; i+=2) {
            long seed_start = numbers[i];
            long seed_length = numbers[i+1];
            long seed_end = seed_start + seed_length;
            // cout << "seed " << seed_start << "-" << seed_end << " Changed: " << changed[i/2] << endl;

            // if they overlap and haven't been changed yet
            if (source_end > seed_start && seed_end > source_start && !changed[i/2]) {
                // entire range of seeds contained in the range
                if (seed_start >= source_start && seed_end <= source_end) {
                    numbers[i] = dest_start + (seed_start - source_start);
                    changed[i/2] = true;
                    // cout << "entire range" << endl;
                    // cout << "Changed " << seed_start << " to " << numbers[i] << endl;
                }
                // trim right
                if (seed_start >= source_start && seed_end > source_end) {
                    numbers[i] = dest_start + (seed_start - source_start);
                    numbers[i+1] = source_end - seed_start;
                    numbers.push_back(source_end);
                    numbers.push_back(seed_end - source_end);
                    changed[i/2] = true;
                    // cout << "trim right" << endl;
                    // cout << "Changed " << seed_start << " to " << numbers[i] << " with range " << source_end - seed_start << endl;
                    // cout << "Added " << source_end << " and " << seed_end - source_end << endl;
                }
                // trim left
                if (seed_start < source_start && seed_end <= source_end) {
                    numbers[i+1] = source_start - seed_start;
                    numbers.push_back(dest_start);
                    numbers.push_back(seed_end - source_start);
                    changed[i/2] = true;
                    // cout << "trim left" << endl;
                    // cout << "Changed range to " << source_start - seed_start << endl;
                    // cout << "Added " << source_start << " and " << seed_end - source_start << endl;
                }
                // trim both
                if (seed_start < source_start && seed_end > source_end) {
                    numbers[i+1] = source_start - seed_start;
                    numbers.push_back(dest_start);
                    numbers.push_back(length);
                    numbers.push_back(dest_start + length);
                    numbers.push_back(seed_end - source_end);
                    changed[i/2] = true;
                    // cout << "trim both" << endl;
                    // cout << "Changed range to " << source_start - seed_start << endl;
                    // cout << "Added " << source_start << " and " << length << endl;
                    // cout << "Added " << source_end << " and " << seed_end - source_end << endl;
                }
            }
        }
    }
    return numbers;
}

void print_seeds(vector<long> seeds) {
    cout << endl;
    for (int i = 0; i < seeds.size(); i++) {
        cout << seeds[i] << " ";
    }
    cout << endl;
}

long find_location(vector<long> seeds) {
    long answer = seeds[0];
    for (int i = 0; i < seeds.size(); i++) {
        if (seeds[i] < answer) {
            answer = seeds[i];
        }
    }
    return answer;
}

long find_location_2(vector<long> seeds) {
    long answer = seeds[0];
    for (int i = 0; i < seeds.size(); i+=2) {
        for (long j = 0; j < seeds[i+1]; j++) {
            if (seeds[i]+j < answer) {
                answer = seeds[i]+j;
            }
        }
    }
    return answer;
}

int main() {
    ifstream input("input.txt");
    // ifstream input("short_input.txt");
    string line;

    getline(input, line);
    pair<string, string> split_line = split(line, ":");
    vector<long> seeds = parse_seeds(split_line.second);

    getline(input, line); // skip empty line
    // print_seeds(seeds);
    for (int i = 0; i < 7; i++) {
        seeds = parse_map_x_to_x(input, seeds);
        // seeds = parse_map_x_to_x_2(input, seeds);
        // print_seeds(seeds);
    }

    cout << "lowest location: " << find_location(seeds) << endl;
    // cout << "lowest location: " << find_location_2(seeds) << endl;
}