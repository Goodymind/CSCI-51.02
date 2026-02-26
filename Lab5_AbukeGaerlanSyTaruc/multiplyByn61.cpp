#include "IntArray.h"
 
void multiplyByn61(IntArray *p) 
{ 
    for (int i = 0; i < p->size; i++)
    {
        p->elements[i] = p->elements[i] * -61;
    }
    
} 