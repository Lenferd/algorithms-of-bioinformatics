#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>

#define IND_DIR(i, j) ((i) * (m - 1) + (j) )  // index for direction
#define IND_VERT(i, j) ((i) * (m) + (j)) // index for vertex_weight

enum direction{
    UNKNOWN,
    DOWN,
    RIGHT,
    DIAGONAL,
};

void PrintLCS(std::string &result, const std::vector<direction>& way, const std::string& first_str,
              unsigned long i, unsigned long j, const unsigned long m) {
    if (way[IND_DIR(i,j)] == DIAGONAL) {
        result += first_str[i];
        if (i == 0 || j == 0)   // That's here because index from 0 and should use last direction
            return;
        PrintLCS(result, way, first_str, i - 1, j - 1, m);
    } else {
        if (i == 0 || j == 0)
            return;
        if (way[IND_DIR(i, j)] == DOWN) {
            PrintLCS(result, way, first_str, i - 1, j, m);
        } else {
            PrintLCS(result, way, first_str, i, j - 1, m);
        }
    }
}

std::string LCS(const std::string &first_s, const std::string &second_s) {
    unsigned long n = first_s.size() + 1;
    unsigned long m = second_s.size() + 1;
    std::vector<int> vertex_weight(n * m, 0);
    std::vector<direction> way((n - 1) * ( m - 1), UNKNOWN);

    for (int i = 0; i < n; ++i) {
        vertex_weight[IND_VERT(i, 0)] = 0;
    }
    for (int j = 0; j < m; ++j) {
        vertex_weight[IND_VERT(0, j)] = 0;
    }
    for (int i = 1; i < n; ++i) {
        for (int j = 1; j < m; ++j) {
            vertex_weight[IND_VERT(i,j)] = std::max(
                    vertex_weight[IND_VERT(i - 1, j)],
                    vertex_weight[IND_VERT(i, j - 1)]
            );
            if (first_s[i - 1] == second_s[j - 1]) {
                vertex_weight[IND_VERT(i,j)] = std::max(
                        vertex_weight[IND_VERT(i,j)],
                        vertex_weight[IND_VERT(i - 1, j - 1)] + 1
                );
            }

            //  set direction
            if (vertex_weight[IND_VERT(i, j)] == vertex_weight[IND_VERT(i - 1, j - 1)] + 1) {
                way[IND_DIR(i - 1, j - 1)] = DIAGONAL;
            } else if (vertex_weight[IND_VERT(i, j)] == vertex_weight[IND_VERT(i - 1, j)]) {
                way[IND_DIR(i - 1, j - 1)] = DOWN;
            } else {
                way[IND_DIR(i - 1, j - 1)] = RIGHT;
            }
        }
    }

    std::string result_str;
    PrintLCS(result_str, way, first_s, n - 2, m - 2, m);
    std::reverse(result_str.begin(), result_str.end());

    return result_str;
}

int main() {
    // Open file
    std::ifstream infile("input.txt");
    if (!infile || infile.peek() == std::ifstream::traits_type::eof()) {
        std::cout << "File not found or is empty" << std::endl;
        return -1;
    }

    std::string first_str, second_str;
    infile >> first_str >> second_str;

    infile.close();

    std::string result = LCS(first_str, second_str);
    std::ofstream outfile("output.txt");
    outfile << result << std::endl;
    outfile.close();
    return 0;
}