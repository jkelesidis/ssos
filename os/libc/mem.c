#include "mem.h"

/**
 * memcpy implementation
 */
void memory_copy(uint8_t *source, uint8_t *dest, int nbytes) {

    for (int i = 0; i < nbytes; i++) {

        *(dest + i) = *(source + i);
    }
}

/**
 * memset implementation
 */
void memory_set(uint8_t *dest, uint8_t val, uint32_t len) {

    uint8_t *temp = (uint8_t *)dest;
    for ( ; len != 0; len--) {
        
        *temp++ = val;
    }
}

/**
 * malloc implementation
 */
uint32_t free_mem_addr = 0x10000;

uint32_t kmalloc(size_t size, int align, uint32_t *phys_addr) {

    // pages are aligned to 4k or 0x1000
    if (align == 1 && (free_mem_addr & 0xfffff000)) {

        free_mem_addr &= 0xfffff000;
        free_mem_addr += 0x1000;
    }

    // save the physical address
    if (phys_addr) {

        *phys_addr = free_mem_addr;
    }

    uint32_t ret = free_mem_addr;
    free_mem_addr += size; // increment the pointer to reserve the memory

    return ret;
}