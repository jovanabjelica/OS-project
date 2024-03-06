//
// Created by os on 9/3/23.
//

#include "../h/mem.hpp"

Mem::Elem* Mem::head = nullptr;

void *Mem::add_fragment(size_t blocks) {
    //there is nothing allocated
    if (head == nullptr) {
        head = (Elem*)HEAP_START_ADDR;
        head->blocks = calculate_blocks((size_t)HEAP_END_ADDR - (size_t)(HEAP_START_ADDR));
        head->next = nullptr;
    }

    Elem* temp = head, * prev = nullptr;
    while (temp and temp->blocks < blocks) { //finding corresponding block
        prev = temp;
        temp = temp->next;
    }

    if (!temp) //there is no space to allocate elem
        return nullptr;

    if (temp->blocks == blocks) { //deleting elem from linked list
        if (prev)
            prev->next = temp->next;
        else
            head = head->next;

        return (void*)(temp + sizeof(Elem) + 1);
    }

    //allocating just needed blocks
    size_t rest_blocks = temp->blocks - blocks;
    temp->blocks = blocks;

    Elem* rest_fragment = temp + blocks * MEM_BLOCK_SIZE + 1;

    rest_fragment->blocks = rest_blocks;
    rest_fragment->next = temp->next;

    if (prev)
        prev->next = rest_fragment;
    else
        head = rest_fragment;

    return (void*)(temp + sizeof(Elem) + 1);
}

void *Mem::mem_alloc(size_t blocks) {
    return add_fragment(blocks);
}

int Mem::delete_fragment(void *p) {
    if (!p)
        return -1;

    if (p < (Elem*)HEAP_START_ADDR or  p >= (Elem*)HEAP_END_ADDR)
        return -2;

    Elem* new_elem = (Elem*)p - sizeof(Elem) - 1;

    Elem* temp = head, * prev = nullptr;

    while (temp and temp < new_elem){
        prev = temp;
        temp = temp->next;
    }

    if (prev && (reinterpret_cast<char*>(prev) + prev->blocks * MEM_BLOCK_SIZE + 1) == reinterpret_cast<char*>(new_elem)) {
        prev->blocks += new_elem->blocks;
        new_elem = prev;

    } else {
        new_elem->next = temp;
        if (prev) {
            prev->next = new_elem;
        } else {
            head = new_elem;
        }
    }

    // Check if the new_elem can be merged with the next block
    if (temp && (reinterpret_cast<char*>(new_elem) + new_elem->blocks * MEM_BLOCK_SIZE) == reinterpret_cast<char*>(temp)) {
        new_elem->blocks += temp->blocks;
        new_elem->next = temp->next;
    }

    return 0;
}

int Mem::mem_free(void *p) noexcept {
    return delete_fragment(p);
}

size_t Mem::calculate_blocks(size_t size) {
    size_t blocks = (size + sizeof(Elem))/MEM_BLOCK_SIZE;
    if (blocks * MEM_BLOCK_SIZE != size + sizeof(Elem))
        blocks += 1;

    return blocks;
}