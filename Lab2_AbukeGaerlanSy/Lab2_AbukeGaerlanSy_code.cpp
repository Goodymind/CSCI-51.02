#include <iostream>
#include <stdio.h>
#include <stdlib.h>
using namespace std;

void rstrip(string* s) 
{
    int trimsize = 0;                               // initialize number of whitespaces to remove to 0l
    for (int i = s->length() - 1; i >= 0; i--)      // iterate through every character from the right.
    {
        if ((*s)[i] == 32 || (*s)[i] == 9 || (*s)[i] == 10 || (*s)[i] == 11 || (*s)[i] == 12 || (*s)[i] == 13) 
        { // if character is a whitespace (defined by ASCII), add 1 to trimsize;
            trimsize++;
        }
        else 
        {
            break;  // break loop if non whitespace detected.
        }
    }
    s->resize(s->length() - trimsize);      // string function that resizes a string given length. This removes the whitespaces
    s->shrink_to_fit();                     // reduces the string's capacity to its new reduced size.
}

int parseMessage(string msgp)
{
    for (int i = 0; i < msgp.length(); i++)             // iterates through every character.
    {
        if (!isdigit(msgp[i]))                          // checks if the character is a digit
        {
            return 0;                                   // returns 0 immediately if nondigit detected.
        }
    }

    return atoi(msgp.c_str());
}

int main(void)
{
    int n, x, y;                                        // Initialize Variables
    string message;                                     // --
    cin >> n;                                           // get n;
    for (int i = 1; i <= n; i++)                        // for loop that iterates from Agent #1 to Agent #n
    {

        cin >> x >> y;                                  // get x, y
        getline(cin >> ws, message);                    // discard the whitespace after y (ws), save message string as whole

        cout << "Agent #" << i << " is at (" << x << "," << y << ")" << endl;   // output coordinates of Agent #i
        
        int m = parseMessage(message);                                          // custom function;
        
        if (m == 0)                                                             // if nondigit message
        {
            rstrip(&message);                                                   // custom function that removes trailing whitespaces 
            cout << "Agent #" << i << " yells: \"" << message << "\"" << endl;  // output  message
        }
        else
        {
            cout << "Agent #" << i << " holds up the number: " << m << endl;    // output the number
        }
    }
    return 0;
}