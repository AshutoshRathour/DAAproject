#include <iostream>
#include <vector>
#include <queue>
#include <climits>
#include <algorithm>
using namespace std;

// ---------------- EDGE STRUCT ----------------
struct Edge {
    int dest_id;
    int distance;
};

// ---------------- GRAPH CLASS ----------------
class Graph {
public:
    int city_count;
    vector<vector<Edge>> adj;

    Graph(int n) {
        city_count = n;
        adj.resize(n);
    }

    void add_edge(int u, int v, int w) {
        adj[u].push_back({v, w});
        adj[v].push_back({u, w}); // undirected graph
    }

    const vector<Edge>& get_edges(int u) const {
        return adj[u];
    }
};

// ---------------- RESULT STRUCT ----------------
struct PathResult {
    vector<int> path;
    int total_distance;
    bool success;
};

// ---------------- DIJKSTRA FUNCTION ----------------
PathResult dijkstra(const Graph& graph, int src, int dest) {
    PathResult result;
    result.total_distance = -1;
    result.success = false;

    if (graph.city_count == 0) return result;

    int n = graph.city_count;
    vector<int> dist(n, INT_MAX);
    vector<int> parent(n, -1);
    vector<bool> visited(n, false);

    // Min Heap (distance, node)
    priority_queue<pair<int,int>, vector<pair<int,int>>, greater<pair<int,int>>> pq;

    dist[src] = 0;
    pq.push({0, src});

    while (!pq.empty()) {
        int u = pq.top().second;
        pq.pop();

        if (visited[u]) continue;
        visited[u] = true;

        if (u == dest) break;

        for (const Edge& edge : graph.get_edges(u)) {
            int v = edge.dest_id;
            int weight = edge.distance;

            if (!visited[v] && dist[u] + weight < dist[v]) {
                dist[v] = dist[u] + weight;
                parent[v] = u;
                pq.push({dist[v], v});
            }
        }
    }

    if (dist[dest] == INT_MAX) return result;

    // Build path
    result.total_distance = dist[dest];
    result.success = true;

    int curr = dest;
    while (curr != -1) {
        result.path.push_back(curr);
        curr = parent[curr];
    }

    reverse(result.path.begin(), result.path.end());
    return result;
}

// ---------------- MAIN FUNCTION ----------------
int main() {
    int n, e;
    cout << "Enter number of cities and roads: ";
    cin >> n >> e;

    Graph graph(n);

    cout << "Enter edges (u v weight):\n";
    for (int i = 0; i < e; i++) {
        int u, v, w;
        cin >> u >> v >> w;
        graph.add_edge(u, v, w);
    }

    int src, dest;
    cout << "Enter source and destination: ";
    cin >> src >> dest;

    PathResult res = dijkstra(graph, src, dest);

    if (!res.success) {
        cout << "No path found\n";
    } else {
        cout << "Shortest Distance: " << res.total_distance << endl;
        cout << "Path: ";
        for (int node : res.path) {
            cout << node << " ";
        }
        cout << endl;
    }

    return 0;
}
