#include "../../include/memory.h"

namespace Memory {
    // Memory map structure
    struct MemoryBlock {
        size_t size;
        bool is_free;
        MemoryBlock* next;
    };

    static MemoryBlock* heap_start = nullptr;
    static size_t total_memory = 0;
    static size_t used_memory = 0;

    void initialize() {
        // Initialize heap starting at HEAP_START
        heap_start = reinterpret_cast<MemoryBlock*>(HEAP_START);
        heap_start->size = total_memory - sizeof(MemoryBlock);
        heap_start->is_free = true;
        heap_start->next = nullptr;
    }

    void* allocate(size_t size) {
        MemoryBlock* current = heap_start;
        while (current) {
            if (current->is_free && current->size >= size) {
                // Split block if it's too large
                if (current->size > size + sizeof(MemoryBlock)) {
                    MemoryBlock* new_block = reinterpret_cast<MemoryBlock*>(
                        reinterpret_cast<uint8_t*>(current) + sizeof(MemoryBlock) + size
                    );
                    new_block->size = current->size - size - sizeof(MemoryBlock);
                    new_block->is_free = true;
                    new_block->next = current->next;
                    
                    current->size = size;
                    current->next = new_block;
                }
                
                current->is_free = false;
                used_memory += current->size;
                return reinterpret_cast<void*>(reinterpret_cast<uint8_t*>(current) + sizeof(MemoryBlock));
            }
            current = current->next;
        }
        return nullptr;
    }

    void deallocate(void* ptr) {
        if (!ptr) return;

        MemoryBlock* block = reinterpret_cast<MemoryBlock*>(
            reinterpret_cast<uint8_t*>(ptr) - sizeof(MemoryBlock)
        );
        
        block->is_free = true;
        used_memory -= block->size;

        // Merge with next block if it's free
        if (block->next && block->next->is_free) {
            block->size += block->next->size + sizeof(MemoryBlock);
            block->next = block->next->next;
        }
    }

    size_t get_free_memory() {
        return total_memory - used_memory;
    }

    size_t get_used_memory() {
        return used_memory;
    }

    void* allocate_page() {
        return allocate(PAGE_SIZE);
    }

    void free_page(void* page) {
        deallocate(page);
    }

    bool protect_page(void* page, uint32_t flags) {
        // TODO: Implement page protection using CPU's paging mechanism
        return true;
    }

    void* map_physical(uint64_t physical_addr, size_t size) {
        // TODO: Implement physical memory mapping
        return reinterpret_cast<void*>(physical_addr);
    }

    void unmap_physical(void* virtual_addr) {
        // TODO: Implement physical memory unmapping
    }
}