#ifndef SHARED_H
#define SHARED_H

#include <sys>

const int MAX_FRAME_SIZE = 1 << 20; // 1mb is probs enough per frame, but i'll see w bad apple

const key_t SEM_KEY = 1234
const key_t SHM_KEY = 5678

struct SharedMemory {
    char frameData[MAX_FRAME_SIZE]; // string doesnt work apparently
    int frameSize;
    long currentFrame;
    long frameCount;
};

#endif