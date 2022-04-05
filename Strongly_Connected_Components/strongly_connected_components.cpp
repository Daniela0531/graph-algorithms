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

enum Colors {
    WHITE,
    GRAY,
    BLACK,
};

void Dfs(Graph& graph, int vertex, std::vector<int>& path, std::vector<Colors>& colors) {
    colors[vertex] = GRAY;
    for(auto vertex_neighbor : graph.Neighbors(vertex)) {
        if (colors[vertex_neighbor.vertex] == WHITE) {
            Dfs(graph, vertex_neighbor.vertex, path, colors);
        }
    }
    path.push_back(vertex);
    colors[vertex] = BLACK;
}

std::vector<int> VisitingAllVertexes(Graph& graph) {
    std::vector<int> vertexes_in_visiting_order;
    std::vector<Colors> colors(graph.GetNumberOfVertixes(), WHITE);
    for(int i = 0; i < graph.GetNumberOfVertixes(); ++i) {
        if (colors[i] == WHITE) {
            Dfs(graph, i, vertexes_in_visiting_order, colors);
        }
    }
    return vertexes_in_visiting_order;
}

int StrongConnectivityComponents(Graph& graph, std::vector<int>& components_numbers_to_which_vertexes_belong) {
    std::vector<int> vertexes_in_visiting_order = VisitingAllVertexes(graph);
    reverse(vertexes_in_visiting_order.begin(), vertexes_in_visiting_order.end());

    int number_of_vertexes = graph.GetNumberOfVertixes();
    GraphList transposed_graph(number_of_vertexes);
    for(int vertex = 0; vertex < number_of_vertexes; ++vertex) {
        for (auto vertex_neighbor : graph.Neighbors(vertex)) {
            transposed_graph.AddEdge(vertex_neighbor.vertex, vertex);
        }
    }

    components_numbers_to_which_vertexes_belong.resize(number_of_vertexes);
    components_numbers_to_which_vertexes_belong.assign(number_of_vertexes, -1);
    std::vector<Colors> colors(number_of_vertexes, WHITE);
    int sum_of_components = 0;

    for(int component_identifier_vertex : vertexes_in_visiting_order) {
        std::vector<int> vertexes_in_cur_component;
        if (colors[component_identifier_vertex] == WHITE) {
            Dfs(transposed_graph, component_identifier_vertex, vertexes_in_cur_component, colors);
            for(auto vertex : vertexes_in_cur_component) {
                components_numbers_to_which_vertexes_belong[vertex] = sum_of_components;
            }
            ++sum_of_components;
        }
    }
    return sum_of_components;
}


int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);

    int number_of_vertexes = 0;
    int number_of_edges = 0;

    std::cin >> number_of_vertexes >> number_of_edges;
    
    GraphList graph(number_of_vertexes);

    int from_vertex = 0;
    int to_vertex = 0;

    for(int i = 0; i < number_of_edges; ++i) {
        std::cin >> from_vertex >> to_vertex;
        graph.AddEdge(from_vertex - 1, to_vertex - 1);
    }
    
    std::vector<int> components_numbers_to_which_vertexes_belong;
    int sum_of_components = StrongConnectivityComponents(graph, components_numbers_to_which_vertexes_belong);
    std::cout << sum_of_components << std::endl;
    for (int i = 0; i < graph.GetNumberOfVertixes(); ++i) {
        std::cout << components_numbers_to_which_vertexes_belong[i] + 1 << " ";
    }
    std::cout << std::endl;
    return 0;
}
