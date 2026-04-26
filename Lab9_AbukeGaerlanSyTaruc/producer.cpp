#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>

#include <stdio.h>
#include <stdlib.h>
#include <atomic>
#include <thread>
#include "shared.h"

// based off sem_sample.cpp

using namespace std;

atomic<bool> running(true); // need atomic so the listener thread can safely flip this
                             // source: https://stackoverflow.com/questions/9200951/

// waits for enter then sets running to false
void enterListener(){
    getchar();
    running = false;
}

int main( int argc, char* argv[] )
{
    // TODO: validate args

    // TODO: cleanup old leftover IPC objects from crashed runs (IPC_RMID)

    // --- SHARED MEMORY setup ---
    int shmId;
    int shmSize  = sizeof(SharedMemory);
    int shmFlag  = IPC_CREAT | 0666;
    SharedMemory* sharedMem;

    shmId     = shmget(SHM_KEY, shmSize, shmFlag);
    sharedMem = (SharedMemory*)shmat(shmId, NULL, 0);

    if((int*)sharedMem == (int*)-1){
        perror("shmat");
        exit(1);
    }

    // --- SEMAPHORE setup ---
    int semId;
    int nSems    = 2; // mutex + (future use)
    int semFlags = IPC_CREAT | 0666;

    semId = semget(SEM_KEY, nSems, semFlags);
    if(semId == -1)
    {
        perror("semget");
        exit(1);
    }

    // TODO: semctl to explicitly initialize MUTEX to 0 (unlocked)
    // TODO: open video.txt, store total frame count in sharedMem->frameCount

    thread listener(enterListener);

    while(running) {
        // TODO: read next frame from video.txt into a local buffer

        // --- acquire mutex ---
        // wait-for-zero then increment atomically = lock
        // source: https://man7.org/linux/man-pages/man2/semop.2.html
        struct sembuf sema[2];
        sema[0].sem_num = MUTEX;
        sema[0].sem_op  = 0; // wait until free
        sema[0].sem_flg = SEM_UNDO;

        sema[1].sem_num = MUTEX;
        sema[1].sem_op  = 1; // lock
        sema[1].sem_flg = SEM_UNDO | IPC_NOWAIT;

        semop(semId, sema, 2);

        // TODO: write frame to sharedMem->frameData, update frameSize + currentFrame

        // --- release mutex ---
        struct sembuf release[1];
        release[0].sem_num = MUTEX;
        release[0].sem_op  = -1; // unlock
        release[0].sem_flg = SEM_UNDO | IPC_NOWAIT;

        semop(semId, release, 1);

        // TODO: sleep for (1000 / fps) ms then grab next frame
    }

    listener.join();

    // TODO: cleanup (shmdt, shmctl IPC_RMID, semctl IPC_RMID)
}

// sources
/*
    semaphores
    https://man7.org/linux/man-pages/man2/semop.2.html
    https://man7.org/linux/man-pages/man2/semget.2.html
    https://www.softprayog.in/programming/system-v-semaphores

    atomic bool for thread safety
    https://stackoverflow.com/questions/9200951/is-it-ok-to-read-a-shared-boolean-flag-without-locking-it-when-another-thread-ma
*/