//
// Created by os on 9/3/23.
//

#include "../h/syscall_cpp.hpp"
#include "../h/tcb.hpp"

void* operator new (size_t size) {
    return mem_alloc(size);
}

void operator delete(void *pVoid) noexcept {
    mem_free(pVoid);
}

Thread::Thread(void (*body)(void *), void *arg) : body(body), arg(arg) {}

int Thread::start() {
    return thread_create(&myHandle, threadWrapper, this);
}

void Thread::join() {
    thread_join(this->myHandle);
}

void Thread::dispatch() {
    thread_dispatch();
}

int Thread::sleep(time_t)  {
    return 0;
}

void Thread::threadWrapper(void *arg)  {
    auto thread = static_cast<Thread*>(arg);
    thread->run();
}

Thread::~Thread() {
    if (myHandle) {
        Scheduler::remove(myHandle);
        delete myHandle->stack;
        delete myHandle;
    }
}

Semaphore::Semaphore(unsigned int init) {
    sem_open(&myHandle, init);
}

int Semaphore::wait() {
    return sem_wait(myHandle);
}

int Semaphore::signal() {
    return sem_signal(myHandle);
}
