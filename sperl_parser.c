#include <stdlib.h>
#include <string.h>

#include "sperl_parser.h"
#include "sperl_resolved_type.h"
#include "sperl_package.h"
#include "sperl_type_component_name.h"
#include "sperl_type.h"
#include "sperl_op.h"
#include "sperl_memory_pool.h"
#include "sperl_hash.h"
#include "sperl_array.h"
#include "sperl_allocator.h"
#include "sperl_yacc.h"
#include "sperl_array.h"
#include "sperl_use.h"

SPerl_PARSER* SPerl_PARSER_new() {
  SPerl_PARSER* parser = calloc(1, sizeof(SPerl_PARSER));
  
  // Manipulate memory. This is freed last.
  parser->array_ptrs = SPerl_ARRAY_new(0);
  parser->hash_ptrs = SPerl_ARRAY_new(0);
  parser->long_str_ptrs = SPerl_ARRAY_new(0);
  parser->memory_pool = SPerl_MEMORY_POOL_new(0);
  
  // Parser information
  parser->cur_op_subs = SPerl_ALLOCATOR_new_array(parser, 0);
  parser->sub_abs_name_symtable = SPerl_ALLOCATOR_new_hash(parser, 0);
  parser->op_packages = SPerl_ALLOCATOR_new_array(parser, 0);
  parser->package_symtable = SPerl_ALLOCATOR_new_hash(parser, 0);
  parser->op_types = SPerl_ALLOCATOR_new_array(parser, 0);
  parser->op_use_stack = SPerl_ALLOCATOR_new_array(parser, 0);
  parser->field_abs_name_symtable = SPerl_ALLOCATOR_new_hash(parser, 0);
  parser->include_pathes = SPerl_ALLOCATOR_new_array(parser, 0);
  parser->bufptr = "";
  parser->resolved_types = SPerl_ALLOCATOR_new_array(parser, 0);
  parser->resolved_type_symtable = SPerl_ALLOCATOR_new_hash(parser, 0);
  parser->use_package_symtable = SPerl_ALLOCATOR_new_hash(parser, 0);
  parser->cur_op_cases = SPerl_ALLOCATOR_new_array(parser, 0);
  
  // Core types
  for (int32_t i = 0; i < SPerl_RESOLVED_TYPE_C_CORE_LENGTH; i++) {
    // Name
    const char* name = SPerl_RESOLVED_TYPE_C_CORE_NAMES[i];
    
    // Name
    SPerl_OP* op_name = SPerl_OP_newOP(parser, SPerl_OP_C_CODE_NAME, "CORE", 1);
    
    op_name->uv.name = name;
    
    // Resolved type
    SPerl_RESOLVED_TYPE* resolved_type = SPerl_RESOLVED_TYPE_new(parser);
    SPerl_ARRAY_push(resolved_type->part_names, (void*) name);
    resolved_type->name = name;
    resolved_type->name_length = strlen(name);
    resolved_type->id = i;
    SPerl_ARRAY_push(parser->resolved_types, resolved_type);
    SPerl_HASH_insert(parser->resolved_type_symtable, name, strlen(name), resolved_type);
    
    // Type name
    SPerl_TYPE_COMPONENT_NAME* type_component_name = SPerl_TYPE_COMPONENT_NAME_new(parser);
    type_component_name->op_name = op_name;
    
    // Type
    SPerl_TYPE* type = SPerl_TYPE_new(parser);
    type->code = SPerl_TYPE_C_CODE_NAME;
    type->uv.type_component_name = type_component_name;
    type->resolved_type = resolved_type;
    
    // Type OP
    SPerl_OP* op_type = SPerl_OP_newOP(parser, SPerl_OP_C_CODE_TYPE, "CORE", 1);
    op_type->uv.type = type;
    
    // Add type
    SPerl_ARRAY_push(parser->op_types, op_type);
    
    // Package
    SPerl_PACKAGE* package = SPerl_PACKAGE_new(parser);
    package->size = SPerl_RESOLVED_TYPE_C_CORE_SIZES[i];
    SPerl_OP* op_package_name = SPerl_OP_newOP(parser, SPerl_OP_C_CODE_NAME, "CORE", 1);
    op_package_name->uv.name = name;
    package->op_name = op_package_name;
    package->op_type = op_type;
    
    // Package OP
    SPerl_OP* op_package = SPerl_OP_newOP(parser, SPerl_OP_C_CODE_DECL_PACKAGE, "CORE", 1);
    op_package->uv.package = package;
    
    SPerl_ARRAY_push(parser->op_packages, op_package);
    SPerl_HASH_insert(parser->package_symtable, name, strlen(name), type);
  }
  
  // Core array types
  for (int32_t i = 0; i < SPerl_RESOLVED_TYPE_C_CORE_LENGTH; i++) {
    // Name
    const char* name = SPerl_RESOLVED_TYPE_C_CORE_ARRAY_NAMES[i];
    const char* core_name = SPerl_RESOLVED_TYPE_C_CORE_NAMES[i];
    
    // Name
    SPerl_OP* op_name = SPerl_OP_newOP(parser, SPerl_OP_C_CODE_NAME, "CORE", 1);
    op_name->uv.name = name;
    
    // Resolved type
    SPerl_RESOLVED_TYPE* resolved_type = SPerl_RESOLVED_TYPE_new(parser);
    SPerl_ARRAY_push(resolved_type->part_names, (void*) core_name);
    SPerl_ARRAY_push(resolved_type->part_names, (void*) "[");
    SPerl_ARRAY_push(resolved_type->part_names, (void*) "]");
    
    resolved_type->name = name;
    resolved_type->name_length = strlen(name);
    resolved_type->id = SPerl_RESOLVED_TYPE_C_CORE_LENGTH + i;
    SPerl_ARRAY_push(parser->resolved_types, resolved_type);
    SPerl_HASH_insert(parser->resolved_type_symtable, name, strlen(name), resolved_type);
  }
  
  return parser;
}

int32_t SPerl_PARSER_parse(SPerl_PARSER* parser, const char* package_name) {

  /* Build use information */
  SPerl_USE* use = SPerl_USE_new(parser);
  SPerl_OP* op_package_name = SPerl_OP_newOP(parser, SPerl_OP_C_CODE_NAME, package_name, 1);
  op_package_name->uv.name = package_name;
  use->op_package_name = op_package_name;
  
  // Use OP
  SPerl_OP* op_use = SPerl_OP_newOP(parser, SPerl_OP_C_CODE_USE, package_name, 1);
  op_use->uv.use = use;
  
  /* Push package use information stack */
  SPerl_ARRAY_push(parser->op_use_stack, op_use);
  
  // Entry point
  char* entry_point = SPerl_ALLOCATOR_new_string(parser, strlen(package_name) + 6);
  strncpy(entry_point, package_name, strlen(package_name));
  strncpy(entry_point + strlen(package_name), "::main", 6);
  parser->entry_point = entry_point;
  
  /* call SPerl_yyparse */
  SPerl_yydebug = 0;
  int32_t parse_success = SPerl_yyparse(parser);
  
  return parse_success;
}

void SPerl_PARSER_free(SPerl_PARSER* parser) {

  // Free all array pointers
  for (int32_t i = 0; i < parser->array_ptrs->length; i++) {
    SPerl_ARRAY* array = SPerl_ARRAY_fetch(parser->array_ptrs, i);
    SPerl_ARRAY_free(array);
  }
  SPerl_ARRAY_free(parser->array_ptrs);
  
  // Free all hash pointers
  for (int32_t i = 0; i < parser->hash_ptrs->length; i++) {
    SPerl_HASH* hash = SPerl_ARRAY_fetch(parser->hash_ptrs, i);
    SPerl_HASH_free(hash);
  }
  SPerl_ARRAY_free(parser->hash_ptrs);
  
  // Free all string pointers;
  for (int32_t i = 0; i < parser->long_str_ptrs->length; i++) {
    void* str = SPerl_ARRAY_fetch(parser->long_str_ptrs, i);
    free(str);
  }
  SPerl_ARRAY_free(parser->long_str_ptrs);
  
  // Free memory pool */
  SPerl_MEMORY_POOL_free(parser->memory_pool);
  
  free(parser);
}
