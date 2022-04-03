//-----------------------------------------------------------------------------
// Cole Anderson
// comander
// pa2
//
// BigInteger.c
// Implementation file for BigInteger ADT
//-----------------------------------------------------------------------------


// includes -------------------------------------------------------------------
#include<stdio.h>
#include<stdlib.h>

#include "List.h"

#include "BigInteger.h"


// macros ---------------------------------------------------------------------
#define TRUE 1
#define FALSE 0

const long BASE = 1000000000;
const long POWER = 9;


// structs --------------------------------------------------------------------

// private BigIntegerObj type
typedef struct BigIntegerObj {
    int sign;
    List magnitude;  // uneccessary digits are not allowed to exist so that length
            // of this list is a count of necessary digits, rightmost digit
            // represents ones place
} BigIntegerObj;


// Helper funcs ---------------------------------------------------------------

// compareMagnitude()
// Returns -1 if |A|<|B|, 1 if |A|>|B|, and 0 if |A|=|B|.
// Assumes each List input is a distinct List instance
int compareMagnitude(List MA, List MB) {
    if (MA == NULL || MB == NULL) {
        printf("ERROR: BigInteger ADT: calling compareMagnitude(): NULL List "
                "reference\n");
        exit(EXIT_FAILURE);
    }

    if (length(MA) > length(MB)) {
        return 1;
    }
    if (length(MA) < length(MB)) {
        return -1;
    }
    // here A and B have the same number of necessary digits
    moveFront(MA);
    moveFront(MB);
    while (index(MA) >= 0) {
        long aValue = get(MA);
        long bValue = get(MB);
        if (aValue > bValue) {
            return 1;
        }
        if (aValue < bValue) {
            return -1;
        }

        moveNext(MA);
        moveNext(MB);
    }
    // here A and B are equal
    return 0;
}

// fixLeadingZeroes()
// Rid leading zeroes on a magnitude. Of course, no BigInteger can have leading 
//          zeroes, so this method is only called as part of constructing a BigInteger    
void fixLeadingZeroes(List magnitude) {
    if (magnitude == NULL) {
        printf("ERROR: BigInteger ADT: calling fixLeadingZeroes(): NULL List "
                "reference\n");
        exit(EXIT_FAILURE);
    }

    while (length(magnitude) > 0) {
        moveFront(magnitude);
        if (get(magnitude) == 0) {
            deleteFront(magnitude);
        } else {
            break;
        }
    }
}

// normalize()
// Takes the magnitude of a BigInteger and fixes any digits that are out of range. 
//          The first element of the magnitude may be negative indicating that the 
//          magnitude cannot be normalized to represent a positive value.
//          Can leave the magnitude with leading zeroes.
void normalize(List magnitude) {
    if (magnitude == NULL) {
        printf("ERROR: BigInteger ADT: calling normalize(): NULL List "
                "reference\n");
        exit(EXIT_FAILURE);
    }

    long carryValue = 0;
    moveBack(magnitude);
    for (; index(magnitude) >= 0; movePrev(magnitude)) {
        long rawDigitValue = get(magnitude) + carryValue;
        if (rawDigitValue >= 0 && rawDigitValue < BASE) {  // digit does not require 
                // normalization
            carryValue = 0;
            set(magnitude, rawDigitValue);
        } else {
            long fixedDigitValue = rawDigitValue % BASE;
            if (fixedDigitValue < 0) {
                fixedDigitValue += BASE;
            }
            carryValue = (rawDigitValue - fixedDigitValue) / BASE;
            set(magnitude, fixedDigitValue);
        }
    }
    if (carryValue != 0) {
        prepend(magnitude, carryValue);
    }
}

// combineAbs()
// If add is TRUE: sets C to |A| + |B|, else: sets C to |A| - |B|, all addition
//          and subtractions are easily built from these base operations
// Assumes each BigInteger input is a distinct BigInteger instance
void combineAbs(BigInteger C, BigInteger A, BigInteger B, int add) {
    if (C == NULL || A == NULL || B == NULL) {
        printf("ERROR: BigInteger ADT: calling combineAbs(): NULL BigInteger "
                "reference\n");
        exit(EXIT_FAILURE);
    }

    makeZero(C);
    int isNegativeResult = add == FALSE &&
            compareMagnitude(A->magnitude, B->magnitude) == -1 ? TRUE : FALSE;  //
        // only true if we are doing |A| - |B| and |B| > |A|, if this is true
        // then we will do -1 * (|B| - |A|) instead
    List baseMagnitude;
    List appliedMagnitude;
    if (isNegativeResult) {
        baseMagnitude = B->magnitude;
        appliedMagnitude = A->magnitude;
    } else {
        baseMagnitude = A->magnitude;
        appliedMagnitude = B->magnitude;
    }
    List CMagnitude = C->magnitude;
    moveBack(baseMagnitude);
    moveBack(appliedMagnitude);
    for (;;) {
        if (index(baseMagnitude) >= 0) {
            if (index(appliedMagnitude) >= 0) {
                prepend(CMagnitude, 
                        add == TRUE ? get(baseMagnitude) + get(appliedMagnitude) :
                        get(baseMagnitude) - get(appliedMagnitude));
                movePrev(appliedMagnitude);
            } else {
                prepend(CMagnitude, get(baseMagnitude));
            }
            movePrev(baseMagnitude);
        } else if (index(appliedMagnitude) >= 0) {
            prepend(CMagnitude, add == TRUE ? get(appliedMagnitude) :
                    -1 * get(appliedMagnitude));  // the negative case should not
                    // be possible since we do not do |A| - |B| where |B| > |A|
            movePrev(appliedMagnitude);
        } else {
            break;
        }
    }
    normalize(CMagnitude);
    fixLeadingZeroes(CMagnitude);

    if (length(CMagnitude) == 0) {
        C->sign = 0;
    } else if (isNegativeResult) {
        C->sign = -1;
    } else {
        C->sign = 1;
    }
}


// Constructors-Destructors ---------------------------------------------------

// newBigInteger()
// Returns a reference to a new BigInteger object in the zero state.
BigInteger newBigInteger() {
    BigInteger nbi = malloc(sizeof(BigIntegerObj));
    nbi->sign = 0;
    nbi->magnitude = newList();
    return nbi;
}

// freeBigInteger()
// Frees heap memory associated with *pN, sets *pN to NULL.
void freeBigInteger(BigInteger* pN) {
    if (pN != NULL && *pN != NULL) {
        freeList(&((*pN)->magnitude));
        free(*pN);
        *pN = NULL;
    }
}


// Access functions -----------------------------------------------------------

// sign()
// Returns -1 if N is negative, 1 if N is positive, and 0 if N is in the zero state.
int sign(BigInteger N) {
    if (N == NULL) {
        printf("ERROR: BigInteger ADT: calling sign(): NULL BigInteger reference\n");
        exit(EXIT_FAILURE);
    }

    return N->sign;
}

// compare()
// Returns -1 if A<B, 1 if A>B, and 0 if A=B.
int compare(BigInteger A, BigInteger B) {
    if (A == NULL || B == NULL) {
        printf("ERROR: BigInteger ADT: calling compare(): NULL BigInteger reference\n");
        exit(EXIT_FAILURE);
    }

    BigInteger BCopy = copy(B);


    if (A->sign > BCopy->sign) {


        freeBigInteger(&BCopy);
        return 1;
    }
    if (A->sign < BCopy->sign) {


        freeBigInteger(&BCopy);
        return -1;
    }
    if (A->sign == 0) {
        
        
        freeBigInteger(&BCopy);
        return 0;
    }
    // here we know that A and B are both positive or both negative
    int returnValue = (A->sign == 1 ? 1 : -1) *
            compareMagnitude(A->magnitude, BCopy->magnitude);
    
    
    freeBigInteger(&BCopy);
    return returnValue;
}

// equals()
// Return true (1) if A and B are equal, false (0) otherwise.
int equals(BigInteger A, BigInteger B) {
    if (A == NULL || B == NULL) {
        printf("ERROR: BigInteger ADT: calling equals(): NULL BigInteger reference\n");
        exit(EXIT_FAILURE);
    }

    // FUNC 'compare()' HANDLES A AND B SAME REFERENCE POSSIBILITY


    return compare(A, B) == 0 ? TRUE : FALSE;
}


// Manipulation procedures ----------------------------------------------------

// makeZero()
// Re-sets N to the zero state.
void makeZero(BigInteger N) {
    if (N == NULL) {
        printf("ERROR: BigInteger ADT: calling makeZero(): NULL BigInteger reference\n");
        exit(EXIT_FAILURE);
    }

    N->sign = 0;
    clear(N->magnitude);
}

// negate()
// Reverses the sign of N: positive <--> negative. Does nothing if N is in the
//			zero state.
void negate(BigInteger N) {
    if (N == NULL) {
        printf("ERROR: BigInteger ADT: calling negate(): NULL BigInteger reference\n");
        exit(EXIT_FAILURE);
    }

    N->sign *= -1;
}


// BigInteger Arithmetic operations -----------------------------------------------

// stringToBigInteger()
// Returns a reference to a new BigInteger object representing the decimal integer
//			represented in base 10 by the string s.
// Pre: s is a non-empty string containing only base ten digits {0,1,2,3,4,5,6,7,8,9}
//			and an optional sign {+, -} prefix.
BigInteger stringToBigInteger(char* s) {

    // check Pre and count the characters so that we can reverse iterate
    int stringChars = 0;
    for (;; stringChars++) {
        char thisChar = s[stringChars];
        if (thisChar == '\0') {
            break;
        }
        if (thisChar == '+' || thisChar == '-') {
            if (stringChars > 0) {
                printf("ERROR: BigInteger ADT: calling stringToBigInteger(): "
                        "Bad string input\n");
                exit(EXIT_FAILURE);
            }
        } else if (thisChar == '0' || thisChar == '1' || thisChar == '2' ||
                thisChar == '3' || thisChar == '4' || thisChar == '5' ||
                thisChar == '6' || thisChar == '7' || thisChar == '8' ||
                thisChar == '9') {
        } else {
            printf("ERROR: BigInteger ADT: calling stringToBigInteger(): "
                    "Bad string input\n");
            exit(EXIT_FAILURE);
        }
    }

    // create initials
    BigInteger nbi = newBigInteger();
    int isNegative = FALSE;

    // deal with the optional sign prefix
    int firstIndex = 0;
    if (s[0] == '+') {
        firstIndex = 1;
    } else if (s[0] == '-') {
        isNegative = TRUE;
        firstIndex = 1;
    }

    // reverse iterate to construct the BigInteger digit by digit
    long digitValue = 0;  // the value of this BigInteger digit
    long digitPlace = 1;  // the place that each string digit is for within a
            // BigInteger digit
    for (int i = stringChars - 1; i >= firstIndex; i--) {
        if (digitPlace >= BASE) {  // our digit is finished
            prepend(nbi->magnitude, digitValue);
            digitValue = 0;
            digitPlace = 1;
        }
        digitValue += digitPlace * ((long)(s[i] - '0'));
        digitPlace *= 10;
    }
    if (digitValue > 0) {
        prepend(nbi->magnitude, digitValue);
    }

    // delete any leading zeroes
    fixLeadingZeroes(nbi->magnitude);

    // set sign
    if (length(nbi->magnitude) == 0) {
        nbi->sign = 0;
    } else if (isNegative == TRUE) {
        nbi->sign = -1;
    } else {
        nbi->sign = 1;
    }

    // return
    return nbi;
}

// copy()
// Returns a reference to a new BigInteger object in the same state as N.
BigInteger copy(BigInteger N) {
    if (N == NULL) {
        printf("ERROR: BigInteger ADT: calling copy(): NULL BigInteger reference\n");
        exit(EXIT_FAILURE);
    }

    BigInteger nbi = newBigInteger();
    nbi->sign = N->sign;
    freeList(&(nbi->magnitude));
    nbi->magnitude = copyList(N->magnitude);
    return nbi;
}

// add()
// Places the sum of A and B in the existing BigInteger S, overwriting its
//			current state: S = A + B
void add(BigInteger S, BigInteger A, BigInteger B) {
    if (S == NULL || A == NULL || B == NULL) {
        printf("ERROR: BigInteger ADT: calling add(): NULL BigInteger reference\n");
        exit(EXIT_FAILURE);
    }

    BigInteger ACopy = copy(A);
    BigInteger BCopy = copy(B);
    


    if (sign(ACopy) == -1) {
        if (sign(BCopy) == -1) {
            combineAbs(S, ACopy, BCopy, TRUE);
            negate(S);
        } else {
            combineAbs(S, ACopy, BCopy, FALSE);
            negate(S);
        }
    } else {
        if (sign(BCopy) == -1) {
            combineAbs(S, ACopy, BCopy, FALSE);
        } else {
            combineAbs(S, ACopy, BCopy, TRUE);
        }
    }


    freeBigInteger(&ACopy);
    freeBigInteger(&BCopy);
}

// sum()
// Returns a reference to a new BigInteger object representing A + B.
BigInteger sum(BigInteger A, BigInteger B) {
    if (A == NULL || B == NULL) {
        printf("ERROR: BigInteger ADT: calling sum(): NULL BigInteger reference\n");
        exit(EXIT_FAILURE);
    }

    // FUNC 'add()' HANDLES A AND B SAME REFERENCE POSSIBILITY


    BigInteger nbi = newBigInteger();
    add(nbi, A, B);
    return nbi;
}

// subtract()
// Places the difference of A and B in the existing BigInteger D, overwriting
//			its current state: D = A - B
void subtract(BigInteger D, BigInteger A, BigInteger B) {
    if (D == NULL || A == NULL || B == NULL) {
        printf("ERROR: BigInteger ADT: calling subtract(): NULL BigInteger reference\n");
        exit(EXIT_FAILURE);
    }

    // FUNC 'add()' HANDLES D AND A SAME REFERENCE POSSIBILITY
    BigInteger BCopy = copy(B);


    negate(BCopy);
    add(D, A, BCopy);
    

    freeBigInteger(&BCopy);
}

// diff()
// Returns a reference to a new BigInteger object representing A - B.
BigInteger diff(BigInteger A, BigInteger B) {
    if (A == NULL || B == NULL) {
        printf("ERROR: BigInteger ADT: calling diff(): NULL BigInteger reference\n");
        exit(EXIT_FAILURE);
    }

    // FUNC 'subtract()' HANDLES A AND B SAME REFERENCE POSSIBILITY


    BigInteger nbi = newBigInteger();
    subtract(nbi, A, B);
    return nbi;
}

// multiply()
// Places the product of A and B in the existing BigInteger P, overwriting
//			its current state: P = A*B
void multiply(BigInteger P, BigInteger A, BigInteger B) {
    if (P == NULL || A == NULL || B == NULL) {
        printf("ERROR: BigInteger ADT: calling multiply(): NULL BigInteger reference\n");
        exit(EXIT_FAILURE);
    }

    BigInteger ACopy = copy(A);
    BigInteger BCopy = copy(B);


    makeZero(P);
    // A is the top number and B is the bottom, in a by-hand multiplication problem
    List BMagnitude = BCopy->magnitude;
    List AMagnitude = ACopy->magnitude;
    List PMagnitude = P->magnitude;
    moveBack(BMagnitude);
    for (int bPlace = 0; index(BMagnitude) >= 0; movePrev(BMagnitude),bPlace++) {
        long bValue = get(BMagnitude);
        moveBack(PMagnitude);
        for (int i = 0; i < bPlace; i++) {
            movePrev(PMagnitude);
        }
        moveBack(AMagnitude);
        for (; index(AMagnitude) >= 0; movePrev(AMagnitude)) {
            
            // put this small product into the ultimate product
            long product = bValue * get(AMagnitude);
            if (index(PMagnitude) >= 0) {
                set(PMagnitude, get(PMagnitude) + product);

                movePrev(PMagnitude);
            } else {
                prepend(PMagnitude, product);
            }
        }
        normalize(PMagnitude);
    }
    fixLeadingZeroes(PMagnitude);

    if (length(PMagnitude) == 0) {
        P->sign = 0;
    } else {
        P->sign = ACopy->sign + BCopy->sign == 0 ? -1 : 1;
    }


    freeBigInteger(&ACopy);
    freeBigInteger(&BCopy);
}

// prod()
// Returns a reference to a new BigInteger object representing A*B
BigInteger prod(BigInteger A, BigInteger B) {
    if (A == NULL || B == NULL) {
        printf("ERROR: BigInteger ADT: calling prod(): NULL BigInteger reference\n");
        exit(EXIT_FAILURE);
    }

    // FUNC 'multiply()' HANDLES A AND B SAME REFERENCE POSSIBILITY


    BigInteger nbi = newBigInteger();
    multiply(nbi, A, B);
    return nbi;
}


// Other operations -----------------------------------------------------------

// printBigInteger()
// Prints a base 10 string representation of N to filestream out.
void printBigInteger(FILE* out, BigInteger N) {
    if (N == NULL) {
        printf("ERROR: BigInteger ADT: calling printBigInteger(): NULL BigInteger "
            "reference\n");
        exit(EXIT_FAILURE);
    }

    if (sign(N) == 0) {
#pragma GCC diagnostic ignored "-Wformat-zero-length"
        fprintf(out, "0");
#pragma GCC diagnostic warning "-Wformat-zero-length"
    } else {
        if (sign(N) == -1) {
            fprintf(out, "-");
        }
        long baseOverTen = BASE / 10;
        moveFront(N->magnitude);
        for (;;) {
            long digitValue = get(N->magnitude);
            if (index(N->magnitude) != 0) {
                for (int bDiv = baseOverTen; bDiv >= 10 && digitValue / bDiv == 0; bDiv /= 10) {
                    fprintf(out, "%s", "0");
                }
            }
            fprintf(out, "%ld", digitValue);
            moveNext(N->magnitude);
            if (index(N->magnitude) == -1) {
                break;
            }
            //fprintf(out, " ");
        }
    }
}
