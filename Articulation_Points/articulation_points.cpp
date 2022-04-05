#include <bits/stdc++.h>

const int distance_poison = -1;
const int parent_poison = -1;

struct Edge {
    int vertex;
    int edge_cost;
};

class Graph {
public:
    virtual void AddEdge(int from_vertex, int to_vertex, int edge = 1) = 0;
    virtual std::vector<Edge> Neighbors(int vertex) const = 0;
    virtual std::vector<Edge>& Neighbors(int vertex) = 0;
    virtual size_t GetNumberOfVertixes() const = 0;
    virtual size_t GetNumberOfEdges() const = 0;
};

class GraphList : public Graph {
    std::vector<std::vector<Edge>> graph_;
    size_t number_of_vertices = 0;
    size_t number_of_edges = 0;
public:

    GraphList(size_t input_size) {
        graph_.resize(input_size);
        number_of_vertices = input_size;
    }

    void AddEdge(int from_vertex, int to_vertex, int edge_cost = 1) {
        Edge edge = {.vertex = to_vertex, .edge_cost = edge_cost};
        graph_[from_vertex].push_back(edge);
        ++number_of_edges;
    }

    std::vector<Edge> Neighbors(int vertex) const {
        return graph_[vertex];
    }

    std::vector<Edge>& Neighbors(int vertex) {
        return graph_[vertex];
    }

    size_t GetNumberOfVertixes() const {
        return number_of_vertices;
    }

    size_t GetNumberOfEdges() const {
        return number_of_edges;
    }
};

void Dfs(Graph& graph, int vertex, int parent_vertex, int& time, std::vector<int>& res, std::vector<bool>& is_added, std::vector<bool>& is_visited, std::vector<int>& time_in, std::vector<int>& time_up);

std::vector<int> FindBridges(Graph& graph);

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);
    int n = 0, m = 0;
    int left = 0, right = 0;

    std::cin >> n >> m;

    GraphList graph(n);

    for(int i = 1; i <= m; ++i) {
        std::cin >> left >> right;
        graph.AddEdge(left - 1, right - 1);
        graph.AddEdge(right - 1, left - 1);
    }
    std::vector<int> res = FindBridges(graph);
    std::cout << res.size() << std::endl;
    std::sort(res.begin(), res.end());
    for(auto& elem : res) {
        std::cout << elem + 1 << " ";
    }
    std::cout << std::endl;
    return 0;
}



std::vector<int> FindBridges(Graph& graph) {
    int time = 0;
    std::vector<int> time_in(graph.GetNumberOfVertixes());
    std::vector<int> time_up(graph.GetNumberOfVertixes());
    std::vector<bool> is_added(graph.GetNumberOfVertixes(), false);
    std::vector<bool> is_visited(graph.GetNumberOfVertixes(), false);

    std::vector<int> res;
    for(int i = 0; i < graph.GetNumberOfVertixes(); ++i) {
        if (!is_visited[i]) {
            Dfs(graph, i, parent_poison, time, res, is_added, is_visited, time_in, time_up);
        }
    }
    return res;
}

void Dfs(Graph& graph, int vertex, int parent_vertex, int& time, std::vector<int>& res, std::vector<bool>& is_added, std::vector<bool>& is_visited, std::vector<int>& time_in, std::vector<int>& time_up) {
    is_visited[vertex] = true;
    time_in[vertex] = time;
    time_up[vertex] = time++;
    int key_point = 0;
    bool key = false;
    for(auto& vertex_neighbor : graph.Neighbors(vertex)) {
        if (vertex_neighbor.vertex == parent_vertex) {
            if (!key) {
                key = true;
                continue;
            }
        }
        if (!is_visited[vertex_neighbor.vertex]) {
            Dfs(graph, vertex_neighbor.vertex, vertex, time, res, is_added, is_visited, time_in, time_up);
            time_up[vertex] = std::min(time_up[vertex], time_up[vertex_neighbor.vertex]);
            if (parent_vertex != parent_poison) {
                if (time_up[vertex_neighbor.vertex] >= time_in[vertex]) {
                    if (!is_added[vertex]) {
                        res.push_back(vertex);
                        is_added[vertex] = true;
                    }
                }
            }
            ++key_point;
        } else {
            time_up[vertex] = std::min(time_up[vertex], time_in[vertex_neighbor.vertex]);
        }
    }
    if (parent_vertex == parent_poison && key_point > 1) {
        if (!is_added[vertex]) {
            res.push_back(vertex);
            is_added[vertex] = true;
        }
    }
}