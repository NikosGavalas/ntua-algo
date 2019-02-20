#include <assert.h>

#include <iostream>
#include <utility>
#include <list>
#include <vector>
#include <queue>
#include <algorithm>
#include <climits>

// TODO: remove this
using namespace std;

typedef pair<int, int> P;

class Graph {
    int V;
    list<P> *adj;
    vector<bool> visited;

public:
    Graph(int V) {
        this->V = V;
        this->adj = new list<P>[V];
        this->visited = vector<bool>(V, false);
    }

    void add_edge(int source, int dest, int dist) {
        this->adj[source].push_back(make_pair(dest, dist));
    }

    // dijkstra
    void find_shortest_distances(int source, vector<int> &results) {
        assert(this->V == (int)results.size());

        priority_queue<P, vector<P>, greater<P>> q;
        // (distance from source vertex, current vertex) pair
        int dist_from_source = 0;
        int curr_vertex = 0;
        results[source] = 0;
        q.push(make_pair(dist_from_source, curr_vertex));

        while (!q.empty()) {
            P popped = q.top();
            q.pop();
            // additions for readability
            dist_from_source = popped.first;
            curr_vertex = popped.second;
            
            if (!this->visited[curr_vertex]) {
                this->visited[curr_vertex] = true;
                for (auto &neighbor : this->adj[curr_vertex]) {
                    int update = dist_from_source + neighbor.second;
                    if (update < results[neighbor.first]) {
                        results[neighbor.first] = update;
                        q.push(make_pair(update, neighbor.first));
                    }
                }
            }
        }
    }
};

int main(int argc, char const *argv[]) {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int N, M, K, Q;
    cin >> N >> M >> K >> Q;

    Graph graph(N * (K + 1));

    int source, dest, dist;
    //TODO: replace postfix with prefix ++
    for (int i = 0; i < M; i++) {
        cin >> source >> dest >> dist;
        // TODO: cleanup this mess here
        for (int k = 0; k <= K; k++) {
            // add all forward edges
            graph.add_edge(source + (k * N) - 1, dest + (k * N) - 1, dist);
            // add heper backward edges
            if (k != K)
                graph.add_edge(dest + (k * N) - 1, source + ((k + 1) * N) - 1, dist);
        }
    }

    vector<int> results = vector<int>(N * (K + 1), INT_MAX);
    graph.find_shortest_distances(0, results);
    for (auto &dist : results)
        cout << dist << "\n";

    int num_offenses;
    for (int i = 0; i < Q; i++) {
        cin >> source >> dest >> num_offenses;
    }

    return 0;
}
