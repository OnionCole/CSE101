//-----------------------------------------------------------------------------
// Cole Anderson
// comander
// pa3
//
// GraphTest.c
//
// Just running the functions in different cases, not real unit tests
//-----------------------------------------------------------------------------

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include "Graph.h"

#define TRUE 1
#define FALSE 0


int main(int argc, char* argv[]) {
	int tn = 1;

	Graph G = newGraph(7);


	printf(getOrder(G) == 7 ? "%3d:         pass\n" : "%3d: FAIL\n", tn); tn++;
	printf(getSize(G) == 0 ? "%3d:         pass\n" : "%3d: FAIL\n", tn); tn++;
	printf(getSource(G) == NIL ? "%3d:         pass\n" : "%3d: FAIL\n", tn); tn++;
	printf(getParent(G, 1) == NIL ? "%3d:         pass\n" : "%3d: FAIL\n", tn); tn++;
	printf(getDist(G, 1) == INF ? "%3d:         pass\n" : "%3d: FAIL\n", tn); tn++;

	addArc(G, 1, 4);
	addEdge(G, 4, 3);
	addEdge(G, 4, 5);
	addArc(G, 5, 3);
	addArc(G, 5, 6);
	addArc(G, 3, 2);
	addArc(G, 6, 2);
	addArc(G, 6, 7);
	BFS(G, 5);

	printf(getOrder(G) == 7 ? "%3d:         pass\n" : "%3d: FAIL\n", tn); tn++;
	printf(getSize(G) == 8 ? "%3d:         pass\n" : "%3d: FAIL\n", tn); tn++;
	printf(getSource(G) == 5 ? "%3d:         pass\n" : "%3d: FAIL\n", tn); tn++;
	printf(getParent(G, 1) == NIL ? "%3d:         pass\n" : "%3d: FAIL\n", tn); tn++;
	printf(getParent(G, 2) == 3 ? "%3d:         pass\n" : "%3d: FAIL\n", tn); tn++;
	printf(getParent(G, 3) == 5 ? "%3d:         pass\n" : "%3d: FAIL\n", tn); tn++;
	printf(getParent(G, 4) == 5 ? "%3d:         pass\n" : "%3d: FAIL\n", tn); tn++;
	printf(getParent(G, 5) == NIL ? "%3d:         pass\n" : "%3d: FAIL\n", tn); tn++;
	printf(getParent(G, 6) == 5 ? "%3d:         pass\n" : "%3d: FAIL\n", tn); tn++;
	printf(getParent(G, 7) == 6 ? "%3d:         pass\n" : "%3d: FAIL\n", tn); tn++;
	printf(getDist(G, 1) == INF ? "%3d:         pass\n" : "%3d: FAIL\n", tn); tn++;
	printf(getDist(G, 2) == 2 ? "%3d:         pass\n" : "%3d: FAIL\n", tn); tn++;
	printf(getDist(G, 3) == 1 ? "%3d:         pass\n" : "%3d: FAIL\n", tn); tn++;
	printf(getDist(G, 4) == 1 ? "%3d:         pass\n" : "%3d: FAIL\n", tn); tn++;
	printf(getDist(G, 5) == 0 ? "%3d:         pass\n" : "%3d: FAIL\n", tn); tn++;
	printf(getDist(G, 6) == 1 ? "%3d:         pass\n" : "%3d: FAIL\n", tn); tn++;
	printf(getDist(G, 7) == 2 ? "%3d:         pass\n" : "%3d: FAIL\n", tn); tn++;
	printf(getDist(G, 7) == 2 ? "%3d:         pass\n" : "%3d: FAIL\n", tn); tn++;
	List L = newList();
	getPath(L, G, 2);
	moveFront(L);
	printf(get(L) == 5 ? "%3d:         pass\n" : "%3d: FAIL\n", tn); tn++;
	moveNext(L);
	printf(get(L) == 3 ? "%3d:         pass\n" : "%3d: FAIL\n", tn); tn++;
	moveNext(L);
	printf(get(L) == 2 ? "%3d:         pass\n" : "%3d: FAIL\n", tn); tn++;
	freeList(&L);

	FILE* out = fopen("GraphTestOUT", "w");
	if (out == NULL) {
		printf("ERROR: GraphTest.c: Unable to open out file\n");
		exit(EXIT_FAILURE);
	}
	printGraph(out, G);
	fclose(out);
	remove("GraphTestOUT");
	printf(TRUE == TRUE ? "%3d:         pass\n" : "%3d: FAIL\n", tn); tn++;

	makeNull(G);
	printf(getOrder(G) == 7 ? "%3d:         pass\n" : "%3d: FAIL\n", tn); tn++;
	printf(getSize(G) == 0 ? "%3d:         pass\n" : "%3d: FAIL\n", tn); tn++;
	printf(getSource(G) == NIL ? "%3d:         pass\n" : "%3d: FAIL\n", tn); tn++;
	printf(getParent(G, 1) == NIL ? "%3d:         pass\n" : "%3d: FAIL\n", tn); tn++;
	printf(getDist(G, 1) == INF ? "%3d:         pass\n" : "%3d: FAIL\n", tn); tn++;


	freeGraph(&G);
}
