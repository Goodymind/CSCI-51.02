#include <iostream>
#include <string.h>
#include <stdio.h>
#include <string>
using namespace std;

int main(void) {
    int agentCount;
    cin >> agentCount;

    for (int i = 1; i <= agentCount; i++)
    {
        int xValue, yValue, placeholderInt;
        string message;

        cin >> xValue >> yValue;

        getline(cin, message);

        placeholderInt = atoi(message.c_str());        

        cout << "Agent #" << i << " is at (" << xValue << ", " << yValue << ")" << "\n";

        if (to_string(placeholderInt) == message)
        {
            cout << "Agent #" << i << " holds up the number " << message << endl;
        }
        else
        {
            cout << "Agent #" << i << " yells: \"" << message << '"' << endl;
        }
    }
    return 0;
}