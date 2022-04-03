//-----------------------------------------------------------------------------
// Cole Anderson
// comander
// pa1
//
//-----------------------------------------------------------------------------

#include<stdio.h>
#include<stdlib.h>
#include <string.h>
#include "List.h"

#define TRUE 1
#define FALSE 0

#define MAX_LEN 3000


int main(int argc, char* argv[]) {
	if (argc != 3) {
		printf("ERROR: Lex.c: Wrong number of command line args\n");
		exit(EXIT_FAILURE);
	}

    // open input file
    FILE* in = fopen(argv[1], "r");
    if (in == NULL) {
        printf("Unable to open file %s for reading\n", argv[1]);
        exit(EXIT_FAILURE);
    }

    // count number of lines in input file
    char line[MAX_LEN];
    int linesNumber = 0;
    while (fgets(line, MAX_LEN, in) != NULL) {
        linesNumber++;
    }

    // close input file
    fclose(in);

    // open input file
    in = fopen(argv[1], "r");
    if (in == NULL) {
        printf("Unable to open file %s for reading\n", argv[1]);
        exit(EXIT_FAILURE);
    }

    // read in input file to array
    char inputFileLines[linesNumber][MAX_LEN];
    int i = 0;
    while (fgets(inputFileLines[i], MAX_LEN, in) != NULL) {
        for (int j = 0; j < MAX_LEN; j++) {
            if (inputFileLines[i][j] == '\n') {
                inputFileLines[i][j] = '\0';
                //break;
            }
        }

        i++;
    }

    // close input file
    fclose(in);

    // build index list
    List indexList = newList();
    if (linesNumber > 0) {
        append(indexList, 0);
        for (i = 1; i < linesNumber; i++) {

            // add another index to the indexList in the proper position
            moveFront(indexList);
            for (;;) {
                if (strcmp(inputFileLines[i], inputFileLines[get(indexList)]) < 0) {  // addingString
                        // comes before this string
                    insertBefore(indexList, i);
                    break;
                }
                if (index(indexList) == i - 1) {
                    append(indexList, i);
                    break;
                }
                moveNext(indexList);
            }
        }
    }
    
    // open output file
    FILE* out = fopen(argv[2], "w");
    if (out == NULL) {
        printf("Unable to open file %s for writing\n", argv[2]);
        exit(EXIT_FAILURE);
    }

    // out
    if (linesNumber == 0) {
#pragma GCC diagnostic ignored "-Wformat-zero-length"
        fprintf(out, "");
#pragma GCC diagnostic warning "-Wformat-zero-length"
    } else {
        moveFront(indexList);
        for (i = 0;; i++) {
            fprintf(out, "%s", inputFileLines[get(indexList)]);
            if (i == linesNumber - 1) {
                break;
            }
            fprintf(out, "\n");
            moveNext(indexList);
        }
    }
    
    // close output file
    fclose(out);

    // free List
    freeList(&indexList);

    // return
    return(0);
}
