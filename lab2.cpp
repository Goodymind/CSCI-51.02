#include <iostream>
#include <string>

using namespace std;

int main(){
    
    int N;      // number of agents
    cin >> N;   // Read the first line: number of agents

    //process agents

    for (int i = 1; i <= N; i++)
    {
        int x, y;         // agent coordinates
        string message;   // message after X and Y

        // Read the X and Y coordinates
        cin >> x >> y;

        // Read the rest of the line as the message
        getline(cin, message);

        // Print the agent's coordinates
        cout << "Agent #" << i << " is at (" << x << ", " << y << ")" << endl;
    }
    return 0;
}
