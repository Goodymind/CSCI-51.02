#ifndef SHARED_H
#define SHARED_H

#include <sys/types.h>

// 1mb per frame, probably enough for bad apple lol
const int MAX_FRAME_SIZE = 1 << 20;

// IPC keys - producer and consumer need to agree on these
const key_t SEM_KEY = 1234;
const key_t SHM_KEY = 5678;

// one frame at a time in shared mem
struct SharedMemory {
    char frameData[MAX_FRAME_SIZE]; // string doesnt work in shared mem apparently
    int frameSize;
    long currentFrame;
    long frameCount;
};

// semaphore indices (we use 2 semaphores in the set)
#define MUTEX 0  // controls exclusive access to shared mem

#endif