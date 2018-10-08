#include <fstream>
#include <iostream>
#include <vector>
#include <map>
#include <sstream>
#include <stack>
#include <list>
#include <algorithm>

enum t_Errors {
    ALL_OK,
    EMPTY,
};

class Graph_vert {
public:
    uint vert;
    std::vector<uint> child_vert;

    explicit Graph_vert() = default;

    explicit Graph_vert(const uint vert){
        this->vert = vert;
    }

    Graph_vert(uint vert, std::vector<uint>& childs) {
        this->vert = vert;
        this->child_vert.insert(this->child_vert.begin(), childs.begin(), childs.end());
    }

    t_Errors get_child_remove_edge(uint &first_child) {
        if (this->child_vert.empty()) {
            return EMPTY;
        }
        first_child = this->child_vert.back();
        this->child_vert.pop_back();
        return ALL_OK;
    }

    unsigned long get_child_size() {
        return this->child_vert.size();
    }

    friend bool operator==(const Graph_vert& left, const Graph_vert& right) {
        if (left.vert == right.vert && left.child_vert.size() == right.child_vert.size()) {
            return true;
        }
    };
};

void split_str(const std::string source, uint & vertex, std::vector<uint> & child_vert) {
    std::istringstream is(source);
    std::string temp;

    std::getline(is, temp, ' ');    // first num
    vertex = (uint)std::stoul(temp);
    std::getline(is, temp, ' ');    // temp ->
    while(std::getline(is, temp, ',')) {
        child_vert.push_back((uint)std::stoul(temp));
    }
}

void read_graph(std::list<Graph_vert>& overlap_graph) {
    // Open file
    std::ifstream infile("input.txt");
    if (!infile || infile.peek() == std::ifstream::traits_type::eof()) {
        std::cout << "File not found or is empty" << std::endl;
        exit(-1);
    }
    uint vertex;
    std::vector<uint> child_vert;

    while (!infile.eof()){
        std::string temp_str;
        std::getline(infile, temp_str);
        if (!temp_str.empty()) {
            vertex = 0;
            child_vert.clear();
            split_str(temp_str, vertex, child_vert);
            overlap_graph.emplace_back(Graph_vert(vertex, child_vert));
        }
    }
}

void EulerianCycle(std::list<Graph_vert>& overlap_graph, std::vector<uint> &result) {
    std::stack<Graph_vert *> stack;

    stack.push(&overlap_graph.back());

    while (!stack.empty()) {
        Graph_vert* vert = stack.top();
        if (vert->get_child_size() == 0) {
            result.push_back(vert->vert);
            stack.pop();
        } else {
            uint child;
            vert->get_child_remove_edge(child);
            for (auto &it: overlap_graph) {
                if (it.vert == child) {
                    stack.push(&it);
                    break;
                }
            }
        }
    }
}

int main() {
    std::list<Graph_vert> overlap_graph;
    read_graph(overlap_graph);

//    for (auto &it : overlap_graph) {
//        std::cout << it.vert << " -> ";
//        for (int i = 0; i < it.child_vert.size(); ++i) {
//            std::cout << it.child_vert[i];
//            if (it.child_vert.size() > 1 && i < it.child_vert.size() - 1) {
//                std::cout << ",";
//            }
//        }
//        std::cout << std::endl;
//    }

    std::vector<uint> result;
    EulerianCycle(overlap_graph, result);

    std::reverse(result.begin(), result.end());
//    for (int j = 0; j < result.size(); ++j) {
//        std::cout << result[j] ;
//        if (j < result.size() - 1)
//            std::cout << "->";
//    }
//    std::cout << std::endl;

    std::ofstream outfile("output.txt");
    for (int j = 0; j < result.size(); ++j) {
        outfile << result[j] ;
        if (j < result.size() - 1)
            outfile << "->";
    }
    outfile << std::endl;
    outfile.close();
}