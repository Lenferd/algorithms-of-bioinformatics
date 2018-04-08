#include <fstream>
#include <iostream>
#include <vector>
#include <map>

struct word {
    std::string full_word;
    std::string prefix;
    std::string suffix;

    word(std::string word, const unsigned long overlap_size) {
        this->full_word = word;
        this->prefix = word.substr(0, overlap_size);
        this->suffix = word.substr(word.size() - overlap_size, overlap_size);
    }
};

const int OVERLAP_SIZE = 4;

int main() {
    // Open file
    std::ifstream infile("input.txt");
    if (!infile || infile.peek() == std::ifstream::traits_type::eof()) {
        std::cout << "File not found or is empty" << std::endl;
        return -1;
    }

    int amount_of_lines;
    std::vector<word> k_mers_vect;

    std::map<std::string, std::vector<word>> overlap_graph;

    // Read file
    while (!infile.eof()){
        std::string temp_str;
        infile >> temp_str;
        if (!temp_str.empty()) {
            k_mers_vect.emplace_back(word(temp_str, OVERLAP_SIZE));
            ++amount_of_lines;
        }
    }

    for (const auto& word: k_mers_vect) {
        for (const auto& patter_word: k_mers_vect)
        if (word.suffix == patter_word.prefix) {
            overlap_graph[word.full_word].push_back(patter_word);
        }
    }
//    for (auto& it : overlap_graph) {
//        std::cout << it.first << " -> ";
//        for (int i = 0; i < it.second.size(); ++i) {
//            std::cout << it.second[i].full_word;
//            if (it.second.size() > 1 && i < it.second.size() - 1) {
//                std::cout << ",";
//            }
//        }
//        std::cout << std::endl;
//    }

    std::ofstream outfile("output.txt");
    for (auto& it : overlap_graph) {
        outfile << it.first << " -> ";
        for (int i = 0; i < it.second.size(); ++i) {
            outfile << it.second[i].full_word;
            if (it.second.size() > 1 && i < it.second.size() - 1) {
                outfile << ",";
            }
        }
        outfile << std::endl;
    }
    outfile.close();
}