//
// Created by os on 9/3/23.
//

#ifndef PROJECT_BASE_SYSCALL_CPP_HPP
#define PROJECT_BASE_SYSCALL_CPP_HPP

#include "syscall_c.h"

void* operator new (size_t size);

void operator delete (void* pVoid) noexcept;

class Thread {
public:
    Thread (void (*body)(void*), void* arg);
    virtual ~Thread ();

    int start ();

    void join();

    static void dispatch ();

    static int sleep (time_t);

protected:
    Thread () : body(nullptr), arg(nullptr) {}
    virtual void run () { this->body(this->arg); }

private:
    thread_t myHandle;
    void(*body)(void*);
    void* arg;

    static void threadWrapper(void* arg);
};

class Semaphore {
public:
    Semaphore (unsigned init = 1);

    virtual ~Semaphore () { sem_close(myHandle); }

    int wait ();
    int signal ();
private:
    sem_t myHandle;
};

class PeriodicThread : public Thread {
public:
    void terminate () {}
protected:
    PeriodicThread (time_t period) : period(period) {}
    virtual void periodicActivation () {}
private:
    time_t period;
};

class Console {
public:
    static char getc () { return ::getc(); }
    static void putc (char chr) { ::putc(chr); }
};


#endif //PROJECT_BASE_SYSCALL_CPP_HPP
