// This is basically Frankenstein code with quite a few global variables
// (which have no need to be global) and a lot of code merged from
// external sources, but I was on a hurry so...
// see:
// https://www.geeksforgeeks.org/bridge-in-a-graph/
// https://www.geeksforgeeks.org/kruskals-minimum-spanning-tree-algorithm-greedy-algo-2/

#include <algorithm>
#include <iostream>
#include <list>
#include <map>

#include <string.h>  // for memcpy

#define BUFFERSIZE 1 << 15

using namespace std;

/* Fast I/O */
char buffer[BUFFERSIZE];
long bpos = 0L, bsize = 0L;
long readLong() {
    long d = 0L, x = 0L;
    char c;

    while (1) {
        if (bpos >= bsize) {
            bpos = 0;
            if (feof(stdin))
                return x;
            bsize = fread(buffer, 1, BUFFERSIZE, stdin);
        }
        c = buffer[bpos++];
        if (c >= '0' && c <= '9') {
            x = x * 10 + (c - '0');
            d = 1;
        } else if (d == 1)
            return x;
    }
    return -1;
}

class Edge {
   public:
    int src, dest;
    long weight;
    int x, y;

    Edge() {}

    Edge(int src, int dest, long weight) {
        this->src = min(src, dest);
        this->dest = max(src, dest);
        this->weight = weight;
    }

    bool operator<(const Edge &o) const {
        return src < o.src || (src == o.src && dest < o.dest);
    }
};

struct subset {
    int parent;
    int rank;
};

int cycles = 0;
int bridges = 0;
int useless = 0;

struct subset *oldSubsets;
struct subset *newSubsets;

map<Edge, int> currEdges;

/* sorting function modified from http://geeksquiz.com/merge-sort/ */
void merge(Edge arr[], int l, int m, int r) {
    int i, j, k;
    int n1 = m - l + 1;
    int n2 = r - m;

    Edge L[n1], R[n2];

    for (i = 0; i < n1; i++)
        L[i] = arr[l + i];
    for (j = 0; j < n2; j++)
        R[j] = arr[m + 1 + j];

    i = 0;
    j = 0;
    k = l;
    while (i < n1 && j < n2) {
        if (L[i].weight <= R[j].weight) {
            arr[k] = L[i];
            i++;
        } else {
            arr[k] = R[j];
            j++;
        }
        k++;
    }

    while (i < n1) {
        arr[k] = L[i];
        i++;
        k++;
    }

    while (j < n2) {
        arr[k] = R[j];
        j++;
        k++;
    }
}

void mergeSort(Edge arr[], int l, int r) {
    if (l < r) {
        int m = l + (r - l) / 2;
        mergeSort(arr, l, m);
        mergeSort(arr, m + 1, r);
        merge(arr, l, m, r);
    }
}

int find(struct subset subsets[], int i) {
    if (subsets[i].parent != i)
        subsets[i].parent = find(subsets, subsets[i].parent);

    return subsets[i].parent;
}

void Union(struct subset subsets[], int x, int y) {
    int xroot = find(subsets, x);
    int yroot = find(subsets, y);

    if (subsets[xroot].rank < subsets[yroot].rank)
        subsets[xroot].parent = yroot;
    else if (subsets[xroot].rank > subsets[yroot].rank)
        subsets[yroot].parent = xroot;

    else {
        subsets[yroot].parent = xroot;
        subsets[xroot].rank++;
    }
}

class Graph {
   public:
    int V, E;
    Edge *edge;

    Graph(int V, int E) {
        this->V = V;
        this->E = E;

        this->edge = new Edge[this->E];
    }

    void sortEdges() {
        mergeSort(this->edge, 0, this->E - 1);
        /* sort(this->edge, this->edge + this->E - 1, 
            [](Edge const a, Edge const b)
            {
                return a.weight > b.weight;
            }); */
    }
};

class BridgeGraph {
    int V;
    list<int> *adj;

    void bridgeUtil(int u, bool visited[], int disc[],
                    int low[], int parent[]) {
        static int time = 0;

        visited[u] = true;

        disc[u] = low[u] = ++time;

        for (auto i = adj[u].begin(); i != adj[u].end(); ++i) {
            int v = *i;

            if (!visited[v]) {
                parent[v] = u;
                bridgeUtil(v, visited, disc, low, parent);

                low[u] = min(low[u], low[v]);

                if (low[v] > disc[u]) {
                    bridges++;
                    Edge tempEdge(u, v, 1);
                    if (currEdges[tempEdge] > 1)
                        bridges--;
                }
            }

            else if (v != parent[u])
                low[u] = min(low[u], disc[v]);
        }
    }

   public:
    BridgeGraph(int V) {
        this->V = V;
        adj = new list<int>[V];
    }

    void addEdge(int v, int w) {
        adj[v].push_back(w);
        adj[w].push_back(v);
    }

    void bridge() {
        bool *visited = new bool[V];
        int *disc = new int[V];
        int *low = new int[V];
        int *parent = new int[V];

        for (int i = 0; i < V; i++) {
            parent[i] = -1;
            visited[i] = false;
        }

        for (int i = 0; i < V; i++)
            if (visited[i] == false)
                bridgeUtil(i, visited, disc, low, parent);

        delete visited;
        delete disc;
        delete low;
        delete parent;

        currEdges.clear();
    }
};

bool checkIfCausesCycle(Edge e) {
    Edge next_edge = e;

    int x = find(oldSubsets, next_edge.src);
    int y = find(oldSubsets, next_edge.dest);
    if (x == -1 || y == -1)
        return true;

    if (x != y) {
        Union(newSubsets, x, y);
        return true;
    }

    return false;
}

void determineEdgeValues(Graph *graph) {
    graph->sortEdges();

    oldSubsets = new struct subset[graph->V];
    newSubsets = new struct subset[graph->V];

    for (int v = 0; v < graph->V; ++v) {
        newSubsets[v] = {v, 0};
        oldSubsets[v] = {v, 0};
    }

    int iter = 0;

    while (iter < graph->E) {
        int len = 0;
        BridgeGraph tempGraph(graph->V);

        do {
            if (checkIfCausesCycle(graph->edge[iter])) {
                int start = find(oldSubsets, graph->edge[iter].src);
                int finish = find(oldSubsets, graph->edge[iter].dest);

                Edge nowEdge(start, finish, graph->edge[iter].weight);
                currEdges[nowEdge]++;

                len += 2;

                tempGraph.addEdge(start, finish);
            } else
                useless++;
            iter++;
        } while (graph->edge[iter - 1].weight == graph->edge[iter].weight);

        if (len > 1)
            tempGraph.bridge();

        memcpy((void *)oldSubsets, (void *)newSubsets,
               graph->V * sizeof(struct subset));
    }
}

int main() {
    int N, M;
    cin >> N >> M;

    Graph *graph = new Graph(N, M);

    for (int i = 0; i < M; i++) {
        graph->edge[i].src = readLong() - 1;
        graph->edge[i].dest = readLong() - 1;
        graph->edge[i].weight = readLong();
    }

    determineEdgeValues(graph);

    cycles = M - useless - bridges;

    cout << bridges << "\n";
    cout << useless << "\n";
    cout << cycles << "\n";

    return 0;
}
