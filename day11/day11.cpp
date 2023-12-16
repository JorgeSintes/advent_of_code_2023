#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;
using Map = vector<vector<char>>;

struct Galaxy {
    int id;
    int i;
    int j;
};

Map parse_map(ifstream &file) {
    Map map;
    string line;
    while (getline(file, line)) {
        vector<char> row;
        for (char c : line) {
            row.push_back(c);
        }
        map.push_back(row);
    }
    return map;
}

void expand_map(Map &map) {
    Map new_map;
    for (int i = 0; i < map.size(); i++) {
        bool empty_row = true;
        for (int j = 0; j < map[i].size(); j++) {
            if (map[i][j] == '#') {
                empty_row = false;
                break;
            }
        }
        new_map.push_back(map[i]);
        if (empty_row) {
            new_map.push_back(map[i]);
        }
    }
    for (int j=0; j < new_map[0].size(); j++) {
        bool empty_col = true;
        for (int i = 0; i < new_map.size(); i++) {
            if (new_map[i][j] == '#') {
                empty_col = false;
                break;
            }
        }
        if (empty_col) {
            for (int i = 0; i < new_map.size(); i++) {
                new_map[i].insert(new_map[i].begin() + j, '.');
            }
            j++;
        }
    }
    map = new_map;
}

vector<Galaxy> find_galaxies(Map &map) {
    vector<Galaxy> galaxies;
    int galaxy_id = 0;
    for (int i = 0; i < map.size(); i++) {
        for (int j = 0; j < map[i].size(); j++) {
            if (map[i][j] == '#') {
                galaxies.push_back(Galaxy{galaxy_id++, i, j});
            }
        }
    }
    return galaxies;
}

int sum_pair_shortest_paths(vector<Galaxy> &galaxies) {
    int sum = 0;
    for (int i = 0; i < galaxies.size(); i++) {
        for (int j = i + 1; j < galaxies.size(); j++) {
            int dist = abs(galaxies[i].i - galaxies[j].i) + abs(galaxies[i].j - galaxies[j].j);
            sum += dist;
        }
    }
    return sum;
}

void print_map(Map &map) {
    for (int i = 0; i < map.size(); i++) {
        for (int j = 0; j < map[i].size(); j++) {
            cout << map[i][j];
        }
        cout << endl;
    }
    cout << endl;
}

int main() {
    // ifstream file("short_input.txt");
    ifstream file("input.txt");
    Map map = parse_map(file);
    file.close();
    // print_map(map);
    expand_map(map);
    // print_map(map);
    vector<Galaxy> galaxies = find_galaxies(map);
    int sum = sum_pair_shortest_paths(galaxies);
    cout << "Sum of pair shortest paths: " << sum << endl;
}
