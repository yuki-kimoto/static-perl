#include "sperl_type_part.h"
#include "sperl.h"
#include "sperl_allocator_parser.h"

const char* const SPerl_TYPE_PART_C_CODE_NAMES[] = {
  "sub",
  "name",
  "char"
};

SPerl_TYPE_PART* SPerl_TYPE_PART_new(SPerl* sperl) {
  return SPerl_ALLOCATOR_PARSER_alloc_memory_pool(sperl, sperl->parser, sizeof(SPerl_TYPE_PART));
}
