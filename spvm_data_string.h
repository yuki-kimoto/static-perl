#ifndef SPVM_DATA_STRING_H
#define SPVM_DATA_STRING_H

#include "spvm_value.h"
#include "spvm_base.h"

struct SPVM_data_string {
  int8_t type;
  int32_t ref_count;
};

#endif

