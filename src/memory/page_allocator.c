#include "page_allocator.h"
#include <types.h>

// TODO: Rewrite the page_allocator to use a buddy system when the kernel gets big enough

// char[] is a simple way to make C99 express a externally linked symbol while keeping pointer
// arithmetic, void* needs a bunch of casting while `_end + 0x3001` for example is valid C code
extern char _end[];

// Start of physical memory
static uintptr_t base_ptr = (uintptr_t)_end;
static uint64_t *bitmap = NULL;
static uint64_t total_frames = 0;

// This is an early initialization function, it reserves the space necessary to map memory to pages.
// We kick the base_ptr upwards, this becomes the 'start' of managed physical memory
//  so the mapper does not need to manage pages containing memory that the kernel is in
static void *boot_alloc(size_t size) {
    base_ptr = PAGE_ALIGN(base_ptr);
    void *ptr = (void *)base_ptr;
    base_ptr += size;

    // ZERO the mem
    for (size_t i = 0; i < size; i++) {
        ((char *)ptr)[i] = 0;
    }
    return ptr;
}

// ========= IMPLEMENTATION OF PMM =========

void pmm_init(uintptr_t mem_end) {
    size_t mem_size = (mem_end - base_ptr);
    total_frames = mem_size / PAGE_SIZE;

    size_t num_chunks = (total_frames + 63) / 64;
    bitmap = (uint64_t *)boot_alloc(num_chunks * sizeof(uint64_t));

    for (uint64_t i = 0; i < num_chunks; i++)
        bitmap[i] = 0ULL;
}

uintptr_t pmm_alloc(void) {
    for (uint64_t i = 0; i < total_frames / 64; i++) {
        if (bitmap[i] == ~0ULL)
            continue; // entire frame is filled

        for (uint64_t bit = 0; bit < 64; bit++) {
            if (!(bitmap[i] & (1ULL << bit))) {
                bitmap[i] |= (1ULL << bit);

                uint64_t offset = ((i * 64) + bit) << PAGE_SHIFT;
                return PAGE_ALIGN(base_ptr + offset);
            }
        }
    }

    return (uintptr_t)NULL; // OOM
}

void pmm_free(uintptr_t addr) {
    uintptr_t ptr = addr - base_ptr;

    uint64_t page_id = ptr >> PAGE_SHIFT;
    uint64_t chunk_id = page_id / 64;
    uint64_t bit = page_id % 64;

    bitmap[chunk_id] &= ~(1ULL << bit);
}
