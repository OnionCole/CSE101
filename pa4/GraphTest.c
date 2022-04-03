//-----------------------------------------------------------------------------
// Cole Anderson
// comander
// pa4
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

	int order = 7;
	Graph G = newGraph(order);
	List S = newList();


	printf(getOrder(G) == order ? "%3d:         pass\n" : "%3d: FAIL\n", tn); tn++;
	printf(getSize(G) == 0 ? "%3d:         pass\n" : "%3d: FAIL\n", tn); tn++;
	printf(getParent(G, 1) == NIL ? "%3d:         pass\n" : "%3d: FAIL\n", tn); tn++;
	printf(getDiscover(G, 1) == UNDEF ? "%3d:         pass\n" : "%3d: FAIL\n", tn); tn++;
	printf(getFinish(G, 1) == UNDEF ? "%3d:         pass\n" : "%3d: FAIL\n", tn); tn++;

	addArc(G, 1, 4);
	addEdge(G, 4, 3);
	addEdge(G, 4, 5);
	addArc(G, 5, 3);
	addArc(G, 5, 6);
	addArc(G, 3, 2);
	addArc(G, 6, 2);
	addArc(G, 6, 7);
	for (int i = 1; i < order + 1; i++) {
		append(S, i);
	}
	DFS(G, S);

	printf(getOrder(G) == order ? "%3d:         pass\n" : "%3d: FAIL\n", tn); tn++;
	printf(getSize(G) == 8 ? "%3d:         pass\n" : "%3d: FAIL\n", tn); tn++;
	printf(getParent(G, 1) == NIL ? "%3d:         pass\n" : "%3d: FAIL\n", tn); tn++;
	printf(getParent(G, 2) == 3 ? "%3d:         pass\n" : "%3d: FAIL\n", tn); tn++;
	printf(getParent(G, 3) == 4 ? "%3d:         pass\n" : "%3d: FAIL\n", tn); tn++;
	printf(getParent(G, 4) == 1 ? "%3d:         pass\n" : "%3d: FAIL\n", tn); tn++;
	printf(getParent(G, 5) == 4 ? "%3d:         pass\n" : "%3d: FAIL\n", tn); tn++;
	printf(getParent(G, 6) == 5 ? "%3d:         pass\n" : "%3d: FAIL\n", tn); tn++;
	printf(getParent(G, 7) == 6 ? "%3d:         pass\n" : "%3d: FAIL\n", tn); tn++;
	printf(getDiscover(G, 1) == 1 ? "%3d:         pass\n" : "%3d: FAIL\n", tn); tn++;
	printf(getDiscover(G, 2) == 4 ? "%3d:         pass\n" : "%3d: FAIL\n", tn); tn++;
	printf(getDiscover(G, 3) == 3 ? "%3d:         pass\n" : "%3d: FAIL\n", tn); tn++;
	printf(getDiscover(G, 4) == 2 ? "%3d:         pass\n" : "%3d: FAIL\n", tn); tn++;
	printf(getDiscover(G, 5) == 7 ? "%3d:         pass\n" : "%3d: FAIL\n", tn); tn++;
	printf(getDiscover(G, 6) == 8 ? "%3d:         pass\n" : "%3d: FAIL\n", tn); tn++;
	printf(getDiscover(G, 7) == 9 ? "%3d:         pass\n" : "%3d: FAIL\n", tn); tn++;
	printf(getFinish(G, 1) == 14 ? "%3d:         pass\n" : "%3d: FAIL\n", tn); tn++;
	printf(getFinish(G, 2) == 5 ? "%3d:         pass\n" : "%3d: FAIL\n", tn); tn++;
	printf(getFinish(G, 3) == 6 ? "%3d:         pass\n" : "%3d: FAIL\n", tn); tn++;
	printf(getFinish(G, 4) == 13 ? "%3d:         pass\n" : "%3d: FAIL\n", tn); tn++;
	printf(getFinish(G, 5) == 12 ? "%3d:         pass\n" : "%3d: FAIL\n", tn); tn++;
	printf(getFinish(G, 6) == 11 ? "%3d:         pass\n" : "%3d: FAIL\n", tn); tn++;
	printf(getFinish(G, 7) == 10 ? "%3d:         pass\n" : "%3d: FAIL\n", tn); tn++;

	Graph C = copyGraph(G);
	printf(getOrder(C) == order ? "%3d:         pass\n" : "%3d: FAIL\n", tn); tn++;
	printf(getSize(C) == 8 ? "%3d:         pass\n" : "%3d: FAIL\n", tn); tn++;
	printf(getParent(C, 1) == NIL ? "%3d:         pass\n" : "%3d: FAIL\n", tn); tn++;
	printf(getDiscover(C, 1) == UNDEF ? "%3d:         pass\n" : "%3d: FAIL\n", tn); tn++;
	printf(getFinish(C, 1) == UNDEF ? "%3d:         pass\n" : "%3d: FAIL\n", tn); tn++;
	freeGraph(&C);

	Graph T = transpose(G);
	printf(getOrder(T) == order ? "%3d:         pass\n" : "%3d: FAIL\n", tn); tn++;
	printf(getSize(T) == 8 ? "%3d:         pass\n" : "%3d: FAIL\n", tn); tn++;
	printf(getParent(T, 1) == NIL ? "%3d:         pass\n" : "%3d: FAIL\n", tn); tn++;
	printf(getDiscover(T, 1) == UNDEF ? "%3d:         pass\n" : "%3d: FAIL\n", tn); tn++;
	printf(getFinish(T, 1) == UNDEF ? "%3d:         pass\n" : "%3d: FAIL\n", tn); tn++;
	freeGraph(&T);

	FILE* out = fopen("GraphTestOUT", "w");
	if (out == NULL) {
		printf("ERROR: GraphTest.c: Unable to open out file\n");
		exit(EXIT_FAILURE);
	}
	printGraph(out, G);
	fclose(out);
	remove("GraphTestOUT");
	printf(TRUE == TRUE ? "%3d:         pass\n" : "%3d: FAIL\n", tn); tn++;

	freeGraph(&G);
	freeList(&S);
	printf(G == NULL ? "%3d:         pass\n" : "%3d: FAIL\n", tn); tn++;
	printf(S == NULL ? "%3d:         pass\n" : "%3d: FAIL\n", tn); tn++;
}
