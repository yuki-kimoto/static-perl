#ifndef SPERL_RESOLVED_TYPE_H
#define SPERL_RESOLVED_TYPE_H

#include "sperl_base.h"

enum {
  SPerl_RESOLVED_TYPE_C_CORE_LENGTH = 14
};

enum {
  SPerl_RESOLVED_TYPE_C_ID_BYTE,
  SPerl_RESOLVED_TYPE_C_ID_SHORT,
  SPerl_RESOLVED_TYPE_C_ID_INT,
  SPerl_RESOLVED_TYPE_C_ID_LONG,
  SPerl_RESOLVED_TYPE_C_ID_FLOAT,
  SPerl_RESOLVED_TYPE_C_ID_DOUBLE,
  SPerl_RESOLVED_TYPE_C_ID_STRING,
  SPerl_RESOLVED_TYPE_C_ID_ARRAY_BYTE,
  SPerl_RESOLVED_TYPE_C_ID_ARRAY_SHORT,
  SPerl_RESOLVED_TYPE_C_ID_ARRAY_INT,
  SPerl_RESOLVED_TYPE_C_ID_ARRAY_LONG,
  SPerl_RESOLVED_TYPE_C_ID_ARRAY_FLOAT,
  SPerl_RESOLVED_TYPE_C_ID_ARRAY_DOUBLE,
  SPerl_RESOLVED_TYPE_C_ID_ARRAY_STRING,
};

extern const char* const SPerl_RESOLVED_TYPE_C_CORE_NAMES[];

struct SPerl_resolved_type {
  SPerl_ARRAY* part_names;
  const char* name;
  int32_t name_length;
  int32_t id;
  int32_t code;
};

SPerl_RESOLVED_TYPE* SPerl_RESOLVED_TYPE_new(SPerl* sperl);
_Bool SPerl_RESOLVED_TYPE_is_array(SPerl* sperl, SPerl_RESOLVED_TYPE* resolved_type);
_Bool SPerl_RESOLVED_TYPE_contain_sub(SPerl* sperl, SPerl_RESOLVED_TYPE* resolved_type);
_Bool SPerl_RESOLVED_TYPE_is_integral(SPerl* sperl, SPerl_RESOLVED_TYPE* resolved_type);
_Bool SPerl_RESOLVED_TYPE_is_core_value_array(SPerl* sperl, SPerl_RESOLVED_TYPE* resolved_type);
_Bool SPerl_RESOLVED_TYPE_is_core_value(SPerl* sperl, SPerl_RESOLVED_TYPE* resolved_type);
int32_t SPerl_RESOLVED_TYPE_get_array_dimention(SPerl* sperl, SPerl_RESOLVED_TYPE* resolved_type);
_Bool SPerl_RESOLVED_TYPE_is_calculatable_type(SPerl* sperl, SPerl_RESOLVED_TYPE* resolved_type);

#endif
