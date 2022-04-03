//-----------------------------------------------------------------------------
// Cole Anderson
// comander
// pa7
//
// DictionaryTest.c
//
// Just running the functions in different cases, not real unit tests
//-----------------------------------------------------------------------------

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include "Dictionary.h"

#define TRUE 1
#define FALSE 0


int main(int argc, char* argv[]) {
	int zero = 0;
	int one = 1;
	int two = 2;
	int three = 3;
	int four = 4;
	int five = 5;
	int six = 6;
	int* zeroP = &zero;
	int* oneP = &one;
	int* twoP = &two;
	int* threeP = &three;
	int* fourP = &four;
	int* fiveP = &five;
	int* sixP = &six;


	int tn = 1;

	printf(TRUE == TRUE ? "%3d:         pass\n" : "%3d: FAIL\n", tn); tn++;

	Dictionary D = newDictionary(TRUE);
	printf(TRUE == TRUE ? "%3d:         pass\n" : "%3d: FAIL\n", tn); tn++;
	
	printf(size(D) == 0 ? "%3d:         pass\n" : "%3d: FAIL\n", tn); tn++;
	printf(getUnique(D) == TRUE ? "%3d:         pass\n" : "%3d: FAIL\n", tn); tn++;
	
	insert(D, "50", zeroP);
	insert(D, "75", oneP);
	insert(D, "30", fourP);
	insert(D, "80", twoP);
	insert(D, "65", threeP);
	insert(D, "20", fiveP);
	insert(D, "45", sixP);
	printf(size(D) == 7 ? "%3d:         pass\n" : "%3d: FAIL\n", tn); tn++;
	printf(lookup(D, "50") == zeroP ? "%3d:         pass\n" : "%3d: FAIL\n", tn); tn++;
	printf(lookup(D, "75") == oneP ? "%3d:         pass\n" : "%3d: FAIL\n", tn); tn++;
	printf(lookup(D, "80") == twoP ? "%3d:         pass\n" : "%3d: FAIL\n", tn); tn++;
	printf(lookup(D, "65") == threeP ? "%3d:         pass\n" : "%3d: FAIL\n", tn); tn++;
	printf(lookup(D, "30") == fourP ? "%3d:         pass\n" : "%3d: FAIL\n", tn); tn++;
	printf(lookup(D, "20") == fiveP ? "%3d:         pass\n" : "%3d: FAIL\n", tn); tn++;
	printf(lookup(D, "45") == sixP ? "%3d:         pass\n" : "%3d: FAIL\n", tn); tn++;

	beginForward(D);
	next(D);
	next(D);
	next(D);
	next(D);
	next(D);
	next(D);
	next(D);
	printf(currentKey(D) == KEY_UNDEF ? "%3d:         pass\n" : "%3d: FAIL\n", tn); tn++;
	printf(currentVal(D) == VAL_UNDEF ? "%3d:         pass\n" : "%3d: FAIL\n", tn); tn++;
	beginReverse(D);
	prev(D);
	prev(D);
	next(D);
	next(D);
	next(D);
	printf(currentKey(D) == KEY_UNDEF ? "%3d:         pass\n" : "%3d: FAIL\n", tn); tn++;
	printf(currentVal(D) == VAL_UNDEF ? "%3d:         pass\n" : "%3d: FAIL\n", tn); tn++;

	delete(D, "80");
	printf(size(D) == 6 ? "%3d:         pass\n" : "%3d: FAIL\n", tn); tn++;
	delete(D, "50");
	printf(size(D) == 5 ? "%3d:         pass\n" : "%3d: FAIL\n", tn); tn++;
	printf(lookup(D, "75") == oneP ? "%3d:         pass\n" : "%3d: FAIL\n", tn); tn++;
	printf(lookup(D, "65") == threeP ? "%3d:         pass\n" : "%3d: FAIL\n", tn); tn++;
	printf(lookup(D, "30") == fourP ? "%3d:         pass\n" : "%3d: FAIL\n", tn); tn++;
	printf(lookup(D, "20") == fiveP ? "%3d:         pass\n" : "%3d: FAIL\n", tn); tn++;
	printf(lookup(D, "45") == sixP ? "%3d:         pass\n" : "%3d: FAIL\n", tn); tn++;

	makeEmpty(D);
	printf(size(D) == 0 ? "%3d:         pass\n" : "%3d: FAIL\n", tn); tn++;

	FILE* out = fopen("DictionaryTestOUT", "w");
	if (out == NULL) {
		printf("ERROR: DictionaryTest.c: Unable to open out file\n");
		exit(EXIT_FAILURE);
	}
	printDictionary(out, D, "foobar");
	fclose(out);
	remove("DictionaryTestOUT");
	printf(TRUE == TRUE ? "%3d:         pass\n" : "%3d: FAIL\n", tn); tn++;

	freeDictionary(&D);
	printf(TRUE == TRUE ? "%3d:         pass\n" : "%3d: FAIL\n", tn); tn++;
}
