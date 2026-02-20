#include <iostream>
using namespace std;


struct IntNode{     // linked list with basic operations
    int data;       // integer calue
    IntNode* next;  // next node
}

struct IntList{
    IntNode* first;     // pointer to first node

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
        return currentNode->next; // return next node
    }

    // 2E delete a node

    // 2F display list

    ~IntList() {                            // destructor
        IntNode* current = first;           // start at first
        while (current != NULL) {           // while nodes exist
            IntNode* next = current->next;  // aave next
            delete current;                 // free current
            current = next;                 // move to next
        }
    }
}

struct IntStack{

    // constructor
    // pop
    // push
    // size
}

int main(){

    IntList myList;

}
