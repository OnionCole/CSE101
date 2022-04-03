//-----------------------------------------------------------------------------
// Cole Anderson
// comander
// pa3
//
// List.c
// Implementation file for List ADT
//-----------------------------------------------------------------------------

#include<stdio.h>
#include<stdlib.h>
#include "List.h"

#define TRUE 1
#define FALSE 0

#define LIST_ELEMENT int  // have to check print statements for the % key

// structs --------------------------------------------------------------------

// private NodeObj type
typedef struct NodeObj {
    LIST_ELEMENT data;
    struct NodeObj* next;
    struct NodeObj* previous;
} NodeObj;

// private Node type
typedef NodeObj* Node;

// private ListObj type
typedef struct ListObj {
    Node front;
    Node back;
    Node cursor;
    int cursorIndex;
    int length;
} ListObj;



// Constructors-Destructors ---------------------------------------------------


// newNode()
// Returns reference to new Node object.
Node newNode(LIST_ELEMENT data) {
    Node N = malloc(sizeof(NodeObj));
    N->data = data;
    N->next = NULL;
    N->previous = NULL;
    return(N);
}

// freeNode()
// Frees heap memory used by a Node instance.
void freeNode(Node* pN) {
    if (pN != NULL && *pN != NULL) {
        free(*pN);
        *pN = NULL;
    }
}

// newList()
// Creates and returns a new empty List
List newList(void) {
    List L;
    L = malloc(sizeof(ListObj));
    L->front = NULL;
    L->back = NULL;
    L->cursor = NULL;
    L->cursorIndex = -1;
    L->length = 0;
    return(L);
}

// freeList()
// Frees all heap memory associated with *pL, and sets *pL to NULL.
void freeList(List* pL) {
    if (pL != NULL && *pL != NULL) {
        clear(*pL);
        free(*pL);
        *pL = NULL;
    }
}


// Access functions -----------------------------------------------------------

// length()
// Returns the number of elements in L.
int length(List L) {
    if (L == NULL) {
        printf("ERROR: List ADT: calling length(): NULL List reference\n");
        exit(EXIT_FAILURE);
    }

    return(L->length);
}

// index()
// Returns index of cursor element if defined, -1 otherwise.
int index(List L) {
    if (L == NULL) {
        printf("ERROR: List ADT: calling index(): NULL List reference\n");
        exit(EXIT_FAILURE);
    }

    return(L->cursorIndex);
}

// front()
// Returns front element of L.
// Pre: length()>0
LIST_ELEMENT front(List L) {
    if (L == NULL) {
        printf("ERROR: List ADT: calling front(): NULL List reference\n");
        exit(EXIT_FAILURE);
    }
    if (!(length(L) > 0)) {
        printf("ERROR: List ADT: calling front(): Length of list NOT greater than 0\n");
        exit(EXIT_FAILURE);
    }

    return(L->front->data);
}

// back()
// Returns back element of L.
// Pre: length()>0
LIST_ELEMENT back(List L) {
    if (L == NULL) {
        printf("ERROR: List ADT: calling back(): NULL List reference\n");
        exit(EXIT_FAILURE);
    }
    if (!(length(L) > 0)) {
        printf("ERROR: List ADT: calling back(): Length of list NOT greater than 0\n");
        exit(EXIT_FAILURE);
    }

    return(L->back->data);
}

// get()
// Returns cursor element of L.
// Pre: length()>0, index()>=0
LIST_ELEMENT get(List L) {
    if (L == NULL) {
        printf("ERROR: List ADT: calling get(): NULL List reference\n");
        exit(EXIT_FAILURE);
    }
    if (!(length(L) > 0)) {
        printf("ERROR: List ADT: calling get(): Length of list NOT greater than 0\n");
        exit(EXIT_FAILURE);
    }
    if (!(index(L) >= 0)) {
        printf("ERROR: List ADT: calling get(): Cursor index NOT greater than or equal to 0\n");
        exit(EXIT_FAILURE);
    }

    return(L->cursor->data);
}

// equals()
// Returns true (1) iff Lists A and B are in same state (cursor position not counted), 
//			and returns false (0) otherwise.
int equals(List A, List B) {
    if (A == NULL) {
        printf("ERROR: List ADT: calling equals(): NULL List reference for List A\n");
        exit(EXIT_FAILURE);
    }
    if (B == NULL) {
        printf("ERROR: List ADT: calling equals(): NULL List reference for List B\n");
        exit(EXIT_FAILURE);
    }

    //if (A->length != B->length || A->cursorIndex != B->cursorIndex) {
    if (A->length != B->length) {
        return FALSE;
    }
    Node ANode = A->front;
    Node BNode = B->front;
    int ALength = length(A);
    for (int i = 0; i < ALength; i++) {
        if (ANode->data != BNode->data) {
            return FALSE;
        }
        ANode = ANode->next;
        BNode = BNode->next;
    }
    return TRUE;
}


// Manipulation procedures ----------------------------------------------------

// clear()
// Resets L to its original empty state.
void clear(List L) {
    if (L == NULL) {
        printf("ERROR: List ADT: calling clear(): NULL List reference\n");
        exit(EXIT_FAILURE);
    }

    while (length(L) > 0) {
        deleteFront(L);
    }

    // below should not be necessary
    L->front = NULL;
    L->back = NULL;
    L->cursor = NULL;
    L->cursorIndex = -1;
    L->length = 0;
}

// moveFront()
// If L is non-empty, sets cursor under the front element, otherwise does nothing.
void moveFront(List L) {
    if (L == NULL) {
        printf("ERROR: List ADT: calling moveFront(): NULL List reference\n");
        exit(EXIT_FAILURE);
    }

    if (length(L) > 0) {
        L->cursor = L->front;
        L->cursorIndex = 0;
    }
}

// moveBack()
// If L is non-empty, sets cursor under the back element, otherwise does nothing.
void moveBack(List L) {
    if (L == NULL) {
        printf("ERROR: List ADT: calling moveBack(): NULL List reference\n");
        exit(EXIT_FAILURE);
    }

    if (length(L) > 0) {
        L->cursor = L->back;
        L->cursorIndex = length(L) - 1;
    }
}

// movePrev()
// If cursor is defined and not at front, move cursor one step toward the front of L; if cursor 
        // is defined and at front, cursor becomes undefined; if cursor is undefined do nothing
void movePrev(List L) {
    if (L == NULL) {
        printf("ERROR: List ADT: calling moveBack(): NULL List reference\n");
        exit(EXIT_FAILURE);
    }

    if (L->cursorIndex != -1) {  // if cursor is defined
        L->cursorIndex--;
        L->cursor = L->cursor->previous;
    }
}

// moveNext()
// If cursor is defined and not at back, move cursor one step toward the back of L; if cursor is 
        // defined and at back, cursor becomes undefined; if cursor is undefined do nothing
void moveNext(List L) {
    if (L == NULL) {
        printf("ERROR: List ADT: calling moveNext(): NULL List reference\n");
        exit(EXIT_FAILURE);
    }

    if (L->cursorIndex != -1) {  // if cursor is defined
        L->cursorIndex++;
        if (L->cursorIndex == length(L)) {
            L->cursorIndex = -1;
        }
        L->cursor = L->cursor->next;
    }
}

// prepend()
// Insert new element into L. If L is non-empty, insertion takes place before front element.
void prepend(List L, LIST_ELEMENT data) {
    if (L == NULL) {
        printf("ERROR: List ADT: calling prepend(): NULL List reference\n");
        exit(EXIT_FAILURE);
    }

    Node N = newNode(data);
    if (length(L) == 0) {
        L->front = N;
        L->back = N;
    }
    else {
        N->next = L->front;
        L->front->previous = N;
        L->front = N;
        if (L->cursorIndex != -1) {
            L->cursorIndex++;
        }
    }
    L->length++;
}

// append()
// Insert new element into L. If L is non-empty, insertion takes place after back element.
void append(List L, LIST_ELEMENT data) {
    if (L == NULL) {
        printf("ERROR: List ADT: calling append(): NULL List reference\n");
        exit(EXIT_FAILURE);
    }

    Node N = newNode(data);
    if (length(L) == 0) {
        L->front = N;
        L->back = N;
    }
    else {
        N->previous = L->back;
        L->back->next = N;
        L->back = N;
    }
    L->length++;
}

// set()
// Overwrites the cursor element with x.
// Pre: length()>0, index()>=0
void set(List L, LIST_ELEMENT x) {
    if (L == NULL) {
        printf("ERROR: List ADT: calling set(): NULL List reference\n");
        exit(EXIT_FAILURE);
    }
    if (!(length(L) > 0)) {
        printf("ERROR: List ADT: calling set(): Length of list NOT greater than 0\n");
        exit(EXIT_FAILURE);
    }
    if (!(index(L) >= 0)) {
        printf("ERROR: List ADT: calling set(): Cursor index NOT greater than or equal to 0\n");
        exit(EXIT_FAILURE);
    }

    L->cursor->data = x;
}

// insertBefore()
// Insert new element before cursor.
// Pre: length()>0, index()>=0
void insertBefore(List L, LIST_ELEMENT data) {
    if (L == NULL) {
        printf("ERROR: List ADT: calling insertBefore(): NULL List reference\n");
        exit(EXIT_FAILURE);
    }
    if (!(length(L) > 0)) {
        printf("ERROR: List ADT: calling insertBefore(): Length of list NOT greater than 0\n");
        exit(EXIT_FAILURE);
    }
    if (!(index(L) >= 0)) {
        printf("ERROR: List ADT: calling insertBefore(): Cursor index NOT greater than or equal to 0\n");
        exit(EXIT_FAILURE);
    }

    Node N = newNode(data);
    if (L->cursor->previous == NULL) {
        L->front = N;
    }
    else {
        N->previous = L->cursor->previous;
        N->previous->next = N;
    }
    N->next = L->cursor;
    L->cursor->previous = N;
    L->cursorIndex++;
    L->length++;
}

// insertAfter()
// Insert new element after cursor.
// Pre: length()>0, index()>=0
void insertAfter(List L, LIST_ELEMENT data) {
    if (L == NULL) {
        printf("ERROR: List ADT: calling insertAfter(): NULL List reference\n");
        exit(EXIT_FAILURE);
    }
    if (!(length(L) > 0)) {
        printf("ERROR: List ADT: calling insertAfter(): Length of list NOT greater than 0\n");
        exit(EXIT_FAILURE);
    }
    if (!(index(L) >= 0)) {
        printf("ERROR: List ADT: calling insertAfter(): Cursor index NOT greater than or equal to 0\n");
        exit(EXIT_FAILURE);
    }

    Node N = newNode(data);
    if (L->cursor->next == NULL) {
        L->back = N;
    }
    else {
        N->next = L->cursor->next;
        N->next->previous = N;
    }
    N->previous = L->cursor;
    L->cursor->next = N;
    L->length++;
}

// deleteFront()
// Delete the front element. Pre: length()>0
void deleteFront(List L) {
    if (L == NULL) {
        printf("ERROR: List ADT: calling deleteFront(): NULL List reference\n");
        exit(EXIT_FAILURE);
    }
    if (!(length(L) > 0)) {
        printf("ERROR: List ADT: calling deleteFront(): Length of list NOT greater than 0\n");
        exit(EXIT_FAILURE);
    }

    Node oldFront = L->front;
    if (length(L) == 1) {
        L->front = NULL;
        L->back = NULL;
    }
    else {
        L->front = oldFront->next;
        L->front->previous = NULL;
    }
    freeNode(&oldFront);
    if (L->cursorIndex == 0) {
        L->cursorIndex = -1;
        L->cursor = NULL;
    }
    else if (L->cursorIndex != -1) {
        L->cursorIndex--;
    }
    L->length--;
}

// deleteBack()
// Delete the back element. Pre: length()>0
void deleteBack(List L) {
    if (L == NULL) {
        printf("ERROR: List ADT: calling deleteBack(): NULL List reference\n");
        exit(EXIT_FAILURE);
    }
    if (!(length(L) > 0)) {
        printf("ERROR: List ADT: calling deleteBack(): Length of list NOT greater than 0\n");
        exit(EXIT_FAILURE);
    }

    Node oldBack = L->back;
    if (length(L) == 1) {
        L->front = NULL;
        L->back = NULL;
    }
    else {
        L->back = oldBack->previous;
        L->back->next = NULL;
    }
    freeNode(&oldBack);
    if (L->cursorIndex == length(L) - 1) {
        L->cursorIndex = -1;
        L->cursor = NULL;
    }
    L->length--;
}

// delete()
// Delete cursor element, making cursor undefined.
// Pre: length()>0, index()>=0
void delete(List L) {
    if (L == NULL) {
        printf("ERROR: List ADT: calling delete(): NULL List reference\n");
        exit(EXIT_FAILURE);
    }
    if (!(length(L) > 0)) {
        printf("ERROR: List ADT: calling delete(): Length of list NOT greater than 0\n");
        exit(EXIT_FAILURE);
    }
    if (!(index(L) >= 0)) {
        printf("ERROR: List ADT: calling delete(): Cursor index NOT greater than or equal to 0\n");
        exit(EXIT_FAILURE);
    }

    if (L->cursorIndex == 0) {
        deleteFront(L);
    }
    else if (L->cursorIndex == length(L) - 1) {
        deleteBack(L);
    }
    else {  // this is neither the front nor the back node
        L->cursor->previous->next = L->cursor->next;
        L->cursor->next->previous = L->cursor->previous;
        freeNode(&L->cursor);
        L->cursorIndex = -1;
        L->cursor = NULL;
        L->length--;
    }
}


// Other Functions ------------------------------------------------------------

// printList()
// Prints to the file pointed to by out, a string representation of L consisting of a space 
        // separated sequence of integers, with front on left.
void printList(FILE* out, List L) {
    if (L == NULL) {
        printf("ERROR: List ADT: calling printList(): NULL List reference\n");
        exit(EXIT_FAILURE);
    }

    //// open file
    //out = fopen(out, "w");
    //if (out == NULL) {
    //    printf("ERROR: List ADT: calling printList(): cannot open this file\n");
    //    exit(EXIT_FAILURE);
    //}

    // print
    if (length(L) == 0) {
#pragma GCC diagnostic ignored "-Wformat-zero-length"
        fprintf(out, "");
#pragma GCC diagnostic warning "-Wformat-zero-length"
    }
    else {
        Node LNode = L->front;
        for (;;) {
            fprintf(out, "%d", LNode->data);
            LNode = LNode->next;
            if (LNode == NULL) {
                break;
            }
            fprintf(out, " ");
        }
    }

    //// close file
    //fclose(out);
}

// copyList()
// Returns a new List representing the same integer sequence as L. The cursor in the new list 
        // is undefined, regardless of the state of the cursor in L. The state of L is unchanged.
List copyList(List L) {
    if (L == NULL) {
        printf("ERROR: List ADT: calling copyList(): NULL List reference\n");
        exit(EXIT_FAILURE);
    }

    List NL = newList();
    Node LNode = L->front;
    while (LNode != NULL) {
        append(NL, LNode->data);

        LNode = LNode->next;
    }
    return NL;
}
