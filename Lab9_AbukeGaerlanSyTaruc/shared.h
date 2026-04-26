#ifndef SHARED_H
#define SHARED_H

#include <sys/types.h>

// 1mb per frame
const int MAX_FRAME_SIZE = 1 << 20; // 1mb is probs enough per frame, but i'll see w bad apple

// IPC keys - producer and consumer need to agree on these
const key_t SEM_KEY = 1234;
const key_t SHM_KEY = 5678;

// one frame at a time in shared memory
struct SharedMemory {
    char frameData[MAX_FRAME_SIZE]; // string doesnt work apparently
    int frameSize;
    long currentFrame;
    long frameCount;
};

#endif