#ifndef ALTOS_MEMORY_H
#define ALTOS_MEMORY_H

#include <stddef.h>
#include <stdint.h>

namespace Memory {
    // Memory management constants
    static const ::size_t PAGE_SIZE = 4096;
    static const ::size_t HEAP_START = 0x100000;
    
    // Memory allocation functions
    void* allocate(size_t size);
    void deallocate(void* ptr);
    
    // Memory information
    ::size_t get_free_memory();
    ::size_t get_used_memory();
    
    // Memory initialization
    void initialize();
    
    // Page management
    void* allocate_page();
    void free_page(void* page);
    
    // Memory protection
    bool protect_page(void* page, uint32_t flags);
    
    // Memory mapping
    void* map_physical(uint64_t physical_addr, ::size_t size);
    void unmap_physical(void* virtual_addr);
}

#endif // ALTOS_MEMORY_H