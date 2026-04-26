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

// write a frame's string data into shared memory
void writeSharedMemory(frame *frame)
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
        const char* buffer = frame->data.c_str();
        strcpy(sharedMem, buffer);
    }
}

// acquire semaphore, write frame, release - skips write if can't get lock
void trySharedMemory(frame *frame)
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
        writeSharedMemory(frame);

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

// scan ahead to figure out how many lines are in the next frame
// (reads until the next ESC+c delimiter)
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

// read one full frame from the file (calls getFrameHeight internally so it reads twice)
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

int main(int argc, char const *argv[])
{
    // args
    std::string fileName = argv[1];
    int fps = atoi(argv[2]);

    // #5, listener process
    pid_t producerPid = getpid();
    pid_t listener = fork();
    if (listener == 0)
    {
        std::string res;
        std::getline(std::cin, res);
        std::cout << "Listener received input: " << res << std::endl;
        kill(producerPid, SIGINT);
        exit(0);
    }

    // main loop
    std::string line;
    std::ifstream file(fileName);
    int interval = 1000 / fps;
    if (file.is_open())
    {
        // For videos that may have same frames throughout
        // specs are so confusing
        // int height = getFrameHeight(file);
        while (true)
        {
            auto nextFrame = getFrame(file);
            trySharedMemory(nextFrame);
            delete nextFrame;

            if (file.tellg() == file.end)
            {
                file.seekg(0, file.beg);
            }

            // https://www.geeksforgeeks.org/cpp/how-to-sleep-for-milliseconds-in-cpp/
            std::this_thread::sleep_for(std::chrono::milliseconds(interval));
        }
    }

    return 0;
}
