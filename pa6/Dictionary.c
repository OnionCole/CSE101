//-----------------------------------------------------------------------------
// Cole Anderson
// comander
// pa6
//
// Dictionary.c
// Implementation file for Dictionary ADT
//-----------------------------------------------------------------------------


#include<stdlib.h>
#include<stdio.h>
#include<string.h>

#include "Dictionary.h"

#define TRUE 1
#define FALSE 0


// structs --------------------------------------------------------------------

// private NodeObj type
typedef struct NodeObj {
    KEY_TYPE key;
    VAL_TYPE value;
    struct NodeObj* parent;
    struct NodeObj* left;
    struct NodeObj* right;
} NodeObj;

// private Node type
typedef NodeObj* Node;

// private DictionaryObj type
typedef struct DictionaryObj {
    int unique;  // TRUE for unique keys required, false otherwise,
            // if TRUE, identical keys will be inserted to the right of
            // one another
    Node root;
    int size;
    
    int forwardsIteration;  // bool, iteration direction, TRUE for forwards,
            // FALSE for reverse
    Node iterationNode;  // redundant with a Node in the tree
} DictionaryObj;


// Helper Funcs ---------------------------------------------------------------

// newNode()
// Returns reference to new Node object.
Node newNode(KEY_TYPE key, VAL_TYPE value) {
    Node N = malloc(sizeof(NodeObj));
    N->key = key;
    N->value = value;
    N->parent = NULL;
    N->left = NULL;
    N->right = NULL;
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

// restoreClippedBranch()
// Insert a branch into the tree. Dictionary size field is not modified as the 
//          supposition is that the branch was clipped in a function due to a 
//          deleted node and is now being reinserted. 
// Branch is assumed to be legitimate.
// PRE: root != null
void restoreClippedBranch(Dictionary D, Node BranchRoot) {
    if (D == NULL) {
        printf("ERROR: Dictionary ADT: calling restoreClippedBranch(): NULL Dictionary reference\n");
        exit(EXIT_FAILURE);
    }
    if (BranchRoot == NULL) {
        printf("ERROR: Dictionary ADT: calling restoreClippedBranch(): NULL Node reference\n");
        exit(EXIT_FAILURE);
    }
    if (D->root == NULL) {
        printf("ERROR: Dictionary ADT: calling restoreClippedBranch(): NULL root\n");
        exit(EXIT_FAILURE);
    }

    KEY_TYPE key = BranchRoot->key;
    Node parentNode = D->root;
    for (;;) {
        int keyCmpResult = KEY_CMP(key, parentNode->key);
        if (keyCmpResult >= 0) {  // key is bigger than or equal to parentNode->key
            if (parentNode->right == NULL) {
                parentNode->right = BranchRoot;
                break;
            }
            parentNode = parentNode->right;
        } else {  // parentNode->key is bigger than key
            if (parentNode->left == NULL) {
                parentNode->left = BranchRoot;
                break;
            }
            parentNode = parentNode->left;
        }
    }
    BranchRoot->parent = parentNode;
}

// freeAllNodes()
// Free all nodes in Dictionary given the root node. NULL argument is OK.
void freeAllNodes(Node RootNode) {
    if (RootNode != NULL) {
        if (RootNode->left != NULL) {
            freeAllNodes(RootNode->left);
        }
        if (RootNode->right != NULL) {
            freeAllNodes(RootNode->right);
        }
        freeNode(&RootNode);
    }
}

// move()
// Move in the iteration
// moveForwards is bool, FALSE for reverse
VAL_TYPE move(Dictionary D, int moveForwards) {
    if (D == NULL) {
        printf("ERROR: Dictionary ADT: calling move(): NULL Dictionary reference\n");
        exit(EXIT_FAILURE);
    }

    // set D->iterationNode to the next node in the iteration, or NULL if iteration
    //          is complete
    Node iterationNode = D->iterationNode;
    if (iterationNode == NULL) {
        return VAL_UNDEF;
    }
    KEY_TYPE key = iterationNode->key;
    if (moveForwards == TRUE) {  // looking for bigger
        if (iterationNode->right != NULL) {
            /*
            * If you have a right child, then the next node bigger is to go right
            *           and then left as many times as possible.
            */

            iterationNode = iterationNode->right;
            for (; iterationNode->left != NULL; iterationNode = iterationNode->left) {}
        } else {
            /*
            * If you have no right child, then the next node bigger is to go up
            *           and find the first node bigger.
            */

            iterationNode = iterationNode->parent;
            for (; iterationNode != NULL && KEY_CMP(key, iterationNode->key) >= 0;
                    iterationNode = iterationNode->parent) {}
        }
    } else {  // looking for smaller
        if (iterationNode->left != NULL) {
            /*
            * If you have a left child, then the next node smaller is to go left
            *           and then right as many times as possible.
            */

            iterationNode = iterationNode->left;
            for (; iterationNode->right != NULL; iterationNode = iterationNode->right) {}
        } else {
            /*
            * If you have no left child, then the next node smaller is to go up
            *           and find the first node smaller.
            */

            iterationNode = iterationNode->parent;
            for (; iterationNode != NULL && KEY_CMP(key, iterationNode->key) < 0;
                    iterationNode = iterationNode->parent) {}
        }
    }
    D->iterationNode = iterationNode;

    // return
    if (iterationNode == NULL) {  // the iteration has completed
        D->forwardsIteration = -1;
        return VAL_UNDEF;
    }
    return iterationNode->value;
}


// Constructors-Destructors ---------------------------------------------------

// newDictionary()
// Creates a new empty Dictionary. If unique==false (0), then the Dictionary 
// will accept duplicate keys, i.e. distinct pairs with identical keys. If 
// unique==true (1 or any non-zero value), then duplicate keys will not be 
// accepted. In this case, the operation insert(D, k) will enforce the 
// precondition: lookup(D, k)==VAL_UNDEF
Dictionary newDictionary(int unique) {
    Dictionary D = malloc(sizeof(DictionaryObj));
    
    D->unique = unique;
    D->root = NULL;
    D->size = 0;
    
    D->forwardsIteration = -1;
    D->iterationNode = NULL;
    
    return D;
}

// freeDictionary()
// Frees heap memory associated with *pD, sets *pD to NULL.
void freeDictionary(Dictionary* pD) {
    if (pD != NULL && *pD != NULL) {
        makeEmpty(*pD);
        
        free(*pD);
        *pD = NULL;
    }
}


// Access functions -----------------------------------------------------------

// size()
// Returns the number of (key, value) pairs in Dictionary D.
int size(Dictionary D) {
    if (D == NULL) {
        printf("ERROR: Dictionary ADT: calling size(): NULL Dictionary reference\n");
        exit(EXIT_FAILURE);
    }

    return D->size;
}

// getUnique()
// Returns true (1) if D requires that all pairs have unique keys. Returns
// false (0) if D accepts distinct pairs with identical keys.
int getUnique(Dictionary D) {
    if (D == NULL) {
        printf("ERROR: Dictionary ADT: calling getUnique(): NULL Dictionary reference\n");
        exit(EXIT_FAILURE);
    }

    return D->unique;
}

// lookup()
// If Dictionary D contains a (key, value) pair whose key matches k (i.e. if
// KEY_CMP(key, k)==0), then returns (first such) value. If D contains no such pair, then
// returns VAL_UNDEF.
VAL_TYPE lookup(Dictionary D, KEY_TYPE k) {
    if (D == NULL) {
        printf("ERROR: Dictionary ADT: calling lookup(): NULL Dictionary reference\n");
        exit(EXIT_FAILURE);
    }

    for (Node thisNode = D->root; thisNode != NULL;) {
        int keyCmpResult = KEY_CMP(k, thisNode->key);
        if (keyCmpResult > 0) {  // k is bigger than thisNode->key
            thisNode = thisNode->right;
        } else if (keyCmpResult < 0) {  // thisNode->key is bigger than k
            thisNode = thisNode->left;
        } else {  // this is the node we are looking for
            return thisNode->value;
        }
    }
    return VAL_UNDEF;
}


// Manipulation procedures ----------------------------------------------------

// insert()
// Insert the pair (k,v) into Dictionary D. 
// If getUnique(D) is false (0), then there are no preconditions.
// If getUnique(D) is true (1), then the precondition lookup(D, k)==VAL_UNDEF
// is enforced.
void insert(Dictionary D, KEY_TYPE k, VAL_TYPE v) {
    if (D == NULL) {
        printf("ERROR: Dictionary ADT: calling insert(): NULL Dictionary reference\n");
        exit(EXIT_FAILURE);
    }
    // PRE: (getUnique(D) == FALSE || lookup(D, k)==VAL_UNDEF) checked in function

    Node NN = newNode(k, v);
    if (D->root == NULL) {
        D->root = NN;
    } else {
        int identicalKeyFound = FALSE;  // if we found a key equal to k
        Node parentNode = D->root;
        for (;;) {
            int keyCmpResult = KEY_CMP(k, parentNode->key);
            if (keyCmpResult > 0) {  // k is bigger than parentNode->key
                if (parentNode->right == NULL) {
                    parentNode->right = NN;
                    break;
                }
                parentNode = parentNode->right;
            } else if (keyCmpResult < 0) {  // parentNode->key is bigger than k
                if (parentNode->left == NULL) {
                    parentNode->left = NN;
                    break;
                }
                parentNode = parentNode->left;
            } else {  // k equals parentNode->key
                identicalKeyFound = TRUE;
                if (parentNode->right == NULL) {
                    parentNode->right = NN;
                    break;
                }
                parentNode = parentNode->right;
            }
        }
        if (getUnique(D) == TRUE && identicalKeyFound == TRUE) {
            printf("ERROR: Dictionary ADT: calling insert(): identical key found\n");
            exit(EXIT_FAILURE);
        }
        NN->parent = parentNode;
    }
    D->size++;
}

// delete()
// Remove the (first such) pair whose key is k from Dictionary D.
// Pre: lookup(D,k)!=VAL_UNDEF (i.e. D contains a pair whose key is k.)
// Running will cancel an iteration in progress
void delete(Dictionary D, KEY_TYPE k) {
    if (D == NULL) {
        printf("ERROR: Dictionary ADT: calling delete(): NULL Dictionary reference\n");
        exit(EXIT_FAILURE);
    }
    // PRE: (lookup(D, k)!=VAL_UNDEF) checked in function

    if (D->root == NULL) {
        printf("ERROR: Dictionary ADT: calling delete(): no such key (msg 1)\n");
        exit(EXIT_FAILURE);
    }
    Node deleteNode = NULL;
    if (KEY_CMP(D->root->key, k) == 0) {  // we are deleting the root
        deleteNode = D->root;  // ref saved for delete
        Node rootLeft = D->root->left;
        Node rootRight = D->root->right;
        if (rootRight != NULL) {  // right chosen as right is likely bigger, to help
                // tree balance
            rootRight->parent = NULL;
            D->root = rootRight;
            if (rootLeft != NULL) {
                restoreClippedBranch(D, rootLeft);
            }
        } else if (rootLeft != NULL) {
            rootLeft->parent = NULL;
            D->root = rootLeft;
        } else {  // root was only node in tree
            D->root = NULL;
        }
    } else {  // root is an ancestor of delete node, if delete node exists

        // determine parentNode and deleteNode
        Node parentNode = D->root;
        int deleteRightChild = -1;  // bool, FALSE for left
        for (;;) {
            int keyCmpResult = KEY_CMP(k, parentNode->key);
            if (keyCmpResult > 0) {  // k is bigger than parentNode->key
                if (parentNode->right == NULL) {
                    printf("ERROR: Dictionary ADT: calling delete(): no such key (msg 2)\n");
                    exit(EXIT_FAILURE);
                }
                else if (KEY_CMP(parentNode->right->key, k) == 0) {
                    deleteRightChild = TRUE;
                    deleteNode = parentNode->right;
                    break;
                }
                parentNode = parentNode->right;
            } else if (keyCmpResult < 0) {  // parentNode->key is bigger than k
                if (parentNode->left == NULL) {
                    printf("ERROR: Dictionary ADT: calling delete(): no such key (msg 3)\n");
                    exit(EXIT_FAILURE);
                } else if (KEY_CMP(parentNode->left->key, k) == 0) {
                    deleteRightChild = FALSE;
                    deleteNode = parentNode->left;
                    break;
                }
                parentNode = parentNode->left;
            } else {  // k equals parentNode->key, this would mean we missed the correct node
                printf("ERROR: Dictionary ADT: calling delete(): impossible logic path\n");
                exit(EXIT_FAILURE);
            }
        }

        // fix tree
        Node deleteLeft = deleteNode->left;
        Node deleteRight = deleteNode->right;
        if (deleteRightChild == TRUE) {
            if (deleteRight == NULL) {
                parentNode->right = NULL;
            } else {
                deleteRight->parent = parentNode;
                parentNode->right = deleteRight;
            }
            
            if (deleteLeft != NULL) {
                restoreClippedBranch(D, deleteLeft);
            }
        } else {
            if (deleteLeft == NULL) {
                parentNode->left = NULL;
            } else {
                deleteLeft->parent = parentNode;
                parentNode->left = deleteLeft;
            }
            
            if (deleteRight != NULL) {
                restoreClippedBranch(D, deleteRight);
            }
        }
    }

    // stop iteration if deleteNode was iteration node
    if (deleteNode == D->iterationNode) {
        D->forwardsIteration = -1;
        D->iterationNode = NULL;
    }
    
    // free deleteNode    
    freeNode(&deleteNode);

    // decrement size
    D->size--;
}

// makeEmpty()
// Reset Dictionary D to the empty state, containing no pairs.
// Running will cancel an iteration in progress
void makeEmpty(Dictionary D) {
    if (D == NULL) {
        printf("ERROR: Dictionary ADT: calling makeEmpty(): NULL Dictionary reference\n");
        exit(EXIT_FAILURE);
    }

    freeAllNodes(D->root);
    D->root = NULL;
    D->size = 0;

    // cancel iteration
    D->forwardsIteration = -1;
    D->iterationNode = NULL;
}

// beginForward()
// If D is non-empty, starts a forward iteration over D at the first key 
// (as defined by the order operator KEY_CMP()), then returns the first
// value. If D is empty, returns VAL_UNDEF. 
VAL_TYPE beginForward(Dictionary D) {
    if (D == NULL) {
        printf("ERROR: Dictionary ADT: calling beginForward(): NULL Dictionary reference\n");
        exit(EXIT_FAILURE);
    }

    if (D->root == NULL) {
        return VAL_UNDEF;
    }
    D->forwardsIteration = TRUE;
    Node firstNode = D->root;
    for (; firstNode->left != NULL; firstNode = firstNode->left) {}
    D->iterationNode = firstNode;
    return firstNode->value;
}

// beginReverse()
// If D is non-empty, starts a reverse iteration over D at the last key 
// (as defined by the order operator KEY_CMP()), then returns the last
// value. If D is empty, returns VAL_UNDEF.
VAL_TYPE beginReverse(Dictionary D) {
    if (D == NULL) {
        printf("ERROR: Dictionary ADT: calling beginReverse(): NULL Dictionary reference\n");
        exit(EXIT_FAILURE);
    }

    if (D->root == NULL) {
        return VAL_UNDEF;
    }
    D->forwardsIteration = FALSE;
    Node lastNode = D->root;
    for (; lastNode->right != NULL; lastNode = lastNode->right) {}
    D->iterationNode = lastNode;
    return lastNode->value;
}

// currentKey()
// If an iteration (forward or reverse) over D has started, returns the 
// the current key. If no iteration is underway, returns KEY_UNDEF.
KEY_TYPE currentKey(Dictionary D) {
    if (D == NULL) {
        printf("ERROR: Dictionary ADT: calling currentKey(): NULL Dictionary reference\n");
        exit(EXIT_FAILURE);
    }

    if (D->iterationNode == NULL) {
        return KEY_UNDEF;
    }
    return D->iterationNode->key;
}

// currentVal()
// If an iteration (forward or reverse) over D has started, returns the 
// value corresponding to the current key. If no iteration is underway, 
// returns VAL_UNDEF.
VAL_TYPE currentVal(Dictionary D) {
    if (D == NULL) {
        printf("ERROR: Dictionary ADT: calling currentVal(): NULL Dictionary reference\n");
        exit(EXIT_FAILURE);
    }

    if (D->iterationNode == NULL) {
        return VAL_UNDEF;
    }
    return D->iterationNode->value;
}

// next()
// If an iteration (forward or reverse) over D has started, and has not
// reached the last pair, moves to the next key in D (as defined by the 
// order operator KEY_CMP()), and returns the value corresponding to the 
// new key. If an iteration has started, and has reached the last pair,
// ends the iteration and returns VAL_UNDEF. If no iteration is underway, 
// returns VAL_UNDEF.
VAL_TYPE next(Dictionary D) {
    if (D == NULL) {
        printf("ERROR: Dictionary ADT: calling next(): NULL Dictionary reference\n");
        exit(EXIT_FAILURE);
    }

    return move(D, TRUE);
}

// prev()
// If an iteration (forward or reverse) over D has started, and has not
// reached the first pair, moves to the previous key in D (as defined by the 
// order operator KEY_CMP()), and returns the value corresponding to the 
// new key. If an iteration has started, and has reached the first pair,
// ends the iteration and returns VAL_UNDEF. If no iteration is underway, 
// returns VAL_UNDEF. 
VAL_TYPE prev(Dictionary D) {
    if (D == NULL) {
        printf("ERROR: Dictionary ADT: calling prev(): NULL Dictionary reference\n");
        exit(EXIT_FAILURE);
    }

    return move(D, FALSE);
}


// Other operations -----------------------------------------------------------

// printDictionary()
// Prints a text representation of D to the file pointed to by out. Each key-
// value pair is printed on a single line, with the two items separated by a
// single space.  The pairs are printed in the order defined by the operator
// KEY_CMP().
void printDictionary(FILE* out, Dictionary D) {
    if (D == NULL) {
        printf("ERROR: Dictionary ADT: calling printDictionary(): NULL Dictionary reference\n");
        exit(EXIT_FAILURE);
    }

    char pairFormatStr[strlen(KEY_FORMAT) + strlen(VAL_FORMAT) + 3];
    strcpy(pairFormatStr, KEY_FORMAT);
    strcat(pairFormatStr, " ");
    strcat(pairFormatStr, VAL_FORMAT);
    strcat(pairFormatStr, "\n");
    for (beginForward(D); currentVal(D) != VAL_UNDEF; next(D)) {
        fprintf(out, pairFormatStr, currentKey(D), currentVal(D));
    }
}
