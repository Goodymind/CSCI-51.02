#include <iostream>
using namespace std;

void changeChar(char* x){ //takes character pointer x
    // in case of lowercase
    if (*x >= 'a' && *x <= 'z') {
        if (*x == 'z') {
            *x = 'a'; // z to a
        } else {
            *x += 1; // move to next letter
        }
    }

    //in case of uppercase
    else if (*x >= 'A' && *x <= 'Z') {
        if (*x == 'Z') {
            *x = 'A'; // Z to A
        } else {
            *x += 1; // move to next letter
        }
    }

}

int main(void)
{
    string name; // Name variable
    string nameCopy; //copy for (4)

    cin >> name; // takes name input (1)

    cout << name << endl; // outputs the name (2)

    cout << name[0]; // Print first letter (3)
    for (int i = 1; i < name.length(); i++) {
        cout << "#"; // others as #
    }
    cout << endl;

    nameCopy = name; // copy to change letters
    for (int i = 0; i < nameCopy.length(); i++) {
        char current = nameCopy[i];
        
        // Check lowercase letters
        if (current >= 'a' && current <= 'z') {
            if (current == 'z') {
                nameCopy[i] = 'a'; // z to a
            } else {
                nameCopy[i] = current + 1; // Shift to next letter
            }
        }
        // Check uppercase letters
        else if (current >= 'A' && current <= 'Z') {
            if (current == 'Z') {
                nameCopy[i] = 'A'; // Z to A
            } else {
                nameCopy[i] = current + 1; // Shift to next letter
            }
        }
        // Other characters stay the same
    }
    cout << nameCopy << endl; // Print shifted name

    // Use a function to change letters (5)
    for (int i = 0; i < name.length(); i++)
    {
        changeChar(&name[i]); // pass address of each character
    }
    cout << name << endl; // output the modified string
    
    return 0;
}