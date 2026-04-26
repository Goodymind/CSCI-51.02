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

// might include this to shared.h later
struct frame
{
    int height;
    std::string *data;

    ~frame()
    {
        delete[] data;
    }
};

void writeSharedMemory(frame *frame)
{
    for (size_t i = 0; i < frame->height; i++)
    {
        std::cout << frame->data[i] << std::endl;
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

frame* getFrame(std::ifstream &file)
{
    // I'm reading twice;
    int height = getFrameHeight(file);
    std::string* data = new std::string[height];

    std::string start;
    std::getline(file, start);
    data[0] = start.substr(2, start.length() - 2);
    for (int i = 1; i < height; i++)
    {
        std::getline(file, data[i]);
    }

    frame* output = new frame;
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
            writeSharedMemory(nextFrame);
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
