#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>

#include <stdio.h>
#include <string.h>
#include <cstdlib>
#include <algorithm>
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
    int shmId;
    int semId;
    SharedMemory* sharedMem;

    // attach to existing shared mem + semaphores (producer should already be running)
    shmId = shmget(SHM_KEY, 0, 0666);
    semId = semget(SEM_KEY, 0, 0666); // 0 semaphores = just get, don't create
    if(shmId == -1 || semId == -1) {
        perror("shmget/semget failed - is the producer running?");
        exit(1);
    }

    sharedMem = (SharedMemory*)shmat(shmId, NULL, 0);
    if((int*)sharedMem == (int*)-1)
    {
        perror("shmat failed");
        exit(1);
    }

    long framesSkipped = 0;
    long lastFrame = 0;

    thread listener(enterListener);

    while(running) {
        // --- acquire mutex ---
        // wait-for-zero then increment: standard System V mutex pattern
        // both ops are atomic together via semop, so no race between them
        // source: https://man7.org/linux/man-pages/man2/semop.2.html
        struct sembuf sema[2];
        sema[0].sem_num = MUTEX;
        sema[0].sem_op  = 0; // wait until semaphore == 0 (free)
        sema[0].sem_flg = SEM_UNDO;

        sema[1].sem_num = MUTEX;
        sema[1].sem_op  = 1; // lock: increment to 1
        sema[1].sem_flg = SEM_UNDO | IPC_NOWAIT;

        semop(semId, sema, 2);

        // --- critical section: read shared mem ---
        char localBuffer[MAX_FRAME_SIZE];

        // clamp frameSize before copy to avoid overflowing localBuffer
        // if frameSize is somehow bigger than our buffer, we'd stomp memory
        // source: https://www.fromdev.com/2025/11/memcpy-buffer-overflow-prevention-best-practices.html
        int safeSize = min(sharedMem->frameSize, MAX_FRAME_SIZE - 1);
        memcpy(localBuffer, sharedMem->frameData, safeSize);
        localBuffer[safeSize] = '\0';

        long currentFrame = sharedMem->currentFrame;
        long frameCount   = sharedMem->frameCount;

        // --- release mutex ---
        struct sembuf release[1];
        release[0].sem_num = MUTEX;
        release[0].sem_op  = -1; // unlock: decrement back to 0
        release[0].sem_flg = SEM_UNDO | IPC_NOWAIT;

        semop(semId, release, 1);

        // TODO: looping (including fixing framesSkipped)
        // TODO: fix filtering out ESC+c in video.txt before printing
        printf("%s\n", localBuffer);
        printf("Current frame: %ld / %ld (%ld frames skipped)\n",
               currentFrame, frameCount, framesSkipped);

        lastFrame = currentFrame;
    }

    listener.join();

    // TODO: cleanup (shmdt, semctl IPC_RMID if we own it)
}

// sources
/*
    semaphores
    https://man7.org/linux/man-pages/man2/semop.2.html
    https://man7.org/linux/man-pages/man2/semget.2.html
    https://www.softprayog.in/programming/system-v-semaphores

    memcpy overflow prevention
    https://www.fromdev.com/2025/11/memcpy-buffer-overflow-prevention-best-practices.html
    https://en.cppreference.com/c/string/byte/memcpy
*/