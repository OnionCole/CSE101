//-----------------------------------------------------------------------------
// Cole Anderson
// comander
// pa4
//
// Graph.c
// Implementation file for Graph ADT
//-----------------------------------------------------------------------------


// includes -------------------------------------------------------------------
#include<stdio.h>
#include<stdlib.h>

#include "List.h"

#include "Graph.h"


// macros ---------------------------------------------------------------------
#define TRUE 1
#define FALSE 0

#define WHITE 'W'
#define GREY 'G'
#define BLACK 'B'


// structs --------------------------------------------------------------------

// private GraphObj type
typedef struct GraphObj {
    // note that for all vertex arrays, the length is vertices + 1, as
    //          there is no value at the 0 index

    int order;  // the number of vertices
    int size;  // the number of edges (number of connections)
    List* connections;  // an entry for each vertex, with the values of the List
            // being the vertices that that vertex connects to, sorted in
            // increasing order

    int* parents;  // the parent vertex of each vertex
    char* colors;  // the "color" of each vertex
    int* discovers;  // the discover time for each vertex
    int* finishes;  // the finish time for each vertex
} GraphObj;


// Helper funcs ---------------------------------------------------------------

// visit all child nodes with depth first search, returns "time" value,
//          should be run only in course of a depth first search
int Visit(Graph G, List S, int vertex, int time) {
    if (G == NULL) {
        printf("ERROR: Graph ADT: calling Visit(): NULL Graph\n");
        exit(EXIT_FAILURE);
    }
    if (S == NULL) {
        printf("ERROR: Graph ADT: calling Visit(): NULL List\n");
        exit(EXIT_FAILURE);
    }

    time++;
    G->discovers[vertex] = time;
    G->colors[vertex] = GREY;
    List vertexConnections = G->connections[vertex];
    for (moveFront(vertexConnections); index(vertexConnections) >= 0; 
            moveNext(vertexConnections)) {
        if (G->colors[get(vertexConnections)] == WHITE) {
            G->parents[get(vertexConnections)] = vertex;
            time = Visit(G, S, get(vertexConnections), time);
        }
    }
    G->colors[vertex] = BLACK;
    time++;
    G->finishes[vertex] = time;
    prepend(S, vertex);
    return time;
}


// Constructors-Destructors ---------------------------------------------------

// Returns a Graph pointing to a newly created GraphObj representing a graph having
//			n vertices and no edges.
Graph newGraph(int n) {
    Graph NG = malloc(sizeof(GraphObj));

    if (NG == NULL) {
        printf("ERROR: Graph ADT: calling newGraph(): malloc returned NULL\n");
        exit(EXIT_FAILURE);
    }

    NG->order = n;
    NG->size = 0;
    NG->connections = (List*)malloc(sizeof(List) * (n + 1));
    
    NG->parents = (int*)malloc(sizeof(int) * (n + 1));
    NG->colors = (char*)malloc(sizeof(char) * (n + 1));
    NG->discovers = (int*)malloc(sizeof(int) * (n + 1));
    NG->finishes = (int*)malloc(sizeof(int) * (n + 1));

    if (NG->connections == NULL || NG->parents == NULL ||
            NG->colors == NULL || NG->discovers == NULL || NG->finishes == NULL) {
        printf("ERROR: Graph ADT: calling newGraph(): malloc returned NULL (2)\n");
        exit(EXIT_FAILURE);
    }

    for (int i = 1; i < n + 1; i++) {
        NG->connections[i] = newList();

        NG->parents[i] = NIL;
        NG->colors[i] = WHITE;
        NG->discovers[i] = UNDEF;
        NG->finishes[i] = UNDEF;
    }

    return NG;
}

// Frees all dynamic memory associated with the Graph *pG, then sets the handle* pG 
//			to NULL.
void freeGraph(Graph* pG) {
    if (pG != NULL && *pG != NULL) {
        for (int i = 1; i < (*pG)->order + 1; i++) {
            freeList(&((*pG)->connections[i]));
        }

        free((*pG)->connections);
        free((*pG)->parents);
        free((*pG)->colors);
        free((*pG)->discovers);
        free((*pG)->finishes);

        free(*pG);
        *pG = NULL;
    }
}


// Access functions -----------------------------------------------------------

// Returns the value of the order field.
int getOrder(Graph G) {
    if (G == NULL) {
        printf("ERROR: Graph ADT: calling getOrder(): NULL Graph\n");
        exit(EXIT_FAILURE);
    }

    return G->order;
}

// Returns the value of the size field.
int getSize(Graph G) {
    if (G == NULL) {
        printf("ERROR: Graph ADT: calling getSize(): NULL Graph\n");
        exit(EXIT_FAILURE);
    }

    return G->size;
}

// Returns the parent of vertex u, can be NIL, or NIL if DFS() has not yet been called.
// PRE: 1 <= u <= getOrder(G)
int getParent(Graph G, int u) {
    if (G == NULL) {
        printf("ERROR: Graph ADT: calling getParent(): NULL Graph\n");
        exit(EXIT_FAILURE);
    }
    if (!(u >= 1 && u <= getOrder(G))) {
        printf("ERROR: Graph ADT: calling getParent(): !(1 <= u <= getOrder(G))\n");
        exit(EXIT_FAILURE);
    }

    return G->parents[u];
}

// Returns the discover of vertex u, or UNDEF if DFS() has not yet been called.
// PRE: 1 <= u <= getOrder(G)
int getDiscover(Graph G, int u) {
    if (G == NULL) {
        printf("ERROR: Graph ADT: calling getDiscover(): NULL Graph\n");
        exit(EXIT_FAILURE);
    }
    if (!(u >= 1 && u <= getOrder(G))) {
        printf("ERROR: Graph ADT: calling getDiscover(): !(1 <= u <= getOrder(G))\n");
        exit(EXIT_FAILURE);
    }

    return G->discovers[u];
}

// Returns the finish of vertex u, or UNDEF if DFS() has not yet been called.
// PRE: 1 <= u <= getOrder(G)
int getFinish(Graph G, int u) {
    if (G == NULL) {
        printf("ERROR: Graph ADT: calling getFinish(): NULL Graph\n");
        exit(EXIT_FAILURE);
    }
    if (!(u >= 1 && u <= getOrder(G))) {
        printf("ERROR: Graph ADT: calling getFinish(): !(1 <= u <= getOrder(G))\n");
        exit(EXIT_FAILURE);
    }

    return G->finishes[u];
}


// Manipulation procedures ----------------------------------------------------

// Inserts a new directed edge from u to v, i.e. v is added to the adjacency List 
//			of u (but not u to the adjacency List of v). (Your program is required 
//          to maintain these lists in sorted order by increasing labels.)
// PRE: 1 <= u <= getOrder(G)
// PRE: 1 <= v <= getOrder(G)
void addArc(Graph G, int u, int v) {
    if (G == NULL) {
        printf("ERROR: Graph ADT: calling addArc(): NULL Graph\n");
        exit(EXIT_FAILURE);
    }
    if (!(u >= 1 && u <= getOrder(G))) {
        printf("ERROR: Graph ADT: calling addArc(): !(1 <= u <= getOrder(G))\n");
        exit(EXIT_FAILURE);
    }
    if (!(v >= 1 && v <= getOrder(G))) {
        printf("ERROR: Graph ADT: calling addArc(): !(1 <= v <= getOrder(G))\n");
        exit(EXIT_FAILURE);
    }

    G->size++;
    List uConnections = G->connections[u];
    for (moveFront(uConnections); index(uConnections) >= 0; moveNext(uConnections)) {
        if (get(uConnections) == v) {  // another arc with same u and v already
                // exists, so no need to change the GraphObj
            return;
        }
        if (get(uConnections) > v) {
            insertBefore(uConnections, v);
            return;
        }
    }
    append(uConnections, v);
}

// Inserts a new edge joining u to v, i.e. u is added to the adjacency List of v, 
//			and v to the adjacency List of u. (Your program is required to maintain 
//			these lists in sorted order by increasing labels.)
// PRE: 1 <= u <= getOrder(G)
// PRE: 1 <= v <= getOrder(G)
void addEdge(Graph G, int u, int v) {
    if (G == NULL) {
        printf("ERROR: Graph ADT: calling addEdge(): NULL Graph\n");
        exit(EXIT_FAILURE);
    }
    if (!(u >= 1 && u <= getOrder(G))) {
        printf("ERROR: Graph ADT: calling addEdge(): !(1 <= u <= getOrder(G))\n");
        exit(EXIT_FAILURE);
    }
    if (!(v >= 1 && v <= getOrder(G))) {
        printf("ERROR: Graph ADT: calling addEdge(): !(1 <= v <= getOrder(G))\n");
        exit(EXIT_FAILURE);
    }

    addArc(G, u, v);
    addArc(G, v, u);
    G->size--;  // each addArc call should increment size once, but since an edge
            // is NOT two arcs, decrement so that total increment is 1 for the one
            // new edge
}

// Performs the depth first search algorithm on G
// S has two purposes: First it defines the order in which vertices are to be 
//			processed in the main loop of DFS (THESE GET POPPED OFF OF S). 
//          Second, when DFS is complete, it will store the vertices by decreasing 
//          finish times (hence S is considered to be a stack). Thus S can be 
//          classified as both an input and output parameter to function DFS().
// DFS() has two preconditions : (i)length(S) == n, and (ii)S contains some 
//			permutation of the integers{ 1, 2, … , n } where n = getOrder(G).
//			You are required to check the first precondition but not the second.
// PRE: length(S) == getOrder(G)
void DFS(Graph G, List S) {
    if (G == NULL) {
        printf("ERROR: Graph ADT: calling DFS(): NULL Graph\n");
        exit(EXIT_FAILURE);
    }
    if (S == NULL) {
        printf("ERROR: Graph ADT: calling DFS(): NULL List\n");
        exit(EXIT_FAILURE);
    }
    if (!(length(S) == getOrder(G))) {
        printf("ERROR: Graph ADT: calling DFS(): length of S not equal to order of G\n");
        exit(EXIT_FAILURE);
    }

    int order = G->order;
    for (int i = 1; i < order + 1; i++) {
        G->parents[i] = NIL;
        G->colors[i] = WHITE;
        G->discovers[i] = UNDEF;
        G->finishes[i] = UNDEF;
    }
    int time = 0;
    for (moveFront(S); index(S) >= 0; moveNext(S)) {
        if (G->colors[get(S)] == WHITE) {
            time = Visit(G, S, get(S), time);
        }
    }
    for (int i = 0; i < order; i++) {  // easier to "pop" at end
        deleteBack(S);
    }
}


// Other operations -----------------------------------------------------------

// Returns a reference to a new graph representing the transpose of G.
Graph transpose(Graph G) {
    if (G == NULL) {
        printf("ERROR: Graph ADT: calling transpose(): NULL Graph\n");
        exit(EXIT_FAILURE);
    }

    Graph NG = malloc(sizeof(GraphObj));

    if (NG == NULL) {
        printf("ERROR: Graph ADT: calling transpose(): malloc returned NULL\n");
        exit(EXIT_FAILURE);
    }

    NG->order = G->order;
    NG->size = 0;  // will fix later in function
    NG->connections = (List*)malloc(sizeof(List) * (G->order + 1));

    NG->parents = (int*)malloc(sizeof(int) * (G->order + 1));
    NG->colors = (char*)malloc(sizeof(char) * (G->order + 1));
    NG->discovers = (int*)malloc(sizeof(int) * (G->order + 1));
    NG->finishes = (int*)malloc(sizeof(int) * (G->order + 1));

    if (NG->connections == NULL || NG->parents == NULL ||
            NG->colors == NULL || NG->discovers == NULL || NG->finishes == NULL) {
        printf("ERROR: Graph ADT: calling transpose(): malloc returned NULL (2)\n");
        exit(EXIT_FAILURE);
    }

    for (int i = 1; i < G->order + 1; i++) {
        NG->connections[i] = newList();

        NG->parents[i] = NIL;
        NG->colors[i] = WHITE;
        NG->discovers[i] = UNDEF;
        NG->finishes[i] = UNDEF;
    }

    for (int i = 1; i < G->order + 1; i++) {
        List vertexConnections = G->connections[i];
        for (moveFront(vertexConnections); index(vertexConnections) >= 0;
                moveNext(vertexConnections)) {
            addArc(NG, get(vertexConnections), i);
        }
    }

    NG->size = G->size;  // size may have been messed up from adding arcs

    return NG;
}

// Returns a reference to a new graph that is a copy of G.
Graph copyGraph(Graph G) {
    if (G == NULL) {
        printf("ERROR: Graph ADT: calling copyGraph(): NULL Graph\n");
        exit(EXIT_FAILURE);
    }

    Graph NG = malloc(sizeof(GraphObj));

    if (NG == NULL) {
        printf("ERROR: Graph ADT: calling copyGraph(): malloc returned NULL\n");
        exit(EXIT_FAILURE);
    }

    NG->order = G->order;
    NG->size = G->size;
    NG->connections = (List*)malloc(sizeof(List) * (G->order + 1));

    NG->parents = (int*)malloc(sizeof(int) * (G->order + 1));
    NG->colors = (char*)malloc(sizeof(char) * (G->order + 1));
    NG->discovers = (int*)malloc(sizeof(int) * (G->order + 1));
    NG->finishes = (int*)malloc(sizeof(int) * (G->order + 1));

    if (NG->connections == NULL || NG->parents == NULL ||
            NG->colors == NULL || NG->discovers == NULL || NG->finishes == NULL) {
        printf("ERROR: Graph ADT: calling copyGraph(): malloc returned NULL (2)\n");
        exit(EXIT_FAILURE);
    }

    for (int i = 1; i < G->order + 1; i++) {
        NG->connections[i] = copyList(G->connections[i]);

        NG->parents[i] = NIL;
        NG->colors[i] = WHITE;
        NG->discovers[i] = UNDEF;
        NG->finishes[i] = UNDEF;
    }

    return NG;
}

// Prints the adjacency list representation of G to the file pointed to by out. 
//			The format of this representation should match the above examples, 
//			so all that is required by the client is a single call to printGraph().
void printGraph(FILE* out, Graph G) {
    if (G == NULL) {
        printf("ERROR: Graph ADT: calling printGraph(): NULL Graph\n");
        exit(EXIT_FAILURE);
    }

    for (int i = 1; i < G->order + 1; i++) {
        fprintf(out, "%d: ", i);
        printList(out, G->connections[i]);
        fprintf(out, "\n");
    }
}
