#include <iostream>
#include <fstream>
#include <vector>
#include <climits>


int DPChange(unsigned long& coin, std::vector<unsigned long> &coin_values) {
    std::vector<int> bestNumCoins(coin + 1, 0);
    for (int c = 1; c <= coin; ++c) {
        bestNumCoins[c] = INT_MAX;
        for (int j = 0; j < coin_values.size() ; ++j) {
            if (c >= coin_values[j]) {
                if (bestNumCoins[c - coin_values[j]] + 1 < bestNumCoins[c]) {
                    bestNumCoins[c] = bestNumCoins[c - coin_values[j]] + 1;
                }
            }
        }
    }
    return bestNumCoins[coin];
}

int main() {
    // Open file
    std::ifstream infile("input.txt");
    if (!infile || infile.peek() == std::ifstream::traits_type::eof()) {
        std::cout << "File not found or is empty" << std::endl;
        return -1;
    }

    // Init variables
    unsigned long coin = 0;
    std::vector<unsigned long> coin_values;

    infile >> coin;

    std::string temp_str;
    while (std::getline(infile, temp_str, ',')) {
        coin_values.push_back(std::stoul(temp_str));
    }

    int min_amount_of_coins = DPChange(coin, coin_values);
    std::ofstream outfile("output.txt");
    outfile << min_amount_of_coins << std::endl;
    std::cout << min_amount_of_coins << std::endl;
    return 0;
}