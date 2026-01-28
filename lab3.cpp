#include <iostream>
using namespace std;

struct IntNode
{                         // linked list with basic operations
    int data;             // integer value
    IntNode* next = NULL; // next node
    IntNode* previous = NULL;
};

struct IntList
{
    IntNode* first; // pointer to first node
    IntNode* last;  // pointer to last node

    IntList()
    {                 // constructors
        first = NULL; // initialize empty list
        last = NULL;
    }

    // 2A create 1st node
    IntNode* createFirstNode(int value)
    {
        IntNode* newNode = new IntNode; // allocate new node
        newNode->data = value;          // set data
        newNode->next = NULL;           // no next one yet

        first = newNode;
        last = newNode;
        return newNode;
    }

    // 2B insert after a node
    IntNode* insertAfter(IntNode* prevNode, int value)
    {
        IntNode* newNode = new IntNode;         // allocate new node
        newNode->data = value;                  // store value

        IntNode* nextNode = prevNode->next;     // set address node to previous node's 'next node' pointer

        prevNode->next = newNode;               // set node after previous node
        newNode->previous = prevNode;           // set this node's previous node to the previous node

        if (nextNode != NULL)                   // if next node is the last node in list
        {
            newNode->next = nextNode;           // set next node 
            nextNode->previous = newNode;       // set the pointer back to previous node
        }

        return newNode;
    }

    // 2C get 1st node
    IntNode* getFirst()
    {
        return first; // return 1st pointer
    }

    // 2D get next node
    IntNode* getNext(IntNode* currentNode)
    {
        return currentNode->next; // return next node
    }

    // 2E delete a node
    void deleteNode(IntNode* currentNode)          // when deleting node...
    {
        IntNode* prevNode = currentNode->previous; // take address of previous node
        IntNode* nextNode = currentNode->next;     // take address of next node
        prevNode->next = nextNode;                 // connect previous and next node
        nextNode->previous = prevNode;

        delete currentNode;                         
    }

    // helper function to easily add nodes
    IntNode* add(int value)
    {
        if (first == NULL)                                      // if list is empty
        {
            IntNode* node = this->createFirstNode(value);       // create first node 
            return node;
        }

        IntNode* node = this->insertAfter(last, value);         // make node and insert after last node
        last = node;                                            // set pointer to last node
        return node;                                            
    }   

    // 2F display list

    ~IntList()
    {                             // destructor
        IntNode* current = first; // start at first
        while (current != NULL)
        {                                  // while nodes exist
            IntNode* next = current->next; // have next
            delete current;                // free current
            current = next;                // move to next
        }
    }
};

struct IntStack
{
    private: IntNode* top = NULL;   // setting top of stack as null
    public: int size;               // setting size variable

    IntStack()
    {
        size = 0;   // stacks start empty
    }

    void push(int i)
    {
        IntNode* node = new IntNode();  // make new node
        node->data = i;                 // store data i 
        size++;                         // update size
        if (top == NULL)
        {                               // if stack is empty
            top = node;                 // assign node to top
            return;
        }

        top->next = node;               // update next to newly pushed node
        node->previous = top;           // set pointer 
        top = node;                     // new top node
    }

    int pop()
    {

        if (top == NULL || size == 0)       // runtime error if popping empty stack
        {
            throw runtime_error("Popping an Empty Stack!");
        }

        int output = top->data;             // stores topmost data

        IntNode* trash = top;               // set topmost node as trash
        top = top->previous;                // reassign top node
        delete trash;                       // delete trash
        trash = NULL;                       // make pointer null
        return output;                      // returns recently popped data
    }

    int currentSize()
    {
        return size;        
    }

    ~IntStack()
    {
        while (top != NULL)         // while stack has data
        {
            IntNode* trash = top;   // set top node to trash
            top = top->previous;    // reassign top
            delete trash;           // delete trash
            trash = NULL;           // make pointer null
        }
    }
};

int main()
{

    IntList* myList = new IntList();

    IntNode* firstnode = myList->createFirstNode(5);
    IntNode* one = myList->add(1);
    IntNode* two = myList->add(2);
    IntNode* three = myList->add(3);
    IntNode* four = myList->add(4);

    cout << "data of firstnode->next: " << myList->getNext(firstnode)->data << endl;
    cout << "first->data:" << myList->first->data << endl;
    cout << "last->data" << myList->last->data << endl;
    cout << "two->next->data" << myList->getNext(two)->data << endl;
    cout << "deleting node two" << endl;
    myList->deleteNode(two);
    cout << "one->next->data" << myList->getNext(one)->data << endl;
    myList->insertAfter(one, 9);
    cout << "check inserted data: " << myList->getNext(one)->data << endl;;
    delete myList;

    IntStack* stack = new IntStack();
    stack->push(2);
    stack->push(4);
    stack->push(6);
    cout << stack->currentSize() << endl;
    cout << stack->pop() << endl;
    cout << stack->pop() << endl;
    // cout << stack->pop() << endl;
    // cout << stack->pop() << endl; //throws an error when popping an empty stack
    delete stack;
}
