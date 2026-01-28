#include <iostream>
#include <stdio.h>
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

bool parseMessage(string* msgp, int* output)        // function that returns true if the string is an integer, stores the result in output
{
    bool negative = false;                          // true when we detect a -
    bool digits_in = false;                         // true when we detect numbers
    for (int i = 0; i < msgp->length(); i++)        // iterate through each character
    {
        char c = (*msgp)[i];                        // get ith character
        if (isdigit(c)) {                           // if character is a digit:
            digits_in = true;                       //      We are now going through the digits
        }
        else if (c == '-' && !digits_in && !negative)// if character is a bar and we haven't detected any numbers or bars before         
        {   
            negative = true;                        //      set negative flag 
                                                    //      next characters should be digits
        }
        else                                        // if not a bar or a digit
        {
            output = nullptr;                       // return false and null output.
            return false;
        }
    }
    
    if (!digits_in)                                 // if we never found any digits
    {
        output = nullptr;                           // return false
        return false;
    }

    (*output) = stoi(*msgp);                        // after validating, return the integer
    return true;                                    // parseMessage = true;
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
        
        int m = 0;                                    
        rstrip(&message);                                                   // custom function that removes trailing whitespaces 
        
        if (!parseMessage(&message, &m))                 // if nondigit message
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