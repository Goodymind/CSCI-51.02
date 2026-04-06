//std
#include <iostream>

// fork(), and sleep()
#include <unistd.h>

// date time
#include <ctime>

// zero padding
#include <iomanip>

// signals, atexit
#include <csignal>

// waitpid
#include <sys/wait.h>

using namespace std;

pid_t parent;
pid_t xclockpid;
pid_t listener;
pid_t group_pid;
bool dying = false;

void signalHandler(int sig) 
{
    if (!dying)
    {
        dying = true;
        kill(-abs(group_pid), sig);
    }
    exit(0);
}

void atexit_handler()
{
    if (getpid() == parent)
    {
        cout << "\"Terminated\"\n";
    }
}

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
    // ctrl c command
    signal(SIGINT, signalHandler);

    signal(SIGKILL, signalHandler);
    
    // kill command from console
    signal(SIGTERM, signalHandler);

    signal(SIGABRT, signalHandler);

    parent = getpid();

    group_pid = getpgid(parent);
    
    xclockpid = fork();

    setpgid(xclockpid, group_pid);

    if (xclockpid == 0)
    {
        // alinus     22464  0.0  0.0  11300  7680 pts/5    S+   20:54   0:00 myXclock
        execl("/usr/bin/xclock", "myXclock", NULL);
        // no need to return cuz the child process image was replaced
    }

    listener = fork();

    setpgid(xclockpid, group_pid);

    if (listener == 0)
    {
        string res;
        getline(cin, res);
        // Terminal ENTER
        raise(SIGINT);
        exit(1);
    }


    atexit(atexit_handler);
    signal(SIGCHLD, signalHandler);
    
    // keeps track of how many print outs up to 3;
    int counter = 0;
    while(true)
    {
        sleep(1);
        printTime();
        counter++;
        if (counter == 3)
        {
            counter = 0;
            cout << "\"This program has gone on for far too long. Close the myXclock window or press Enter on this window to exit.\"\n";
        }
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

    
    signal handling
    
    https://web.stanford.edu/class/archive/cs/cs110/cs110.1214/static/lectures/cs110-lecture-07-signals-w21.pdf

*/
