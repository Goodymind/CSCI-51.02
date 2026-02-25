#include <iostream>
#include "IntArray.h"
#include <array>
#include <vector>
#include <bitset>

using namespace std;

// output is in reverse order
vector<int> decToBin(int x)
{
    int i = 0;
    vector<int> result;
    while (x > 0)
    {
        result.push_back(x % 2);
        x = x / 2;
        i++;
    }
    // // cout << "binary (reverse): ";
    // for (int i = 0; i < result.size(); i++)
    // {
    //     cout << result[i];
    // }
    // cout << endl;
    
    return result;
}

// returns a vector of non adjacent form of an integer
// Introduced by G. W. Reit
// faster way by Prodinger
// used here, https://oeis.org/A184616
array<vector<int>, 2> naf(int x)
{
    int xh = x >> 1;
    int x3 = x + xh;
    int c = xh ^ x3;
    int np = x3 & c;
    int nm = xh & c;
    
    array<vector<int>, 2> result;
    result[0] = decToBin(np);
    result[1] = decToBin(nm);

    return result;
}

void multiply(int x)
{
    auto result = naf(x);

    // since binary is stored in reverse order
    // value = np[i] * (2 ** i)
    // value = 1 * (2 ** 5) = np[i] * 32
    // np contains the positive bits of naf
    auto np = result[0];
    // nm contains the negative bits of naf
    auto nm = result[1];

    // move the original value of x to ebx;
    cout << " movl (%rdx), %ebx" << endl;
    // ecx = 0; ecx is the accumulator
    cout << " movl $0, %ecx" << endl;
    // we save how many times eb has been shifted so afr
    int current_ebx_shift = 0;

    for (int i = 0; i < np.size(); i++)
    {
        if (np[i] == 1)
        {
            // shift original value by i, unless i = 0, then 
            // we don't shift and we just add
            if (i != 0)
            {
                cout << " shl $" << i - current_ebx_shift << ", %ebx" << endl;
            }

            // add x * (2 ** i) to the accumulator
            cout << " addl %ebx, %ecx" << endl;

            // update the current ebx shift
            current_ebx_shift = i;
        }
    }
    // reset eax back to original unshifted value
    cout << " movl (%rdx), %ebx" << endl;
    current_ebx_shift = 0;

    for (int i = 0; i < nm.size(); i++)
    {
        if (nm[i] == 1)
        {
            if (i != 0)
            {
                cout << " shl $" << i - current_ebx_shift << ", %ebx" << endl;
            }

            cout << " subl %ebx, %ecx" << endl;
            current_ebx_shift = i;
        }
    }
}

// remove temp_ to enable tester
int main(int argc, char* argv[]) 
{
    int x = stoi(argv[1]);
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
    // cout << " imull $" << x << ", (%rdx), %ecx" <<  endl;
    multiply(x);
    cout << " movl %ecx, (%rdx)" <<  endl;
    cout << " addq $1, %rax" <<  endl;
    cout << " cmpl %eax, (%rdi)" <<  endl;
    cout << " jg .L3" <<  endl;

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