#include <iostream>
#include <stdio.h>
#include <stdlib.h>
using namespace std;

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
            cout << "Agent #" << i << " yells: \"" << message << "\"" << endl;  // output  message
        }
        else
        {
            cout << "Agent #" << i << " holds up the number: " << m << endl;    // output the number
        }
    }
    return 0;
}