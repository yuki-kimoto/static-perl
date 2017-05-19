#ifndef SPVM_DATA_HEADER_STRING_H
#define SPVM_DATA_HEADER_STRING_H

#include "spvm_value.h"
#include "spvm_base.h"

struct SPVM_data_header_string {
  int8_t type;
  int32_t ref_count;
  SPVM_SV* sv;
};

#endif
