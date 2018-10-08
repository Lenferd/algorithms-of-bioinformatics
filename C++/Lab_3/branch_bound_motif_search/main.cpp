#include <iostream>
#include <vector>
#include <algorithm>
#include <fstream>

void bypass_move(std::vector<unsigned int> & l_mer, unsigned int & level, const unsigned int max_elem_num) {
    for (int i = (int)l_mer.size() - 1; i >= 0; --i) {
        if (l_mer[i] < max_elem_num) {
            ++l_mer[i];
            level = (unsigned int)i + 1;
            return ;
        }
    }
    level = 0;
}

void next_vertex(std::vector<unsigned int> & l_mer, unsigned int & level, const unsigned int max_elem_num) {
    if (level < l_mer.size()) {
        l_mer[level] = 0;
        ++level;
        return ;
    } else {
        bypass_move(l_mer, level, max_elem_num);
    }
}

unsigned int Score(std::vector<unsigned int> & start_positions,  std::vector<std::string>& dataset, int template_size,
          int level){
    int profile_matrix[template_size][4] = {0};
    for (int i = 0; i < level; ++i) {
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
    unsigned int result = 0;
    for (int k = 0; k < template_size; ++k) {
        result += *std::max_element(profile_matrix[k], profile_matrix[k] + 4);
    }
    return result;
}

std::vector<std::string> branch_and_bound_motif_search(std::vector<std::string> dataset, const unsigned int template_size,
                                          unsigned int amount_of_lines) {
    std::vector<unsigned int> start_positions(amount_of_lines, 0);
    std::vector<unsigned int> best_start_positions(amount_of_lines, 0);

    unsigned int best_score = 0;
    auto line_length = static_cast<unsigned int>(dataset[0].size());

    unsigned int level = 1;
    unsigned int optimistic_score = 0;
    unsigned int max_number = line_length - template_size;

    unsigned int temp_score = 0;
    while (level > 0) {
        if (level < template_size) {
            optimistic_score = Score(start_positions, dataset, template_size, level)
                               + (amount_of_lines - level) * template_size;
            if (optimistic_score < best_score) {
                bypass_move(start_positions, level, max_number);
            } else {
                next_vertex(start_positions, level, max_number);
            }
        } else {
            temp_score = Score(start_positions, dataset, template_size, level);
            if (temp_score > best_score) {
                best_score = temp_score;
                best_start_positions = start_positions;
            }
            next_vertex(start_positions, level, max_number);
        }
    }

    // Cut best motifs
    std::vector<std::string> motifs;
    for (int i = 0; i < amount_of_lines; ++i) {
        motifs.push_back(dataset[i].substr(best_start_positions[i], template_size));
    }

    return motifs;
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

    infile >> template_size;
    infile >> amount_of_lines;

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

    std::vector<std::string> motifs = branch_and_bound_motif_search(dataset, template_size, amount_of_lines);

    // Write data
    std::ofstream outfile("output.txt");
    for (auto& line : motifs) {
        outfile << line << std::endl;
    }
    return 0;
}