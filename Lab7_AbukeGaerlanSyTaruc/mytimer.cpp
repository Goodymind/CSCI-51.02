//std
#include <iostream>

// fork(), and sleep()
#include <unistd.h>

// date time
#include <ctime>

// zero padding
#include <iomanip>

// signals
#include <csignal>

using namespace std;



void printTime()
{
    time_t t = time(0);

    tm *now = localtime(&t);

    cout << "[" << now->tm_year + 1900;
    cout << "-" << setfill('0') << setw(2) << now->tm_mon + 1;
    cout << "-" << setfill('0') << setw(2) << now->tm_mday << "] ";
    cout << setfill('0') << setw(2) << now->tm_hour;
    cout << ":" << setfill('0') << setw(2) << now->tm_min;
    cout << ":" << setfill('0') << setw(2) << now->tm_sec;
    cout << endl;
}

int main(int argc, char const *argv[])
{
    pid_t parent = getpid();
    
    pid_t xclockpid = fork();

    if (xclockpid == 0)
    {
        // alinus     22464  0.0  0.0  11300  7680 pts/5    S+   20:54   0:00 myXclock
        execl("/usr/bin/xclock", "myXclock", NULL);
        // no need to return cuz the child process image was replaced
    }

    pid_t listener = fork();

    if (listener == 0)
    {
        string res;
        getline(cin, res);
        // Terminal ENTER
        cout << "\"Terminated\"\n";
        kill(parent, SIGINT);
        kill(xclockpid, SIGINT);
        exit(1);
    }

    // keeps track of how many print outs up to 3;
    int counter = 0;
    while(true)
    {
        printTime();
        counter++;
        if (counter == 3)
        {
            counter = 0;
            cout << "\"This program has gone on for far too long. Close the myXclock window or press Enter on this window to exit.\"\n";
        }
        sleep(1);
    }
}

// sources
/*
    date-time

    https://www.w3schools.com/cpp/cpp_date.asp
    https://stackoverflow.com/questions/997946/how-can-i-get-current-time-and-date-in-c
    

    zero padding
    
    https://stackoverflow.com/questions/1714515/how-can-i-pad-an-int-with-leading-zeros-when-using-cout-operator

    
    sleep

    https://www.geeksforgeeks.org/cpp/sleep-function-in-cpp/
*/
