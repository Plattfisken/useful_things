/*
This is an stb-style single header library providing simple quality of life utilities for C programming.

To use simply add these two lines once in your project:

#define USEFUL_THINGS_IMPLEMENTATION
#include "useful_things.h"

*/

#include <stdio.h>
#include <stdint.h>

#ifndef USEFUL_THINGS_H
#define USEFUL_THINGS_H

// common types
#define KILOBYTES(n) ((1024) * (n))
#define MEGABYTES(n) ((1024) * (KILOBYTES(n)))
#define GIGABYTES(n) ((1024) * (MEGABYTES(n)))

typedef int8_t int8;
typedef int16_t int16;
typedef int32_t int32;
typedef int64_t int64;

typedef uint8_t uint8;
typedef uint16_t uint16;
typedef uint32_t uint32;
typedef uint64_t uint64;

typedef int8_t i8;
typedef int16_t i16;
typedef int32_t i32;
typedef int64_t i64;

// Maybe?
typedef int8_t s8;
typedef int16_t s16;
typedef int32_t s32;
typedef int64_t s64;

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

typedef float f32;
typedef double f64;

// memory management and arenas
#define UT_ARENA_DEFAULT_SIZE KILOBYTES(64)

typedef struct s_arena UT_Arena;
struct s_arena {
    struct s_arena *next;
    size_t size;
    u8 *cur;
    u8 memory[];
};

UT_Arena *UT_arena_create_size(size_t size);
UT_Arena *UT_arena_create();

void *UT_arena_alloc(UT_Arena *arena, size_t num_objects, size_t object_size);
void UT_arena_free(UT_Arena *arena);

// TODO: dynamic array implementation?

// file management
size_t UT_get_file_size(FILE *f);

// TODO:
// UT_read_entire_file
// UT_read_entire_file_and_null_terminate
// UT_read_entire_file_as_dynamic_array?
//
// UT_write_string_to_file

#ifdef USEFUL_THINGS_IMPLEMENTATION
UT_Arena *UT_arena_create_size(size_t size) {
    UT_Arena *arena = malloc(sizeof(*arena) + size);
    if(!arena) return NULL;
    arena->next = NULL;
    arena->size = size;
    arena->cur = arena->memory;
    return arena;
}

UT_Arena *UT_arena_create() {
    u32 default_size = UT_ARENA_DEFAULT_SIZE;
    return UT_arena_create_size(default_size);
}

void *UT_arena_alloc(UT_Arena *arena, size_t num_objects, size_t object_size) {
    const size_t allocation_size = num_objects * object_size;
    UT_Arena *linked_arena = arena;
    // Loop through all the arenas in the linked list. If one has space, allocate there and return.
    while(linked_arena) {
        size_t space_left = (linked_arena->memory + linked_arena->size) - linked_arena->cur;
        // handle misalignment
        if((ptrdiff_t)linked_arena->cur % object_size != 0) {
            u32 bytes_to_add_for_alignment = object_size - ((ptrdiff_t)linked_arena->cur % object_size);
            space_left -= bytes_to_add_for_alignment;
            if(allocation_size <= space_left)
                linked_arena->cur += bytes_to_add_for_alignment;
        }
        if(allocation_size <= space_left) {
            void *ret = linked_arena->cur;
            linked_arena->cur += allocation_size;
            assert((ptrdiff_t)ret % object_size == 0 && "Misaligned allocation");
            return ret;
        }
        else {
            linked_arena = linked_arena->next;
        }
    }
    // No arena had space. Create a new one and add to linked list
    // oversize it slightly to handle alignment
    size_t new_arena_size = allocation_size + object_size > arena->size ? allocation_size + object_size : arena->size;
    UT_Arena *new_arena = UT_arena_create_size(new_arena_size);

    new_arena->next = arena->next;
    arena->next = new_arena;
    // handle misalignment
    if((ptrdiff_t)new_arena->cur % object_size != 0) {
        u32 bytes_to_add_for_alignment = object_size - ((ptrdiff_t)new_arena->cur % object_size);
        new_arena->cur += bytes_to_add_for_alignment;
    }
    void *ret = new_arena->cur;
    assert((ptrdiff_t)ret % object_size == 0 && "Misaligned allocation");
    new_arena->cur += allocation_size;
    return ret;
}

void UT_arena_free(UT_Arena *arena) {
    UT_Arena *next = arena;
    while(next->next) {
        arena = next;
        next = next->next;
        free(arena);
    }
    free(next);
}

size_t UT_get_file_size(FILE *f) {
    fseek(f, 0, SEEK_END);
    size_t size = ftell(f);
    fseek(f, 0, SEEK_SET);
    return size;
}

#endif
#endif
