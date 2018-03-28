#include <iostream>
#include <fstream>
#include <vector>

#define IND_SO_W(i, j) ((i) * (m + 1) + (j) ) // index for south_weight
#define IND_WE_W(i, j) ((i) * m + (j) )  // index for west_weight
#define IND_VERT(i, j) ((i) * (m + 1) + (j)) // index for vertex_weight

int ManhattanTourist(std::vector<int> &south_weight, std::vector<int> &west_weight,
                     int n, int m) {
    std::vector<int> vertex_weight((n + 1) * (m + 1), 0);

    for (int i = 1; i <= n; ++i) {
        vertex_weight[IND_VERT(i,0)] =
                vertex_weight[IND_VERT(i - 1, 0)] + south_weight[IND_SO_W(i - 1, 0)];
    }

    for (int i = 0; i <= m ; ++i) {
        vertex_weight[IND_VERT(0, i)] =
                vertex_weight[IND_VERT(0, i - 1)] + west_weight[IND_WE_W(0, i - 1)];
    }

    for (int i = 1; i <= n ; ++i) {
        for (int j = 1; j <= m; ++j) {
            vertex_weight[IND_VERT(i, j)] =
                    std::max(
                            vertex_weight[IND_VERT(i - 1, j)] + south_weight[IND_SO_W(i - 1, j)],
                            vertex_weight[IND_VERT(i, j - 1)] + west_weight[IND_WE_W(i, j - 1)]
            );
        }

    }

    return vertex_weight[IND_VERT(n,m)];
}

int main() {
    // Open file
    std::ifstream infile("input.txt");
    if (!infile || infile.peek() == std::ifstream::traits_type::eof()) {
        std::cout << "File not found or is empty" << std::endl;
        return -1;
    }

    int n = 0, m = 0;
    infile >> n >> m;

    std::vector<int> south_weight(n * (m + 1), 0);
    std::vector<int> west_weight((n + 1) * m, 0);

    for (int i = 0; i < south_weight.size(); ++i) {
        infile >> south_weight[i];
    }

    char temp_val;
    infile >> temp_val;

    for (int j = 0; j < west_weight.size(); ++j) {
        infile >> west_weight[j];
    }
    infile.close();

    int best_way = ManhattanTourist(south_weight, west_weight, n, m);
    std::ofstream outfile("output.txt");
    outfile << best_way << std::endl;
    outfile.close();
    return 0;
}