#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include <algorithm>

void increase_start_position(std::vector<int> & vector, int max_value){
    unsigned int position = 0;
    while (true) {
        if (position > vector.size()){
            std::cout << "Out of range increase" << std::endl;
            exit(-2);
        }
        if (vector[position] < max_value) {
            ++vector[position];
            break;
        } else {
            vector[position] = 0;
            ++position;
            continue;
        }
    }
}

int Score(std::vector<int> & start_positions,  std::vector<std::string>& dataset, int template_size){
    int profile_matrix[template_size][4] = {0};
    for (int i = 0; i < dataset.size(); ++i) {
        for (int j = 0; j < template_size; ++j) {
            switch (dataset[i][start_positions[i] + j]) {
                case 'A':
                    ++profile_matrix[j][0];
                    break;
                case 'C':
                    ++profile_matrix[j][1];
                    break;
                case 'G':
                    ++profile_matrix[j][2];
                    break;
                case 'T':
                    ++profile_matrix[j][3];
                    break;
                default:
                    std::cout << "Unknown char: "  << dataset[i][start_positions[i] + j] << std::endl;
                    break;
            }
        }
    }
    int result = 0;
    for (int k = 0; k < template_size; ++k) {
        result += *std::max_element(profile_matrix[k], profile_matrix[k] + 4);
    }
    return result;
}


int main() {
    std::vector<std::string> dataset;

    // Open file
    std::ifstream infile("input.txt");
    if (!infile || infile.peek() == std::ifstream::traits_type::eof()) {
        std::cout << "File not found or is empty" << std::endl;
        return -1;
    }

    // Init variables
    unsigned int template_size;
    unsigned int amount_of_lines;
    unsigned int line_length;
    int score = 0;

    infile >> template_size;
    infile >> amount_of_lines;

    std::vector<int> start_positions(amount_of_lines, 0);
    std::vector<int> best_start_positions(amount_of_lines, 0);

    // Read file
    for (int i = 0; i < amount_of_lines; ++i) {
        std::string temp_str;
        if (!infile.eof()){
            infile >> temp_str;
            dataset.push_back(temp_str);
        } else {
            std::cout << "Dataset size is wrong" << std::endl;
        }
    }

    line_length = static_cast<unsigned int>(dataset[0].size());

    for (int i = 0; i < pow(line_length - template_size + 1, amount_of_lines); ++i) {
        int temp_score = 0;
        temp_score = Score(start_positions, dataset, template_size);
        if (temp_score > score) {
            score = temp_score;
            best_start_positions = start_positions;
        }
        increase_start_position(start_positions, line_length - template_size);
    }

    // Cut best motifs
    std::vector<std::string> motifs;
    for (int i = 0; i < amount_of_lines; ++i) {
        motifs.push_back(dataset[i].substr(best_start_positions[i], template_size));
    }


    // Write data
    std::ofstream outfile("output.txt");
    for (auto& line : motifs) {
        outfile<< line << std::endl;
    }
    outfile << std::endl;
    return 0;
}