#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>

#include <stdio.h>
#include <string.h>
#include <cstdlib> // for exit()
#include <algorithm> // for min()
#include <atomic>
#include <thread>
#include "shared.h"

using namespace std;

// based off of shm_example.cpp

atomic<bool> running(true);

// waits for enter then kills the loop
void enterListener(){
    getchar();
    running = false;
}

int main( int argc, char* argv[] )
{
    // Id for the shared memory
    int shmId;
    int semId;
    SharedMemory* sharedMem;

    // attach to existing shared mem + semaphores (producer should already be running)
    shmId = shmget(SHM_KEY, 0, 0666);
    semId = semget(SEM_KEY, 0, 0666);
    if(shmId == -1 || semId == -1) {
        perror ("shmget failed - producer probs dead");
        exit(1);
    }

    // shmat() returns the starting address of the shared memory
    // segment, so we assign it to sharedMem.
    sharedMem = (SharedMemory*)shmat( shmId, NULL, 0 );
    if( ((int*)sharedMem) == (int*)-1 )
    {
        perror( "shmop: shmat failed" );
        exit(1);
    }

    long framesSkipped = 0;
    long lastFrame = 0;

    thread listener(enterListener);

    while(running) {
         // acquire mutex
        // wait-for-zero then increment: standard System V mutex pattern
        // both ops are atomic together via semop, so no race between them
        // source: https://man7.org/linux/man-pages/man2/semop.2.html
        struct sembuf sema[2];
        sema[0].sem_num = MUTEX;
        sema[0].sem_op = 0; // wait until semaphore == 0 (free)
        sema[0].sem_flg = SEM_UNDO;

        sema[1].sem_num = MUTEX;
        sema[1].sem_op = 1; // lock: increment to 1
        sema[1].sem_flg = SEM_UNDO | IPC_NOWAIT;

        semop(semId, sema, 2);

        // critical section: read shared mem
        char localBuffer[MAX_FRAME_SIZE];
        memcpy(localBuffer, sharedMem->frameData, sharedMem->frameSize);
        localBuffer[sharedMem->frameSize] = '\0';
        long currentFrame = sharedMem->currentFrame;
        long frameCount = sharedMem->frameCount;

        struct sembuf release[1]; // release mutex
        release[0].sem_num = MUTEX;
        release[0].sem_op = -1; //unlock: decrement back to 0
        release[0].sem_flg = SEM_UNDO | IPC_NOWAIT;

        semop(semId, release, 1);

        // TODO: looping (including fixing frameskipped)
        // TODO: fix filtering out c in video.txt
        printf("%s\n", localBuffer);
        printf("Current frame: bla");

        lastFrame = currentFrame;
    }

    listener.join();  // waits for the listener thread to finish cleanly before the program exits.

    // TODO: cleanup
}

// sources
/*

    semaphores

    https://docs.oracle.com/cd/E19683-01/816-5042/auto32/index.html
    https://docs.oracle.com/cd/E19683-01/816-5042/svipc-41256/index.html


*/
