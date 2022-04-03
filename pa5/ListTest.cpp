//-----------------------------------------------------------------------------
// Cole Anderson
// comander
// pa5
//
// ListTest.cpp
//
// Just running the functions in different cases, not real unit tests
//-----------------------------------------------------------------------------

#include<iostream>
#include<string>
#include"List.h"

using namespace std;


int main(int argc, char* argv[]) {
	int tn = 1;

	List L = List();

	printf(L.isEmpty() == true ? "%3d:         pass\n" : "%3d: FAIL\n", tn); tn++;
	printf(L.size() == 0 ? "%3d:         pass\n" : "%3d: FAIL\n", tn); tn++;
	printf(L.position() == 0 ? "%3d:         pass\n" : "%3d: FAIL\n", tn); tn++;

	L.moveFront();
	printf(L.isEmpty() == true ? "%3d:         pass\n" : "%3d: FAIL\n", tn); tn++;
	printf(L.size() == 0 ? "%3d:         pass\n" : "%3d: FAIL\n", tn); tn++;
	printf(L.position() == 0 ? "%3d:         pass\n" : "%3d: FAIL\n", tn); tn++;

	L.moveBack();
	printf(L.isEmpty() == true ? "%3d:         pass\n" : "%3d: FAIL\n", tn); tn++;
	printf(L.size() == 0 ? "%3d:         pass\n" : "%3d: FAIL\n", tn); tn++;
	printf(L.position() == 0 ? "%3d:         pass\n" : "%3d: FAIL\n", tn); tn++;

	L.insertAfter(1);
	printf(L.peekNext() == 1 ? "%3d:         pass\n" : "%3d: FAIL\n", tn); tn++;
	L.moveNext();
	printf(L.isEmpty() == false ? "%3d:         pass\n" : "%3d: FAIL\n", tn); tn++;
	printf(L.size() == 1 ? "%3d:         pass\n" : "%3d: FAIL\n", tn); tn++;
	printf(L.position() == 1 ? "%3d:         pass\n" : "%3d: FAIL\n", tn); tn++;
	L.eraseBefore();
	printf(L.isEmpty() == true ? "%3d:         pass\n" : "%3d: FAIL\n", tn); tn++;

	L.insertBefore(2);
	printf(L.peekPrev() == 2 ? "%3d:         pass\n" : "%3d: FAIL\n", tn); tn++;
	L.movePrev();
	printf(L.isEmpty() == false ? "%3d:         pass\n" : "%3d: FAIL\n", tn); tn++;
	printf(L.size() == 1 ? "%3d:         pass\n" : "%3d: FAIL\n", tn); tn++;
	printf(L.position() == 0 ? "%3d:         pass\n" : "%3d: FAIL\n", tn); tn++;
	L.eraseAfter();
	printf(L.isEmpty() == true ? "%3d:         pass\n" : "%3d: FAIL\n", tn); tn++;

	L.insertBefore(1);
	L.insertBefore(2);
	L.insertBefore(3);
	L.insertBefore(4);
	L.insertBefore(3);
	L.insertBefore(2);
	L.moveFront();
	L.findNext(4);
	printf(L.position() == 4 ? "%3d:         pass\n" : "%3d: FAIL\n", tn); tn++;
	L.findPrev(2);
	printf(L.position() == 1 ? "%3d:         pass\n" : "%3d: FAIL\n", tn); tn++;
	
	List otherL = List();
	otherL.insertBefore(1);
	otherL.insertBefore(2);
	otherL.insertBefore(3);
	otherL.insertBefore(4);
	L.cleanup();
	printf(L.equals(otherL) ? "%3d:         pass\n" : "%3d: FAIL\n", tn); tn++;
	printf(L == otherL ? "%3d:         pass\n" : "%3d: FAIL\n", tn); tn++;

	printf(L.to_string() == "(1,2,3,4)" ? "%3d:         pass\n" : "%3d: FAIL\n", tn); tn++;

	L.clear();
	printf(L.isEmpty() == true ? "%3d:         pass\n" : "%3d: FAIL\n", tn); tn++;
	printf(L.size() == 0 ? "%3d:         pass\n" : "%3d: FAIL\n", tn); tn++;
	printf(L.position() == 0 ? "%3d:         pass\n" : "%3d: FAIL\n", tn); tn++;

	L.concat(otherL);
	List otherL2 = List(L);
	printf(L == otherL2 ? "%3d:         pass\n" : "%3d: FAIL\n", tn); tn++;

	List C = L;
	printf(L == C ? "%3d:         pass\n" : "%3d: FAIL\n", tn); tn++;

	otherL.~List();
	otherL2.~List();
	printf(true == true ? "%3d:         pass\n" : "%3d: FAIL\n", tn); tn++;
	
	cerr << "\t\t\t\tTEST " << tn << " SPECIAL PRINT: '" << L << "'" << endl;
	printf(true == true ? "%3d:         pass\n" : "%3d: FAIL\n", tn); tn++;
}
