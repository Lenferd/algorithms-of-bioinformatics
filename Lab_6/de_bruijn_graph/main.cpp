#include <fstream>
#include <iostream>
#include <vector>
#include <map>
#include <set>

const unsigned int PREFIX_OVERLAP = 3;

int main() {
    // Open file
    std::ifstream infile("input.txt");
    if (!infile || infile.peek() == std::ifstream::traits_type::eof()) {
        std::cout << "File not found or is empty" << std::endl;
        return -1;
    }

    int amount_of_lines;
    std::vector<std::string> k_mers_spectr;

    std::map<std::string, std::vector<std::string>> overlap_graph;
    // Read file
    while (!infile.eof()){
        std::string temp_str;
        infile >> temp_str;
        if (!temp_str.empty()) {
            k_mers_spectr.push_back(temp_str);
            ++amount_of_lines;
        }
    }

    for (const auto& word: k_mers_spectr) {
        overlap_graph[word.substr(0, PREFIX_OVERLAP)].push_back(word.substr(word.size() - PREFIX_OVERLAP, PREFIX_OVERLAP));
    }

//    for (auto& it : overlap_graph) {
//        std::cout << it.first << " -> ";
//        for (int i = 0; i < it.second.size(); ++i) {
//            std::cout << it.second[i];
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
            outfile << it.second[i];
            if (it.second.size() > 1 && i < it.second.size() - 1) {
                outfile << ",";
            }
        }
        outfile << std::endl;
    }
    outfile.close();
}