//
// Created by os on 9/3/23.
//

#ifndef PROJECT_BASE_SCHEDULER_HPP
#define PROJECT_BASE_SCHEDULER_HPP

#include "mem.hpp"

class TCB;

typedef struct thread {
    TCB* thread;
    struct thread* next;
}T;

class Scheduler {
private:
    Scheduler() = default;

    static int count;
    static int created;
    static Scheduler only_instance;

    static T * head, * tail;
public:
    static TCB* get();
    static void put(TCB* thread);
    static void remove(TCB* thread);

    inline static int get_created() { return count; }

    Scheduler(const Scheduler&) = default;
    Scheduler& operator=(const Scheduler&) = default;

    static Scheduler& get_only_instance();
};


#endif //PROJECT_BASE_SCHEDULER_HPP
