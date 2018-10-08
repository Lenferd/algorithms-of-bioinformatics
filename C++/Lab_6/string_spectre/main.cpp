#include <fstream>
#include <iostream>
#include <vector>

static std::vector<std::string> split_string(const int kmer_size, const std::string str) {
    std::vector<std::string> result_vect;
    for (int i = 0; i < str.size() - kmer_size + 1; ++i) {
        result_vect.emplace_back(str.substr(i, kmer_size));
    }
    return  result_vect;
}

int main() {
    // Open file
    std::ifstream infile("input.txt");
    if (!infile || infile.peek() == std::ifstream::traits_type::eof()) {
        std::cout << "File not found or is empty" << std::endl;
        return -1;
    }

    int kmer_size = 0;
    std::string str;

    infile >> kmer_size;
    infile >> str;
    infile.close();

    std::vector<std::string> result = split_string(kmer_size, str);

    for (auto it : result) {
        std::cout << it << std::endl;
    }

    std::ofstream outfile("output.txt");
    for (auto it : result) {
        outfile << it << std::endl;
    }
    outfile.close();
}