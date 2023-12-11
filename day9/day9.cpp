#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;
using Sequence = vector<long>;
using Sequences = vector<Sequence>;

Sequences parse_input(ifstream& file) {
    Sequences seqs;
    string line;

    while (getline(file, line)) {
        Sequence seq = {};
        bool started_reading_number = false;
        int first_index = 0;
        for (int i = 0; i < line.length(); i++) {
            if (!started_reading_number && (isdigit(line[i]) || line[i] == '-')) {
                started_reading_number = true;
                first_index = i;
            }
            else if (started_reading_number && !isdigit(line[i])) {
                started_reading_number = false;
                seq.push_back(stoi(line.substr(first_index, i - first_index)));
            }
        }
        if (started_reading_number) {
            seq.push_back(stoi(line.substr(first_index, line.length() - first_index)));
        }
        seqs.push_back(seq);
    }
    return seqs;
}

void print_seqs(Sequences seqs) {
    for (Sequence seq : seqs) {
        for (int num : seq) {
            cout << num << " ";
        }
        cout << endl;
    }
}

bool all_zeros(Sequence seq) {
    for (int num : seq) {
        if (num != 0) {
            return false;
        }
    }
    return true;
}

Sequences generate_piramid(Sequence seq) {
    Sequences all_seqs = {seq};
    Sequence current_seq = seq;
    // keep adding substracting sequences until we get all zeros
    while (!all_zeros(current_seq)) {
        Sequence next_seq = {};
        for (int i = 0; i < current_seq.size() - 1; i++) {
            next_seq.push_back(current_seq[i+1] - current_seq[i]);
        }
        all_seqs.push_back(next_seq);
        current_seq = next_seq;
    }
    return all_seqs;
}

int extrapolate(Sequence seq) {
    Sequences all_seqs = generate_piramid(seq);
    // print_seqs(all_seqs);

    all_seqs[all_seqs.size() - 1].push_back(0);
    for (int i = all_seqs.size() - 2; i >= 0; i--) {
        Sequence bottom_seq = all_seqs[i+1];
        int last_idx = (int)bottom_seq.size() - 1;
        all_seqs[i].push_back(all_seqs[i][last_idx] + bottom_seq[last_idx]);
    }
    return all_seqs[0][all_seqs[0].size() - 1];
}

int extrapolate_backwards(Sequence seq) {
    Sequences all_seqs = generate_piramid(seq);
    // print_seqs(all_seqs);

    // reverse the sequences 
    for (Sequence& seq : all_seqs) {
        reverse(seq.begin(), seq.end());
    }
    // print_seqs(all_seqs);

    all_seqs[all_seqs.size() - 1].push_back(0);
    // add the last value to each sequence
    for (int i = all_seqs.size() - 2; i >= 0; i--) {
        Sequence bottom_seq = all_seqs[i+1];
        int last_idx = (int)bottom_seq.size() - 1;
        all_seqs[i].push_back(all_seqs[i][last_idx] - bottom_seq[last_idx]);
    }
    return all_seqs[0][all_seqs[0].size() - 1];
}

int main() {
    // ifstream file("short_input.txt");
    ifstream file("input.txt");
    Sequences seqs = parse_input(file);
    long sum = 0;
    for (Sequence seq : seqs) {
        sum += extrapolate(seq);
    }
    cout << "First sum: " << sum << endl;

    sum = 0;
    for (Sequence seq : seqs) {
        sum += extrapolate_backwards(seq);
    }
    cout << "Second sum: " << sum << endl;
}