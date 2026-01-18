#include <iostream>
using namespace std;

char changeCharacter(char* x) // takes in a character pointer x
{
    *x += 1; // increments the value stored in the address which moves the letter by 1 for reasons explained in #4
    return *x; // returns the character pointer (#5)
}

int main(void)
{
    string name; // declaration of name variable to be used

    cin >> name; // takes input, stores it in name (#1)

    cout << name << endl; // outputs the name inputted (#2)

    char first_letter = name[0]; // stores first character of name in variable called first_letter
    cout << first_letter; // outputs it first
    for (int j = 1; j < name.length(); j++) // for the remaining length of name...
    {
        cout << "#"; // output a # sign (#3)
    }
    cout << endl;

    for (int k = 0; k < name.length(); k++) // for every letter in name...
    {
        char current_character = name[k]; // takes the current character of the name
        current_character++; // increments the character (because c++ stores characters just as 8bit integers) to "move" it one character, including keeping the capitalization
        cout << current_character; // outputs the character (#4)
    }
    cout << endl;

    for (int i = 0; i < name.length(); i++)
    {
        changeCharacter(&name[i]);
    }
    cout << name << endl;
}