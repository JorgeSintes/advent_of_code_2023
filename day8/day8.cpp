#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <numeric>

using namespace std;
using NodeMap = map<string, pair<string, string>>;
using Input = pair<string, NodeMap>;

Input parse_input(ifstream& file) {
    string line, directions;
    NodeMap nodemap;
    getline(file, line);
    directions = line;
    getline(file, line);
    while (getline(file, line)) {
        string name = line.substr(0, line.find(' '));
        string left = line.substr(line.find('(') + 1, line.find(',') - line.find('(') - 1);
        string right = line.substr(line.find(',') + 2, line.find(')') - line.find(',') - 2);
        nodemap[name] = make_pair(left, right);
        // cout << name << " " << left << " " << right << endl;
    }
    return make_pair(directions, nodemap);
}

vector<string> find_starting_nodes(NodeMap& nodemap) {
    vector<string> starting_nodes;
    for (NodeMap::iterator it = nodemap.begin(); it != nodemap.end(); it++) {
        if (it->first[2] == 'A') {
            starting_nodes.push_back(it->first);
        }
    }
    return starting_nodes;
}

int main() {
    // ifstream file("short_input.txt");
    // ifstream file("short_input2.txt");
    ifstream file("input.txt");
    Input the_input = parse_input(file);
    string directions = the_input.first;
    NodeMap nodemap = the_input.second;
    string current_node = "AAA";
    int steps = 0;

    while(current_node != "ZZZ") {
        if (directions[steps % directions.size()] == 'L') {
            current_node = nodemap[current_node].first;
        } else {
            current_node = nodemap[current_node].second;
        }
        steps++;
    }
    cout << "Steps: " << steps << endl;

    vector<string> current_nodes = find_starting_nodes(nodemap);
    vector<int> second_steps;
    int second_steps_count = 0;
    for (int i = 0; i < current_nodes.size(); i++) {
        string node = current_nodes[i];
        while (node[2] != 'Z') {
            if (directions[steps % directions.size()] == 'L') {
                node = nodemap[node].first;
            } else {
                node = nodemap[node].second;
            }
            steps++;
        }
        second_steps.push_back(steps);
        steps = 0;
    }
    // find LCM of second_steps
    long combined_steps = second_steps[0];
    for (int i = 1; i < second_steps.size(); i++) {
        combined_steps = lcm(combined_steps, second_steps[i]);
    }
    cout << "Steps: " << combined_steps << endl;
}
