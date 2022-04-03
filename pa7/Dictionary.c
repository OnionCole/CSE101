//-----------------------------------------------------------------------------
// Cole Anderson
// comander
// pa7
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
    int isRed;  // TRUE for red, FALSE for black
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
    Node nullNode;  // the single Null Node used in place of null for a Node's
            // connections
    Node root;
    int size;
    
    int forwardsIteration;  // bool, iteration direction, TRUE for forwards,
            // FALSE for reverse
    Node iterationNode;  // redundant with a Node in the tree or nullNode
} DictionaryObj;


// Helper Funcs ---------------------------------------------------------------

// newNode()
// Returns reference to new Node object.
Node newNode(Node nullNode, KEY_TYPE key, VAL_TYPE value, int isRed) {
    Node N = malloc(sizeof(NodeObj));
    N->key = key;
    N->value = value;
    N->isRed = isRed;
    N->parent = nullNode;
    N->left = nullNode;
    N->right = nullNode;
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

void LeftRotate(Dictionary D, Node x) {
    if (D == NULL) {
        printf("ERROR: Dictionary ADT: calling LeftRotate(): NULL Dictionary reference\n");
        exit(EXIT_FAILURE);
    }
    if (x == NULL) {
        printf("ERROR: Dictionary ADT: calling LeftRotate(): NULL Node reference\n");
        exit(EXIT_FAILURE);
    }

    Node y = x->right;

    x->right = y->left;
    if (y->left != D->nullNode) {
        y->left->parent = x;
    }

    y->parent = x->parent;
    if (x->parent == D->nullNode) {
        D->root = y;
    } else if (x == x->parent->left) {
        x->parent->left = y;
    } else {
        x->parent->right = y;
    }

    y->left = x;
    x->parent = y;
}

void RightRotate(Dictionary D, Node x) {
    if (D == NULL) {
        printf("ERROR: Dictionary ADT: calling RightRotate(): NULL Dictionary reference\n");
        exit(EXIT_FAILURE);
    }
    if (x == NULL) {
        printf("ERROR: Dictionary ADT: calling RightRotate(): NULL Node reference\n");
        exit(EXIT_FAILURE);
    }

    Node y = x->left;

    x->left = y->right;
    if (y->right != D->nullNode) {
        y->right->parent = x;
    }

    y->parent = x->parent;
    if (x->parent == D->nullNode) {
        D->root = y;
    } else if (x == x->parent->right) {
        x->parent->right = y;
    } else {
        x->parent->left = y;
    }

    y->right = x;
    x->parent = y;
}

void RB_Transplant(Dictionary D, Node u, Node v) {
    if (D == NULL) {
        printf("ERROR: Dictionary ADT: calling RB_Transplant(): NULL Dictionary reference\n");
        exit(EXIT_FAILURE);
    }
    if (u == NULL) {
        printf("ERROR: Dictionary ADT: calling RB_Transplant(): NULL Node reference (1)\n");
        exit(EXIT_FAILURE);
    }
    if (v == NULL) {
        printf("ERROR: Dictionary ADT: calling RB_Transplant(): NULL Node reference (2)\n");
        exit(EXIT_FAILURE);
    }

    if (u->parent == D->nullNode) {
        D->root = v;
    } else if (u == u->parent->left) {
        u->parent->left = v;
    } else {
        u->parent->right = v;
    }
    v->parent = u->parent;
}

Node TreeMinimum(Node NullNode, Node x) {
    if (NullNode == NULL) {
        printf("ERROR: Dictionary ADT: calling TreeMinimum(): NULL Node reference (1)\n");
        exit(EXIT_FAILURE);
    }
    if (x == NULL) {
        printf("ERROR: Dictionary ADT: calling TreeMinimum(): NULL Node reference (2)\n");
        exit(EXIT_FAILURE);
    }
    if (x == NullNode) {
        printf("ERROR: Dictionary ADT: calling TreeMinimum(): x == NullNode\n");
        exit(EXIT_FAILURE);
    }

    while (x->left != NullNode) {
        x = x->left;
    }
    return x;
}

// freeAllNodes()
// Free all nodes in Dictionary given the root node.
//          nullNode is not freed
void freeAllNodes(Node NullNode, Node RootNode) {
    if (NullNode == NULL) {
        printf("ERROR: Dictionary ADT: calling freeAllNodes(): NULL Node reference (1)\n");
        exit(EXIT_FAILURE);
    }
    if (RootNode == NULL) {
        printf("ERROR: Dictionary ADT: calling freeAllNodes(): NULL Node reference (2)\n");
        exit(EXIT_FAILURE);
    }

    if (RootNode != NullNode) {
        if (RootNode->left != NullNode) {
            freeAllNodes(NullNode, RootNode->left);
        }
        if (RootNode->right != NullNode) {
            freeAllNodes(NullNode, RootNode->right);
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
    Node nullNode = D->nullNode;
    Node iterationNode = D->iterationNode;
    if (iterationNode == nullNode) {
        return VAL_UNDEF;
    }
    KEY_TYPE key = iterationNode->key;
    if (moveForwards == TRUE) {  // looking for bigger
        if (iterationNode->right != nullNode) {
            /*
            * If you have a right child, then the next node bigger is to go right
            *           and then left as many times as possible.
            */

            iterationNode = iterationNode->right;
            for (; iterationNode->left != nullNode; iterationNode = iterationNode->left) {}
        } else {
            /*
            * If you have no right child, then the next node bigger is to go up
            *           and find the first node bigger.
            */

            iterationNode = iterationNode->parent;
            for (; iterationNode != nullNode && KEY_CMP(key, iterationNode->key) >= 0;
                    iterationNode = iterationNode->parent) {}
        }
    } else {  // looking for smaller
        if (iterationNode->left != nullNode) {
            /*
            * If you have a left child, then the next node smaller is to go left
            *           and then right as many times as possible.
            */

            iterationNode = iterationNode->left;
            for (; iterationNode->right != nullNode; iterationNode = iterationNode->right) {}
        } else {
            /*
            * If you have no left child, then the next node smaller is to go up
            *           and find the first node smaller.
            */

            iterationNode = iterationNode->parent;
            for (; iterationNode != nullNode && KEY_CMP(key, iterationNode->key) < 0;
                    iterationNode = iterationNode->parent) {}
        }
    }
    D->iterationNode = iterationNode;

    // return
    if (iterationNode == nullNode) {  // the iteration has completed
        D->forwardsIteration = -1;
        return VAL_UNDEF;
    }
    return iterationNode->value;
}

// moveINORDER()
// move but forwards is on an inorder path
VAL_TYPE moveINORDER(Dictionary D, int moveForwards) {
    if (D == NULL) {
        printf("ERROR: Dictionary ADT: calling moveINORDER(): NULL Dictionary reference\n");
        exit(EXIT_FAILURE);
    }

    // set D->iterationNode to the next node in the iteration, or NULL if iteration
    //          is complete
    Node nullNode = D->nullNode;
    Node iterationNode = D->iterationNode;
    if (iterationNode == nullNode) {
        return VAL_UNDEF;
    }
    //KEY_TYPE key = iterationNode->key;
    if (moveForwards == TRUE) {  // looking for bigger
        if (iterationNode->right != nullNode) {
            /*
            * If you have a right child, then the next node bigger is to go right
            *           and then left as many times as possible.
            */

            iterationNode = iterationNode->right;
            for (; iterationNode->left != nullNode; iterationNode = iterationNode->left) {}
        } else {
            /*
            * If you have no right child, then the next node bigger is to go up
            *           until you go right.
            */

            for (; iterationNode != nullNode; iterationNode = iterationNode->parent) {
                if (iterationNode == iterationNode->parent->left) {
                    iterationNode = iterationNode->parent;
                    break;
                }
            }
        }
    } else {  // looking for smaller
        if (iterationNode->left != nullNode) {
            /*
            * If you have a left child, then the next node smaller is to go left
            *           and then right as many times as possible.
            */

            iterationNode = iterationNode->left;
            for (; iterationNode->right != nullNode; iterationNode = iterationNode->right) {}
        } else {
            /*
            * If you have no left child, then the next node smaller is to go up
            *           until you go left.
            */

            for (; iterationNode != nullNode; iterationNode = iterationNode->parent) {
                if (iterationNode == iterationNode->parent->right) {
                    iterationNode = iterationNode->parent;
                    break;
                }
            }
        }
    }
    D->iterationNode = iterationNode;

    // return
    if (iterationNode == nullNode) {  // the iteration has completed
        D->forwardsIteration = -1;
        return VAL_UNDEF;
    }
    return iterationNode->value;
}

void InOrderTreeWalk(FILE* out, Node NullNode, Node RootNode) {
    if (NullNode == NULL) {
        printf("ERROR: Dictionary ADT: calling InOrderTreeWalk(): NULL Node reference (1)\n");
        exit(EXIT_FAILURE);
    }
    if (RootNode == NULL) {
        printf("ERROR: Dictionary ADT: calling InOrderTreeWalk(): NULL Node reference (2)\n");
        exit(EXIT_FAILURE);
    }

    if (RootNode != NullNode) {
        InOrderTreeWalk(out, NullNode, RootNode->left);
        fprintf(out, KEY_FORMAT, RootNode->key); fprintf(out, "\n");
        InOrderTreeWalk(out, NullNode, RootNode->right);
    }
}

void PreOrderTreeWalk(FILE* out, Node NullNode, Node RootNode) {
    if (NullNode == NULL) {
        printf("ERROR: Dictionary ADT: calling PreOrderTreeWalk(): NULL Node reference (1)\n");
        exit(EXIT_FAILURE);
    }
    if (RootNode == NULL) {
        printf("ERROR: Dictionary ADT: calling PostOrderTreeWalk(): NULL Node reference (2)\n");
        exit(EXIT_FAILURE);
    }

    if (RootNode != NullNode) {
        fprintf(out, KEY_FORMAT, RootNode->key); fprintf(out, "\n");
        PreOrderTreeWalk(out, NullNode, RootNode->left);
        PreOrderTreeWalk(out, NullNode, RootNode->right);
    }
}

void PostOrderTreeWalk(FILE* out, Node NullNode, Node RootNode) {
    if (NullNode == NULL) {
        printf("ERROR: Dictionary ADT: calling PostOrderTreeWalk(): NULL Node reference (1)\n");
        exit(EXIT_FAILURE);
    }
    if (RootNode == NULL) {
        printf("ERROR: Dictionary ADT: calling PostOrderTreeWalk(): NULL Node reference (2)\n");
        exit(EXIT_FAILURE);
    }

    if (RootNode != NullNode) {
        PostOrderTreeWalk(out, NullNode, RootNode->left);
        PostOrderTreeWalk(out, NullNode, RootNode->right);
        fprintf(out, KEY_FORMAT, RootNode->key); fprintf(out, "\n");
    }
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

    Node nullNode = newNode(NULL, KEY_UNDEF, VAL_UNDEF, FALSE);
    nullNode->parent = nullNode;
    nullNode->left = nullNode;
    nullNode->right = nullNode;
    D->nullNode = nullNode;
    
    D->root = nullNode;
    D->size = 0;
    
    D->forwardsIteration = -1;
    D->iterationNode = nullNode;
    
    return D;
}

// freeDictionary()
// Frees heap memory associated with *pD, sets *pD to NULL.
void freeDictionary(Dictionary* pD) {
    if (pD != NULL && *pD != NULL) {
        makeEmpty(*pD);
        freeNode(&((*pD)->nullNode));
        
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

    for (Node thisNode = D->root; thisNode != D->nullNode;) {
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
    if (getUnique(D) == TRUE && lookup(D, k) != VAL_UNDEF) {
        printf("ERROR: Dictionary ADT: calling insert(): non-unique keys cannot be added to unique instances\n");
        exit(EXIT_FAILURE);
    }

    Node z = newNode(D->nullNode, k, v, TRUE);

    // RB_Insert
    Node y = D->nullNode;
    Node x = D->root;
    while (x != D->nullNode) {
        y = x;
        if (KEY_CMP(z->key, x->key) < 0) {
            x = x->left;
        } else {
            x = x->right;
        }
    }
    z->parent = y;
    if (y == D->nullNode) {
        D->root = z;
    } else if (KEY_CMP(z->key, y->key) < 0) {
        y->left = z;
    } else {
        y->right = z;
    }
    z->left = D->nullNode;
    z->right = D->nullNode;

    // RB_InsertFixUp
    while (z->parent->isRed == TRUE) {
        if (z->parent == z->parent->parent->left) {
            y = z->parent->parent->right;
            if (y->isRed == TRUE) {
                z->parent->isRed = FALSE;
                y->isRed = FALSE;
                z->parent->parent->isRed = TRUE;
                z = z->parent->parent;
            } else {
                if (z == z->parent->right) {
                    z = z->parent;
                    LeftRotate(D, z);
                }
                z->parent->isRed = FALSE;
                z->parent->parent->isRed = TRUE;
                RightRotate(D, z->parent->parent);
            }
        } else {
            y = z->parent->parent->left;
            if (y->isRed == TRUE) {
                z->parent->isRed = FALSE;
                y->isRed = FALSE;
                z->parent->parent->isRed = TRUE;
                z = z->parent->parent;
            } else {
                if (z == z->parent->left) {
                    z = z->parent;
                    RightRotate(D, z);
                }
                z->parent->isRed = FALSE;
                z->parent->parent->isRed = TRUE;
                LeftRotate(D, z->parent->parent);
            }
        }
    }
    D->root->isRed = FALSE;

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
    if (lookup(D, k) == VAL_UNDEF) {
        printf("ERROR: Dictionary ADT: calling delete(): key does not exist in tree\n");
        exit(EXIT_FAILURE);
    }
    
    Node z = D->nullNode;  // could just use regular NULL
    if (KEY_CMP(D->root->key, k) == 0) {
        z = D->root;
    } else {
        for (Node parentNode = D->root;;) {
            int keyCmpResult = KEY_CMP(k, parentNode->key);
            if (keyCmpResult > 0) {  // k is bigger than parentNode->key
                if (KEY_CMP(parentNode->right->key, k) == 0) {
                    z = parentNode->right;
                    break;
                }
                parentNode = parentNode->right;
            } else if (keyCmpResult < 0) {  // parentNode->key is bigger than k
                if (KEY_CMP(parentNode->left->key, k) == 0) {
                    z = parentNode->left;
                    break;
                }
                parentNode = parentNode->left;
            } else {  // k equals parentNode->key, this would mean we missed the correct node
                printf("ERROR: Dictionary ADT: calling delete(): impossible logic path\n");
                exit(EXIT_FAILURE);
            }
        }
    }
    Node deleteNodeRef = z;
    
    // RB_Delete
    Node x;
    Node y = z;
    int y_original_isRed = y->isRed;
    if (z->left == D->nullNode) {
        x = z->right;
        RB_Transplant(D, z, z->right);
    } else if (z->right == D->nullNode) {
        x = z->left;
        RB_Transplant(D, z, z->left);
    } else {
        y = TreeMinimum(D->nullNode, z->right);
        y_original_isRed = y->isRed;
        x = y->right;
        if (y->parent == z) {
            x->parent = y;
        } else {
            RB_Transplant(D, y, y->right);
            y->right = z->right;
            y->right->parent = y;
        }
        RB_Transplant(D, z, y);
        y->left = z->left;
        y->left->parent = y;
        y->isRed = z->isRed;
    }
    if (y_original_isRed == FALSE) {

        // RB_DeleteFixUp
        while (x != D->root && x->isRed == FALSE) {
            if (x == x->parent->left) {
                Node w = x->parent->right;
                if (w->isRed == TRUE) {
                    w->isRed = FALSE;
                    x->parent->isRed = TRUE;
                    LeftRotate(D, x->parent);
                    w = x->parent->right;
                }
                if (w->left->isRed == FALSE && w->right->isRed == FALSE) {
                    w->isRed = TRUE;
                    x = x->parent;
                } else {
                    if (w->right->isRed == FALSE) {
                        w->left->isRed = FALSE;
                        w->isRed = TRUE;
                        RightRotate(D, w);
                        w = x->parent->right;
                    }
                    w->isRed = x->parent->isRed;
                    x->parent->isRed = FALSE;
                    w->right->isRed = FALSE;
                    LeftRotate(D, x->parent);
                    x = D->root;
                }
            } else {
                Node w = x->parent->left;
                if (w->isRed == TRUE) {
                    w->isRed = FALSE;
                    x->parent->isRed = TRUE;
                    RightRotate(D, x->parent);
                    w = x->parent->left;
                }
                if (w->right->isRed == FALSE && w->left->isRed == FALSE) {
                    w->isRed = TRUE;
                    x = x->parent;
                } else {
                    if (w->left->isRed == FALSE) {
                        w->right->isRed = FALSE;
                        w->isRed = TRUE;
                        LeftRotate(D, w);
                        w = x->parent->left;
                    }
                    w->isRed = x->parent->isRed;
                    x->parent->isRed = FALSE;
                    w->left->isRed = FALSE;
                    RightRotate(D, x->parent);
                    x = D->root;
                }
            }
        }
        x->isRed = FALSE;
    }

    // stop iteration if deleteNode was iteration node
    if (deleteNodeRef == D->iterationNode) {
        D->forwardsIteration = -1;
        D->iterationNode = D->nullNode;
    }
    
    // free deleteNode    
    freeNode(&deleteNodeRef);

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

    freeAllNodes(D->nullNode, D->root);
    D->root = D->nullNode;
    D->size = 0;

    // cancel iteration
    D->forwardsIteration = -1;
    D->iterationNode = D->nullNode;
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

    if (D->root == D->nullNode) {
        return VAL_UNDEF;
    }
    D->forwardsIteration = TRUE;
    Node firstNode = D->root;
    for (; firstNode->left != D->nullNode; firstNode = firstNode->left) {}
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

    if (D->root == D->nullNode) {
        return VAL_UNDEF;
    }
    D->forwardsIteration = FALSE;
    Node lastNode = D->root;
    for (; lastNode->right != D->nullNode; lastNode = lastNode->right) {}
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

    if (D->iterationNode == D->nullNode) {
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

    if (D->iterationNode == D->nullNode) {
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

    if (getUnique(D) == FALSE) {
        return moveINORDER(D, TRUE);
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

    if (getUnique(D) == FALSE) {
        return moveINORDER(D, FALSE);
    }
    return move(D, FALSE);
}


// Other operations -----------------------------------------------------------

// printDictionary()
// Prints a text representation of D to the file pointed to by out. Each key-
// value pair is printed on a single line, with the two items separated by a
// single space.  The pairs are printed in the order defined by the operator
// KEY_CMP().
void printDictionary(FILE* out, Dictionary D, const char* ord) {
    if (D == NULL) {
        printf("ERROR: Dictionary ADT: calling printDictionary(): NULL Dictionary reference\n");
        exit(EXIT_FAILURE);
    }

    if (strcmp(ord, "in") == 0) {
        InOrderTreeWalk(out, D->nullNode, D->root);
    } else if (strcmp(ord, "pre") == 0) {
        PreOrderTreeWalk(out, D->nullNode, D->root);
    } else if (strcmp(ord, "post") == 0) {
        PostOrderTreeWalk(out, D->nullNode, D->root);
    }
}
