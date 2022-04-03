//-----------------------------------------------------------------------------
// Cole Anderson
// comander
// pa7
//
// Order.c
// Client file for Dictionary ADT
//-----------------------------------------------------------------------------

#include<stdio.h>
#include<stdlib.h>
#include <string.h>
#include "Dictionary.h"

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

    // count number of lines in input file and the number of chars in the longest line
    char line[MAX_LEN];
    int longestLineChars = 1;  // includes null termination
    int linesNumber = 0;
    while (fgets(line, MAX_LEN, in) != NULL) {
        linesNumber++;
        int i = 1;
        for (; i < MAX_LEN + 1 && line[i] != '\0'; i++) {}
        if (i > longestLineChars) {
            longestLineChars = i;
        }
    }

    // close input file
    fclose(in);

    // create Dictionary
    Dictionary D = newDictionary(FALSE);

    // open input file
    in = fopen(argv[1], "r");
    if (in == NULL) {
        printf("Unable to open file %s for reading\n", argv[1]);
        exit(EXIT_FAILURE);
    }

    // read in input file to array
    char(*inputFileLines)[longestLineChars] = malloc(sizeof(char[linesNumber][longestLineChars]));
    //char inputFileLines[linesNumber][longestLineChars];
    for (int i = 0; i < linesNumber; i++) {  // null array
        for (int j = 0; j < longestLineChars + 1; j++) {
            inputFileLines[i][j] = '\0';
        }
    }
    int i = 0;
    while (fgets(inputFileLines[i], longestLineChars + 5, in) != NULL) {
        
        // replace newline for null in string
        for (int j = 0; j < longestLineChars; j++) {
            if (inputFileLines[i][j] == '\n') {
                inputFileLines[i][j] = '\0';
                //break;
            }
        }

        // add pair to Dictionary
        insert(D, inputFileLines[i], VAL_UNDEF);

        // increment
        i++;
    }

    // close input file
    fclose(in);

    // open output file
    FILE* out = fopen(argv[2], "w");
    if (out == NULL) {
        printf("Unable to open file %s for writing\n", argv[2]);
        exit(EXIT_FAILURE);
    }

    // out
    fprintf(out, "******************************************************\nPRE-ORDER:\n"
            "******************************************************\n");
    printDictionary(out, D, "pre");
    fprintf(out, "******************************************************\nIN-ORDER:\n"
            "******************************************************\n");
    printDictionary(out, D, "in");
    fprintf(out, "******************************************************\nPOST-ORDER:\n"
            "******************************************************\n");
    printDictionary(out, D, "post");
    
    // close output file
    fclose(out);

    // free Dictionary
    freeDictionary(&D);

    // free inputFileLines
    free(inputFileLines);

    // return
    return(0);
}
