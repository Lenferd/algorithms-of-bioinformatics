#include <iostream>
#include <fstream>
#include <array>
#include <vector>
#include <cmath>
#include <climits>

// Looks like a mountain of garbage. Yep, that's it is
void next_word(std::string & word) {
    unsigned int position = 0;
    while (true) {
        if (position > word.size()){
            std::cout << "Out of range increase" << std::endl;
            exit(-2);
        }
        if (word[position] < 'T') {
            switch (word[position]) {
                case 'A':
                    word[position] = 'C';
                    break;
                case 'C':
                    word[position] = 'G';
                    break;
                case 'G':
                    word[position] = 'T';
                    break;
                default:
                    std::cout << "Unknown char: " << word[position] << std::endl;
            }
            break;
        } else {
            word[position] = 'A';
            ++position;
            continue;
        }
    }
}

int Distance(std::string & line, std::string& templ) {
    int minimal_distance = INT_MAX;
    for (int i = 0; i < line.size() - templ.size() + 1; ++i) {
        int distance = 0;
        for (int j = 0; j < templ.size(); ++j) {
            if (line[i + j] != templ[j]){
                ++distance;
            }
        }
        if (distance < minimal_distance) {
            minimal_distance = distance;
        }
    }
    return minimal_distance;
}

int TotalDistance(std::vector<std::string>& dataset, std::string & word) {
    int total_distance = 0;
    for (int i = 0; i < dataset.size(); ++i) {
        total_distance += Distance(dataset[i], word);
    }
    return total_distance;
}

int main() {
    // Open file
    std::ifstream infile("input.txt");
    if (!infile || infile.peek() == std::ifstream::traits_type::eof()) {
      std::cout << "File not found or is empty" << std::endl;
      return -1;
    }

    // Init variables
    unsigned int template_size;
    unsigned int amount_of_lines = 0;

    int best_distance = INT_MAX;

    std::vector<std::string> dataset;

    infile >> template_size;

    while (!infile.eof()){
        std::string temp_str;
        infile >> temp_str;
        dataset.push_back(temp_str);
        ++amount_of_lines;
    }

    std::string word(template_size, 'A');
    std::string best_word(template_size, 'A');

    for (int i = 0; i < pow(4, template_size) - 1; ++i){
        int temp_distance = TotalDistance(dataset, word);
        if (temp_distance < best_distance) {
            best_distance = temp_distance;
            best_word = word;
        }
        next_word(word);
    }

    std::ofstream outfile("output.txt");
    outfile << best_word << std::endl;
    return 0;
}