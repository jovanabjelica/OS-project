//
// Created by os on 9/3/23.
//

#include "../h/syscall_c.h"
#include "../h/riscv.hpp"

#include "../h/userMain.hpp"

int main() {
    //set bits for interrupt routine
    Riscv::w_stvec((uint64)&Riscv::supervisorTrap);
    Riscv::ms_sstatus(Riscv::SSTATUS_SIE);

    thread_t m; //main thread
    thread_create(&m, nullptr, nullptr);

    TCB::running = m; //setting running to main

    thread_t t; //test thread
    thread_create(&t, wrapperUserMain, nullptr);

    thread_join(t);

    return 0;
}

