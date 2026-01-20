#include <iostream>
#include <stdio.h>
#include <stdlib.h>
using namespace std;

int parseMessage(string msgp)
{
    for (int i = 0; i < msgp.length(); i++)
    {
        if (!isdigit(msgp[i]))
        {
            return 0;
        }
    }

    return atoi(msgp.c_str());
}

int main(void)
{
    int n, x, y;
    string message;
    cin >> n;
    for (int i = 1; i <= n; i++)
    {
        string line;

        cin >> x >> y;
        getline(cin >> ws, message);

        cout << "Agent #" << i << " is at (" << x << "," << y << ")" << endl;
        
        int m = parseMessage(message);
        
        if (m == 0) 
        {
            cout << "Agent #" << i << " yells: \"" << message << "\"" << endl;
        }
        else
        {
            cout << "Agent #" << i << " holds up the number: " << m << endl;
        }
    }
    return 0;
}