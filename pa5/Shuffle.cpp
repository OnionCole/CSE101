//-----------------------------------------------------------------------------
// Cole Anderson
// comander
// pa5
//
// Shuffle.cpp
// Shuffle Client
//-----------------------------------------------------------------------------


#include<iostream>
#include<string>
#include"List.h"

using namespace std;


// HELPER FUNCS
void shuffle(List& D) {
    int firstHalfNumber = D.size() % 2 == 0 ? D.size() / 2 : (D.size() - 1) / 2;
    int value = -1;
    for (int i = 0; i < firstHalfNumber; i++) {
        D.moveFront();
        for (;;) {
            if (D.position() == firstHalfNumber + i) {
                D.insertAfter(value);

                break;
            } else {
                if (D.position() == 0) {
                    value = D.peekNext();
                    D.eraseAfter();
                }

                D.moveNext();
            }
        }
    }
}

// MAIN
int main(int argc, char* argv[]) {
    if (argc != 2) {
        cerr << "ERROR: Shuffle.cpp: main: argc incorrect\n" << endl;
        exit(EXIT_FAILURE);
    }
    int maxDeckSize = atoi(argv[1]);
    if (maxDeckSize < 1) {
        cerr << "ERROR: Shuffle.cpp: main: maxDeckSize < 1\n" << endl;
        exit(EXIT_FAILURE);
    }
    
    // print header
    cout << "deck size \t\tshuffle count" << endl;
    cout << "------------------------------" << endl;
    
    // figure and print shuffle count for each deck size up to maxDeckSize
    for (int deckSize = 1; deckSize < maxDeckSize + 1; deckSize++) {
        
        // build list
        List L = List();
        L.moveBack();
        for (int i = 0; i < deckSize; i++) {
            L.insertBefore(i);
        }

        // shuffle until same as original
        List shuffleL = List(L);
        for (int requiredShuffles = 1;; requiredShuffles++) {
            shuffle(shuffleL);
            if (L.equals(shuffleL)) {
                cout << " " << deckSize << "\t\t\t\t" << requiredShuffles << endl;
                break;
            }
        }
    }

    // return
    return(0);
}
