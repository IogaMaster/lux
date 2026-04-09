#ifndef PAGE_ALLOCATOR_H
#define PAGE_ALLOCATOR_H

#include <arch.h>
#include <types.h>

#define PAGE_ALIGN(addr) (((addr) + (PAGE_SIZE - 1)) & ~(PAGE_SIZE - 1))

/**
 * @brief Initializes the physical memory, reserves the bitmap based on mem size
 *
 * @param mem_end Address of the last byte of memory
 */
void pmm_init(uintptr_t mem_end);

/**
 * @brief Allocates a block of physical memory
 *
 * @return Pointer to the start of the allocated block
 */
uintptr_t pmm_alloc(void);

/**
 * @brief Frees a block of memory
 *
 * @param addr The address to the block of memory to be freed
 */
void pmm_free(uintptr_t addr);

#endif
