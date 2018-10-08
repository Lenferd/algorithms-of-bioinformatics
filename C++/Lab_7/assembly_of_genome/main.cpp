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

template <typename T>
class Graph_vert {
public:
    T vert{};
    std::vector<T> child_vert;

    explicit Graph_vert() = default;

    explicit Graph_vert(const T vert){
        this->vert = vert;
    }

    Graph_vert(T vert, std::vector<T>& childs) {
        this->vert = vert;
        this->child_vert.insert(this->child_vert.begin(), childs.begin(), childs.end());
    }

    t_Errors get_child_remove_edge(T &first_child) {
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

template <typename T>
void read_overlap_graph(std::list<Graph_vert<T>>& overlap_graph, ulong & k_mer_size) {
    // Open file
    std::ifstream infile("input.txt");
    if (!infile || infile.peek() == std::ifstream::traits_type::eof()) {
        std::cout << "File not found or is empty" << std::endl;
        exit(-1);
    }

    std::vector<T> k_mers_spectr;

    // Read file
    ulong overlap_size;
    infile >>  k_mer_size;
    overlap_size = k_mer_size - 1;

    while (!infile.eof()){
        std::string temp_str;
        infile >> temp_str;
        if (!temp_str.empty()) {
            k_mers_spectr.push_back(temp_str);
        }
    }

    std::map<T, std::vector<T>> temp_graph;
    for (const auto& word: k_mers_spectr) {
        temp_graph[word.substr(0, overlap_size)].push_back(word.substr(word.size() - overlap_size, overlap_size));
    }

    for (auto& it : temp_graph) {
        overlap_graph.emplace_back(Graph_vert<T>(it.first, it.second));
    }

        // Check is all child is vertex or add it

    std::set<T> all_childs;
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
                overlap_graph.emplace_back(Graph_vert<T>(child));
            }
        }
    }
}

template <typename T>
void EulerianCycle(std::list<Graph_vert<T> >& overlap_graph, std::vector<T> &result) {

    std::stack<Graph_vert<T> *> stack;

    stack.push(&overlap_graph.back());

    while (!stack.empty()) {
        Graph_vert<T> * vert = stack.top();
        if (vert->child_vert.empty()) {
            result.push_back(vert->vert);
            stack.pop();
        } else {
            T child;
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

template <typename T>
void get_rate(std::list<Graph_vert <T>>& overlap_graph,
              std::map<T, uint>& vertex_grade_in,
              std::map<T, uint>& vertex_grade_out) {
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
    std::list<Graph_vert<std::string> > overlap_graph;
    ulong k_mer_size;
    read_overlap_graph(overlap_graph, k_mer_size);

    std::map<std::string, uint> vertex_grade_in, vertex_grade_out;
    get_rate(overlap_graph, vertex_grade_in, vertex_grade_out);

    std::vector<std::tuple<std::string, std::string>> pairs;
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

    std::vector<std::string> result;
    EulerianCycle(overlap_graph, result);

    std::reverse(result.begin(), result.end());
    std::vector<std::string> realresult;

    if (pairs.size() > 1) {
        std::cout << "Not valid situation "<< std::endl;
        return -1;
    }

    if (!pairs.empty()) {
        for (int i = 0; i < result.size() - 1; ++i) {
            if (pairs.empty())
                break;
            if (result[i] == std::get<0>(pairs[0]) && result[i + 1] == std::get<1>(pairs[0])) {
                realresult.assign(&result[i + 1], &result[result.size()]);
                realresult.insert(realresult.end(), &result[0], &result[i]);
                break;
            }
        }
    } else {
        realresult = result;
    }

    ulong overlap_size = k_mer_size - 1;

    if (realresult.empty()) {
        std::cout << "Something go wrong" << std::endl;
        exit(-2);
    }

    std::string result_str;
    result_str.assign(&realresult[0][0], &realresult[0][overlap_size]);
    for (ulong k = 1; k < realresult.size(); ++k) {
        result_str += realresult[k][realresult[k].length() - 1];
    }

//    std::cout << result_str << std::endl;

    std::ofstream outfile("output.txt");
    outfile<< result_str << std::endl;
    outfile.close();
}