#include <iostream>
#include <vector>
#include <queue>
#include <cmath>
#include "IntArray.h"

using namespace std;

vector<int> optimize(int n)
{
    if (n == 0 || n == 1)
        return {n};

    queue<vector<int>> q;
    q.push({n});

    while (!q.empty())
    {
        vector<int> current = q.front();
        q.pop();

        int latest = current.back();

        int p = floor(log2(latest));
        int b = 1 << p;   

        if (b == latest)
            return current;

        int a = b - 1;
        int c = b + 1;

        vector<int> alist = current;
        vector<int> blist = current;
        vector<int> clist = current;

        alist.back() = a;
        alist.push_back(latest - a);

        blist.back() = b;
        blist.push_back(latest - b);

        clist.back() = c;
        clist.push_back(latest - c);

        q.push(blist);
        q.push(alist);
        q.push(clist);
    }

    return {};
}



int main(int argc, char* argv[]) 
{
    int x = stoi(argv[1]);

    vector<int> binary = optimize(x);
    // headers
    cout << " .file" << " \"multiplyB" << x << ".cpp\"" << endl;
    cout << " .text" << endl;
    cout << " .globl" << " _Z12multiplyBy" << x << "P8IntArray" << endl;
    cout << " .type" << " _Z12multiplyBy"<< x << "P8IntArray, @function" << endl;
    cout << "_Z12multiplyBy" << x << "P8IntArray:" << endl;
    
    // LFB0
    cout << ".LFB0:" << endl;
    cout << " .cfi_startproc" << endl;
    cout << " endbr64" << endl;
    cout << " cmpl $0, (%rdi)" << endl;
    cout << " jle .L1" << endl;
    // vscode highlights %e, google search does not show anything
    // doesn't seem to be an issue as its outputting correctly
    cout << " movl $0, %eax" << endl;

    //L3
    cout << ".L3:" << endl;
    cout << " movq 8(%rdi), %rdx" <<  endl;
    cout << " leaq (%rdx,%rax,4), %rdx" <<  endl;
    // TODO
    // UPDATE THIS TO NOT USE IMULL
    cout << " movl (%rdx), %ecx" << endl;
    cout << " movl $0, %r8d" << endl;

    for (int value : binary)
    {
        int shift = log2(value);

        cout << " movl %ecx, %r9d" << endl;
        cout << " shll $" << shift << ", %r9d" << endl;
        cout << " addl %r9d, %r8d" << endl;
    }

    cout << " movl %r8d, (%rdx)" << endl;

    // INCREMENT THROUGH VECTOR
    cout << " addl $1, %eax" << endl;
    cout << " cmpl (%rdi), %eax" << endl;
    cout << " jl .L3" << endl;

    //L1
    cout << ".L1:" << endl;
    cout << " ret" << endl;;
    cout << " .cfi_endproc" << endl;


    // Stuff below is unrelated to main program but stuff the pc needs
    //LFEO
    cout << "LFE0:" << endl;
    cout << " .size _Z12multiplyBy"<< x <<"P8IntArray, .-_Z12multiplyBy" << x <<"P8IntArray" << endl;
    cout << " .ident \"GCC: (Ubuntu 13.3.0-6ubuntu2~24.04) 13.3.0\"" << endl;
    cout << " .section .note.GNU-stack,\"\",@progbits" << endl;
    cout << " .section .note.gnu.property,\"a\"" << endl;
    cout << " .align 8" << endl;
    cout << " .long 1f - 0f" << endl;
    cout << " .long	4f - 1f" << endl;
    cout << " .long 5" << endl;

    //0
    cout << "0:" << endl;
    cout << " .string \"GNU\"" << endl;
    
    //1
    cout << "1:" << endl;
    cout << " .align 8" << endl;
    cout << " .long 0xc0000002" << endl;
    cout << " .long 3f - 2f" << endl;
    
    //2
    cout << "2:" << endl;
    cout << " .long 0x3" << endl;
    
    //3
    cout << "3:" << endl;
    cout << " .align 8" << endl;
    
    //4
    cout << "4:" << endl;
    
    return 0;
}