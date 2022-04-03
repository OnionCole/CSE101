//-----------------------------------------------------------------------------
// Cole Anderson
// comander
// pa2
//
// ListTest.c
//
// Just running the functions in different cases, not real unit tests
//-----------------------------------------------------------------------------

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include "List.h"

#define TRUE 1
#define FALSE 0


int main(int argc, char* argv[]) {
	
	List myList = newList();


	printf(length(myList) == 0 ? "001:         pass\n" : "001: FAIL\n");
	printf(index(myList) == -1 ? "002:         pass\n" : "002: FAIL\n");
	
	append(myList, 0);
	append(myList, 1);
	append(myList, 2);
	
	printf(length(myList) == 3 ? "003:         pass\n" : "003: FAIL\n");
	printf(index(myList) == -1 ? "004:         pass\n" : "004: FAIL\n");
	printf(front(myList) == 0 ? "005:         pass\n" : "005: FAIL\n");
	printf(back(myList) == 2 ? "006:         pass\n" : "006: FAIL\n");

	moveFront(myList);
	printf(index(myList) == 0 ? "007:         pass\n" : "007: FAIL\n");
	printf(get(myList) == 0 ? "008:         pass\n" : "008: FAIL\n");
	moveNext(myList);
	printf(index(myList) == 1 ? "009:         pass\n" : "009: FAIL\n");
	printf(get(myList) == 1 ? "010:         pass\n" : "010: FAIL\n");
	moveNext(myList);
	printf(index(myList) == 2 ? "011:         pass\n" : "011: FAIL\n");
	printf(get(myList) == 2 ? "012:         pass\n" : "012: FAIL\n");
	moveNext(myList);
	printf(index(myList) == -1 ? "013:         pass\n" : "013: FAIL\n");

	moveBack(myList);
	printf(index(myList) == 2 ? "014:         pass\n" : "014: FAIL\n");
	printf(get(myList) == 2 ? "015:         pass\n" : "015: FAIL\n");
	movePrev(myList);
	printf(index(myList) == 1 ? "016:         pass\n" : "016: FAIL\n");
	printf(get(myList) == 1 ? "017:         pass\n" : "017: FAIL\n");
	movePrev(myList);
	printf(index(myList) == 0 ? "018:         pass\n" : "018: FAIL\n");
	printf(get(myList) == 0 ? "019:         pass\n" : "019: FAIL\n");
	movePrev(myList);
	printf(index(myList) == -1 ? "020:         pass\n" : "020: FAIL\n");

	deleteFront(myList);
	deleteBack(myList);
	printf(length(myList) == 1 ? "022:         pass\n" : "022: FAIL\n");
	printf(index(myList) == -1 ? "023:         pass\n" : "023: FAIL\n");
	moveFront(myList);
	printf(get(myList) == 1 ? "024:         pass\n" : "024: FAIL\n");

	freeList(&myList);
	myList = newList();
	append(myList, 0);
	append(myList, 1);
	append(myList, 2);
	append(myList, 3);
	append(myList, 4);

	moveBack(myList);
	movePrev(myList);
	delete(myList);
	printf(length(myList) == 4 ? "025:         pass\n" : "025: FAIL\n");
	printf(index(myList) == -1 ? "026:         pass\n" : "026: FAIL\n");

	moveBack(myList);
	insertBefore(myList, 3);
	printf(length(myList) == 5 ? "027:         pass\n" : "027: FAIL\n");
	printf(index(myList) == 4 ? "028:         pass\n" : "028: FAIL\n");
	printf(get(myList) == 4 ? "029:         pass\n" : "029: FAIL\n");
	insertAfter(myList, 5);
	printf(length(myList) == 6 ? "030:         pass\n" : "031: FAIL\n");
	printf(index(myList) == 4 ? "031:         pass\n" : "031: FAIL\n");
	printf(get(myList) == 4 ? "032:         pass\n" : "032: FAIL\n");
	set(myList, 0);
	printf(length(myList) == 6 ? "033:         pass\n" : "033: FAIL\n");
	printf(index(myList) == 4 ? "034:         pass\n" : "034: FAIL\n");
	printf(get(myList) == 0 ? "035:         pass\n" : "035: FAIL\n");
	set(myList, 4);

	prepend(myList, -1);
	printf(length(myList) == 7 ? "036:         pass\n" : "036: FAIL\n");
	printf(index(myList) == 5 ? "037:         pass\n" : "037: FAIL\n");
	printf(get(myList) == 4 ? "038:         pass\n" : "038: FAIL\n");
	printf(front(myList) == -1 ? "039:         pass\n" : "039: FAIL\n");

	List myCopy = copyList(myList);
	printf(listEquals(myList, myCopy) == 1 ? "040:         pass\n" : "040: FAIL\n");
	deleteFront(myList);
	printf(listEquals(myList, myCopy) == 0 ? "041:         pass\n" : "041: FAIL\n");
	freeList(&myCopy);

	clear(myList);
	printf(length(myList) == 0 ? "042:         pass\n" : "042: FAIL\n");
	printf(index(myList) == -1 ? "043:         pass\n" : "043: FAIL\n");



	freeList(&myList);
}
