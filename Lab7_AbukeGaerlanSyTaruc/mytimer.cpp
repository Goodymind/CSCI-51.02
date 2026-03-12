//std
#include <iostream>

// fork(), and sleep()
#include <unistd.h>

// date time
#include <ctime>

// zero padding
#include <iomanip>

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
        sleep(3);
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
