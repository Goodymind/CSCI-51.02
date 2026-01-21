#include <iostream>
#include <stdio.h>
#include <stdlib.h>
using namespace std;

void rstrip(string* s) 
{
    int trimsize = 0;
    for (int i = (*s).length() - 1; i >= 0; i--)
    {
        if ((*s)[i] == 32, (*s)[i] == 9, (*s)[i] == 10, (*s)[i] == 11, (*s)[i] == 12, (*s)[i] == 13) 
        {
            trimsize++;
        }
        else 
        {
            break;
        }
    }
    (*s).resize((*s).length() - trimsize);
    (*s).shrink_to_fit();
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
        getline(cin >> ws, message);                    // discard the space after y (ws), save message string as whole

        cout << "Agent #" << i << " is at (" << x << "," << y << ")" << endl;   // output coordinates of Agent #i
        
        int m = parseMessage(message);                                          // custom function;
        
        if (m == 0)                                                             // if nondigit message
        {
            rstrip(&message);
            cout << "Agent #" << i << " yells: \"" << message << "\"" << endl;  // output  message
        }
        else
        {
            cout << "Agent #" << i << " holds up the number: " << m << endl;    // output the number
        }
    }
    return 0;
}