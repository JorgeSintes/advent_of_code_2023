#include <iostream>
#include <fstream>
#include <vector>

using namespace std;
using Map = vector<vector<char>>;

struct Node {
    int i;
    int j;
    Node* previous;
    Node* next;
};

Map parse_input(ifstream& input) {
    Map map;
    string line;
    int i = 0;
    while (getline(input, line)) {
        vector<char> row(line.begin(), line.end());
        map.push_back(row);
    }
    return map;
}

Node* find_start(Map &map) {
    Node* node = new Node{-1, -1, nullptr};
    for (int i = 0; i < map.size(); i++) {
        for (int j = 0; j < map[0].size(); j++) {
            if (map[i][j] == 'S') {
                node->i = i;
                node->j = j;
            }
        }
    }
    return node;
}

Node* find_next_node(Map &map, Node* node) {
    int i = node->i;
    int j = node->j;
    int up = max(i - 1, 0);
    int right = min(j + 1, (int) map[0].size() - 1);
    int down = min(i + 1, (int) map.size() - 1);
    int left = max(j - 1, 0);
    int next = -1;

    if (node->previous == nullptr) {
        if (up != i 
                && map[up][j] == '|' 
                || map[up][j] == '7' 
                || map[up][j] == 'F') {
            next = 0;
        }
        if (right != j 
                && map[i][right] == '-' 
                || map[i][right] == 'J' 
                || map[i][right] == '7') {
            next = 1;
        }
        if (down != i 
                && map[down][j] == '|' 
                || map[down][j] == 'J' 
                || map[down][j] == 'L') {
            next = 2;
        }
        if (left != j 
                && map[i][left] == '-' 
                || map[i][left] == 'L' 
                || map[i][left] == 'F') {
            next = 3;
        }
    }
    else {
        switch(map[i][j]) {
            case '|':
                if (node->previous->i == up) {
                    next = 2;
                }
                else if (node->previous->i == down) {
                    next = 0;
                }
                break;
            case '-':
                if (node->previous->j == right) {
                    next = 3;
                }
                else if (node->previous->j == left) {
                    next = 1;
                }
                break;
            case '7':
                if (node->previous->i == down) {
                    next = 3;
                }
                else if (node->previous->j == left) {
                    next = 2;
                }
                break;
            case 'J':
                if (node->previous->j == left) {
                    next = 0;
                }
                else if (node->previous->i == up) {
                    next = 3;
                }
                break;
            case 'L':
                if (node->previous->i == up) {
                    next = 1;
                }
                else if (node->previous->j == right) {
                    next = 0;
                }
                break;
            case 'F':
                if (node->previous->i == down) {
                    next = 1;
                }
                else if (node->previous->j == right) {
                    next = 2;
                }
                break;
        }
    }
    Node* next_node = new Node{-1, -1, nullptr};
    if (next == 0) {
        next_node->i = up;
        next_node->j = j;
        next_node->previous = node;
        node->next = next_node;
    }
    else if (next == 1) {
        next_node->i = i;
        next_node->j = right;
        next_node->previous = node;
        node->next = next_node;
    }
    else if (next == 2) {
        next_node->i = down;
        next_node->j = j;
        next_node->previous = node;
        node->next = next_node;
    }
    else if (next == 3) {
        next_node->i = i;
        next_node->j = left;
        next_node->previous = node;
        node->next = next_node;
    }
    return next_node;
}

int construct_graph(Map &map, Node* start_node) {
    cout << start_node->i << " " << start_node->j << endl;
    Node* node = find_next_node(map, start_node);
    cout << node->i << " " << node->j << endl;
    int i = 1;
    while (node->i != start_node->i || node->j != start_node->j) {
        node = find_next_node(map, node);
        cout << node->i << " " << node->j << endl;
        i++;
    }
    node->previous->next = start_node;
    return i;
}

int main() {
    // ifstream input("short_input.txt");
    ifstream input("input.txt");
    Map map = parse_input(input);
    Node* start_node = find_start(map);
    int steps = construct_graph(map, start_node);
    cout << "Start node: " << start_node->i << " " << start_node->j << endl;
    cout << "Steps: " << steps/2 << endl;
}
