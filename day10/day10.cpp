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
    next_node->previous = node;
    node->next = next_node;
    if (next == 0) {
        next_node->i = up;
        next_node->j = j;
    }
    else if (next == 1) {
        next_node->i = i;
        next_node->j = right;
    }
    else if (next == 2) {
        next_node->i = down;
        next_node->j = j;
    }
    else if (next == 3) {
        next_node->i = i;
        next_node->j = left;
    }
    return next_node;
}

int construct_graph(Map &map, Node* start_node) {
    // cout << start_node->i << " " << start_node->j << endl;
    Node* node = find_next_node(map, start_node);
    // cout << node->i << " " << node->j << endl;
    int i = 1;
    while (node->i != start_node->i || node->j != start_node->j) {
        node = find_next_node(map, node);
        // cout << node->i << " " << node->j << endl;
        i++;
    }
    node->previous->next = start_node;
    start_node->previous = node->previous;
    return i;
}

bool is_in_path(Node* start_node, int i, int j) {
    Node* current = start_node;
    if (current->i == i && current->j == j) {
        return true;
    }
    current = current->next;
    while (current != start_node) {
        if (current->i == i && current->j == j) {
            return true;
        }
        current = current->next;
    }
    return false;
}

void solve_S(Map &map, Node* start_node) {
    Node* next_node = start_node->next;
    Node* previous_node = start_node->previous;
    int next= -1, previous = -1;
    if (next_node->i - start_node->i == -1) {
        next = 0;
    }
    else if (next_node->j - start_node->j == 1) {
        next = 1;
    }
    else if (next_node->i - start_node->i == 1) {
        next = 2;
    }
    else if (next_node->j - start_node->j == -1) {
        next = 3;
    }
    if (previous_node->i - start_node->i == -1) {
        previous = 0;
    }
    else if (previous_node->j - start_node->j == 1) {
        previous = 1;
    }
    else if (previous_node->i - start_node->i == 1) {
        previous = 2;
    }
    else if (previous_node->j - start_node->j == -1) {
        previous = 3;
    }

    if (next == 0) {
        if (previous == 1) {
            map[start_node->i][start_node->j] = 'L';
        }
        else if (previous == 3) {
            map[start_node->i][start_node->j] = 'J';
        }
        else {
            map[start_node->i][start_node->j] = '|';
        }
    }
    else if (next == 1) {
        if (previous == 0) {
            map[start_node->i][start_node->j] = 'L';
        }
        else if (previous == 2) {
            map[start_node->i][start_node->j] = 'F';
        }
        else {
            map[start_node->i][start_node->j] = '-';
        }
    }
    else if (next == 2) {
        if (previous == 1) {
            map[start_node->i][start_node->j] = 'F';
        }
        else if (previous == 3) {
            map[start_node->i][start_node->j] = '7';
        }
        else {
            map[start_node->i][start_node->j] = '|';
        }
    }
    else if (next == 3) {
        if (previous == 0) {
            map[start_node->i][start_node->j] = 'J';
        }
        else if (previous == 2) {
            map[start_node->i][start_node->j] = '7';
        }
        else {
            map[start_node->i][start_node->j] = '-';
        }
    }
}

bool is_inside_path(Map &map, Node* start_node, int i, int j) {
    if (is_in_path(start_node, i, j)) {
        return false;
    }
    int no_crosses = 0;
    char parallel_pipe = ' ';
    for (int k = i; k >= 0; k--) {
        if (is_in_path(start_node, k, j)) {
            if (map[k][j] == '-') {
                no_crosses++;
            }
            if (map[k][j] == 'J' || map[k][j] == 'L') {
                parallel_pipe = map[k][j];
            }
            if (map[k][j] == '7' && parallel_pipe == 'L') {
                no_crosses++;
                parallel_pipe = ' ';
            }
            if (map[k][j] == 'F' && parallel_pipe == 'J') {
                no_crosses++;
                parallel_pipe = ' ';
            }
        }
    }
    return no_crosses % 2 == 1;
}

int main() {
    // ifstream input("short_input.txt");
    ifstream input("input.txt");
    Map map = parse_input(input);
    Node* start_node = find_start(map);
    int steps = construct_graph(map, start_node);
    solve_S(map, start_node);
    cout << "Start node: " << start_node->i << " " << start_node->j << endl;
    cout << "Steps: " << steps/2 << endl;

    int inside_area = 0;
    for (int i = 0; i < map.size(); i++) {
        for (int j = 0; j < map[0].size(); j++) {
            inside_area += is_inside_path(map, start_node, i, j);
        }
    }

    cout << "Inside area: " << inside_area << endl;
}
