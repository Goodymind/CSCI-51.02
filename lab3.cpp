#include <iostream>
using namespace std;

struct IntNode
{                         // linked list with basic operations
    int data;             // integer calue
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
        IntNode* newNode = new IntNode;
        newNode->data = value;

        IntNode* nextNode = prevNode->next;

        prevNode->next = newNode;
        newNode->previous = prevNode;

        if (nextNode != NULL)
        {
            newNode->next = nextNode;
            nextNode->previous = newNode;
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
    void deleteNode(IntNode* currentNode)
    {
        IntNode* prevNode = currentNode->previous;
        IntNode* nextNode = currentNode->next;
        prevNode->next = nextNode;
        nextNode->previous = prevNode;

        delete currentNode;
    }

    // helper function to easily add nodes
    IntNode* add(int value)
    {
        if (first == NULL)
        {
            IntNode* node = this->createFirstNode(value);
            return node;
        }

        IntNode* node = this->insertAfter(last, value);
        last = node;
        return node;
    }

    // 2F display list

    ~IntList()
    {                             // destructor
        IntNode* current = first; // start at first
        while (current != NULL)
        {                                  // while nodes exist
            IntNode* next = current->next; // aave next
            delete current;                // free current
            current = next;                // move to next
        }
    }
};

struct IntStack
{

    int size;
    IntNode* top = NULL;

    IntStack()
    {
        size = 0;
    }

    void push(int i)
    {
        IntNode* node = new IntNode();
        node->data = i;
        size++;
        if (top == NULL)
        {
            top = node;
            return;
        }

        top->next = node;
        node->previous = top;
        top = node;
    }

    int pop()
    {

        if (top == NULL || size == 0)
        {
            throw runtime_error("Popping an Empty Stack!");
        }

        int output = top->data;

        IntNode* trash = top;
        top = top->previous;
        delete trash;
        trash = NULL;
        return output;
    }

    int currentSize()
    {
        return size;
    }
};

int main()
{

    // IntList* myList = new IntList();

    // IntNode* firstnode = myList->createFirstNode(5);
    // IntNode* one = myList->add(1);
    // IntNode* two = myList->add(2);
    // IntNode* three = myList->add(3);
    // IntNode* four = myList->add(4);

    // cout << "data of firstnode->next: " << myList->getNext(firstnode)->data << endl;
    // cout << "first->data:" << myList->first->data << endl;
    // cout << "last->data" << myList->last->data << endl;
    // cout << "two->next->data" << myList->getNext(two)->data << endl;
    // cout << "deleting node two" << endl;
    // myList->deleteNode(two);
    // cout << "one->next->data" << myList->getNext(one)->data << endl;
    // delete myList;

    IntStack* stack = new IntStack();
    stack->push(2);
    stack->push(4);
    stack->push(6);
    cout << stack->currentSize() << endl;
    cout << stack->pop() << endl;
    cout << stack->pop() << endl;
    cout << stack->pop() << endl;
    // cout << stack->pop() << endl; //throws an error when popping an empty stack
    delete stack;
}
