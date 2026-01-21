#include <iostream>     
#include <string>       
#include <stdlib.h>     
#include <stdio.h>      

using namespace std;

int main(void)
{
    int N;
    cin >> N;   // number of agents

    for (int i = 1; i <= N; i++)
    {
        int x, y;
        string message;

        cin >> x >> y;          // read coordinates
        
        getline(cin, message);  // read message 

        cout << "Agent #" << i << " is at (" << x << ", " << y << ")" << endl;

        // convert message to integer
        int num = atoi(message.c_str());

        // convert integer back to string
        char numStr[20];
        snprintf(numStr, 20, "%d", num);

        // check if message is exactly one integer
        if (message == numStr)
        {
            cout << "Agent #" << i << " holds up the number: "
                 << num << endl;
        }
        else
        {
            cout << "Agent #" << i << " yells: \""
                 << message << "\"" << endl;
        }
    }

    return 0;
}
