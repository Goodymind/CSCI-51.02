#ifndef SHARED_H
#define SHARED_H

#include <sys/types.h>

const int MAX_FRAME_SIZE = 1 << 20; // large upper bound for simplicity

const key_t SEM_KEY = 1234;
const key_t SHM_KEY = 5678;

#endif