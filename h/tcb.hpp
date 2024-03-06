//
// Created by os on 9/3/23.
//

#ifndef PROJECT_BASE_TCB_HPP
#define PROJECT_BASE_TCB_HPP


#include "../lib/hw.h"

#include "scheduler.hpp"
#include "syscall_c.h"

#include "../h/riscv.hpp"

class TCB;

typedef TCB* thread_t;

class TCB {
public:
    struct Context {
        uint64 ra; //return address from interrupt routine
        uint64 sp; //stack pointer
    };

    static TCB* running; //current executing thread

    //not static methods
    inline void set_finished(bool f) { this->finished = f; }
    inline bool get_finished() const { return finished; }
    inline thread_t* get_handler() const { return handle; }


    //static methods
    static int thread_exit();
    static void thread_dispatch();
    static int create_thread(thread_t* handle, void(*start_routine)(void*), void* arg,uint64* stack);
    static void thread_join(thread_t handler);

    inline static int get_created() { return Scheduler::get_created(); }
private:
    friend class semaphore;
    friend class Thread;

    using Body = void(*)(void*);

    //class fields
    Body body; //function
    //void* arg; //thread arguments
    thread_t* handle; //thread id
    void* arg; //function arguments

    Context context;
    uint64* stack;

    bool finished;

    int on_semaphore; //is thread blocked on semaphore
    int valid; //is thread unblocked from semaphore or semaphore was closed

    static void thread_wrapper();

    static void create_thread_stack(TCB* thread, thread_t *handle, void (*start_routine)(void*), void* arg, uint64* stack);
    static void create_thread_no_stack(TCB* thread, thread_t *handle);
};


#endif //PROJECT_BASE_TCB_HPP
