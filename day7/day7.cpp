#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>

using namespace std;

struct Play {
    string hand;
    int bid;
};

map<char, int> CARD_VALUES {
    {'2', 2},    
    {'3', 3},
    {'4', 4},
    {'5', 5},
    {'6', 6},
    {'7', 7},
    {'8', 8},
    {'9', 9},
    {'T', 10},
    // {'J', 11},
    {'J', 1},
    {'Q', 12},
    {'K', 13},
    {'A', 14},
};

vector<Play> parse_plays(ifstream& input) {
    string line;
    vector<Play> plays;
    while (getline(input, line)) {
        Play play{
            line.substr(0, 5),
            stoi(line.substr(6, line.length()-6))
        };
        // cout << play.hand << " " << play.bid << endl;
        plays.push_back(play);
    }
    return plays;
}

int get_hand_type(string hand) {
    map<char, int> cards {
        {'A', 0},
        {'K', 0},
        {'Q', 0},
        {'J', 0},
        {'T', 0},
        {'9', 0},
        {'8', 0},
        {'7', 0},
        {'6', 0},
        {'5', 0},
        {'4', 0},
        {'3', 0},
        {'2', 0},
    };
    for (int i=0; i<hand.size(); i++) {
        cards[hand[i]]++;
    }
    int five_of_a_kind = 0;
    int four_of_a_kind = 0;
    int three_of_a_kind = 0;
    int pairs = 0;
    for (auto const& [key, val] : cards) {
        if (val == 5) {
            five_of_a_kind++;
        }
        if (val == 4) {
            four_of_a_kind++;
        }
        if (val == 3) {
            three_of_a_kind++;
        }
        if (val == 2) {
            pairs++;
        }
    }
    if (five_of_a_kind > 0) {
        return 6;
    }
    if (four_of_a_kind > 0) {
        return 5;
    }
    if (three_of_a_kind == 1 && pairs == 1) {
        return 4;
    }
    if (three_of_a_kind == 1) {
        return 3;
    }
    if (pairs == 2) {
        return 2;
    }
    if (pairs == 1) {
        return 1;
    }
    return 0;
}

int get_hand_type2(string hand) {
    map<char, int> cards {
        {'A', 0},
        {'K', 0},
        {'Q', 0},
        {'J', 0},
        {'T', 0},
        {'9', 0},
        {'8', 0},
        {'7', 0},
        {'6', 0},
        {'5', 0},
        {'4', 0},
        {'3', 0},
        {'2', 0},
    };
    for (int i=0; i<hand.size(); i++) {
        cards[hand[i]]++;
    }
    int five_of_a_kind = 0;
    int four_of_a_kind = 0;
    int three_of_a_kind = 0;
    int pairs = 0;
    for (auto const& [key, val] : cards) {
        if (val == 5) {
            five_of_a_kind++;
        }
        else if (key != 'J' && val + cards['J'] == 5) {
            five_of_a_kind++;
        }
        if (val == 4) {
            four_of_a_kind++;
        }
        else if (key != 'J' && val + cards['J'] == 4) {
            four_of_a_kind++;
        }
        if (val == 3) {
            three_of_a_kind++;
        }
        if (val == 2) {
            pairs++;
        }
    }
    if (five_of_a_kind >= 1) {
        return 6;
    }
    if (four_of_a_kind >= 1) {
        return 5;
    }
    if (three_of_a_kind == 1 && pairs == 1) {
        return 4;
    }
    if (three_of_a_kind == 1) {
        return 3;
    }
    if (pairs == 2 && cards['J'] == 1) {
        return 4;
    }
    else if (pairs == 2) {
        return 2;
    }
    if (pairs == 1 && cards['J'] > 0) {
        return 3;
    }
    if (pairs == 1) {
        return 1;
    }
    if (cards['J'] == 1) {
        return 1;
    }
    return 0;
}

int greater_hand(Play play1, Play play2) {
    // int value1 = get_hand_type(play1.hand);
    // int value2 = get_hand_type(play2.hand);
    int value1 = get_hand_type2(play1.hand);
    int value2 = get_hand_type2(play2.hand);
    if (value1 > value2) {
        return 0;
    }
    else if (value1 < value2) {
        return 1;
    }
    else {
        for (int i=0; i<play1.hand.size(); i++) {
            if (CARD_VALUES[play1.hand[i]] > CARD_VALUES[play2.hand[i]]) {
                return 0;
            }
            else if(CARD_VALUES[play1.hand[i]] < CARD_VALUES[play2.hand[i]]) {
                return 1;
            }
        }
    }
    return -1;
}

vector<Play> sort_plays(vector<Play> plays) {
    if (plays.size() <= 1) {
        return plays;
    }
    else {
        Play pivot = plays[plays.size()-1];
        int i = -1, j = 0;
        for (j=0; j<plays.size(); j++) {
            if (
                (j == plays.size()-1 || 
                greater_hand(plays[j], plays[plays.size()-1]) == 1)
            ) {
                i++;
                Play temp = plays[i];
                plays[i] = plays[j];
                plays[j] = temp;
            }
        }
        vector<Play> left = vector<Play>(plays.begin(), plays.begin()+i);
        vector<Play> right = vector<Play>(plays.begin()+i+1, plays.end());
        left = sort_plays(left);
        right = sort_plays(right);
        left.push_back(pivot);
        left.insert(left.end(), right.begin(), right.end());
        return left;
    }
}

int main() {
    ifstream input("input.txt");
    // ifstream input("short_input.txt");
    vector<Play> plays = parse_plays(input);
    plays = sort_plays(plays);
    long result = 0;
    
    for (int i=0; i<plays.size(); i++) {
        // cout <<get_hand_type(plays[i].hand) << " " << plays[i].hand << " " << plays[i].bid << endl;
        result += plays[i].bid * (i+1);
    }

    cout << result << endl;
    return 0;
}