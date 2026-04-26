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

// might include this to shared.h later
struct frame
{
    int height;
    std::string data;
};

void writeSharedMemory(frame *frame, int m, int n, int fps)
{
    int shmId;
    int shmFlags = IPC_CREAT | 0666;
    char *sharedMem;
    shmId = shmget(SHM_KEY, MAX_FRAME_SIZE, shmFlags);
    sharedMem = (char *)shmat(shmId, NULL, 0);

    if (((int *)sharedMem) == (int *)-1)
    {
        perror("shmop: shmat failed");
        return;
    }

    else
    {
        memcpy(sharedMem, &m, sizeof(int));
        memcpy(sharedMem + sizeof(int), &n, sizeof(int));
        memcpy(sharedMem + sizeof(int) * 2, &fps, sizeof(int));
        const char *buffer = frame->data.c_str();
        strcpy(sharedMem + sizeof(int) * 3, buffer);
    }
}

void trySharedMemory(frame *frame, int m, int n, int fps)
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
    sema[1].sem_op = 1; // increment semaphore by 1
    sema[1].sem_flg = SEM_UNDO | IPC_NOWAIT;
    int opResult = semop(semId, sema, nOperations);
    if (opResult != -1)
    {
        writeSharedMemory(frame, m, n, fps);

        // -- Semaphore release
        nOperations = 1;
        sema[0].sem_num = 0;
        sema[0].sem_op = -1; // decrement, return semaphore to 0
        sema[0].sem_flg = SEM_UNDO | IPC_NOWAIT;

        opResult = semop(semId, sema, nOperations);
        if (opResult == -1)
        {
            perror("semop (decrement)");
        }
    }
}

int getFrameHeight(std::ifstream &file)
{
    auto linePosition = file.tellg();
    int start = -1;
    int end = -1;
    int height = 1;
    std::string line;
    while (std::getline(file, line))
    {
        if (start == -1)
        {
            if (line[0] == 27 && line[1] == 'c')
            {
                start = height;
            }
        }
        else if (start != -1)
        {
            if (line[0] == 27 && line[1] == 'c')
            {
                break;
            }
        }
        height++;
    }
    end = height;
    file.clear();
    file.seekg(linePosition);
    return end - start;
}

frame *getFrame(std::ifstream &file)
{
    // I'm reading twice;
    int height = getFrameHeight(file);
    std::string data;
    std::string line;
    std::string start;
    std::getline(file, start);
    data = start.substr(2, start.length() - 2);
    for (int i = 1; i < height; i++)
    {
        std::getline(file, line);
        data = data + "\n" + line;
    }

    frame *output = new frame;
    output->data = data;
    output->height = height;
    return output;
}

int countVideoFrames(std::ifstream &file, int length)
{
    std::cout << "counting frames... ";
    int frames = 0;
    while (true)
    {
        getFrame(file);
        frames++;
        if (file.tellg() == length)
        {
            file.seekg(0, file.beg);
            std::cout << frames << std::endl;
            return frames;
        }
    }
}

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

int main(int argc, char const *argv[])
{
    // args
    std::string fileName = argv[1];
    int fps = atoi(argv[2]);

    // #5, listener process
    signal(SIGFPE, signalHandler);
    signal(SIGILL, signalHandler);
    signal(SIGINT, signalHandler);
    signal(SIGSEGV, signalHandler);
    signal(SIGTERM, signalHandler);
    signal(SIGQUIT, signalHandler);
    signal(SIGCHLD, signalHandler);
    signal(SIGSYS, signalHandler);
    signal(SIGUSR1, signalHandler);

    setpgid(0, 0);
    grp = getpgid(getpid());
    pid_t producerPid = getpid();
    pid_t listener = fork();
    if (listener == 0)
    {
        setpgid(getpid(), grp);
        std::string res;
        std::getline(std::cin, res);
        std::cout << "Listener received input: " << res << std::endl;
        kill(-grp, SIGTERM);
        exit(0);
    }

    // main loop
    std::string line;
    std::ifstream file(fileName);
    int interval = 1000 / fps;
    int m = -1;
    int n = 0;
    if (file.is_open())
    {
        // For videos that may have same frames throughout
        // specs are so confusing
        // int height = getFrameHeight(file);
        file.seekg(0, file.end);
        int length = file.tellg();
        file.seekg(0, file.beg);
        m = countVideoFrames(file, length);
        while (true)
        {
            n++;
            auto nextFrame = getFrame(file);
            trySharedMemory(nextFrame, m, n, fps);
            delete nextFrame;

            if (file.tellg() == length)
            {
                file.seekg(0, file.beg);
                n = 0;
            }

            // https://www.geeksforgeeks.org/cpp/how-to-sleep-for-milliseconds-in-cpp/
            std::this_thread::sleep_for(std::chrono::milliseconds(interval));
        }
        file.close();
    }

    return 0;
}
