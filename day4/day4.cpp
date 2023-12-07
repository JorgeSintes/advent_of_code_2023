#include <iostream>
#include <fstream>
#include <map>
#include <string>
#include <set>

using namespace std;

class Card {
public:
    set<int> winning_numbers;
    set<int> numbers;
    int number_of_hits;
    int score;
    Card (set<int> winning_numbers, set<int> numbers) {
        this->winning_numbers = winning_numbers;
        this->numbers = numbers;
        this->number_of_hits = 0;
        this->score = 0;

        for (auto it = numbers.begin(); it != numbers.end(); it++) {
            // cout << *it << " ";
            if (winning_numbers.find(*it) != winning_numbers.end()) {
                this->number_of_hits++;
                // cout << "hit. no. of hits: " << number_of_hits << endl;
            }
        }
        if (this->number_of_hits > 0) {
            this->score = 1;
            for (int i = 0; i < this->number_of_hits - 1; i++) {
                this->score *= 2;
            }
        }
    }
};

string trim(string word) {
    size_t left = word.find_first_not_of(" ");
    size_t right = word.find_last_not_of(" ");
    return word.substr(left, right - left + 1);
}

pair<string, string> split(string line, string delimiter) {
    size_t pos = line.find(delimiter);
    string left = line.substr(0, pos);
    string right = line.substr(pos + delimiter.length(), line.length() - pos - delimiter.length());
    return make_pair(trim(left), trim(right));
}

set<int> parse_numbers(string line) {
    set<int> numbers;
    bool started_reading_number = false, finished_reading_number = false;
    int number_start = 0;

    for (int i = 0; i < line.length(); i++) {
        if (!started_reading_number && isdigit(line[i])) {
            started_reading_number = true;
            number_start = i;
        }
        if (started_reading_number && !isdigit(line[i])) {
            started_reading_number = false;
            numbers.insert(stoi(line.substr(number_start, i - number_start)));
        }
    }
    if (started_reading_number) {
        numbers.insert(stoi(line.substr(number_start, line.length() - number_start)));
    }
    return numbers;
}

Card parse_card(string line) {
    int card_id;
    set<int> winning_numbers;
    set<int> numbers;
    pair<string, string> card_split, numbers_split;

    card_split = split(line, ":");
    numbers_split = split(card_split.second, "|");
    winning_numbers = parse_numbers(numbers_split.first);
    numbers = parse_numbers(numbers_split.second);
    // for (auto it=winning_numbers.begin(); it != winning_numbers.end(); it++) {
    //     cout << *it << " ";
    // }
    // cout << endl;

    Card card(winning_numbers, numbers);
    return card;
}

void increase_card_num(map<int, int> &num_cards, int card_id, int num) {
    if (num_cards.find(card_id) == num_cards.end()) {
        num_cards[card_id] = num;
    }
    else {
        num_cards[card_id] += num;
    }
}

int main() {
    ifstream input("input.txt");
    string line;
    int total_score = 0;
    map <int, int> num_cards;
    int total_num_cards = 0;

    int i = 1;
    while(getline(input, line)) {
        increase_card_num(num_cards, i, 1);
        // cout << "Card " << i << " has " << num_cards[i] << " instances" << endl;
        Card card = parse_card(line);
        total_score += card.score;
        // cout << "Card " << i << " has score " << card.score << endl;
        for (int j = 1; j <= card.number_of_hits; j++) {
            increase_card_num(num_cards, i+j, num_cards[i]);
            // cout << "Card " << i << " makes you win " << num_cards[i] << " cards of " << i + j << endl;
        }
        i++;
    }

    for(auto it = num_cards.begin(); it != num_cards.end(); it++) {
        if (it->first < i) {
            total_num_cards += it->second;
        }
    }

    cout << "Total score: " << total_score << endl;
    cout << "Total number of cards: " << total_num_cards << endl;

    return 0;
}
