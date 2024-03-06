//
// Created by os on 9/3/23.
//

#include "../h/riscv.hpp"

extern "C" void clear_sstatus();

void Riscv::popSppSpie() {
    clear_sstatus();
}

void Riscv::handle_supervisor_trap() {
    uint64 scause = r_scause();

    if (scause == 0x0000000000000008UL || scause == 0x0000000000000009UL) {
        // interrupt: no; cause code: environment call from User-mode(8) or System-mode(9)
        Handlers::handle_sys_call();
    }
    else if (scause == 0x8000000000000009UL) {
        // interrupt: yes; cause code: supervisor external interrupt (PLIC; could be keyboard)
        Handlers::handle_console_interrupt();

    } else if (scause == 0x8000000000000001UL) {
        //interrupt: yes; cause code: supervisor internal interrupt - timer
        Handlers::handle_timer_interrupt();
    }
    else {
        //exception
        Handlers::handle_exception();
    }
}
