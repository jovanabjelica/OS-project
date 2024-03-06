//
// Created by os on 9/3/23.
//

#include "../h/scheduler.hpp"

Scheduler Scheduler::only_instance;
int Scheduler::created = 0;
int Scheduler::count = 0;

T* Scheduler::head = nullptr;
T* Scheduler::tail = nullptr;

Scheduler& Scheduler::get_only_instance() {
    if (created == 0) {
        only_instance = Scheduler();
        created = 1;
    }

    return only_instance;
}

TCB* Scheduler::get() {
    if (Scheduler::count == 0)
        return nullptr; //there is no threads in scheduler

    T * temp = head;
    head = head->next;

    TCB* ret = temp->thread;

    Mem::mem_free(temp);
    count--;

    return ret;
}

void Scheduler::put(TCB* thread) {
    size_t scheduler_blocks = Mem::calculate_blocks(sizeof(Scheduler));
    auto new_thread = (T*)Mem::mem_alloc(scheduler_blocks);

    new_thread->next= nullptr;
    new_thread->thread = thread;

    if (!head) {
        head = new_thread;
    } else {
        tail->next = new_thread;
    }

    tail = new_thread;

    count++;
}

void Scheduler::remove(TCB *thread) {
    if (!thread)
        return;

    T* temp = head,* prev = nullptr;
    while (temp) {
        if (temp->thread == thread)
            break;
        prev = temp;
        temp = temp->next;
    }

    if (!temp)
        return;

    if (prev) {
        prev->next = temp->next;
    } else {
        head = head->next;
    }

    Mem::mem_free(temp);

    count--;
}
