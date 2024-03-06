//
// Created by os on 9/3/23.
//

#ifndef PROJECT_BASE_RISCV_HPP
#define PROJECT_BASE_RISCV_HPP


#include "handlers.hpp"
#include "../test/printing.hpp"

class Riscv {
public:
    // pop sstatus.spp and sstatus.spie bits (has to be a non-inline function)
    static void popSppSpie();

    // read register scause
    static uint64 r_scause();

    // write register stvec
    static void w_stvec(uint64 stvec);

    enum BitMaskSstatus {
        SSTATUS_SIE = (1 << 1),
    };

    // mask set register sstatus
    static void ms_sstatus(uint64 mask);

    // supervisor trap
    static void supervisorTrap();

private:
    // supervisor trap handler
    static void handle_supervisor_trap();
};

inline uint64 Riscv::r_scause() {
    uint64 volatile scause;
    __asm__ volatile ("csrr %[scause], scause" : [scause] "=r"(scause));
    return scause;
}

inline void Riscv::w_stvec(uint64 stvec) {
    __asm__ volatile ("csrw stvec, %[stvec]" : : [stvec] "r"(stvec));
}

inline void Riscv::ms_sstatus(uint64 mask) {
    __asm__ volatile ("csrs sstatus, %[mask]" : : [mask] "r"(mask));
}

#endif //PROJECT_BASE_RISCV_HPP
