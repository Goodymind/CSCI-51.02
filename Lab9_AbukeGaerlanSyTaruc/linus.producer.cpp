#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>

#include <string>
#include <iostream>

#include "shared.h"

#include <unistd.h>
#include <csignal>

// file reading: https://cplusplus.com/doc/tutorial/files/
#include <fstream>

// sleep for ms: https://en.cppreference.com/w/cpp/thread/sleep_for
#include <chrono>
#include <thread>

#include <cstring>

// might merge this into shared.h later
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
        perror("shmat failed");
    }
    else
    {
        // copy frame string into shared mem
        const char* buffer = frame->data.c_str();
        strcpy(sharedMem, buffer);
    }
}


// acquire semaphore, write frame, release — skips write if can't get lock
void trySharedMemory(frame *frame)
{
    // get (or create) the semaphore set
    // source: https://man7.org/linux/man-pages/man2/semget.2.html
    int nSems   = 1;
    int semFlag = IPC_CREAT | 0666;
    int semId   = semget(SEM_KEY, nSems, semFlag);

    if (semId == -1)
    {
        perror("semget");
        std::exit(1);
    }

    // --- acquire mutex ---
    // wait-for-zero then increment: standard lock pattern
    // both ops execute atomically via semop
    // source: https://man7.org/linux/man-pages/man2/semop.2.html
    int nOperations = 2;
    struct sembuf sema[nOperations];
    sema[0].sem_num = 0;
    sema[0].sem_op  = 0; // wait until semaphore == 0 (unlocked)
    sema[0].sem_flg = SEM_UNDO;

    sema[1].sem_num = 0;
    sema[1].sem_op  = 1; // lock: increment to 1
    sema[1].sem_flg = SEM_UNDO | IPC_NOWAIT;

    int opResult = semop(semId, sema, nOperations);
    if (opResult != -1)
    {
        writeSharedMemory(frame);

        // --- release mutex ---
        nOperations     = 1;
        sema[0].sem_num = 0;
        sema[0].sem_op  = -1; // unlock: decrement back to 0
        sema[0].sem_flg = SEM_UNDO | IPC_NOWAIT;

        opResult = semop(semId, sema, nOperations);
        if (opResult == -1)
        {
            perror("semop (decrement)");
        }
    }
    // if we couldn't get the lock (EAGAIN), we just skip this frame
}


// scan ahead to figure out how many lines are in the next frame
// (reads until the next ESC+c delimiter)
int getFrameHeight(std::ifstream &file)
{
    auto linePosition = file.tellg();
    int start  = -1;
    int end    = -1;
    int height = 1;
    std::string line;

    while (std::getline(file, line))
    {
        if (start == -1)
        {
            if (line[0] == 27 && line[1] == 'c') // ESC + 'c' = frame delimiter
            {
                start = height;
            }
        }
        else
        {
            if (line[0] == 27 && line[1] == 'c') // hit next frame, stop
            {
                break;
            }
        }
        height++;
    }

    end = height;
    file.clear();
    file.seekg(linePosition); // rewind so getFrame can re-read
    return end - start;
}


// read one full frame from the file (calls getFrameHeight internally so it reads twice, I know)
frame *getFrame(std::ifstream &file)
{
    int height = getFrameHeight(file);
    std::string data;
    std::string line;
    std::string start;

    std::getline(file, start);
    data = start.substr(2, start.length() - 2); // strip the ESC+c prefix

    for (int i = 1; i < height; i++)
    {
        std::getline(file, line);
        data = data + "\n" + line;
    }

    frame *output  = new frame;
    output->data   = data;
    output->height = height;
    return output;
}


int main(int argc, char const *argv[])
{
    std::string fileName = argv[1];
    int fps              = atoi(argv[2]);

    // fork a listener process that waits for Enter then SIGINTs us
    pid_t producerPid = getpid();
    pid_t listener    = fork();
    if (listener == 0)
    {
        std::string res;
        std::getline(std::cin, res);
        std::cout << "Listener received input: " << res << std::endl;
        kill(producerPid, SIGINT);
        exit(0);
    }

    std::ifstream file(fileName);
    int interval = 1000 / fps; // ms per frame

    if (file.is_open())
    {
        while (true)
        {
            auto nextFrame = getFrame(file);
            trySharedMemory(nextFrame);
            delete nextFrame;

            // loop back to start when we hit end of file
            // TODO: file.end doesn't work like this — should use file.eof() or check tellg vs file size
            if (file.tellg() == file.end)
            {
                file.seekg(0, file.beg);
            }

            // wait for next frame slot
            // source: https://en.cppreference.com/w/cpp/thread/sleep_for
            std::this_thread::sleep_for(std::chrono::milliseconds(interval));
        }
    }

    return 0;
}

// sources
/*
    semaphores
    https://man7.org/linux/man-pages/man2/semop.2.html
    https://man7.org/linux/man-pages/man2/semget.2.html
    https://www.softprayog.in/programming/system-v-semaphores

    file reading
    https://cplusplus.com/doc/tutorial/files/

    sleep for ms
    https://en.cppreference.com/w/cpp/thread/sleep_for
*/