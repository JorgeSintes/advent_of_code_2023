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

void subs_empty_rows_and_cols(Map &map) {
    for (int i = 0; i < map.size(); i++) {
        bool empty_row = true;
        for (int j = 0; j < map[i].size(); j++) {
            if (map[i][j] == '#') {
                empty_row = false;
                break;
            }
        }
        if (empty_row) {
            for (int j = 0; j < map[i].size(); j++) {
                map[i][j] = 'x';
            }
        }
    }
    for (int j=0; j < map[0].size(); j++) {
        bool empty_col = true;
        for (int i = 0; i < map.size(); i++) {
            if (map[i][j] == '#') {
                empty_col = false;
                break;
            }
        }
        if (empty_col) {
            for (int i = 0; i < map.size(); i++) {
                map[i][j] = 'x';
            }
        }
    }
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

long find_distance(Map &map, Galaxy &galaxy1, Galaxy &galaxy2, int expansion) {
    int start_i = min(galaxy1.i, galaxy2.i);
    int start_j = min(galaxy1.j, galaxy2.j);
    int len_i = abs(galaxy1.i - galaxy2.i);
    int len_j = abs(galaxy1.j - galaxy2.j);
    long dist = 0;
    for (int i = start_i; i < start_i + len_i; i++) {
        if (map[i][start_j] == 'x') {
            dist += expansion;
        }
        else {
            dist++;
        }
    }
    for (int j = start_j; j < start_j + len_j; j++) {
        if (map[start_i][j] == 'x') {
            dist += expansion;
        }
        else {
            dist++;
        }
    }
    return dist;
}

long sum_pair_shortest_paths(Map &map, vector<Galaxy> &galaxies, int expansion) {
    long sum = 0;
    for (int i = 0; i < galaxies.size(); i++) {
        for (int j = i + 1; j < galaxies.size(); j++) {
            sum += find_distance(map, galaxies[i], galaxies[j], expansion);
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
    int expansion = 2;
    // int expansion = 10000;
    Map map = parse_map(file);
    file.close();
    // print_map(map);
    subs_empty_rows_and_cols(map);
    // print_map(map);
    vector<Galaxy> galaxies = find_galaxies(map);
    long sum = sum_pair_shortest_paths(map, galaxies, expansion);
    cout << "Sum of pair shortest paths: " << sum << endl;
}
