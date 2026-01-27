#include <iostream>
#include <cstddef>
//#include <stdio.h>
using namespace std;

struct IntNode{     // linked list with basic operations
    int data;       // integer calue
    IntNode* next;  // next node
};

struct IntList{
    IntNode* first;     // pointer to first node
    IntNode* head = first;

    IntList(){          // constructors
        head = NULL;    // initialize empty list
    }

    // 2A create 1st node
    IntNode* createFirstNode(int value){
        IntNode* newNode = new IntNode; // allocate new node
        newNode->data = value;          // set data
        newNode->next = NULL;           // no next one yet
        first = newNode;                // set as first
        return newNode;                 // return as new node
    }

    // 2B insert after a node
    IntNode* insertAfter(IntNode* prevNode, int value){
        IntNode* newNode = new IntNode;     
        newNode->data = value;              
        newNode->next = prevNode->next;     // link to prevNode's next
        prevNode->next = newNode;           // link prevNode to newNode
        return newNode;                     
    }

    // 2C get 1st node
    IntNode* getFirst(){
        return first;   // return 1st pointer
    }

    // 2D get next node
    IntNode* getNext(){
        return head->next; // return next node
    }

    // 2E delete a node
	IntNode* deleteNode(IntNode* node){
        //delete IntNode node;
        //node = NULL;
        return 0;
	}

    // 2F display list

    ~IntList() {                            // destructor
        IntNode* current = first;           // start at first
        while (current != NULL) {           // while nodes exist
            IntNode* next = current->next;  // aave next
            delete current;                 // free current
            current = next;                 // move to next
        }
    }
};

struct IntStack{
	IntNode* first;
    IntNode* bottom = first;

    IntStack(){
		bottom = NULL;
	}

	IntNode* pop(){
		if (bottom = NULL){
			return 0;
		}
		else {
			// not quite sure 
		}
	}
};

int main(){

    IntList myList; 

    myList.createFirstNode(8);
    cout << myList.first->data << endl;


}