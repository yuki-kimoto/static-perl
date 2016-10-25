#ifndef SPERL_MY_VAR_H
#define SPERL_MY_VAR_H

#include "sperl_core_type.h"
#include "sperl_type.h"

struct SPerl_my_var {
  SPerl_TYPE* type;
  SPerl_WORD* name_word;
  SPerl_int id;
  SPerl_METHOD* method;
  SPerl_ARRAY* descripters;
  SPerl_OP* op;
};

SPerl_MY_VAR* SPerl_MY_VAR_new();

#endif
