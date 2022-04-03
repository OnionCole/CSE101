//-----------------------------------------------------------------------------
// Cole Anderson
// comander
// pa6
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
	int tn = 1;

	printf(TRUE == TRUE ? "%3d:         pass\n" : "%3d: FAIL\n", tn); tn++;

	Dictionary D = newDictionary(FALSE);
	printf(TRUE == TRUE ? "%3d:         pass\n" : "%3d: FAIL\n", tn); tn++;
	
	printf(size(D) == 0 ? "%3d:         pass\n" : "%3d: FAIL\n", tn); tn++;
	printf(getUnique(D) == FALSE ? "%3d:         pass\n" : "%3d: FAIL\n", tn); tn++;
	
	insert(D, "50", 0);
	insert(D, "75", 1);
	insert(D, "80", 2);
	insert(D, "65", 3);
	insert(D, "30", 4);
	insert(D, "20", 5);
	insert(D, "45", 6);
	printf(size(D) == 7 ? "%3d:         pass\n" : "%3d: FAIL\n", tn); tn++;
	printf(lookup(D, "50") == 0 ? "%3d:         pass\n" : "%3d: FAIL\n", tn); tn++;
	printf(lookup(D, "75") == 1 ? "%3d:         pass\n" : "%3d: FAIL\n", tn); tn++;
	printf(lookup(D, "80") == 2 ? "%3d:         pass\n" : "%3d: FAIL\n", tn); tn++;
	printf(lookup(D, "65") == 3 ? "%3d:         pass\n" : "%3d: FAIL\n", tn); tn++;
	printf(lookup(D, "30") == 4 ? "%3d:         pass\n" : "%3d: FAIL\n", tn); tn++;
	printf(lookup(D, "20") == 5 ? "%3d:         pass\n" : "%3d: FAIL\n", tn); tn++;
	printf(lookup(D, "45") == 6 ? "%3d:         pass\n" : "%3d: FAIL\n", tn); tn++;

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
	printf(lookup(D, "75") == 1 ? "%3d:         pass\n" : "%3d: FAIL\n", tn); tn++;
	printf(lookup(D, "65") == 3 ? "%3d:         pass\n" : "%3d: FAIL\n", tn); tn++;
	printf(lookup(D, "30") == 4 ? "%3d:         pass\n" : "%3d: FAIL\n", tn); tn++;
	printf(lookup(D, "20") == 5 ? "%3d:         pass\n" : "%3d: FAIL\n", tn); tn++;
	printf(lookup(D, "45") == 6 ? "%3d:         pass\n" : "%3d: FAIL\n", tn); tn++;

	makeEmpty(D);
	printf(size(D) == 0 ? "%3d:         pass\n" : "%3d: FAIL\n", tn); tn++;

	FILE* out = fopen("DictionaryTestOUT", "w");
	if (out == NULL) {
		printf("ERROR: DictionaryTest.c: Unable to open out file\n");
		exit(EXIT_FAILURE);
	}
	printDictionary(out, D);
	fclose(out);
	remove("DictionaryTestOUT");
	printf(TRUE == TRUE ? "%3d:         pass\n" : "%3d: FAIL\n", tn); tn++;

	freeDictionary(&D);
	printf(TRUE == TRUE ? "%3d:         pass\n" : "%3d: FAIL\n", tn); tn++;
}
