//-----------------------------------------------------------------------------
// Cole Anderson
// comander
// pa5
//
// List.cpp
// Implementation file for List ADT
//-----------------------------------------------------------------------------


#include<iostream>
#include<string>

#include"List.h"

using namespace std;



// Private Constructor --------------------------------------------------------

// Node constructor
List::Node::Node(int x) {
	data = x;
	next = nullptr;
	prev = nullptr;
}


// Class Constructors & Destructors -------------------------------------------

// Creates new List in the empty state.
List::List() {
	Node* d1 = new Node(0);
	Node* d2 = new Node(0);
	d1->next = d2;
	d2->prev = d1;

	frontDummy = d1;
	backDummy = d2;
	beforeCursor = d1;
	afterCursor = d2;
	pos_cursor = 0;
	num_elements = 0;
}

// Copy constructor.
List::List(const List& L) {
	
	// create empty List

	Node* d1 = new Node(0);
	Node* d2 = new Node(0);
	d1->next = d2;
	d2->prev = d1;

	frontDummy = d1;
	backDummy = d2;
	beforeCursor = d1;
	afterCursor = d2;
	pos_cursor = 0;
	num_elements = 0;

	// load elements of L into this
	moveBack();
	for (Node* LNode = L.frontDummy->next; size() < L.num_elements; LNode = LNode->next) {
		insertBefore(LNode->data);
	}
}

// Destructor
List::~List() {
	clear();
	Node* F = frontDummy;
	Node* B = backDummy;
	delete F;
	delete B;
}


// Access functions --------------------------------------------------------

// isEmpty()
// Returns true if this List is empty, false otherwise.
bool List::isEmpty() {
	return num_elements == 0;
}

// size()
// Returns the size of this List.
int List::size() {
	return num_elements;
}

// position()
// Returns the position of the cursor in this List. The value returned
// will be in the range 0 to size().
int List::position() {
	return pos_cursor;
}


// Manipulation procedures -------------------------------------------------

// moveFront()
// Moves cursor to position 0 in this List.
void List::moveFront() {
	beforeCursor = frontDummy;
	afterCursor = frontDummy->next;
	pos_cursor = 0;
}

// moveBack()
// Moves cursor to position size() in this List.
void List::moveBack() {
	afterCursor = backDummy;
	beforeCursor = backDummy->prev;
	pos_cursor = size();
}

// peekNext()
// Returns the element after the cursor.
// pre: position()<size()
int List::peekNext() {
	if (!(position() < size())) {
		cerr << "ERROR: List ADT: calling peekNext(): position not less than size\n" << endl;
		exit(EXIT_FAILURE);
	}

	return afterCursor->data;
}

// peekPrev()
// Returns the element before the cursor.
// pre: position()>0
int List::peekPrev() {
	if (!(position() > 0)) {
		cerr << "ERROR: List ADT: calling peekPrev(): position not greater than 0\n" << endl;
		exit(EXIT_FAILURE);
	}

	return beforeCursor->data;
}

// moveNext()
// Advances cursor to next higher position. Returns the List element that
// was passed over. 
// pre: position()<size() 
int List::moveNext() {
	if (!(position() < size())) {
		cerr << "ERROR: List ADT: calling moveNext(): position not less than size\n" << endl;
		exit(EXIT_FAILURE);
	}

	beforeCursor = afterCursor;
	afterCursor = beforeCursor->next;
	pos_cursor++;
	return beforeCursor->data;
}

// movePrev()
// Advances cursor to next lower position. Returns the List element that
// was passed over. 
// pre: position()>0
int List::movePrev() {
	if (!(position() > 0)) {
		cerr << "ERROR: List ADT: calling movePrev(): position not greater than 0\n" << endl;
		exit(EXIT_FAILURE);
	}

	afterCursor = beforeCursor;
	beforeCursor = afterCursor->prev;
	pos_cursor--;
	return afterCursor->data;
}

// insertAfter()
// Inserts x after cursor.
void List::insertAfter(int x) {
	Node* NN = new Node(x);

	beforeCursor->next = NN;
	afterCursor->prev = NN;
	NN->prev = beforeCursor;
	NN->next = afterCursor;

	afterCursor = NN;
	num_elements++;
}

// insertBefore()
// Inserts x before cursor.
void List::insertBefore(int x) {
	Node* NN = new Node(x);
	
	afterCursor->prev = NN;
	beforeCursor->next = NN;
	NN->next = afterCursor;
	NN->prev = beforeCursor;

	beforeCursor = NN;
	pos_cursor++;
	num_elements++;
}

// eraseAfter()
// Deletes element after cursor.
// pre: position()<size()
void List::eraseAfter() {
	if (!(position() < size())) {
		cerr << "ERROR: List ADT: calling eraseAfter(): position not less than size\n" << endl;
		exit(EXIT_FAILURE);
	}

	Node* eraseNode = afterCursor;

	beforeCursor->next = afterCursor->next;
	afterCursor->next->prev = beforeCursor;

	afterCursor = beforeCursor->next;
	num_elements--;

	delete eraseNode;
}

// eraseBefore()
// Deletes element before cursor.
// pre: position()>0
void List::eraseBefore() {
	if (!(position() > 0)) {
		cerr << "ERROR: List ADT: calling eraseBefore(): position not greater than 0\n" << endl;
		exit(EXIT_FAILURE);
	}

	Node* eraseNode = beforeCursor;

	afterCursor->prev = beforeCursor->prev;
	beforeCursor->prev->next = afterCursor;

	beforeCursor = afterCursor->prev;
	pos_cursor--;
	num_elements--;

	delete eraseNode;
}

// findNext()
// Starting from the current cursor position, performs a linear search (in 
// the direction front-to-back) for the first occurrence of the element x.
// If x is found, places the cursor immediately after the found element (so 
// eraseBefore() would remove the found element), and returns the final 
// cursor position. If x is not found, places the cursor at position size(),
// and returns -1. 
int List::findNext(int x) {
	for (; position() < size();) {
		moveNext();
		if (peekPrev() == x) {
			return position();
		}
	}
	return -1;
}

// findPrev()
// Starting from the current cursor position, performs a linear search (in 
// the direction back-to-front) for the first occurrence of the element x.
// If x is found, places the cursor immediately before the found element (so 
// eraseAfter() would remove the found element), and returns the final 
// cursor position. If x is not found, places the cursor at position 0, and 
// returns -1. 
int List::findPrev(int x) {
	for (; position() > 0;) {
		movePrev();
		if (peekNext() == x) {
			return position();
		}
	}
	return -1;
}

// cleanup()
// Removes any repeated elements in this List, leaving only unique data
// values. The order of the remaining elements is obtained by retaining
// the frontmost occurrance of each element, and removing all other 
// occurances. The cursor is not moved with respect to the retained 
// elements, i.e. it lies between the same two retained elements that it 
// did before cleanup() was called.
void List::cleanup() {
	
	// save initial list before cursor
	List tempList = List();
	for (; position() > 0; movePrev()) {
		tempList.insertAfter(peekPrev());
	}

	// cleanup
	int startIndex = 0;
	for (moveFront(); position() < size();) {
		if (position() == startIndex) {  // this is the first occurrance of some value
			int value = peekNext();
			moveNext();
			for (; position() < size();) {
				if (peekNext() == value) {
					eraseAfter();
				} else {
					moveNext();
				}
			}
			startIndex++;
			moveFront();
		} else {
			moveNext();
		}
	}

	// fix cursor
	moveFront();
	if (tempList.size() == 0) {
		return;
	}
	int dataBeforeCursor = -1;
	for (tempList.moveBack();;) {
		tempList.movePrev();
		dataBeforeCursor = tempList.peekNext();
		tempList.findPrev(dataBeforeCursor);
		if (tempList.position() == 0 && 
				(tempList.peekNext() != dataBeforeCursor || tempList.size() == 1)) {
			break;
		}
		tempList.moveBack();
		tempList.eraseBefore();
	}
	findNext(dataBeforeCursor);
}

// clear()
// Deletes all elements in this List, setting it to the empty state.
void List::clear() {
	for (; !isEmpty();) {
		moveFront();
		eraseAfter();
	}
}

// concat()
// Returns a new List consisting of the elements of this List, followed
// the elements of L. The returned List's cursor will be at its front
// (position 0).
List List::concat(const List& L) {
	List NL = List();
	List LCopy = List(L);

	for (moveFront(); position() < size(); moveNext()) {
		NL.insertAfter(peekNext());
		NL.moveNext();
	}
	for (LCopy.moveFront(); LCopy.position() < LCopy.size(); LCopy.moveNext()) {
		NL.insertAfter(LCopy.peekNext());
		NL.moveNext();
	}
	NL.moveFront();

	return NL;
}


// Other Functions ---------------------------------------------------------

// to_string()
// Returns a string representation of this List consisting of a comma 
// separated sequence of elements, surrounded by parentheses.
string List::to_string() {
	if (size() == 0) {
		return "()";
	}
	string s = "(";

	for (moveFront();;) {
		s += std::to_string(peekNext());
		moveNext();
		if (position() == size()) {
			break;
		}
		s += ",";
	}

	s += ")";
	return s;
}

// equals()
// Returns true if and only if this List is the same integer sequence as R.
// The cursors in this List and in R are unchanged.
bool List::equals(const List& R) {
	if (num_elements != R.num_elements) {
		return false;
	}
	Node* thisBeforeCursor = beforeCursor;
	Node* thisAfterCursor = afterCursor;
	int thisPosCursor = pos_cursor;
	List RCopy = List(R);
	bool rtn = true;
	for (moveFront(), RCopy.moveFront(); position() < size(); moveNext(), RCopy.moveNext()) {
		if (peekNext() != RCopy.peekNext()) {
			rtn = false;
			break;
		}
	}
	beforeCursor = thisBeforeCursor;
	afterCursor = thisAfterCursor;
	pos_cursor = thisPosCursor;
	return rtn;
}


// Overriden Operators -----------------------------------------------------

// operator<<()
// Inserts string representation of L into stream.
ostream& operator<<(ostream& stream, List& L) {
	return stream << L.List::to_string();
}

// operator==()
// Returns true if and only if A is the same integer sequence as B. The 
// cursors in both Lists are unchanged are unchanged.
bool operator==(List& A, const List& B) {
	return A.List::equals(B);
}

// operator=()
// Overwrites the state of this List with state of L.
List& List::operator=(const List& L) {
	if (this != &L) {  // this is not self assignment
		List copy = L;
		for (moveFront(); size() > 0;) {
			eraseAfter();
		}
		for (copy.moveBack(); copy.position() > 0; copy.movePrev()) {
			insertAfter(copy.peekPrev());
		}
	}
	return *this;  // return this with the new data installed 
}
