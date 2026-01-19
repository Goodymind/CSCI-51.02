#include <iostream>
using namespace std;
// useful tests:
//      abcdefghijklmnopqrstuvwxyz
//      ABCDEFGHIJKLMNOPQRSTUVWXYZ
//      abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ
//      1234567890ABCDEFGHIJKLMNOPQRSTUVWXYZ!@#$%^&*()

void incrementChar(char* cpointer)                                                      // function that accepts a reference to a character to be incremented
{

    if ((*cpointer >= 65 && *cpointer < 90) || (*cpointer >= 97 && *cpointer < 122))    // if character is in [A-Ya-y]
    {
        (*cpointer) ++;                                                                 // increment to next character;
    }
    else if (*cpointer == 90)                                                           // if character is Z
    {
        *cpointer = 65;                                                                 // Z -> A
    }
    else if (*cpointer == 122)                                                          // if character is z
    {
        *cpointer = 97;                                                                 // z -> a;
    }
}

int main(void)
{
    string name; // declaration of name variable

    cin >> name; // (1) accept console input and store to name variable

    cout << name << endl; // (2) print name value

    cout << name[0];                        // (3) print first letter;
    for (int i = 1; i < name.length(); i++) // for loop that outputs # on the
    {
        cout << "#"; // remaining number of letters
    }
    cout << endl; // finish the line.

    for (int i = 0; i < name.length(); i++) // (4) iterates through each character
    {
        char c = name[i];                                // get current character
        if ((c >= 65 && c < 90) || (c >= 97 && c < 122)) // if character is in [A-Ya-y]
        {
            c++; // increment to next character;
        }
        else if (c == 90) // if character is Z
        {
            c = 65; // Z -> A
        }
        else if (c == 122) // if character is z
        {
            c = 97; // z -> a;
        }
        cout << c;
    }
    cout << endl; // finish the name

    for (int i = 0; i < name.length(); i++) // (5) Does the same as number 4
    {
        incrementChar(&name[i]);            // passes the reference of the character to the function.
    }

    cout << name << endl;
    return 0;
}