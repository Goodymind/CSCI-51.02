#include <iostream>
#include <cstdlib>
#include <ctime>
#include <pthread.h>

using namespace std;

// Each thread writes its hit count here after it finishes.
long double globalHits[100];

// Mutex to prevent threads from printing at the same time
pthread_mutex_t printLock;

struct ThreadData {
    int threadId;
    int numTrials;
    int reportInterval;  // p
};

void *monteCarloFunc(void *ptr) {
    ThreadData *data = (ThreadData *) ptr;

    int id = data->threadId;
    int n  = data->numTrials;
    int p  = data->reportInterval;

    long double hits = 0;

    globalHits[id] = hits;

    return NULL;
}

int main(int argc, char *argv[]) {

    return 0;
}
