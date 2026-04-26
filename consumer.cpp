#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

using namespace std;

 #define SEM_MUTEX 0
// #include "shared.h"

// global for cleanup
int semId = -1;
char* sharedMem = NULL;


// wait for semaphore == 0, then increment to 1 (lock)
void acquireMutex( int semId )
{
    struct sembuf ops[2];

    ops[0].sem_num = SEM_MUTEX;
    ops[0].sem_op  = 0;
    ops[0].sem_flg = SEM_UNDO;

    ops[1].sem_num = SEM_MUTEX;
    ops[1].sem_op  = 1;
    ops[1].sem_flg = SEM_UNDO;

    if( semop( semId, ops, 2 ) == -1 )
    {
        perror( "semop (acquire)" );
        exit( 1 );
    }
}

void releaseMutex( int semId )
{
    struct sembuf op;

    op.sem_num = SEM_MUTEX;
    op.sem_op  = -1;
    op.sem_flg = SEM_UNDO;

    if( semop( semId, &op, 1 ) == -1 )
    {
        perror( "semop (release)" );
        exit( 1 );
    }
}


int main( int argc, char* argv[] )
{
    return 0;
}