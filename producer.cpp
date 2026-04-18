#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "shared.h"

// global variables for cleanup
int semId = -1;
int shmId = -1;
char* sharedMem = NULL;
FILE* fp = NULL;

void cleanup()
{
    if( sharedMem != NULL )
        shmdt( sharedMem );
    if( fp != NULL )
        fclose( fp );
}

int countFrames( FILE* fp )
{
    int count = 0;
    int c;
    int prevWasEsc = 0;

    rewind( fp );

    while( ( c = fgetc( fp ) ) != EOF )
    {
        if( prevWasEsc && c == 'c' )
            count++;
        prevWasEsc = ( c == ESC_CHAR );
    }

    rewind( fp );
    return count;
}

int main( int argc, char* argv[] )
{

    cleanup();
    return 0;
}