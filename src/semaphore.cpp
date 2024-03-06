//
// Created by os on 9/3/23.
//

#include "../h/semaphore.hpp"

extern "C" void context_switch(TCB::Context* old_context, TCB::Context* new_context);

int semaphore::sem_open(sem_t *handle, unsigned int init) {
    if (!handle)
        return -1; //bad parameter

    size_t sem_blocks = Mem::calculate_blocks(sizeof(semaphore));
    auto sem = (semaphore*)Mem::mem_alloc(sem_blocks);

    if (!sem)
        return -2; //failed to create semaphore

    sem->handle = *handle;
    sem->init = (int)init;

    sem->head = nullptr;
    sem->tail = nullptr;

    *handle = sem;

    return 0;
}

int semaphore::sem_close(sem_t handle) {
    if (!handle)
        return -1; //bad parameter

    Blocked* temp;
    while (handle->head) {
        temp = handle->head;
        temp->thread->valid = -1;

        Scheduler::put(temp->thread);

        handle->head = handle->head->next;

        mem_free(temp);
    }

    return 0;
}

void semaphore::block() {
    size_t blocked_blocks = Mem::calculate_blocks(sizeof(Blocked));

    TCB::running->on_semaphore = 1;

    auto new_blocked = (Blocked*)Mem::mem_alloc(blocked_blocks);
    if (!new_blocked)
        return;

    new_blocked->thread = TCB::running;
    new_blocked->next = nullptr;

    if (!this->head) {
        this->head = new_blocked;
    } else {
        this->tail->next = new_blocked;
    }

    this->tail = new_blocked;

    TCB* old = TCB::running;
    TCB::running = Scheduler::get();

    context_switch(&old->context, &TCB::running->context);
}

void semaphore::unblock() {
    if (!head)
        return;

    auto free = head;
    head = head->next;

    Scheduler::put(free->thread);
    Mem::mem_free(free);

    //TCB::thread_dispatch();
}

int semaphore::sem_wait(sem_t id) {
    if (!id)
        return -1; //bad parameter

    id->init -= 1;
    if (id->init < 0)
        id->block();

    if (TCB::running->on_semaphore == 0)
        return 0;

    int valid = TCB::running->valid;

    TCB::running->on_semaphore = 0;
    TCB::running->valid = 0;

    if (valid == -1)
        return -2; //semaphore was closed, thread is not regular unblocked from semaphore

    return 0;
}

int semaphore::sem_signal(sem_t id) {
    if (!id)
        return -1; //bad parameter

    id->init += 1;

    if (id->init <= 0)
        id->unblock();

    return 0;
}

