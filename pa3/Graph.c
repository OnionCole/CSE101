//-----------------------------------------------------------------------------
// Cole Anderson
// comander
// pa3
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

#define INF -2
#define NIL -1

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
    
    int source;  // the most recent source for BFS
    int* parents;  // the parent vertex of each vertex
    int* distances;  // the distance to the most recent source for each vertex
    char* colors;  // the "color" of each vertex
} GraphObj;


// Helper funcs ---------------------------------------------------------------


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
    
    NG->source = NIL;
    NG->parents = (int*)malloc(sizeof(int) * (n + 1));
    NG->distances = (int*)malloc(sizeof(int) * (n + 1));
    NG->colors = (char*)malloc(sizeof(char) * (n + 1));

    if (NG->connections == NULL || NG->parents == NULL ||
            NG->distances == NULL || NG->colors == NULL) {
        printf("ERROR: Graph ADT: calling newGraph(): malloc returned NULL\n");
        exit(EXIT_FAILURE);
    }

    for (int i = 1; i < n + 1; i++) {
        NG->connections[i] = newList();
        NG->parents[i] = NIL;
        NG->distances[i] = INF;
        NG->colors[i] = WHITE;
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
        free((*pG)->distances);
        free((*pG)->colors);

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

// Returns the source vertex most recently used in function BFS(), or NIL if
//			BFS() has not yet been called.
int getSource(Graph G) {
    if (G == NULL) {
        printf("ERROR: Graph ADT: calling getSource(): NULL Graph\n");
        exit(EXIT_FAILURE);
    }

    return G->source;
}

// Returns the parent of vertex u in the BreadthFirst tree created by BFS(), 
//			or NIL if BFS() has not yet been called.
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

// Returns the distance from the most recent BFS source to vertex u, or INF 
//			if BFS() has not yet been called.
// PRE: 1 <= u <= getOrder(G)
int getDist(Graph G, int u) {
    if (G == NULL) {
        printf("ERROR: Graph ADT: calling getDist(): NULL Graph\n");
        exit(EXIT_FAILURE);
    }
    if (!(u >= 1 && u <= getOrder(G))) {
        printf("ERROR: Graph ADT: calling getDist(): !(1 <= u <= getOrder(G))\n");
        exit(EXIT_FAILURE);
    }

    return G->distances[u];
}

// Appends to the List L the vertices of a shortest path in G from source to u, 
//			or appends to L the value NIL if no such path exists.
// PRE: 1 <= u <= getOrder(G)
// PRE: getSource(G)!=NIL (BFS() must be called before getPath())
void getPath(List L, Graph G, int u) {
    if (L == NULL) {
        printf("ERROR: Graph ADT: calling getPath(): NULL List\n");
        exit(EXIT_FAILURE);
    }
    if (G == NULL) {
        printf("ERROR: Graph ADT: calling getPath(): NULL Graph\n");
        exit(EXIT_FAILURE);
    }
    if (!(u >= 1 && u <= getOrder(G))) {
        printf("ERROR: Graph ADT: calling getPath(): !(1 <= u <= getOrder(G))\n");
        exit(EXIT_FAILURE);
    }
    if (!(getSource(G) != NIL)) {
        printf("ERROR: Graph ADT: calling getPath(): no source defined, BFS() not "
                "yet called\n");
        exit(EXIT_FAILURE);
    }

    int dist = getDist(G, u);
    if (dist == INF) {  // there is no path
        append(L, NIL);
        return;
    }
    // here we know that a path does exist
    append(L, u);
    moveBack(L);
    int currentVertex = u;
    for (int i = 0; i < dist; i++) {
        currentVertex = getParent(G, currentVertex);
        insertBefore(L, currentVertex);
        movePrev(L);
    }
}


// Manipulation procedures ----------------------------------------------------

// Deletes all edges of G, restoring it to its original (no edge) state. 
//			(This is called a null graph in graph theory literature).
void makeNull(Graph G) {
    if (G == NULL) {
        printf("ERROR: Graph ADT: calling makeNull(): NULL Graph\n");
        exit(EXIT_FAILURE);
    }

    G->size = 0;
    G->source = NIL;
    for (int i = 1; i < G->order + 1; i++) {
        clear(G->connections[i]);
        G->parents[i] = NIL;
        G->distances[i] = INF;
        G->colors[i] = WHITE;
    }
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

// Runs the BFS algorithm on the Graph G with source s, setting the color, 
//			distance, parent, and source fields of G accordingly.
void BFS(Graph G, int s) {
    if (G == NULL) {
        printf("ERROR: Graph ADT: calling BFS(): NULL Graph\n");
        exit(EXIT_FAILURE);
    }

    G->source = s;
    
    // setup
    char* colors = G->colors;
    int* parents = G->parents;
    int* distances = G->distances;
    for (int i = 1; i < G->order + 1; i++) {
        colors[i] = WHITE;
        parents[i] = NIL;
        distances[i] = INF;
    }
    List queue = newList();
    colors[s] = GREY;
    append(queue, s);

    // algo
    for (; length(queue) > 0;) {
        moveFront(queue);
        int vertex = get(queue);

        List vertexConnections = G->connections[vertex];
        for (moveFront(vertexConnections); index(vertexConnections) >= 0; 
                moveNext(vertexConnections)) {
            if (colors[get(vertexConnections)] == WHITE) {  // add whites to queue
                colors[get(vertexConnections)] = GREY;
                append(queue, get(vertexConnections));
                parents[get(vertexConnections)] = vertex;
            }
        }

        colors[vertex] = BLACK;
        int thisDistance = 0;
        for (int parentVertex = vertex; parentVertex != s; 
                parentVertex = parents[parentVertex], thisDistance++) {}
        distances[vertex] = thisDistance;

        deleteFront(queue);
    }

    // finish
    freeList(&queue);
}


// Other operations -----------------------------------------------------------

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
