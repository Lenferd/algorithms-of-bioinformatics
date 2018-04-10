#include <fstream>
#include <iostream>
#include <vector>
#include <map>
#include <sstream>
#include <stack>
#include <list>
#include <algorithm>
#include <set>

enum t_Errors {
    ALL_OK,
    EMPTY,
};

class Graph_vert {
public:
    uint vert{};
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

    friend bool operator==(const Graph_vert& left, const Graph_vert& right) {
        if (left.vert == right.vert && left.child_vert.size() == right.child_vert.size()) {
            return true;
        }
    };
};

void split_str(const std::string& source, uint & vertex, std::vector<uint> & child_vert) {
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

    // Check is all child is vertex or add it

    std::set<uint> all_childs;
    for (const auto& it: overlap_graph) {
        for (const auto& child : it.child_vert) {
            all_childs.insert(child);
        }
    }

    for (const auto& child: all_childs) {
        for (auto it = overlap_graph.begin(); it != overlap_graph.end(); it++) {
            if (it->vert == child) {
                break;
            } else if (it == --overlap_graph.end()) {
                overlap_graph.emplace_back(Graph_vert(child));
            }
        }
    }

}

void EulerianCycle(std::list<Graph_vert>& overlap_graph, std::vector<uint> &result) {
    std::stack<Graph_vert *> stack;

    stack.push(&overlap_graph.back());

    while (!stack.empty()) {
        Graph_vert* vert = stack.top();
        if (vert->child_vert.empty()) {
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

void get_rate(std::list<Graph_vert>& overlap_graph,
              std::map<uint, uint>& vertex_grade_in,
              std::map<uint, uint>& vertex_grade_out) {
    for (const auto& it : overlap_graph) {
        if (!it.child_vert.empty()) {
            for (int i = 0; i < it.child_vert.size(); ++i) {
                ++vertex_grade_out[it.vert];
                ++vertex_grade_in[it.child_vert[i]];
            }
        }
    }
}

int main() {
    std::list<Graph_vert> overlap_graph;
    read_graph(overlap_graph);

//        for (auto &it : overlap_graph) {
//        std::cout << it.vert << " -> ";
//        for (int i = 0; i < it.child_vert.size(); ++i) {
//            std::cout << it.child_vert[i];
//            if (it.child_vert.size() > 1 && i < it.child_vert.size() - 1) {
//                std::cout << ",";
//            }
//        }
//        std::cout << std::endl;
//    }


    std::map<uint, uint> vertex_grade_in, vertex_grade_out;
    get_rate(overlap_graph, vertex_grade_in, vertex_grade_out);
//    for (const auto& it: vertex_grade_out) {
//        std::cout << it.first <<" " << it.second  << std::endl;
//    }
//    std::cout << "in" << std::endl;
//    for (const auto& it: vertex_grade_in) {
//        std::cout << it.first <<" " << it.second  << std::endl;
//    }

    std::vector<std::tuple<uint, uint>> pairs;
    for (const auto& out_it : vertex_grade_out) {
        if (out_it.second > vertex_grade_in[out_it.first]) {
            for (const auto& in_it : vertex_grade_in) {
                if (in_it.second > vertex_grade_out[in_it.first]) {
                    for (auto & item : overlap_graph) {
                        if (item.vert == in_it.first) {
                            item.child_vert.push_back(out_it.first);
                            ++vertex_grade_in[out_it.first];
                            ++vertex_grade_out[in_it.first];
                            pairs.push_back(std::make_tuple(in_it.first, out_it.first));
                            break;
                        }
                    }
                    break;
                }
            }
        }
    }

    std::vector<uint> result;
    EulerianCycle(overlap_graph, result);

    std::reverse(result.begin(), result.end());

    std::vector<uint> realresult;

    if (pairs.size() > 1) {
        std::cout << "Not valid situation "<< std::endl;
        return -1;
    }

    for (int i = 0; i < result.size() - 1; ++i) {
        if (pairs.empty())
            break;
        if (result[i] == std::get<0>(pairs[0]) && result[i+1] == std::get<1>(pairs[0])) {
            realresult.assign(&result[i + 1], &result[result.size()]);
            realresult.insert(realresult.end(), &result[0], &result[i]);
            break;
        }
    }

//    for (int j = 0; j < realresult.size(); ++j) {
//        std::cout << realresult[j] ;
//        if (j < realresult.size() - 1)
//            std::cout << "->";
//    }
//    std::cout << std::endl;

    std::ofstream outfile("output.txt");
    for (int j = 0; j < realresult.size(); ++j) {
        outfile << realresult[j] ;
        if (j < realresult.size() - 1)
            outfile << "->";
    }
    outfile << std::endl;
    outfile.close();
}