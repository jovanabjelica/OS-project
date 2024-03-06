//
// Created by os on 9/3/23.
//

#include "../h/syscall_c.h"

size_t calculate_blocks(size_t size) {
    size_t blocks = (size + sizeof(Elem))/MEM_BLOCK_SIZE;
    if (blocks * MEM_BLOCK_SIZE != size + sizeof(Elem))
        blocks += 1;

    return blocks;
}

void* mem_alloc(size_t size) {
    size_t blocks = calculate_blocks(size); //calculate the num of cal_blocks for allocation

    void* result; //return value

    __asm__ volatile(
    "mv a1, %1\n\t"    // Move the number of blocks to a1
    "li a0, 0x01\n\t"  // Set the system call code to 0x01 (mem_alloc)
    "ecall\n\t"        // Enter the privileged mode
    "mv %0, a0"        // Move the return value from a0 to the result variable
    : "=r"(result)     // Output operand: result variable stores the return value
    : "r"(blocks)      // Input operand: cal_blocks variable as input
    : "a0", "a1", "memory"); //"memory" informs that this function is going to change memory

    return result; //return value from a0
}

int mem_free(void* p) {
    int result; //return value

    __asm__ volatile(
    "mv a1, %1\n\t"    // Move pointer to a1
    "li a0, 0x02\n\t"  // Set the system call code to 0x02 (mem_free)
    "ecall\n\t"        // Enter the privileged mode
    "mv %0, a0"        // Move the return value from a0 to the result variable
    : "=r"(result)     // Output operand: result variable stores the return value
    : "r"(p)           // Input operand: pointer p as input
    : "a0", "a1", "memory"); //"memory" informs that this function is going to change memory

    return result; //return value from a0
}

int thread_create(thread_t* handle, void(*start_routine)(void*), void* arg) {
    uint64* stack = !start_routine ? (uint64*)start_routine : (uint64*)mem_alloc(sizeof(uint64) * DEFAULT_STACK_SIZE);
    int result;

    __asm__ volatile(
    "mv a1, %1\n\t"    // Move handle to a1
    "mv a2, %2\n\t"    // Move start routine to a2
    "mv a6, %3\n\t"    // Move arg to a6
    "mv a7, %4\n\t"    // Move stack to a7
    "li a0, 0x11\n\t"  // Set the system call code to 0x11 (thread_create)
    "ecall\n\t"        // Enter the privileged mode
    "mv %0, a0"        // Move the return value from a0 to the result variable
    : "=r"(result)     // Output operand: result variable stores the return value
    : "r"(handle),     // Input operand: pointer handle as input
    "r"(start_routine),     // Input operand: function start routine as input
    "r"(arg),               // Input operand: arg of function as input
    "r"(stack)              // Input operand: stack
    : "a0", "a1", "a2", "a3", "a4", "memory");

    return result;
}

int thread_exit() {
    int result; //return value

    __asm__ volatile(
    "li a0, 0x12\n\t"  // Set the system call code to 0x02 (mem_free)
    "ecall\n\t"        // Enter the privileged mode
    "mv %0, a0"        // Move the return value from a0 to the result variable
    : "=r"(result)     // Output operand: result variable stores the return value
    :
    : "a0", "memory"); //"memory" informs that this function is going to change memory

    return result; //return value from a0
}

void thread_dispatch() {
    __asm__ volatile(
    "li a0, 0x13\n\t" //Set the system code to 0x13 (thread_dispatch)
    "ecall\n\t"); //Enter the privileged mode
}

void thread_join(thread_t handle) {
    __asm__ volatile(
    "mv a1, %0\n\t" //Move handle to a1
    "li a0, 0x14\n\t" // Set the system code to 0x14 (thread_join)
    "ecall\n\t" //Enter the privileged mode
    :
    : "r"(handle) //Input operand
    : "a0", "a1");
}

int sem_open(sem_t* handle, unsigned init) {
    int result; //return value

    __asm__ volatile(
    "mv a1, %1\n\t"    // Move handle to a1
    "mv a2, %2\n\t"    // Move init
    "li a0, 0x21\n\t"  // Set the system call code to 0x21 (sem_open)
    "ecall\n\t"        // Enter the privileged mode
    "mv %0, a0"        // Move the return value from a0 to the result variable
    : "=r"(result)     // Output operand: result variable stores the return value
    : "r"(handle),           // Input operand: pointer handle as input
    "r"((int)init)              // Input operand: init as input
    : "a0", "a1", "a2", "memory"); //"memory" informs that this function is going to change memory

    return result;
}

int sem_close (sem_t handle) {
    int result; //return value

    __asm__ volatile(
    "mv a1, %1\n\t"    // Move pointer to a1
    "li a0, 0x22\n\t"  // Set the system call code to 0x22 (sem_close)
    "ecall\n\t"        // Enter the privileged mode
    "mv %0, a0"        // Move the return value from a0 to the result variable
    : "=r"(result)     // Output operand: result variable stores the return value
    : "r"(handle)             // Input operand: pointer p as input
    : "a0", "a1", "memory"); //"memory" informs that this function is going to change memory

    return result; //return value from a0
}

int sem_wait (sem_t id) {
    int result; //return value

    __asm__ volatile(
    "mv a1, %1\n\t"    // Move pointer to a1
    "li a0, 0x23\n\t"  // Set the system call code to 0x23 (sem_wait)
    "ecall\n\t"        // Enter the privileged mode
    "mv %0, a0"        // Move the return value from a0 to the result variable
    : "=r"(result)     // Output operand: result variable stores the return value
    : "r"(id)             // Input operand: pointer p as input
    : "a0", "a1", "memory"); //"memory" informs that this function is going to change memory


    return result; //return value from a0
}

int sem_signal(sem_t id) {
    int result; //return value

    __asm__ volatile(
    "mv a1, %1\n\t"    // Move pointer to a1
    "li a0, 0x24\n\t"  // Set the system call code to 0x24 (sem_signal)
    "ecall\n\t"        // Enter the privileged mode
    "mv %0, a0"        // Move the return value from a0 to the result variable
    : "=r"(result)     // Output operand: result variable stores the return value
    : "r"(id)             // Input operand: pointer p as input
    : "a0", "a1", "memory"); //"memory" informs that this function is going to change memory

    return result; //return value from a0
}

int time_sleep (time_t time) {
    return 0;
}

char getc() {
    char result;

    __asm__ volatile(
    "li a0, 0x41\n\t" //Set the system call code to 0x41 (getc)
    "ecall\n\t" //Enter the privileged mode
    "mv %0, a0"
    :"=r"(result)
    :
    :"a0", "a1", "memory");

    return result;
}

void putc(char chr) {
    __asm__ volatile(
    "mv a1, %0\n\t"    // Move chr to a1
    "li a0, 0x42\n\t"  // Set the system call code to 0x42 (putc)
    "ecall\n\t"        // Enter the privileged mode
    :
    : "r"(chr)
    : "a0", "a1");
}
