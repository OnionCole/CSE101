//-----------------------------------------------------------------------------
// Cole Anderson
// comander
// pa2
//
// BigIntegerTest.c
//
// Just running the functions in different cases, not real unit tests
//-----------------------------------------------------------------------------

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include "BigInteger.h"

#define TRUE 1
#define FALSE 0


int main(int argc, char* argv[]) {

	BigInteger myBigInteger = newBigInteger();


	printf(sign(myBigInteger) == 0 ? "001:         pass\n" : "001: FAIL\n");
	freeBigInteger(&myBigInteger);

	myBigInteger = stringToBigInteger("5");
	BigInteger myBigInteger2 = stringToBigInteger("3");
	BigInteger myResult = sum(myBigInteger, myBigInteger2);
	BigInteger expected = stringToBigInteger("8");
	printf(sign(myResult) == 1 ? "002:         pass\n" : "002: FAIL\n");
	printf(equals(myResult, expected) == 1 ? "003:         pass\n" : "003: FAIL\n");
	freeBigInteger(&myResult);
	freeBigInteger(&expected);
	
	myResult = diff(myBigInteger, myBigInteger2);
	expected = stringToBigInteger("2");
	printf(sign(myResult) == 1 ? "004:         pass\n" : "004: FAIL\n");
	printf(equals(myResult, expected) == 1 ? "005:         pass\n" : "005: FAIL\n");
	freeBigInteger(&myResult);
	freeBigInteger(&expected);

	myResult = prod(myBigInteger, myBigInteger2);
	expected = stringToBigInteger("15");
	printf(sign(myResult) == 1 ? "006:         pass\n" : "006: FAIL\n");
	printf(equals(myResult, expected) == 1 ? "007:         pass\n" : "007: FAIL\n");
	freeBigInteger(&myResult);
	freeBigInteger(&expected);

	myResult = copy(myBigInteger);
	expected = stringToBigInteger("5");
	printf(sign(myResult) == 1 ? "008:         pass\n" : "008: FAIL\n");
	printf(equals(myResult, expected) == 1 ? "009:         pass\n" : "009: FAIL\n");
	freeBigInteger(&myResult);
	freeBigInteger(&expected);

	negate(myBigInteger2);
	expected = stringToBigInteger("-3");
	printf(equals(myBigInteger2, expected) == 1 ? "010:         pass\n" : "010: FAIL\n");
	freeBigInteger(&expected);
	negate(myBigInteger2);

	makeZero(myBigInteger2);
	expected = stringToBigInteger("0");
	printf(equals(myBigInteger2, expected) == 1 ? "011:         pass\n" : "011: FAIL\n");
	freeBigInteger(&expected);
	freeBigInteger(&myBigInteger2);
	myBigInteger2 = stringToBigInteger("3");

	printf(compare(myBigInteger, myBigInteger2) == 1 ? "012:         pass\n" : "012: FAIL\n");
	


	freeBigInteger(&myBigInteger);
	freeBigInteger(&myBigInteger2);
}
