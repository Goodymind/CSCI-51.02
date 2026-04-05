#include <iostream>
#include <cstdlib>
#include <ctime>
#include <pthread.h>
#include <iomanip>
#include <random>

using namespace std;

long double globalHits[100];
pthread_mutex_t printLock = PTHREAD_MUTEX_INITIALIZER;

struct ThreadData {
    int threadId;
    int numTrials;
    int reportInterval;
    unsigned int seed;
};

long double calculatePi(long double hits, long double total) {
    if (total == 0) return 0;
    return 4.0L * hits / total;
}

void *monteCarloFunc(void *ptr) {
    ThreadData *data = (ThreadData *) ptr;
    
    mt19937_64 rng(data->seed);
    uniform_real_distribution<long double> dist(-1.0L, 1.0L);
    
    long double hits = 0;
    
    for (int trial = 1; trial <= data->numTrials; trial++) {
        long double x = dist(rng);
        long double y = dist(rng);
        
        if (x * x + y * y <= 1.0L) {
            hits++;
        }
        
        if (trial % data->reportInterval == 0) {
            long double estimate = calculatePi(hits, trial);
            
            pthread_mutex_lock(&printLock);
            printf("Thread %d: %lld / %d (estimate: %.18Lg)\n", 
                data->threadId, (long long)hits, trial, estimate);
            pthread_mutex_unlock(&printLock);
        }
    }
    
    globalHits[data->threadId] = hits;
    return NULL;
}

int main(int argc, char *argv[]) {
    if (argc != 4) {
        cerr << "Usage: " << argv[0] << " <numThreads> <numTrials> <reportInterval>" << endl;
        return 1;
    }
    
    int numThreads = atoi(argv[1]);
    int numTrials = atoi(argv[2]);
    int reportInterval = atoi(argv[3]);
    
    if (numThreads <= 0 || numTrials <= 0 || reportInterval <= 0) {
        cerr << "Error: All parameters must be positive integers." << endl;
        return 1;
    }
    
    if (numThreads > 100) {
        cerr << "Error: numThreads cannot exceed 100." << endl;
        return 1;
    }    

    pthread_t threads[100];
    ThreadData threadData[100];
    unsigned int baseSeed = time(NULL);
    
    for (int i = 0; i < numThreads; i++) {
        threadData[i].threadId = i;
        threadData[i].numTrials = numTrials;
        threadData[i].reportInterval = reportInterval;
        threadData[i].seed = baseSeed + i * 1000000;
        
        pthread_create(&threads[i], NULL, monteCarloFunc, &threadData[i]);
    }
    
    long double totalHits = 0;
    for (int i = 0; i < numThreads; i++) {
        pthread_join(threads[i], NULL);
        totalHits += globalHits[i];
    }
    
    cout << "Final result: " << fixed << setprecision(5) 
         << calculatePi(totalHits, (long double)numTrials * numThreads) << endl;
    
    pthread_mutex_destroy(&printLock);
    return 0;
}
