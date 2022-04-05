#include <iostream>
#include <vector>

struct Edge {
    int to_vertex;
    int flow;
    int capacity;
    int back;
};

const int kDistancePoison = -1;
const int kParentPoison = -1;
const int kInf = 1000000;

class IGraph {
public:
    virtual void AddEdge(int from_vertex, int to_vertex, int max_flow_v) = 0;
    virtual int& GetFlowOfBackEdge(Edge edge) = 0;
    virtual std::vector<Edge> Neibhors(int vertex) const = 0;
    virtual std::vector<Edge>& Neibhors(int vertex) = 0;
    virtual size_t GetNumberOfVertexes() const = 0;
    virtual size_t GetNumberOfEdges() const = 0;
};

class Graph final : public IGraph {
    std::vector<std::vector<Edge>> graph_;
    int number_of_vertexes_ = 0;
    int number_of_edges_ = 0;

public:
    explicit Graph(int vertex_number) {
        graph_.resize(vertex_number);
        number_of_vertexes_ = vertex_number;
    }

    void AddEdge(int from_v, int to_v, int max_flow_v) final {
        graph_[from_v].push_back({to_v, 0, max_flow_v, static_cast<int>(graph_[to_v].size())});
        graph_[to_v].push_back({from_v, 0, 0, static_cast<int>(graph_[from_v].size() - 1)});
        ++number_of_edges_;
    }

    std::vector<Edge> Neibhors(int vertex) const final {
        return graph_[vertex];
    }

    std::vector<Edge>& Neibhors(int vertex) final {
        return graph_[vertex];
    }

    int& GetFlowOfBackEdge(Edge edge) final {
        return graph_[edge.to_vertex][edge.back].flow;
    }

    size_t GetNumberOfVertexes() const final {
        return number_of_vertexes_;
    }

    size_t GetNumberOfEdges() const final {
        return number_of_edges_;
    }
};

int FindFlow(IGraph& graph, std::vector<bool>& is_visited, int from_vertex, int to_vertex, int flow) {
    if (from_vertex == to_vertex) {
        return flow;
    }
    is_visited[from_vertex] = true;
    int res = 0;
    for (auto& edge : graph.Neibhors(from_vertex)) {
        if (!is_visited[edge.to_vertex] && (edge.capacity - edge.flow) > 0) {
            int new_from_vertex = edge.to_vertex;
            int new_flow = std::min(flow, edge.capacity - edge.flow);
            int min_result = FindFlow(graph, is_visited, new_from_vertex, to_vertex, new_flow);
            if (min_result > 0) {
                graph.GetFlowOfBackEdge(edge) -= min_result;
                edge.flow += min_result;
                res = min_result;
                break;
            }
        }
    }
    return res;
}

int MaxFlow(IGraph& graph, int from_vertex, int to_vertex) {
    int max_flow = 0;
    int cur_result = 0;
    std::vector<bool> is_visited(graph.GetNumberOfVertexes(), false);
    cur_result = FindFlow(graph, is_visited, from_vertex, to_vertex, kInf);
    while (cur_result) {
        is_visited.assign(graph.GetNumberOfVertexes(), false);
        max_flow += cur_result;
        cur_result = FindFlow(graph, is_visited, from_vertex, to_vertex, kInf);
    }
    return max_flow;
}

int main() {
    int num_of_vertex = 0;
    int num_of_edge = 0;
    std::cin >> num_of_vertex >> num_of_edge;
    Graph graph(num_of_vertex);
    int from_vertex = 0;
    int to_vertex = 0;
    int max_flow = 0;
    for (int i = 0; i < num_of_edge; ++i) {
        std::cin >> from_vertex >> to_vertex >> max_flow;
        graph.AddEdge(from_vertex - 1, to_vertex - 1, max_flow);
    }
    std::cout << MaxFlow(graph, 0, num_of_vertex - 1) << std::endl;
    return 0;
}