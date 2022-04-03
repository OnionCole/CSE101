//-----------------------------------------------------------------------------
// Cole Anderson
// comander
// pa4
//
// Graph.h
// Header file for Graph ADT
//-----------------------------------------------------------------------------

#ifndef GRAPH_H_INCLUDE_
#define GRAPH_H_INCLUDE_


#include "List.h"


#define UNDEF -2
#define NIL -1


// Exported type -------------------------------------------------------------
// Graph reference type
typedef struct GraphObj* Graph;


// Constructors-Destructors ---------------------------------------------------

// Returns a Graph pointing to a newly created GraphObj representing a graph having
//			n vertices and no edges.
Graph newGraph(int n);

// Frees all dynamic memory associated with the Graph *pG, then sets the handle* pG 
//			to NULL.
void freeGraph(Graph* pG);


// Access functions -----------------------------------------------------------

// Returns the value of the order field.
int getOrder(Graph G);

// Returns the value of the size field.
int getSize(Graph G);

// Returns the parent of vertex u, can be NIL, or NIL if DFS() has not yet been called.
// PRE: 1 <= u <= getOrder(G)
int getParent(Graph G, int u);

// Returns the discover of vertex u, or UNDEF if DFS() has not yet been called.
// PRE: 1 <= u <= getOrder(G)
int getDiscover(Graph G, int u);

// Returns the finish of vertex u, or UNDEF if DFS() has not yet been called.
// PRE: 1 <= u <= getOrder(G)
int getFinish(Graph G, int u);


// Manipulation procedures ----------------------------------------------------

// Inserts a new directed edge from u to v, i.e. v is added to the adjacency List 
//			of u (but not u to the adjacency List of v). (Your program is required 
//          to maintain these lists in sorted order by increasing labels.)
// PRE: 1 <= u <= getOrder(G)
// PRE: 1 <= v <= getOrder(G)
void addArc(Graph G, int u, int v);

// Inserts a new edge joining u to v, i.e. u is added to the adjacency List of v, 
//			and v to the adjacency List of u. (Your program is required to maintain 
//			these lists in sorted order by increasing labels.)
// PRE: 1 <= u <= getOrder(G)
// PRE: 1 <= v <= getOrder(G)
void addEdge(Graph G, int u, int v);

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
void DFS(Graph G, List S);


// Other operations -----------------------------------------------------------

// Returns a reference to a new graph representing the transpose of G.
Graph transpose(Graph G);

// Returns a reference to a new graph that is a copy of G.
Graph copyGraph(Graph G);

// Prints the adjacency list representation of G to the file pointed to by out. 
//			The format of this representation should match the above examples, 
//			so all that is required by the client is a single call to printGraph().
void printGraph(FILE* out, Graph G);



#endif
