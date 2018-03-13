#include <set>
#include <fstream>
#include <iostream>

/*
 * We say that Pattern is a most frequent k-mer in Text if it maximizes Count(Text, Pattern) among all k-mers.
 * For example, "ACTAT" is a most frequent 5-mer in "ACAACTATGCATCACTATCGGGAACTATCCT",
 * and "ATA" is a most frequent 3-mer of "CGATATATCCATAG".

    Frequent Words Problem
    Find the most frequent k-mers in a string.

    Given: A DNA string Text and an integer k.

    Return: All most frequent k-mers in Text (in any order).

    Sample Dataset:
    ACGTTGCATGTCGCATGATGCATGAGAGCT
    4

    Sample Output:
    CATG GCAT
 */

int main() {
    std::string dataset;
    unsigned int k;

    std::set<std::string> set;

    std::ifstream infile("input.txt");
    if (!infile || infile.peek() == std::ifstream::traits_type::eof()) {
        std::cout << "File not found or is empty" << std::endl;
        return -1;
    }

    infile >> dataset;
    infile >> k;

    unsigned max_frequency = 0u;
    
    for (int i = 0; i < dataset.size() - k + 1; ++i) {
        std::string base_str = dataset.substr(i, k);
        unsigned str_frequency = 1u;
        for (int j = i + 1; j < dataset.size() - k + 1; ++j) {
            if (!base_str.compare(dataset.substr(j,k))) {
                str_frequency++;
            }
        }
        if (str_frequency > max_frequency) {
            max_frequency = str_frequency;
            set.clear();
            set.emplace(base_str);
        } else if (str_frequency == max_frequency) {
            set.emplace(base_str);
        }
    }

    std::ofstream outfile("output.txt");
    for (const std::string& word: set) {
        outfile << word << " ";
    }
    outfile << std::endl;
    return 0;
}