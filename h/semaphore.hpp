//
// Created by os on 9/3/23.
//

#ifndef PROJECT_BASE_SEMAPHORE_HPP
#define PROJECT_BASE_SEMAPHORE_HPP


#include "../lib/hw.h"
#include "tcb.hpp"
#include "mem.hpp"

class semaphore;

typedef semaphore* sem_t;

class semaphore {
public:
    static int sem_open(sem_t* handle, unsigned init);
    static int sem_close(sem_t handle);

    static int sem_wait(sem_t id);
    static int sem_signal(sem_t id);

protected:
    void block();
    void unblock();

private:
    //Struct for blocked threads queue
    typedef struct blocked {
        TCB* thread;
        struct blocked* next;
    }Blocked;

    Blocked* head, * tail;

    int init;
    sem_t handle;
};


#endif //PROJECT_BASE_SEMAPHORE_HPP
