#ifndef SPERL_CONSTANT_POOL_H
#define SPERL_CONSTANT_POOL_H

#include "sperl_base.h"

// Array
struct SPerl_constant_pool {
  int32_t length;
  int32_t capacity;
  int32_t* values;
};

// Array function
SPerl_CONSTANT_POOL* SPerl_CONSTANT_POOL_new();
void SPerl_CONSTANT_POOL_extend(SPerl_CONSTANT_POOL* constant_pool, int32_t extend);
void SPerl_CONSTANT_POOL_push_int(SPerl_CONSTANT_POOL* constant_pool, int value);
void SPerl_CONSTANT_POOL_push_long(SPerl_CONSTANT_POOL* constant_pool, int64_t value);
void SPerl_CONSTANT_POOL_push_float(SPerl_CONSTANT_POOL* constant_pool, float value);
void SPerl_CONSTANT_POOL_push_double(SPerl_CONSTANT_POOL* constant_pool, double value);
void SPerl_CONSTANT_POOL_push_string(SPerl_CONSTANT_POOL* constant_pool, const char* string);
void SPerl_CONSTANT_POOL_push_constant(SPerl_CONSTANT_POOL* constant_pool, SPerl_CONSTANT* constant);

#endif
