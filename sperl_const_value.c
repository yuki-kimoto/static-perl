#include <stdlib.h>

#include "sperl_const_value.h"
#include "sperl_memory_pool.h"
#include "sperl_parser.h"

SPerl_CONST_VALUE* SPerl_CONST_VALUE_new(SPerl_PARSER* parser) {
  return (SPerl_CONST_VALUE*)SPerl_MEMORY_POOL_alloc(parser->memory_pool, sizeof(SPerl_CONST_VALUE));
}
