//-----------------------------------------------------------------------------
// Cole Anderson
// comander
// pa3
//
// FindPath.c
//-----------------------------------------------------------------------------

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include "List.h"
#include "Graph.h"

#define TRUE 1
#define FALSE 0

#define MAX_LEN 30


int main(int argc, char* argv[]) {
    if (argc != 3) {
        printf("ERROR: Graph.c: Wrong number of command line args\n");
        exit(EXIT_FAILURE);
    }

    // open input file
    FILE* in = fopen(argv[1], "r");
    if (in == NULL) {
        printf("ERROR: Graph.c: Unable to open file %s for reading\n", argv[1]);
        exit(EXIT_FAILURE);
    }

    // read in number of vertices
    char readStr[MAX_LEN];
    for (int i = 0; i < MAX_LEN; i++) { readStr[i] = '\0'; }
    fgets(readStr, MAX_LEN, in);
    int vertices = atoi(readStr);

    // create graph
    Graph G = newGraph(vertices);

    // read in and build graph
    for (;;) {
        for (int i = 0; i < MAX_LEN; i++) { readStr[i] = '\0'; }
        fgets(readStr, MAX_LEN, in);
        if (readStr[0] == '0') {
            break;
        }
        char* tempStr = strtok(readStr, " ");
        if (tempStr == NULL) {
            printf("ERROR: Graph.c: Input file is broken\n");
            fclose(in);
            exit(EXIT_FAILURE);
        }
        int u = atoi(tempStr);
        tempStr = strtok(NULL, " ");
        if (tempStr == NULL) {
            printf("ERROR: Graph.c: Input file is broken (2)\n");
            fclose(in);
            exit(EXIT_FAILURE);
        }
        int v = atoi(tempStr);

        addEdge(G, u, v);
    }

    // open output file
    FILE* out = fopen(argv[2], "w");
    if (out == NULL) {
        printf("ERROR: Graph.c: Unable to open file %s for writing\n", argv[2]);
        fclose(in);
        exit(EXIT_FAILURE);
    }

    // print graph
    printGraph(out, G);

    // calculate and print paths
    List pathList = newList();
    for (;;) {
        for (int i = 0; i < MAX_LEN; i++) { readStr[i] = '\0'; }
        fgets(readStr, MAX_LEN, in);
        if (readStr[0] == '0') {
            break;
        }
        char* tempStr = strtok(readStr, " ");
        if (tempStr == NULL) {
            printf("ERROR: Graph.c: Input file is broken (3)\n");
            fclose(in);
            fclose(out);
            exit(EXIT_FAILURE);
        }
        int s = atoi(tempStr);
        tempStr = strtok(NULL, " ");
        if (tempStr == NULL) {
            printf("ERROR: Graph.c: Input file is broken (4)\n");
            fclose(in);
            fclose(out);
            exit(EXIT_FAILURE);
        }
        int d = atoi(tempStr);

        BFS(G, s);
        clear(pathList);
        getPath(pathList, G, d);
        fprintf(out, "\nThe distance from %d to %d is ", s, d);
        int distance = getDist(G, d);
        if (distance == INF) {
            fprintf(out, "infinity");
            fprintf(out, "\nNo %d-%d path exists\n", s, d);
        } else {
            fprintf(out, "%d", distance);
            fprintf(out, "\nA shortest %d-%d path is:", s, d);
            for (moveFront(pathList); index(pathList) >= 0; moveNext(pathList)) {
                fprintf(out, " %d", get(pathList));
            }
            fprintf(out, "\n");
        }
    }
    freeList(&pathList);

    // close input file
    fclose(in);

    // close output file
    fclose(out);

    // out and free computation BigIntegers (BIA and BIB are included)
    freeGraph(&G);

    // return
    return(0);
}
