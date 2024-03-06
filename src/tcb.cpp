//
// Created by os on 9/3/23.
//

#include "../h/tcb.hpp"

extern "C" void context_switch(TCB::Context* old_context, TCB::Context* new_context);

TCB* TCB::running;

int TCB::thread_exit() {
    Context old_context = running->context;

    TCB* old = running;
    running = Scheduler::get();

    Mem::mem_free(old->stack);
    Mem::mem_free(old);

    context_switch(&old_context, &running->context);

    return 0;
}

void TCB::thread_dispatch() {
    TCB* old = running;

    if (!old->finished) {
        Scheduler::put(old);
    }

    running = Scheduler::get();
    if (!running)
        return;

    context_switch(&old->context, &running->context);
}

void TCB::thread_join(thread_t handler) {
    while (handler and !handler->finished)
        TCB::thread_dispatch();
}

void TCB::thread_wrapper() {
    Riscv::popSppSpie();

    running->body(running->arg);
    running->set_finished(true);

    ::thread_dispatch();
}

void TCB::create_thread_stack(TCB *thread, thread_t *handle, void (*start_routine)(void *), void *arg, uint64 *stack) {
    thread->handle = handle;
    thread->body = start_routine;
    thread->stack = stack;

    thread->arg = arg;
    thread->finished = false;

    thread->context = {
            (uint64) &thread_wrapper,
            (uint64)&stack[DEFAULT_STACK_SIZE]
    };

    Scheduler::put(thread);
}

void TCB::create_thread_no_stack(TCB *thread, thread_t *handle) {
    thread->handle = handle;
    thread->body = nullptr;
    thread->stack = nullptr;
    thread->arg = nullptr;
    thread->finished = false;

    thread->context = {
            (uint64) &thread_wrapper,
            0
    };
}

int TCB::create_thread(thread_t *handle, void (*start_routine)(void*), void* arg, uint64* stack) {
    if (!handle)
        return -1; //Bad parameter
    size_t tcb_size = sizeof(TCB);

    TCB* thread = (TCB*)Mem::mem_alloc(Mem::calculate_blocks(tcb_size));
    if (!thread)
        return -2; //Failed to allocate memory

    if (start_routine)
        create_thread_stack(thread, handle, start_routine, arg, stack);
    else
        create_thread_no_stack(thread, handle);

    thread->on_semaphore = 0;
    thread->valid = 0;

    *handle = thread;

    return 0;
}

