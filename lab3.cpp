#include <iostream>
using namespace std;


struct IntNode{     // linked list with basic operations
    int data;       // integer calue
    IntNode* next;  // next node
}

struct IntList{
    IntNode* first;     // pointer to first node

    IntList(){
        head = NULL;    //initialize empty list
    }

    // 2A create 1st node
    IntNode* createFirstNode(int value){
        IntNode* newNode = new IntNode;
        newNode->data = value;  // set data
        newNode->next = NULL;   // no next one yet
        first = newNode;        // set as first
        return newNode;         // return as new node
    }

    // 2B insert after a node
    IntNode* insertAfter(){

    }

    // 2C get 1st node
    IntNode* getFirst(){
        return first;   // return 1st pointer
    }

    // 2D get next node
    IntNode* getNext(){

    }

    // 2E delete a node

    // 2F display list
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