#include <string.h>
#include "spvm_resolved_type.h"

#include "spvm_compiler_allocator.h"
#include "spvm_package.h"
#include "spvm_compiler.h"

const char* const SPVM_RESOLVED_TYPE_C_CORE_NAMES[] = {
  "byte",
  "short",
  "int",
  "long",
  "float",
  "double",
  "byte[]",
  "short[]",
  "int[]",
  "long[]",
  "float[]",
  "double[]",
};

SPVM_RESOLVED_TYPE* SPVM_RESOLVED_TYPE_new(SPVM_COMPILER* compiler) {
  SPVM_RESOLVED_TYPE* resolved_type = SPVM_COMPILER_ALLOCATOR_alloc_memory_pool(compiler, compiler->allocator, sizeof(SPVM_RESOLVED_TYPE));
  
  resolved_type->part_names = SPVM_COMPILER_ALLOCATOR_alloc_array(compiler, compiler->allocator, 0);
  
  return resolved_type;
}

_Bool SPVM_RESOLVED_TYPE_is_array(SPVM_COMPILER* compiler, SPVM_RESOLVED_TYPE* resolved_type) {
  (void)compiler;
  
  int32_t length = (int32_t)strlen(resolved_type->name);
  
  if (strlen(resolved_type->name) >= 2) {
    char char1 = resolved_type->name[length - 2];
    char char2 = resolved_type->name[length - 1];
    
    if (char1 == '[' && char2 == ']') {
      return 1;
    }
    else {
      return 0;
    }
  }
  else {
    return 0;
  }
}

_Bool SPVM_RESOLVED_TYPE_is_array_numeric(SPVM_COMPILER* compiler, SPVM_RESOLVED_TYPE* resolved_type) {
  (void)compiler;
  
  const char* name = resolved_type->name;
  
  if (strcmp(name, "char[]") == 0 || strcmp(name, "byte[]") == 0 || strcmp(name, "short[]") == 0
    || strcmp(name, "int[]") == 0 || strcmp(name, "long[]") == 0 || strcmp(name, "float[]") == 0 || strcmp(name, "double[]") == 0)
  {
    return 1;
  }
  else {
    return 0;
  }
}

_Bool SPVM_RESOLVED_TYPE_is_integral(SPVM_COMPILER* compiler, SPVM_RESOLVED_TYPE* resolved_type) {
  (void)compiler;
  
  if (resolved_type->id <= SPVM_RESOLVED_TYPE_C_ID_LONG) {
    return 1;
  }
  else {
    return 0;
  }
}

_Bool SPVM_RESOLVED_TYPE_is_numeric(SPVM_COMPILER* compiler, SPVM_RESOLVED_TYPE* resolved_type) {
  (void)compiler;
  
  if (resolved_type && resolved_type->id <= SPVM_RESOLVED_TYPE_C_ID_DOUBLE) {
    return 1;
  }
  else {
    return 0;
  }
}

