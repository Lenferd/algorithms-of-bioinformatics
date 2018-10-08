#include <iostream>
#include <vector>
#include <algorithm>
#include <fstream>
#include <climits>

const std::string CONVERT_STR = {'A', 'C', 'G', 'T'};
std::string convert_vect_to_string(const std::vector<unsigned int> & word) {
    std::string result(word.size(), ' ');
    for (int i = 0; i < word.size(); ++i) {
        result[i] = CONVERT_STR[word[i]];
    }
    return result;
}
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

unsigned int Distance(std::string & line, std::string& templ, unsigned int level) {
    unsigned int minimal_distance = INT_MAX;
    for (int i = 0; i < line.size() - templ.size() + 1; ++i) {
        unsigned int distance = 0;
        for (int j = 0; j < level; ++j) {
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

unsigned int TotalDistance(std::vector<std::string>& dataset, std::string& word,
                           unsigned int level) {
    unsigned int total_distance = 0;
    for (int i = 0; i < dataset.size(); ++i) {
        total_distance += Distance(dataset[i], word, level);
    }
    return total_distance;
}

std::string branch_and_bound_median_string_search(std::vector<std::string> dataset, const unsigned int template_size,
                                                       unsigned int amount_of_lines) {
    std::vector<unsigned int> word(template_size, 0);
    std::vector<unsigned int> best_word(template_size, 0);

    unsigned int best_distance = INT_MAX;

    unsigned int level = 1;
    unsigned int optimistic_distance = 0;
    unsigned int max_word_number = 3; // 0 - A, 1 - C, 2 - G, 3 -T

    unsigned int temp_distance = 0;

    while (level > 0) {
        if (level < template_size) {
            std::string prefix = convert_vect_to_string(word);
            optimistic_distance = TotalDistance(dataset, prefix, level);

            if (optimistic_distance > best_distance) {
                bypass_move(word, level, max_word_number);
            } else {
                next_vertex(word, level, max_word_number);
            }
        } else {
            std::string prefix = convert_vect_to_string(word);
            temp_distance = TotalDistance(dataset, prefix, level);
            if (temp_distance < best_distance) {
                best_distance = temp_distance;
                best_word = word;
            }
            next_vertex(word, level, max_word_number);
        }
    }

    // get word
    return convert_vect_to_string(best_word);
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
    unsigned int template_size = 0;
    unsigned int amount_of_lines = 0;

    infile >> template_size;

    // Read file
    while (!infile.eof()){
        std::string temp_str;
        infile >> temp_str;
        dataset.push_back(temp_str);
        ++amount_of_lines;
    }

    std::string best_word = branch_and_bound_median_string_search(dataset, template_size, amount_of_lines);

    // Write data
    std::ofstream outfile("output.txt");
    outfile << best_word << std::endl;
    return 0;
}