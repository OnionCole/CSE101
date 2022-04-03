//-----------------------------------------------------------------------------
// Cole Anderson
// comander
// pa4
//
// FindComponents.c
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
        printf("ERROR: FindComponents.c: Wrong number of command line args\n");
        exit(EXIT_FAILURE);
    }

    // open input file
    FILE* in = fopen(argv[1], "r");
    if (in == NULL) {
        printf("ERROR: FindComponents.c: Unable to open file %s for reading\n", argv[1]);
        exit(EXIT_FAILURE);
    }

    // read in number of vertices
    char readStr[MAX_LEN];
    for (int i = 0; i < MAX_LEN; i++) { readStr[i] = '\0'; }
    fgets(readStr, MAX_LEN, in);
    int order = atoi(readStr);

    // create graph
    Graph G = newGraph(order);

    // read in and build graph
    for (;;) {
        for (int i = 0; i < MAX_LEN; i++) { readStr[i] = '\0'; }
        fgets(readStr, MAX_LEN, in);
        if (readStr[0] == '0') {
            break;
        }
        char* tempStr = strtok(readStr, " ");
        if (tempStr == NULL) {
            printf("ERROR: FindComponents.c: Input file is broken\n");
            fclose(in);
            exit(EXIT_FAILURE);
        }
        int u = atoi(tempStr);
        tempStr = strtok(NULL, " ");
        if (tempStr == NULL) {
            printf("ERROR: FindComponents.c: Input file is broken (2)\n");
            fclose(in);
            exit(EXIT_FAILURE);
        }
        int v = atoi(tempStr);

        addArc(G, u, v);
    }

    // close input file
    fclose(in);

    // run necessary depth first searches
    List S = newList();
    for (int i = 1; i < order + 1; i++) {
        append(S, i);
    }
    DFS(G, S);
    List FirstDFSStackOutSave = copyList(S);
    Graph GTranspose = transpose(G);
    DFS(GTranspose, S);
    
    // find number of strong components and build tree from forest from 
    //          depth first search on GTranspose
    int componentsNumber = 0;
    List forestChildren[order + 1];  // the children of each vertex in this forest,
            // each List is sorted in increasing order
    for (int thisVertex = 1; thisVertex < order + 1; thisVertex++) {
        forestChildren[thisVertex] = newList();
    }
    for (moveFront(S); index(S) >= 0; moveNext(S)) {
        int thisVertex = get(S);
        if (getParent(GTranspose, thisVertex) == NIL) {  // this is a root vertex,
                // so count it, but it has no parent so nothing do to regarding the tree
            componentsNumber++;
        } else {  // this is not a root vertex
            List childrenList = forestChildren[getParent(GTranspose, thisVertex)];  // 
                    // list of the children for the parent of this vertex
            append(childrenList, thisVertex);
        }
    }

    // open output file
    FILE* out = fopen(argv[2], "w");
    if (out == NULL) {
        printf("ERROR: FindComponents.c: Unable to open file %s for writing\n", argv[2]);
        exit(EXIT_FAILURE);
    }

    // print graph
    fprintf(out, "Adjacency list representation of G:\n");
    printGraph(out, G);

    // out number of components
    fprintf(out, "\nG contains %d strongly connected components:", componentsNumber);

    // find and out components
    for (int i = 1; i < order + 1; i++) {  // only needs to be done once at top as
            // there is no overlap between trees in the DFS forest
        moveFront(forestChildren[i]);
    }
    List stack = newList();
    int thisComponentNumber = 1;
    for (moveFront(FirstDFSStackOutSave); index(FirstDFSStackOutSave) >= 0;
            moveNext(FirstDFSStackOutSave)) {
        int i = get(FirstDFSStackOutSave);
        if (getParent(GTranspose, i) == NIL) {  // this is a root vertex
            fprintf(out, "\nComponent %d:", thisComponentNumber);
            thisComponentNumber++;

            for (int vertex = i;;) {
                List vertexChildren = forestChildren[vertex];
                if (index(vertexChildren) == -1) {  // (GO UP) if there 
                        // is nothing yet to explore below this node
                    if (length(vertexChildren) == 0) {  // if this is a leaf
                            // (and it is our first time here)
                        fprintf(out, " %d", vertex);  
                    }
                    if (vertex == i) {  // we explored the whole tree
                        break;
                    }
                    vertex = front(stack);
                    deleteFront(stack);
                } else {  // (GO DOWN) there are yet children of this vertex to explore
                    if (index(vertexChildren) == 0) {  // (GO DOWN) if this is 
                            // our first time coming to this vertex (and it is not a leaf)
                        fprintf(out, " %d", vertex);
                    }
                    prepend(stack, vertex);
                    vertex = get(vertexChildren);
                    moveNext(vertexChildren);
                }
            }
        }
    }
    freeList(&stack);

    // close output file
    fclose(out);

    // last frees
    freeGraph(&G);
    freeGraph(&GTranspose);
    freeList(&S);
    freeList(&FirstDFSStackOutSave);
    for (int i = 1; i < order + 1; i++) {
        freeList(&(forestChildren[i]));
    }

    // return
    return(0);
}
