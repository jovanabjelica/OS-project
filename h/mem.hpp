//
// Created by os on 9/3/23.
//

#ifndef PROJECT_BASE_MEM_HPP
#define PROJECT_BASE_MEM_HPP


#include "../lib/hw.h"

class Mem {
private:
    typedef struct elem {
        size_t blocks;
        struct elem* next;
    } Elem;

    static Elem* head;

    Mem() = default;

    static void* add_fragment(size_t blocks);
    static int delete_fragment(void* p);

    static size_t calculate_blocks(size_t size);

public:

    friend class Scheduler;

    friend class TCB;
    friend class semaphore;

    static void* mem_alloc(size_t blocks);
    static int mem_free(void* p) noexcept;
};

#endif //PROJECT_BASE_MEM_HPP
