//-----------------------------------------------------------------------------
// Cole Anderson
// comander
// pa2
//
// Arithmetic.c
//-----------------------------------------------------------------------------

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include "BigInteger.h"

#define TRUE 1
#define FALSE 0

#define MAX_LEN 100100

const int INPUTS = 4;


int main(int argc, char* argv[]) {
    if (argc != 3) {
        printf("ERROR: Arithmetic.c: Wrong number of command line args\n");
        exit(EXIT_FAILURE);
    }

    // open input file
    FILE* in = fopen(argv[1], "r");
    if (in == NULL) {
        printf("ERROR: Arithmetic.c: Unable to open file %s for reading\n", argv[1]);
        exit(EXIT_FAILURE);
    }

    // read in input file to array
    char inputFileLines[INPUTS][MAX_LEN];
    int i;
    for (i = 0; i < INPUTS && fgets(inputFileLines[i], MAX_LEN, in) != NULL; i++) {
        for (int j = 0; j < MAX_LEN; j++) {
            if (inputFileLines[i][j] == '\n') {
                inputFileLines[i][j] = '\0';
                //break;
            }
        }
    }
    if (i < 4) {
        printf("ERROR: Arithmetic.c: Insufficient inputs\n");
        exit(EXIT_FAILURE);
    }

    // close input file
    fclose(in);

    // build base BigIntegers
    BigInteger BIA = stringToBigInteger(inputFileLines[1]);
    BigInteger BIB = stringToBigInteger(inputFileLines[3]);
    
    // perform computations
    BigInteger computations[11];
    computations[10] = NULL;  // null terminated
    //A
    computations[0] = BIA;
    //B
    computations[1] = BIB;
    //A+B
    computations[2] = sum(BIA, BIB);
    //A-B
    computations[3] = diff(BIA, BIB);
    //A-A
    computations[4] = diff(BIA, BIA);
    //3A-2B
    BigInteger temp1 = stringToBigInteger("3\0");
    BigInteger temp2 = prod(temp1, BIA);
    freeBigInteger(&temp1);
    temp1 = stringToBigInteger("2\0");
    BigInteger temp3 = prod(temp1, BIB);
    freeBigInteger(&temp1);
    computations[5] = diff(temp2, temp3);
    freeBigInteger(&temp2);
    freeBigInteger(&temp3);
    //AB
    computations[6] = prod(BIA, BIB);
    //A^2
    computations[7] = prod(BIA, BIA);
    //B^2
    computations[8] = prod(BIB, BIB);
    //9(A^4)+16(B^5)
    temp1 = prod(BIA, BIA);
    temp2 = prod(temp1, BIA);
    freeBigInteger(&temp1);
    temp1 = prod(temp2, BIA);
    freeBigInteger(&temp2);
    temp2 = stringToBigInteger("9\0");
    temp3 = prod(temp2, temp1);
    freeBigInteger(&temp1);
    freeBigInteger(&temp2);
    temp1 = prod(BIB, BIB);
    temp2 = prod(temp1, BIB);
    freeBigInteger(&temp1);
    temp1 = prod(temp2, BIB);
    freeBigInteger(&temp2);
    temp2 = prod(temp1, BIB);
    freeBigInteger(&temp1);
    temp1 = stringToBigInteger("16\0");
    BigInteger temp4 = prod(temp1, temp2);
    freeBigInteger(&temp1);
    freeBigInteger(&temp2);
    computations[9] = sum(temp3, temp4);
    freeBigInteger(&temp3);
    freeBigInteger(&temp4);

    // open output file
    FILE* out = fopen(argv[2], "w");
    if (out == NULL) {
        printf("ERROR: Arithmetic.c: Unable to open file %s for writing\n", argv[2]);
        exit(EXIT_FAILURE);
    }

    // out and free computation BigIntegers (BIA and BIB are included)
    for (i = 0;;) {
        printBigInteger(out, computations[i]);
        freeBigInteger(&(computations[i]));
        i++;
        if (computations[i] == NULL) {
            break;
        }
        fprintf(out, "\n\n");
    }
    fprintf(out, "\n\n");

    // close output file
    fclose(out);

    // return
    return(0);
}
