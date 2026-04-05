#include <iostream>
#include <cstdlib>
#include <ctime>
#include <pthread.h>
#include <vector>
#include <random>
#include <syncstream>

using namespace std;

// Each thread writes its hit count here after it finishes.
// long double globalHits[100];
// changed to vector because I don't want to handle memory leaks -Linus
vector<double long> globalHits;

// Mutex to prevent threads from printing at the same time
pthread_mutex_t printLock;

// I'm gonna be honest, the only data we need to pass is threadId -Linus
struct ThreadData {
    long threadId;
    long numTrials;
    long reportInterval;  // p
};

// Source - https://stackoverflow.com/a/21238187
// Posted by Konrad Rudolph, modified by community. See post 'Timeline' for change history
// Retrieved 2026-04-05, License - CC BY-SA 3.0
// the numbers Mason, what do they mean?
// modified to use double
double doubleRand() {
    static thread_local mt19937 generator(random_device{}());
    uniform_real_distribution<double> distribution(-1.0, 1.0);
    return distribution(generator);
}


bool test()
{
    double x = doubleRand();
    double y = doubleRand();
    return (x * x) + (y * y) <= 1;
}

void *monteCarloFunc(void *ptr) {
    ThreadData *data = (ThreadData *) ptr;

    long id = data->threadId;
    long n  = data->numTrials;
    long p  = data->reportInterval;

    // memory cleanup, we don't need the struct at this point
    delete data;

    std::osyncstream synced_out(cout); // synchronized wrapper for std::cout

    double long hits = 0;

    // trial numbers start at 1
    for (long i = 1; i <= n; i++)
    {
        hits += test();
        long remainder = i % p;
        if (remainder == 0)
        {
            double long estimate = hits / i * 4;
            // a/(rr) = pi
            synced_out << "Thread " << id << ": " << hits << " / " << i << "(estimate: " << estimate << ")" << endl;
        }
    }
    globalHits[id] = hits / n * 4;

    return NULL;
}

int main(int argc, char *argv[]) {
    if (argc != 4)
    {
        std::fprintf(stderr, "Usage: %s <threads> <trials> <reportInterval>\n", argv[0]);
        return EXIT_FAILURE;
    }

    long threads = atol(argv[1]);
    long trials = atol(argv[2]);
    long p = atol(argv[3]);

    vector<pthread_t> thread_list(threads);
    // fill with zeroes
    globalHits.resize(threads, 0);

    for (int t = 0; t < threads; t++)
    {
        pthread_t thread;

        ThreadData *data = new ThreadData();
        data->threadId = t;
        data->numTrials = trials;
        data->reportInterval = p;
        
        long retVal = pthread_create(&thread, NULL, monteCarloFunc, (void*) data);

        if (retVal)
        {
            fprintf(stderr, "Error - pthread_create() return code: %ld\n", retVal);
            exit(EXIT_FAILURE);
        }

        thread_list[t] = thread;
    }

    // wait for all threads
    for (long t = 0; t < threads; t++)
    {
        pthread_join(thread_list[t], NULL);
    }

    // Do we average the pis?
    for (long t = 0; t < threads; t++)
    {
        cout << "(FINAL) Thread " << t << ": " << globalHits[t] << endl;
    }
    

    return 0;
}
