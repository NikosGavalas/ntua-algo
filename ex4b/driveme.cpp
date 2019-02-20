#include <assert.h>

#include <algorithm>
#include <climits>
#include <iostream>
#include <list>
#include <map>
#include <queue>
#include <utility>
#include <vector>

// I know this is not a good practice but saves typing
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
        assert(source < this->V);
        assert(dest < this->V);
        this->adj[source].push_back(make_pair(dest, dist));
    }

    // dijkstra
    void find_shortest_distances(int source, vector<int> &results) {
        assert(this->V == (int)results.size());

        priority_queue<P, vector<P>, greater<P>> q;
        // reset vector
        fill(this->visited.begin(), this->visited.end(), false);
        // (distance from source vertex, current vertex) pair
        int dist_from_source = 0;
        int curr_vertex = source;
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
        cout << source << " " << dest << " " << dist << "\n"; //
        // adjustments for zero indexing
        source--;
        dest--;
        for (int k = 0; k <= K; k++) {
            int step = k * N;
            // add all forward edges
            graph.add_edge(source + step, dest + step, dist);
            // add heper backward edges
            if (k != K)
                graph.add_edge(dest + step, source + step + N, dist);
        }
    }
    cout << "\n"; //

    // source = 5;
    // vector<int> results = vector<int>(N * (K + 1), INT_MAX);
    // graph.find_shortest_distances(source - 1, results);
    // for (auto &dist : results)
    //     cout << dist << "\n";

    int num_offenses;
    map<int, vector<int>> cache;
    for (int i = 0; i < Q; i++) {
        cin >> source >> dest >> num_offenses;
        cout << source << " " << dest << " " << num_offenses << " | "; // DEBUG
        source--;
        dest--;
        if (cache.count(source) == 0) {
            cache[source] = vector<int>(N * (K + 1), INT_MAX);
            graph.find_shortest_distances(source, cache[source]);
        }
        int res = cache[source][dest + (num_offenses * N)];
        if (res == INT_MAX)
            cout << "IMPOSSIBLE" << "\n";
        else
            cout << res << "\n";
    }

    return 0;
}
