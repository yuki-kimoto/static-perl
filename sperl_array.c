#include <string.h>
#include <stdlib.h>
#include <assert.h>

#include "sperl.h"
#include "sperl_array.h"
#include "sperl_allocator_util.h"
#include "sperl_parser.h"

SPerl_ARRAY* SPerl_ARRAY_new(SPerl* sperl, int64_t capacity) {
  (void)sperl;
  
  assert(capacity >= 0);
  
  SPerl_ARRAY* array = SPerl_ALLOCATOR_UTIL_safe_malloc(1, sizeof(SPerl_ARRAY));
  array->length = 0;
  
  if (capacity == 0) {
    array->capacity = 128;
  }
  else {
    array->capacity = capacity;
  }
  
  SPerl_VALUE_T* values = SPerl_ALLOCATOR_UTIL_safe_malloc(array->capacity, sizeof(SPerl_VALUE_T));
  
  array->values = values;
  
  return array;
}

void SPerl_ARRAY_maybe_extend(SPerl* sperl, SPerl_ARRAY* array) {
  (void)sperl;
  
  assert(array);
  
  int64_t length = array->length;
  int64_t capacity = array->capacity;
  
  if (length >= capacity) {
    int64_t new_capacity = capacity * 2;
    array->values = SPerl_ALLOCATOR_UTIL_safe_realloc(array->values, new_capacity, sizeof(SPerl_VALUE_T));
    array->capacity = new_capacity;
  }
}

void SPerl_ARRAY_free(SPerl* sperl, SPerl_ARRAY* array) {
  (void)sperl;
  
  free(array->values);
  free(array);
}

/* Function templates

This is function templates shared with array manipulate functions.
All functions is genereted from the following templates.

  perl -E 'my $suffix = "address"; my $type = "void*"; my $start = 0; while (my $line = <>) { if ($line =~ /^\[END_TEMPLATE\]/) { last; } if ($start) { $line =~ s/%SUFFIX%/$suffix/g; $line =~ s/%TYPE%/$type/g; print $line; }  if ($line =~ /^\[START_TEMPLATE\]/) { $start = 1 }  }' sperl_array.c > template.tmp
  
  perl -E 'my $suffix = "long"; my $type = "int64_t"; my $start = 0; while (my $line = <>) { if ($line =~ /^\[END_TEMPLATE\]/) { last; } if ($start) { $line =~ s/%SUFFIX%/$suffix/g; $line =~ s/%TYPE%/$type/g; print $line; }  if ($line =~ /^\[START_TEMPLATE\]/) { $start = 1 }  }' sperl_array.c >> template.tmp

[START_TEMPLATE]
// Implementation
// SUFFIX : %SUFFIX%
// TYPE : %TYPE%
void SPerl_ARRAY_push_%SUFFIX%(SPerl* sperl, SPerl_ARRAY* array, %TYPE% value) {
  
  SPerl_ARRAY_maybe_extend(sperl, array);
  
  int64_t length = array->length;
  
  *(%TYPE%*)&array->values[length] = value;
  array->length++;
}

%TYPE% SPerl_ARRAY_fetch_%SUFFIX%(SPerl* sperl, SPerl_ARRAY* array, int64_t index) {
  (void)sperl;
  
  assert(array);
  assert(index >= 0);
  assert(index < array->length);
  
  return *(%TYPE%*)&array->values[index];
}

%TYPE% SPerl_ARRAY_pop_%SUFFIX%(SPerl* sperl, SPerl_ARRAY* array) {
  (void)sperl;
  
  assert(array->length > 0);
  
  array->length--;
  
  return *(%TYPE%*)&array->values[array->length];
}
[END_TEMPLATE]
*/

// Implementation
// SUFFIX : address
// TYPE : void*
void SPerl_ARRAY_push_address(SPerl* sperl, SPerl_ARRAY* array, void* value) {
  
  SPerl_ARRAY_maybe_extend(sperl, array);
  
  int64_t length = array->length;
  
  *(void**)&array->values[length] = value;
  array->length++;
}

void* SPerl_ARRAY_fetch_address(SPerl* sperl, SPerl_ARRAY* array, int64_t index) {
  (void)sperl;
  
  assert(array);
  assert(index >= 0);
  assert(index < array->length);
  
  return *(void**)&array->values[index];
}

void* SPerl_ARRAY_pop_address(SPerl* sperl, SPerl_ARRAY* array) {
  (void)sperl;
  
  assert(array->length > 0);
  
  array->length--;
  
  return *(void**)&array->values[array->length];
}
// Implementation
// SUFFIX : long
// TYPE : int64_t
void SPerl_ARRAY_push_long(SPerl* sperl, SPerl_ARRAY* array, int64_t value) {
  
  SPerl_ARRAY_maybe_extend(sperl, array);
  
  int64_t length = array->length;
  
  *(int64_t*)&array->values[length] = value;
  array->length++;
}

int64_t SPerl_ARRAY_fetch_long(SPerl* sperl, SPerl_ARRAY* array, int64_t index) {
  (void)sperl;
  
  assert(array);
  assert(index >= 0);
  assert(index < array->length);
  
  return *(int64_t*)&array->values[index];
}

int64_t SPerl_ARRAY_pop_long(SPerl* sperl, SPerl_ARRAY* array) {
  (void)sperl;
  
  assert(array->length > 0);
  
  array->length--;
  
  return *(int64_t*)&array->values[array->length];
}
