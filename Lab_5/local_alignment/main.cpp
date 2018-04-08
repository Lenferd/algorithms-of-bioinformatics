#include <fstream>
#include <iostream>
#include <vector>
#include <algorithm>
#include <map>

const int INDEL_PENALTY = -5;
const int BLOSUM62_SIZE = 20;
static const int PAM250[BLOSUM62_SIZE][BLOSUM62_SIZE] = {
        {  2, -2,  0,  0, -3,  1, -1, -1, -1, -2, -1,  0,  1,  0, -2,  1,  1,  0, -6, -3 },
        { -2, 12, -5, -5, -4, -3, -3, -2, -5, -6, -5, -4, -3, -5, -4,  0, -2, -2, -8,  0 },
        {  0, -5,  4,  3, -6,  1,  1, -2,  0, -4, -3,  2, -1,  2, -1,  0,  0, -2, -7, -4 },
        {  0, -5,  3,  4, -5,  0,  1, -2,  0, -3, -2,  1, -1,  2, -1,  0,  0, -2, -7, -4 },
        { -3, -4, -6, -5,  9, -5, -2,  1, -5,  2,  0, -3, -5, -5, -4, -3, -3, -1,  0,  7 },
        {  1, -3,  1,  0, -5,  5, -2, -3, -2, -4, -3,  0,  0, -1, -3,  1,  0, -1, -7, -5 },
        { -1, -3,  1,  1, -2, -2,  6, -2,  0, -2, -2,  2,  0,  3,  2, -1, -1, -2, -3,  0 },
        { -1, -2, -2, -2,  1, -3, -2,  5, -2,  2,  2, -2, -2, -2, -2, -1,  0,  4, -5, -1 },
        { -1, -5,  0,  0, -5, -2,  0, -2,  5, -3,  0,  1, -1,  1,  3,  0,  0, -2, -3, -4 },
        { -2, -6, -4, -3,  2, -4, -2,  2, -3,  6,  4, -3, -3, -2, -3, -3, -2,  2, -2, -1 },
        { -1, -5, -3, -2,  0, -3, -2,  2,  0,  4,  6, -2, -2, -1,  0, -2, -1,  2, -4, -2 },
        {  0, -4,  2,  1, -3,  0,  2, -2,  1, -3, -2,  2,  0,  1,  0,  1,  0, -2, -4, -2 },
        {  1, -3, -1, -1, -5,  0,  0, -2, -1, -3, -2,  0,  6,  0,  0,  1,  0, -1, -6, -5 },
        {  0, -5,  2,  2, -5, -1,  3, -2,  1, -2, -1,  1,  0,  4,  1, -1, -1, -2, -5, -4 },
        { -2, -4, -1, -1, -4, -3,  2, -2,  3, -3,  0,  0,  0,  1,  6,  0, -1, -2,  2, -4 },
        {  1,  0,  0,  0, -3,  1, -1, -1,  0, -3, -2,  1,  1, -1,  0,  2,  1, -1, -2, -3 },
        {  1, -2,  0,  0, -3,  0, -1,  0,  0, -2, -1,  0,  0, -1, -1,  1,  3,  0, -5, -3 },
        {  0, -2, -2, -2, -1, -1, -2,  4, -2,  2,  2, -2, -1, -2, -2, -1,  0,  4, -6, -2 },
        { -6, -8, -7, -7,  0, -7, -3, -5, -3, -2, -4, -4, -6, -5,  2, -2, -5, -6, 17,  0 },
        { -3,  0, -4, -4,  7, -5,  0, -1, -4, -1, -2, -2, -5, -4, -4, -3, -3, -2,  0, 10 }
};

static std::map<char, int> CHR_MAP =
        {{'A', 0},
         {'C', 1},
         {'D', 2},
         {'E', 3},
         {'F', 4},
         {'G', 5},
         {'H', 6},
         {'I', 7},
         {'K', 8},
         {'L', 9},
         {'M', 10},
         {'N', 11},
         {'P', 12},
         {'Q', 13},
         {'R', 14},
         {'S', 15},
         {'T', 16},
         {'V', 17},
         {'W', 18},
         {'Y', 19},
        };

static int get_penalty(char ch1, char ch2) {
    const int line = CHR_MAP[ch1];
    const int column = CHR_MAP[ch2];
    if (line >= BLOSUM62_SIZE || column >= BLOSUM62_SIZE) {
        perror("Can't get penalty from this indexes");
        return 0;
    }
    return PAM250[line][column];
}

#define IND_VERT(i, j) ((i) * (m) + (j)) // index for vertex_weight

enum direction{
    UNKNOWN,
    UP,
    LEFT,
    DIAGONAL,
    TO_START,
};

void PrintLCS(std::string& first_result, std::string& second_result,
              const std::vector<direction>& way,
              const std::string& first_str, const std::string& second_str,
              long i, long j,
              const long n, const long m) {
    if ((i == 0 && j == 0 ) || (way[IND_VERT(i,j)] == TO_START)) {
        return;
    }
    if (way[IND_VERT(i,j)] == DIAGONAL) {
        first_result += first_str[i - 1];
        second_result += second_str[j - 1];
        PrintLCS(first_result, second_result, way, first_str, second_str, i - 1, j - 1, n, m);
    } else {
        if (way[IND_VERT(i, j)] == UP) {
            first_result += first_str[i - 1];
            second_result += "-";
            PrintLCS(first_result, second_result, way, first_str, second_str, i - 1, j, n, m);
        } else {
            first_result += "-";
            second_result += second_str[j - 1];
            PrintLCS(first_result, second_result, way, first_str, second_str, i, j - 1, n, m);
        }
    }
}

void LCS(const std::string &first_s, const std::string &second_s,
                std::string &first_result, std::string &second_result, int& max_score) {
    long n = (long)first_s.size() + 1;   // +1 to indel line/column
    long m = (long)second_s.size() + 1;
    std::vector<int> vertex_weight((unsigned long)n * m, 0);
    std::vector<direction> way((unsigned long)n * m, UNKNOWN);

    vertex_weight[IND_VERT(0,0)] = 0;

    for (int i = 1; i < n; ++i) {
        vertex_weight[IND_VERT(i, 0)] = 0;  // Top indel penalty line
        way[IND_VERT(i, 0)] = TO_START;
    }
    for (int j = 1; j < m; ++j) {
        vertex_weight[IND_VERT(0, j)] = 0;  // Left indel penalty line
        way[IND_VERT(0, j)] = TO_START;
    }
    for (int i = 1; i < n; ++i) {
        for (int j = 1; j < m; ++j) {
            const int move_from_top = vertex_weight[IND_VERT(i - 1, j)] + INDEL_PENALTY;
            const int move_from_left = vertex_weight[IND_VERT(i, j - 1)] + INDEL_PENALTY;

            if (move_from_top >= move_from_left) {
                vertex_weight[IND_VERT(i,j)] = move_from_top;
                way[IND_VERT(i, j)] = UP;
            } else {
                vertex_weight[IND_VERT(i,j)] = move_from_left;
                way[IND_VERT(i, j)] = LEFT;
            }

//            if (first_s[i - 1] == second_s[j - 1]) {
                const int move_diagonal = vertex_weight[IND_VERT(i - 1, j - 1)]
                                    + get_penalty(first_s[i - 1], second_s[j - 1]);
                if (move_diagonal >= vertex_weight[IND_VERT(i,j)]) {
                    vertex_weight[IND_VERT(i,j)] = move_diagonal;
                    way[IND_VERT(i, j)] = DIAGONAL;
                }
//            }

            if (vertex_weight[IND_VERT(i,j)] < 0) {
                vertex_weight[IND_VERT(i,j)] = 0;
                way[IND_VERT(i, j)] = TO_START;
            }
        }
    }

    int start_n = 0;
    int start_m = 0;
    max_score = 0;
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < m; ++j) {
            if (vertex_weight[IND_VERT(i,j)] > max_score) {
                max_score = vertex_weight[IND_VERT(i,j)];
                start_n = i;
                start_m = j;
            }
        }
    }

//    for (int k = 0; k < n; ++k) {
//        for (int i = 0; i < m; ++i) {
//            std::cout << vertex_weight[IND_VERT(k,i)] << "\t";
//        }
//        std::cout << std::endl;
//    }

    PrintLCS(first_result, second_result,
             way,
             first_s, second_s,
             start_n, start_m,
             n, m);
    std::reverse(first_result.begin(), first_result.end());
    std::reverse(second_result.begin(), second_result.end());
}


int main() {
    // Open file
    std::ifstream infile("input.txt");
    if (!infile || infile.peek() == std::ifstream::traits_type::eof()) {
        std::cout << "File not found or is empty" << std::endl;
        return -1;
    }

    std::string fist_str, second_str;
    infile >> fist_str >> second_str;
    infile.close();
    std::string first_result, second_result;
    int score;

    LCS(fist_str, second_str, first_result, second_result, score);

    std::ofstream outfile("output.txt");
    outfile << score << std::endl;
    outfile << first_result << std::endl;
    outfile << second_result << std::endl;
    outfile.close();
    return 0;
}
