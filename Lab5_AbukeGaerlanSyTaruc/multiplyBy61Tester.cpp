#include <iostream> 
#include "IntArray.h" 

using namespace std;

extern void multiplyBy61(IntArray *p); 
 
int main() 
{ 
    IntArray* arr = new IntArray;
    arr->size = 5;
    arr->elements = new int[5];
    arr->elements[0] = 0;
    arr->elements[1] = 1;
    arr->elements[2] = 2;
    arr->elements[3] = 3;
    arr->elements[4] = 4;

    cout << arr->elements[0] << endl;
    cout << arr->elements[1] << endl;
    cout << arr->elements[2] << endl;
    cout << arr->elements[3] << endl;
    cout << arr->elements[4] << endl;

    multiplyBy61(arr);

    cout << arr->elements[0] << endl;
    cout << arr->elements[1] << endl;
    cout << arr->elements[2] << endl;
    cout << arr->elements[3] << endl;
    cout << arr->elements[4] << endl;

    delete arr->elements;
    arr->elements = NULL;

    delete arr;
    arr = NULL;

    return 0; 
} 
