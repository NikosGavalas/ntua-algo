#include <iostream>
#include <list>
#include <stack>
#include <utility>
#include <vector>

// I know this is not a good practice but saves typing
using namespace std;

class Graph {
    int V;
    list<int> *adj;

    // Fill stack with vertices in increasing order of finishing times (i.e.
    // top element has maximum finishing time)
    void fill_order(int v, bool visited[], stack<int> &st) {
        visited[v] = true;

        for (auto &neighbor : adj[v]) {
            if (!visited[neighbor])
                fill_order(neighbor, visited, st);
        }

        st.push(v);
    }

    void util_DFS(int v, bool visited[], vector<int> &accum) {
        visited[v] = true;
        accum.push_back(v);

        // Recur for all the vertices adjacent to this vertex
        for (auto &neighbor : adj[v]) {
            if (!visited[neighbor])
                util_DFS(neighbor, visited, accum);
        }
    }

   public:
    Graph(int V) {
        this->V = V;
        adj = new list<int>[V];
    }

    void add_edge(int source, int dest) {
        adj[source].push_back(dest);
    }

    // The main function that finds and prints strongly connected
    // components
    void find_SCCs(vector<vector<int>> &ret) {
        stack<int> st;

        bool *visited = new bool[V];
        for (int i = 0; i < V; ++i)
            visited[i] = false;

        for (int i = 0; i < V; ++i)
            if (visited[i] == false)
                fill_order(i, visited, st);

        Graph gr = get_reverse();

        for (int i = 0; i < V; ++i)
            visited[i] = false;

        while (!st.empty()) {
            int v = st.top();
            st.pop();

            // Print Strongly connected component of the popped vertex
            if (!visited[v]) {
                vector<int> accum;
                gr.util_DFS(v, visited, accum);
                ret.push_back(accum);
            }
        }
    }

    // Reverse (or transpose) graph
    Graph get_reverse() {
        Graph g(V);
        for (int v = 0; v < V; v++) {
            for (auto &neighbor : adj[v]) {
                g.add_edge(neighbor, v);
            }
        }
        return g;
    }
};

int main(int argc, char const *argv[]) {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int N;
    cin >> N;

    Graph graph(N);

    for (int i = 0; i < N; ++i) {
        int teams;
        cin >> teams;
        for (int j = 0; j < teams; ++j) {
            int team;
            cin >> team;
            team--;
            graph.add_edge(i, team);
        }
    }

    // vector of vectors of vertices (each forming an SCC)
    vector<vector<int>> sccs;
    graph.find_SCCs(sccs);
    // the number of vertices in the last SCC is the answer
    cout << sccs[sccs.size() - 1].size() << "\n";

    return 0;
}
