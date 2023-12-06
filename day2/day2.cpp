#include <fstream>
#include <iostream>
#include <map>
#include <string>
#include <vector>

std::string trim(std::string line) {
    std::string result;
    std::size_t first = line.find_first_not_of(' ');
    std::size_t last = line.find_last_not_of(' ');
    result = line.substr(first, (last - first + 1));
    return result;
}

std::vector<std::string> split(std::string line, char delimiter) {
    std::vector<std::string> result;
    std::string token;
    std::size_t pos = 0;
    std::string new_line = line;

    while ((pos = new_line.find(delimiter)) != std::string::npos) {
        token = new_line.substr(0, pos);
        result.push_back(trim(token));
        new_line.erase(0, pos + 1);
    }
    result.push_back(trim(new_line));

    return result;
}

int is_possible(std::string game, std::map<std::string, int> available_cubes) {
    std::vector<std::string> game_split, round_split, cubes_split, no_cubes_split;

    game_split = split(game, ':');
    if (game_split.size() != 2) {
        std::cout << "Error: game_split.size() != 2" << std::endl;
        return 0;
    }
    int game_id = std::stoi(split(game_split[0], ' ')[1]);
    // std::cout << "\nGame ID: " << game_id << std::endl;

    round_split = split(game_split[1], ';');
    
    for (int i = 0; i < round_split.size(); i++) {
        std::map<std::string, int> game_cubes = {
            {"red", 0},
            {"green", 0},
            {"blue", 0}
        };
        // std::cout << "Round " << i << ": " << round_split[i] << std::endl;

        cubes_split = split(round_split[i], ',');
        for (int j = 0; j < cubes_split.size(); j++) {
            no_cubes_split = split(cubes_split[j], ' ');
            game_cubes[no_cubes_split[1]] += std::stoi(no_cubes_split[0]);
        }
        for (auto it = game_cubes.begin(); it != game_cubes.end(); it++) {
            if (it->second > available_cubes[it->first]) {
                // std::cout << "Error: " << it->second << " " << it->first << " cubes are not available, only " << available_cubes[it->first]  << std::endl;
                return 0;
            }
        }
    }

    return game_id;
}

int find_power(std::string game) {
    std::vector<std::string> game_split, round_split, cubes_split, no_cubes_split;
    std::map<std::string, int> game_cubes = {
        {"red", 0},
        {"green", 0},
        {"blue", 0}
    };

    game_split = split(game, ':');
    if (game_split.size() != 2) {
        std::cout << "Error: game_split.size() != 2" << std::endl;
        return 0;
    }
    int game_id = std::stoi(split(game_split[0], ' ')[1]);
    // std::cout << "\nGame ID: " << game_id << std::endl;

    round_split = split(game_split[1], ';');
    
    for (int i = 0; i < round_split.size(); i++) {
        // std::cout << "Round " << i << ": " << round_split[i] << std::endl;

        cubes_split = split(round_split[i], ',');
        for (int j = 0; j < cubes_split.size(); j++) {
            no_cubes_split = split(cubes_split[j], ' ');
            if (std::stoi(no_cubes_split[0]) > game_cubes[no_cubes_split[1]])
                game_cubes[no_cubes_split[1]] = std::stoi(no_cubes_split[0]);
        }
    }
    // std::cout << "Game cubes: " << game_cubes["red"] << " " << game_cubes["green"] << " " << game_cubes["blue"] << std::endl;
    return game_cubes["red"]*game_cubes["green"]*game_cubes["blue"];
   
}

int main(){
    // std::ifstream input("short_input.txt");
    std::ifstream input("input.txt");
    std::string game;

    std::map<std::string, int> available_cubes = {
        {"red", 12},
        {"green", 13},
        {"blue", 14}
    };
    int id_sum=0, power_sum=0;
    
    if (input.is_open()) {
        while (getline(input, game)) {
            id_sum += is_possible(game, available_cubes);
            power_sum += find_power(game);
        }
        input.close();
    } else {
        std::cout << "Unable to open file" << std::endl;
    }

    std::cout << "ID sum: " << id_sum << std::endl;
    std::cout << "Power sum: " << power_sum << std::endl;

    return 0;
}