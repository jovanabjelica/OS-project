//
// Created by os on 9/3/23.
//

#include "../h/handlers.hpp"

enum sys_call {
    MEM_ALLOC = 0x01,
    MEM_FREE = 0x02,

    THREAD_CREATE = 0x11,
    THREAD_EXIT = 0x12,
    THREAD_DISPATCH = 0x13,
    THREAD_JOIN = 0x14,

    SEM_OPEN = 0x21,
    SEM_CLOSE = 0x22,
    SEM_WAIT = 0x23,
    SEM_SIGNAL = 0x24,

    GETC = 0x41,
    PUTC = 0x42
};

void Handlers::handle_mem_alloc() {
    //load argument from a1 register
    size_t blocks;
    __asm__ volatile("mv %0, a1" : "=r"(blocks));

    //store result into a0 register
    void* result = Mem::mem_alloc(blocks);
    __asm__ volatile("mv a0, %0" : : "r"(result));
}

void Handlers::handle_mem_free() {
    //load argument from a1 register
    void* p;
    __asm__ volatile("mv %0, a1" : "=r"(p));

    //store result into a0 register
    int result = Mem::mem_free(p);
    __asm__ volatile("mv a0, %0" : : "r"(result));
}

void Handlers::handle_thread_create() {
    //load arguments from registers
    thread_t* handle;
    __asm__ volatile("mv %0, a1" : "=r"(handle));

    void(*start_routine)(void*);
    __asm__ volatile("mv %0, a2" : "=r"(start_routine));

    void* arg;
    __asm__ volatile("mv %0, a6" : "=r"(arg));

    uint64* stack;
    __asm__ volatile("mv %0, a7" : "=r"(stack));

    //store result into a0 register
    int result = TCB::create_thread(handle, start_routine, arg, stack);
    __asm__ volatile("mv a0, %0" : : "r"(result));
}

void Handlers::handle_thread_exit() {
    int result = TCB::thread_exit();

    //store result into a0 register
    __asm__ volatile("mv a0, %0" : : "r"(result));
}

void Handlers::handle_thread_dispatch() {
    TCB::thread_dispatch();
}

void Handlers::handle_thread_join() {
    //load handle from a1 register
    thread_t handle;
    __asm__ volatile("mv %0, a1" : "=r"(handle));

    TCB::thread_join(handle);
}

void Handlers::handle_sem_open() {
    //load argument from a1 register
    sem_t* sem;
    __asm__ volatile("mv %0, a1" : "=r"(sem));

    //load argument from a2 register
    unsigned init;
    __asm__ volatile("mv %0, a2" : "=r"(init));

    //store result into a0 register
    int result = semaphore::sem_open(sem, init);
    __asm__ volatile("mv a0, %0" : : "r"(result));
}

void Handlers::handle_sem_close() {
    //load argument from a1 register
    sem_t sem;
    __asm__ volatile("mv %0, a1" : "=r"(sem));

    //store result into a0 register
    int result = semaphore::sem_close(sem);
    __asm__ volatile("mv a0, %0" : : "r"(result));
}

void Handlers::handle_sem_wait() {
    //load argument from a1 register
    sem_t sem;
    __asm__ volatile("mv %0, a1" : "=r"(sem));

    //store result into a0 register
    int result = semaphore::sem_wait(sem);
    __asm__ volatile("mv a0, %0" : : "r"(result));
}

void Handlers::handle_sem_signal() {
    //load argument from a1 register
    sem_t sem;
    __asm__ volatile("mv %0, a1" : "=r"(sem));

    //store result into a0 register
    int result = semaphore::sem_signal(sem);
    __asm__ volatile("mv a0, %0" : : "r"(result));
}

void Handlers::handle_getc() {
    char result = __getc();
    __asm__ volatile("mv a0, %0" : : "r"(result));
}

void Handlers::handle_putc() {
    char chr;
    __asm__ volatile("mv %0, a1" : "=r"(chr));

    __putc(chr);
}

void Handlers::handle_sys_call() {
    uint64 SYS_CALL;
    __asm__ volatile("mv %0, a0" : "=r"(SYS_CALL));

    uint64 volatile sepc = r_sepc() + 4;
    uint64 volatile sstatus = r_sstatus();

    switch (SYS_CALL) {
        case MEM_ALLOC:
            handle_mem_alloc();
            break;

        case MEM_FREE:
            handle_mem_free();
            break;

        case THREAD_CREATE:
            handle_thread_create();
            break;

        case THREAD_EXIT:
            handle_thread_exit();
            break;

        case THREAD_DISPATCH:
            handle_thread_dispatch();
            break;

        case THREAD_JOIN:
            handle_thread_join();
            break;

        case SEM_OPEN:
            handle_sem_open();
            break;

        case SEM_CLOSE:
            handle_sem_close();
            break;

        case SEM_WAIT:
            handle_sem_wait();
            break;

        case SEM_SIGNAL:
            handle_sem_signal();
            break;

        case GETC:
            handle_getc();
            break;

        case PUTC:
            handle_putc();
            break;

        default:
            break;
    }

    w_sstatus(sstatus);
    w_sepc(sepc);
}

void Handlers::handle_exception()  {
    uint64 volatile scause;
    __asm__ volatile ("csrr %[scause], scause" : [scause] "=r"(scause));

    __putc('S');
    __putc('c');
    __putc('a');
    __putc('u');
    __putc('s');
    __putc('e');
    __putc(':');
    __putc(' ');

    if (scause == 2) {
        __putc('2');
    } else if (scause == 5) {
        __putc('5');
    } else {
        __putc('7');
    }

    __putc('\n');

    //thread_exit();
    while(true) { /* do nothing */ }
}

void Handlers::handle_timer_interrupt()  { mc_sip(1<<1); }

void Handlers::handle_console_interrupt() { console_handler(); }