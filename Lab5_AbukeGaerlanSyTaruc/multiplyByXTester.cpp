#include <iostream> 
#include "IntArray.h" 

using namespace std;

extern void multiplyByX(IntArray *p, int x); 

int main() 
{ 
    // - create a new IntArray with your own elements 
    //   (don't forget to properly allocate/deallocate the memory!) 
    // - print it out 
    // - multiply it by a number of your choosing via multiplyByX() 
    // - print the result 

    IntArray arr;
    arr.elements = new int[5];
    arr.elements[0] = 0;
    arr.elements[1] = 1;
    arr.elements[2] = 2;
    arr.elements[3] = 3;
    arr.elements[4] = 4;

    cout << arr.elements[0] << endl;
    cout << arr.elements[1] << endl;
    cout << arr.elements[2] << endl;
    cout << arr.elements[3] << endl;
    cout << arr.elements[4] << endl;

    delete arr.elements;
    arr.elements = NULL;

    return 0; 
} 