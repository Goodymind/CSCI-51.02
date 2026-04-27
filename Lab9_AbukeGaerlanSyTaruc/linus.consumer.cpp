#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>

// #include <stdio.h>
// #include <stdlib.h>

// strings and reading from terminal
#include <string>
#include <iostream>

// shared variables
#include "shared.h"

// listener process
#include <unistd.h>
#include <csignal>

// filereading
// https://cplusplus.com/doc/tutorial/files/
#include <fstream>

// sleep for ms
#include <chrono>
#include <thread>

// strcpy
#include <cstring>

int last_frame = -1;
int skipped_frames = 0;
int fps = 0;

// reads frame + metadata from shared memory and prints it
// also tracks skipped frames by comparing current frame number to last seen
void readSharedMemory()
{
    int shmId;
    int shmFlags = IPC_CREAT | 0666;
    char *sharedMem;
    shmId = shmget(SHM_KEY, MAX_FRAME_SIZE, shmFlags);
    sharedMem = (char *)shmat(shmId, NULL, 0);

    if (((int *)sharedMem) == (int *)-1)
    {
        perror("shmop: shmat failed");
    }

    else
    {
        // read metadata from the front of shared memory (matches producer's write layout)
        int m, n, pfps;
        memcpy(&m, sharedMem, sizeof(int));
        memcpy(&n, sharedMem + sizeof(int), sizeof(int));
        memcpy(&pfps, sharedMem + sizeof(int) * 2, sizeof(int));
        char buffer[MAX_FRAME_SIZE];
        strcpy(buffer, sharedMem + sizeof(int) * 3);
        std::cout << buffer << std::endl;
        std::cout << "Current Frame: " << n << " / " << m;
       
        // if fps=0 was passed, sync to producer's fps
        if (fps == 0)
        {
            fps = pfps;
        }
        
        // count how many frames we missed since last read
        if (last_frame != -1)
        {
            if (n > last_frame + 1)
                skipped_frames += n - last_frame - 1;
        }
        last_frame = n;

        // reset counters when video loops
        if (n == 0)
        {
            last_frame = 0;
            skipped_frames = 0;
        }
        std::cout << " (" << skipped_frames << " frames skipped) " << std::endl;
    }
}

// same semaphore pattern as producer — wait-for-zero then increment to lock, decrement to unlock
void trySharedMemory()
{
    // -- Sempahore get
    int nSems = 1; // number of processes that can access shared memory at the same time (right?)
    int semFlag = IPC_CREAT | 0666;
    int semId = semget(SEM_KEY, nSems, semFlag);

    if (semId == -1)
    {
        perror("semget");
        std::exit(1);
    }
    // -- Semaphore access
    int nOperations = 2;
    struct sembuf sema[nOperations];
    sema[0].sem_num = 0;
    sema[0].sem_op = 0; // wait if semaphore != 0
    sema[0].sem_flg = SEM_UNDO;

    sema[1].sem_num = 0;
    sema[1].sem_op = 1; // increment semaphore by 1; lock
    sema[1].sem_flg = SEM_UNDO | IPC_NOWAIT;
    int opResult = semop(semId, sema, nOperations);
    if (opResult != -1)
    {
        readSharedMemory();

        // -- Semaphore release
        nOperations = 1;
        sema[0].sem_num = 0;
        sema[0].sem_op = -1; // decrement, return semaphore to 0; unlock
        sema[0].sem_flg = SEM_UNDO | IPC_NOWAIT;

        opResult = semop(semId, sema, nOperations);
        if (opResult == -1)
        {
            perror("semop (decrement)");
        }
    }
}

// same as producer — kill the whole group on exit
pid_t grp;
bool dying = false;
void signalHandler(int signal)
{
    if (!dying)
    {
        kill(-abs(grp), signal);
        dying = true;
    }
    exit(0);
}

int main(int argc, char *argv[])
{
    // fps=0 means auto-sync to producer's rate (handled in readSharedMemory)
    if (argc > 1)
    {
        fps = atoi(argv[1]);

    }

    signal(SIGFPE, signalHandler);
    signal(SIGILL, signalHandler);
    signal(SIGINT, signalHandler);
    signal(SIGSEGV, signalHandler);
    signal(SIGTERM, signalHandler);
    signal(SIGQUIT, signalHandler);
    signal(SIGCHLD, signalHandler);
    signal(SIGSYS, signalHandler);
    signal(SIGUSR1, signalHandler);

    // #3, listener process
    // fork a child that waits for Enter then kills the group
    pid_t sellerPid = getpid();
    pid_t listener = fork();
    if (listener == 0)
    {
        std::string res;
        std::getline(std::cin, res);
        std::cout << "Listener received input: " << res << std::endl;
        kill(-grp, SIGTERM);
        exit(0);
    }

    // main loop, as usual

    while (true)
    {
        trySharedMemory();
        std::this_thread::sleep_for(std::chrono::milliseconds(1000 / fps));
    }
}