#include <iostream>
#include <vector>
#include <ctime>
#include <climits>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <list>
#include <map>

#define NIL -1
#define BSIZE 1 << 15

char buffer[BSIZE];
long bpos = 0L, bsize = 0L;
/* function for efficient reading from http://corelab.ntua.gr/courses/algorithms/problemsets/read.c */
long readLong()
{
    long d = 0L, x = 0L;
    char c;

    while (1)
    {
        if (bpos >= bsize)
        {
            bpos = 0;
            if (feof(stdin))
                return x;
            bsize = fread(buffer, 1, BSIZE, stdin);
        }
        c = buffer[bpos++];
        if (c >= '0' && c <= '9')
        {
            x = x * 10 + (c - '0');
            d = 1;
        }
        else if (d == 1)
            return x;
    }
    return -1;
}

using namespace std;

int N, M, K;
struct Edge
{
    int src, dest;
    long weight;

    bool operator==(const Edge &o) const
    {
        return src == o.src && dest == o.dest;
    }

    bool operator<(const Edge &o) const
    {
        return src < o.src || (src == o.src && dest < o.dest);
    }

    int x, y;
};

void pritnEdge(Edge myEdge)
{
    cout << "(" << myEdge.src << "," << myEdge.dest << ")=" << myEdge.weight;
}

/*takes an edge and fix to be sorted in ascending oreder*/
struct Edge fixEdge(struct Edge myEdge)
{
    int swap;
    /*cout << "initial edge is  ";
    pritnEdge(myEdge);
    cout <<"!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n";*/
    if (myEdge.src > myEdge.dest)
    {
        swap = myEdge.src;
        myEdge.src = myEdge.dest;
        myEdge.dest = swap;
    }

    /*cout << "i fixed that to ";
    pritnEdge(myEdge);
    cout <<"!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n";*/
    return myEdge;
}

struct subset
{
    int parent;
    int rank;
};

int numCyrcles = 0;
int numBridges = 0;
struct subset *subsets_old;
struct subset *subsets_new;

map<Edge, int> myCurEdges;

/* sorting function modified from http://geeksquiz.com/merge-sort/  */
void merge(struct Edge[], int l, int m, int r);
void mergeSort(struct Edge[], int l, int r);

void Union(struct subset[], int, int);
int find(struct subset[], int);

Edge *result;
int useless, notNes, nes, tempNes;

int *forestEdges;

struct Graph
{
    // V-> Number of vertices, E-> Number of edges
    int V, E;

    // graph is represented as an array of edges. Since the graph is
    // undirected, the edge from src to dest is also edge from dest
    // to src. Both are counted as 1 edge here.
    struct Edge *edge;
};

// Creates a graph with V vertices and E edges
struct Graph *createGraph(int V, int E)
{
    struct Graph *graph = (struct Graph *)malloc(sizeof(struct Graph));
    graph->V = V;
    graph->E = E;

    graph->edge = (struct Edge *)malloc(graph->E * sizeof(struct Edge));

    return graph;
}

void findAll(struct Graph *graph);
int checkForCyrcle(struct Edge e); //takes an edge and checks if there is cyrcle in forest because of it.

// A C++ program to find bridges in a given undirected graph

/* class and bridge-finding function modified from  http://www.geeksforgeeks.org/bridge-in-a-graph/ */
// A class that represents an undirected graph
class mGraph
{
    int V;          // No. of vertices
    list<int> *adj; // A dynamic array of adjacency lists
    void bridgeUtil(int v, bool visited[], int disc[], int low[], int parent[]);

  public:
    mGraph(int V);              // Constructor
    void addEdge(int v, int w); // function to add an edge to graph
    void bridge();              // prints all bridges
};

mGraph::mGraph(int V)
{
    this->V = V;
    adj = new list<int>[V];
}

void mGraph::addEdge(int v, int w)
{
    adj[v].push_back(w);
    adj[w].push_back(v); // Note: the graph is undirected
}

// A recursive function that finds and prints bridges using DFS traversal
// u --> The vertex to be visited next
// visited[] --> keeps tract of visited vertices
// disc[] --> Stores discovery times of visited vertices
// parent[] --> Stores parent vertices in DFS tree
void mGraph::bridgeUtil(int u, bool visited[], int disc[],
                        int low[], int parent[])
{
    // A static variable is used for simplicity, we can avoid use of static
    // variable by passing a pointer.
    static int time = 0;

    // Mark the current node as visited
    visited[u] = true;

    // Initialize discovery time and low value
    disc[u] = low[u] = ++time;

    // Go through all vertices aadjacent to this
    list<int>::iterator i;
    for (i = adj[u].begin(); i != adj[u].end(); ++i)
    {
        int v = *i; // v is current adjacent of u

        // If v is not visited yet, then recur for it
        if (!visited[v])
        {
            parent[v] = u;
            bridgeUtil(v, visited, disc, low, parent);

            // Check if the subtree rooted with v has a connection to
            // one of the ancestors of u
            low[u] = min(low[u], low[v]);

            // If the lowest vertex reachable from subtree under v is
            // below u in DFS tree, then u-v is a bridge
            if (low[v] > disc[u])
            {
                //cout << u <<" " << v << endl;
                numBridges++;
                struct Edge tempEdge;
                tempEdge.src = u;
                tempEdge.dest = v;
                tempEdge = fixEdge(tempEdge);
                //cout << " my hash is " <<myCurEdges[tempEdge]<<"\n";
                if (myCurEdges[tempEdge] > 1)
                {
                    numBridges--;
                    //cout <<"I found parallel : "<< u <<" " << v << endl;
                }
            }
        }

        // Update low value of u for parent function calls.
        else if (v != parent[u])
            low[u] = min(low[u], disc[v]);
    }
}

// DFS based function to find all bridges. It uses recursive function bridgeUtil()
void mGraph::bridge()
{
    // Mark all the vertices as not visited
    bool *visited = new bool[V];
    int *disc = new int[V];
    int *low = new int[V];
    int *parent = new int[V];

    // Initialize parent and visited arrays
    for (int i = 0; i < V; i++)
    {
        parent[i] = NIL;
        visited[i] = false;
    }

    // Call the recursive helper function to find Bridges
    // in DFS tree rooted with vertex 'i'
    for (int i = 0; i < V; i++)
        if (visited[i] == false)
            bridgeUtil(i, visited, disc, low, parent);

    free(visited);
    free(disc);
    free(low);
    free(parent);
    //clear the map
    myCurEdges.clear();
}

int main()
{

    cin >> N >> M; //read input
    //cout << N << " "<<M;
    //cout << "\n";
    struct Graph *graph = createGraph(N, M);

    result = new Edge[M];

    useless = notNes = nes = 0;

    if (M < N)
    {
        cout << M << "\n";
        cout << useless << "\n";
        cout << numCyrcles << "\n";
        return 0;
    }

    //read input and create the graph
    for (int i = 0; i < M; i++)
    {
        graph->edge[i].src = readLong();
        graph->edge[i].dest = readLong();
        graph->edge[i].weight = readLong();
        graph->edge[i].src--;
        graph->edge[i].dest--;
    }

    findAll(graph);

    numCyrcles = M - useless - numBridges;

    cout << numBridges << "\n";
    cout << useless << "\n";
    cout << numCyrcles << "\n";

    return 0;
}

/* this function do the job */
void findAll(struct Graph *graph)
{

    mergeSort(graph->edge, 0, M - 1);

    // Allocate memory for creating N ssubsets
    subsets_old =
        (struct subset *)malloc(N * sizeof(struct subset));

    subsets_new =
        (struct subset *)malloc(N * sizeof(struct subset));

    // Create V subsets with single elements
    for (int v = 0; v < N; ++v)
    {
        subsets_new[v].parent = v;
        subsets_new[v].rank = 0;
        subsets_old[v].parent = v;
        subsets_old[v].rank = 0;
    }

    for (int v = 0; v < N; ++v)
    {
        subsets_new[v].parent = v;
        subsets_new[v].rank = 0;
        subsets_old[v].parent = v;
        subsets_old[v].rank = 0;
    }

    int goMan = 1;

    int iter = 0;
    //cout <<"Before while 1 \n";
    /*this will check each of the teams of all same weighted edges*/
    while (iter < M)
    { //this is cautious
        goMan = 1;
        int inerIter = 0;
        int tempResult = 0;
        int tempLength = 0;
        mGraph tempGraph(N); //?????

        tempResult = checkForCyrcle(graph->edge[iter]); // this needs union find!!!
        //cout <<"while1: checkForCyrcle finished\n";
        //cout <<"while1: pass2\n";
        if (tempResult)
        { //addToTempGraph(graph->edge[iter]);

            int start = find(subsets_old, graph->edge[iter].src);
            int finish = find(subsets_old, graph->edge[iter].dest);

            struct Edge nowEdge;
            nowEdge.src = start;
            nowEdge.dest = finish;
            nowEdge.weight = graph->edge[iter].weight;

            nowEdge = fixEdge(nowEdge);
            myCurEdges[nowEdge]++;

            tempLength++;

            tempLength++;

            tempGraph.addEdge(start, finish);
        }

        iter++;

        //cout <<"Before while 2 \n";

        /*this will check each edge of the current team if it causes circle in the forest*/
        while (goMan && iter < M)
        {
            //cout <<"while2:start\n";
            tempResult = 0; // shows if there is cyrcle in the forest!

            if (graph->edge[iter - 1].weight == graph->edge[iter].weight)
            {
                //cout <<"1. while2\n";
                tempResult = checkForCyrcle(graph->edge[iter]);
                if (tempResult)
                { //addToTempGraph(graph->edge[iter]);

                    int start = find(subsets_old, graph->edge[iter].src);
                    int finish = find(subsets_old, graph->edge[iter].dest);
                    struct Edge nowEdge;
                    nowEdge.src = start;
                    nowEdge.dest = finish;
                    nowEdge.weight = graph->edge[iter].weight;

                    nowEdge = fixEdge(nowEdge);
                    myCurEdges[nowEdge]++;

                    tempLength++;

                    tempLength++;

                    tempGraph.addEdge(start, finish);
                }

                inerIter++; // do that only when there is an equallity
            }
            else
            {
                goMan = 0; //do that when there is not equallity and you have to go on
                iter--;
            }

            iter++; // do that in all cases ?????*/
        }

        tempNes = tempLength;
        /*this will check the team and will find bridges(=Nesessary) */
        if (tempLength > 1)
            tempGraph.bridge();
        /* update the forest by adding the team that already finished*/
        memcpy((void *)subsets_old, (void *)subsets_new, N * sizeof(struct subset));
    }
}

/* takes an edge and check if there is a circle in the previous forest because of it*/
int checkForCyrcle(struct Edge e)
{ //this must implements union-find

    struct Edge next_edge = e;
    //cout <<"\nCheckForCircle: start\n";

    int x = find(subsets_old, next_edge.src);
    if (x == -1)
    {
        //cout <<"\nCheckForCircle: x==-1\n";
        return 1; //no more edges
    }
    //cout <<"CheckForCircle: pass1\n";
    int y = find(subsets_old, next_edge.dest);
    if (y == -1)
    {
        //cout <<"\nCheckForCircle: y==-1\n";
        return 1; //no more edges
    }
    //cout <<"CheckForCircle: pass1\n";
    // If including this edge does't cause cycle, include it
    // in result and increment the index of result for next edge
    if (x != y)
    {
        Union(subsets_new, x, y);
        //cout <<"\nCheckForCircle: finish with adding edge "<<x<<","<<y<<"\n";
        return 1;
    }
    // Else discard the next_edge
    else
    { // we have found a useless edge
        useless++;
        //cout << "\nCheckForCircle: useless="<<useless<<"\n";
        //cout <<"\nCheckForCircle: finish with useless edge\n";
        return 0;
    }
}

// A utility function to find set of an element i
// (uses path compression technique)
int find(struct subset subsets[], int i)
{
    //cout <<"Find: pass0\n";
    if (i > M)
        return (-1); //we may have no more edges!!!  Is that right???? Maybe not!!!
    // find root and make root as parent of i (path compression)
    //cout <<"Find: pass1\n";
    //cout <<"Find: i="<<i<<"\n";
    if (subsets[i].parent != i)
    {
        //cout <<"Find: pass2\n";
        subsets[i].parent = find(subsets, subsets[i].parent);
    }
    //cout <<"Find: pass3\n";
    return subsets[i].parent;
}

// A function that does union of two sets of x and y
// (uses union by rank)
void Union(struct subset subsets[], int x, int y)
{
    int xroot = find(subsets, x);
    int yroot = find(subsets, y);

    // Attach smaller rank tree under root of high rank tree
    // (Union by Rank)
    if (subsets[xroot].rank < subsets[yroot].rank)
        subsets[xroot].parent = yroot;
    else if (subsets[xroot].rank > subsets[yroot].rank)
        subsets[yroot].parent = xroot;

    // If ranks are same, then make one as root and increment
    // its rank by one
    else
    {
        subsets[yroot].parent = xroot;
        subsets[xroot].rank++;
    }
}

/* Function to merge the two haves arr[l..m] and arr[m+1..r] of array arr[] */
void merge(struct Edge arr[], int l, int m, int r)
{
    int i, j, k;
    int n1 = m - l + 1;
    int n2 = r - m;

    /* create temp arrays */
    struct Edge L[n1], R[n2];

    /* Copy data to temp arrays L[] and R[] */
    for (i = 0; i < n1; i++)
        L[i] = arr[l + i];
    for (j = 0; j < n2; j++)
        R[j] = arr[m + 1 + j];

    /* Merge the temp arrays back into arr[l..r]*/
    i = 0;
    j = 0;
    k = l;
    while (i < n1 && j < n2)
    {
        if (L[i].weight <= R[j].weight)
        {
            arr[k] = L[i];
            i++;
        }
        else
        {
            arr[k] = R[j];
            j++;
        }
        k++;
    }

    /* Copy the remaining elements of L[], if there are any */
    while (i < n1)
    {
        arr[k] = L[i];
        i++;
        k++;
    }

    /* Copy the remaining elements of R[], if there are any */
    while (j < n2)
    {
        arr[k] = R[j];
        j++;
        k++;
    }
}

/* l is for left index and r is right index of the sub-array
  of arr to be sorted */
void mergeSort(struct Edge arr[], int l, int r)
{
    if (l < r)
    {
        int m = l + (r - l) / 2; //Same as (l+r)/2, but avoids overflow for large l and h
        mergeSort(arr, l, m);
        mergeSort(arr, m + 1, r);
        merge(arr, l, m, r);
    }
}
