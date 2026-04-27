#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>


#include <stdio.h>
#include <stdlib.h>
#include <atomic>
#include <pthread.h>
#include "shared.h"




// based off sem_sample.cpp


atomic<bool> running(true); // for enter listener

// waits for enter then sets running to false        
void enterListener(){
    getchar() // TODO: make it tomorrow
    running = false;
}


int main( int argc, char* argv[] )
{
    // TODO: validate args


    // TODO: cleanup old memory (?) see: IPC_RMID


    // SHARED MEMORY setup
    int shmId;
    int shmSize = sizeof(SharedMemory);
    int shmFlag = IPC_CREAT | 0666;
    SharedMemory* sharedMem;


    shmId = shmget(SHM_KEY, shmSize, shmFlags);
    sharedMem = (SharedMemory*)shmat(shmId, NULL, 0); // similar to shm_example


    if((int*)sharedMem == (int*)-1){
        perror( "shmat" );
        exit( 1 );
    }


    // SEMAPHORE setup


    int semId;
    int nSems = 2; // set 2 flags as seen in shared.h
    int semFlags = IPC_CREAT | 0666;


    // Attempt to get a semaphore set
    // Returns -1 if there was an error.
    semId = semget( SEM_KEY, nSems, semFlag );
    if( semId == -1 )
    {
        perror( "semget" );
        exit( 1 );
    }


    // TODO: semctl initialize mutex and isEmpty flags for semaphores
    // TODO: open video.txt store total frames in sharedMem->frameCount


    while(running) {
        // TODO: buffer + loop video


        struct sembuffer sema[2];
        sema[0].sem_num = MUTEX;
        sema[0].sem_op = 0;
        sema[0].sem_flag = SEM_UNDO;


        sema[1].sem_num = MUTEX;
        sema[1].sem_op = 1;
        sema[1].sem_flag = SEM_UNDO | IPC_NOWAIT;


        semop(semId, sema, 2)


        // TODO: write frame to shmem + increments frame idk


        struct sembuffer release[1];
        release[0].sem_num = MUTEX;
        release[0].sem_op = -1;
        release[0].sem_flag = SEM_UNDO | IPC_NOWAIT;


        semop(semId, release, 1);
    }


    // TODO: cleanup
}


// sources
/*


    semaphores


    https://docs.oracle.com/cd/E19683-01/816-5042/auto32/index.html
    https://docs.oracle.com/cd/E19683-01/816-5042/svipc-41256/index.html


    bools not being thread safe


    https://stackoverflow.com/questions/9200951/is-it-ok-to-read-a-shared-boolean-flag-without-locking-it-when-another-thread-ma


*/
