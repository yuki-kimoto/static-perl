#ifndef SPVM_DATA_HEADER_OBJECT_H
#define SPVM_DATA_HEADER_OBJECT_H

#include "spvm_value.h"
#include "spvm_base.h"

struct SPVM_data_header_object {
  int8_t type;
  int32_t ref_count;
  int32_t byte_size;
  SPVM_VALUE array_length_or_sv;
};

#endif
