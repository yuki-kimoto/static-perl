#ifndef SPVM_RUNTIME_ALLOCATOR_H
#define SPVM_RUNTIME_ALLOCATOR_H
#include <stddef.h>
#include "spvm_base.h"

// Parser information
struct SPVM_runtime_allocator {
  // Memory_pool
  SPVM_MEMORY_POOL* memory_pool;
  
  // Free lists
  SPVM_ARRAY** freelists;

  // use memory pool max reference byte size
  int64_t ref_max_byte_size_use_memory_pool;
};

SPVM_RUNTIME_ALLOCATOR* SPVM_RUNTIME_ALLOCATOR_new(SPVM* spvm);
void* SPVM_RUNTIME_ALLOCATOR_malloc(SPVM* spvm, SPVM_RUNTIME_ALLOCATOR* allocator, int64_t size);
void SPVM_RUNTIME_ALLOCATOR_free_ref(SPVM* spvm, SPVM_RUNTIME_ALLOCATOR* allocator, SPVM_REF* ref);
int32_t SPVM_RUNTIME_ALLOCATOR_get_freelist_index(SPVM* spvm, SPVM_RUNTIME_ALLOCATOR* allocator, int64_t size);
void SPVM_RUNTIME_ALLOCATOR_free(SPVM* spvm, SPVM_RUNTIME_ALLOCATOR* allocator);

#endif
