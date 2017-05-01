#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <assert.h>

#include "sperl.h"
#include "sperl_parser.h"
#include "sperl_array.h"
#include "sperl_hash.h"
#include "sperl_allocator_parser.h"
#include "sperl_yacc_util.h"
#include "sperl_op.h"
#include "sperl_sub.h"
#include "sperl_constant.h"
#include "sperl_field.h"
#include "sperl_my_var.h"
#include "sperl_var.h"
#include "sperl_enumeration_value.h"
#include "sperl_type.h"
#include "sperl_type_component_name.h"
#include "sperl_type_component_array.h"
#include "sperl_type_part.h"
#include "sperl_enumeration.h"
#include "sperl_package.h"
#include "sperl_name_info.h"
#include "sperl_resolved_type.h"
#include "sperl_switch_info.h"
#include "sperl_constant_pool.h"
#include "sperl_limit.h"

void SPerl_OP_CHECKER_check(SPerl* sperl) {
  
  SPerl_PARSER* parser = sperl->parser;
  
  for (int32_t package_pos = 0; package_pos < parser->op_packages->length; package_pos++) {
    SPerl_OP* op_package = SPerl_ARRAY_fetch(sperl, parser->op_packages, package_pos);
    SPerl_PACKAGE* package = op_package->uv.package;
    
    if (strchr(package->op_name->uv.name, '_') != NULL) {
      SPerl_yyerror_format(sperl, "Package name can't contain _ at %s line %d\n", op_package->file, op_package->line);
      parser->fatal_error = 1;
      return;
    }
    
    // Constant pool
    SPerl_CONSTANT_POOL* constant_pool = sperl->constant_pool;
    
    // Push package name to constant pool
    const char* package_name = package->op_name->uv.name;
    package->name_constant_pool_address = constant_pool->length;
    SPerl_CONSTANT_POOL_push_string(sperl, constant_pool, package_name);
    
    // Push package information to constant pool
    package->constant_pool_address = constant_pool->length;
    SPerl_CONSTANT_POOL_push_package(sperl, constant_pool, package);
    
    // Add constant pool sub to symbol table
    const char* constant_pool_package_name = (char*)&sperl->constant_pool->values[package->name_constant_pool_address + 1];
    SPerl_HASH_insert(sperl, sperl->constant_pool_package_symtable, constant_pool_package_name, strlen(constant_pool_package_name), (void*)(intptr_t)package->constant_pool_address);
    
    // Push field information to constant pool
    for (int32_t field_pos = 0; field_pos < package->op_fields->length; field_pos++) {
      SPerl_OP* op_field = SPerl_ARRAY_fetch(sperl, package->op_fields, field_pos);
      SPerl_FIELD* field = op_field->uv.field;
      
      // Add field name to constant pool
      field->abs_name_constant_pool_address = sperl->constant_pool->length;
      SPerl_CONSTANT_POOL_push_string(sperl, sperl->constant_pool, field->abs_name);
      
      // Add field to constant pool
      field->constant_pool_address = sperl->constant_pool->length;
      SPerl_CONSTANT_POOL_push_field(sperl, sperl->constant_pool, field);
      
      // Add constant pool field to symbol table
      const char* constant_pool_field_name = (char*)&sperl->constant_pool->values[field->abs_name_constant_pool_address + 1];
      SPerl_HASH_insert(sperl, sperl->constant_pool_field_symtable, constant_pool_field_name, strlen(constant_pool_field_name), (void*)(intptr_t)field->constant_pool_address);
    }
    
    for (int32_t sub_pos = 0; sub_pos < package->op_subs->length; sub_pos++) {
      
      SPerl_OP* op_sub = SPerl_ARRAY_fetch(sperl, package->op_subs, sub_pos);
      SPerl_SUB* sub = op_sub->uv.sub;
      
      // Only process normal subroutine
      if (!sub->is_constant && !sub->is_native) {
        
        // my var informations
        SPerl_ARRAY* op_my_vars = SPerl_ALLOCATOR_PARSER_alloc_array(sperl, parser->allocator, 0);
        
        // my variable stack
        SPerl_ARRAY* op_my_var_stack = SPerl_ALLOCATOR_PARSER_alloc_array(sperl, parser->allocator, 0);
        
        // block base position stack
        SPerl_ARRAY* block_base_stack = SPerl_ALLOCATOR_PARSER_alloc_array(sperl, parser->allocator, 0);
        int32_t block_base = 0;
        
        // In switch statement
        _Bool in_switch = 0;
        
        // Current case statements
        SPerl_ARRAY* cur_case_ops = NULL;
        
        // Current default statement
        SPerl_OP* cur_default_op = NULL;
        
        // op count
        int32_t op_count = 0;
        
        int32_t my_var_address = -1;
        
        // Run OPs
        SPerl_OP* op_base = SPerl_OP_get_op_block_from_op_sub(sperl, op_sub);
        SPerl_OP* op_cur = op_base;
        _Bool finish = 0;
        while (op_cur) {
          
          op_count++;
          
          // [START]Preorder traversal position
          
          switch (op_cur->code) {
            case SPerl_OP_C_CODE_FIELD: {
              SPerl_FIELD* field = op_cur->uv.field;
              SPerl_RESOLVED_TYPE* resolved_type = field->op_type->uv.type->resolved_type;
              if (!SPerl_RESOLVED_TYPE_is_core_value(sperl, resolved_type) && !SPerl_RESOLVED_TYPE_is_core_value_array(sperl, resolved_type)) {
                SPerl_yyerror_format(sperl, "filed type must be core type or core type array at %s line %d\n", op_cur->file, op_cur->line);
                parser->fatal_error = 1;
                return;
              }
              
              break;
            }
            case SPerl_OP_C_CODE_AND: {
              
              // Convert && to if statement
              SPerl_OP_convert_and_to_if(sperl, op_cur);
              
              break;
            }
            case SPerl_OP_C_CODE_OR: {
              
              // Convert || to if statement
              SPerl_OP_convert_or_to_if(sperl, op_cur);
              
              break;
            }
            case SPerl_OP_C_CODE_NOT: {
              // Convert ! to if statement
              SPerl_OP_convert_not_to_if(sperl, op_cur);
              
              break;
            }
            case SPerl_OP_C_CODE_SWITCH: {
              if (in_switch) {
                SPerl_yyerror_format(sperl, "duplicate switch is forbidden at %s line %d\n", op_cur->file, op_cur->line);
                parser->fatal_error = 1;
                return;
              }
              else {
                in_switch = 1;
              }
              
              break;
            }
            // Start scope
            case SPerl_OP_C_CODE_BLOCK: {
              
              // Add return to the end of subroutine
              if (op_cur->flag & SPerl_OP_C_FLAG_BLOCK_SUB) {
                SPerl_OP* op_statements = op_cur->first;
                
                if (op_statements->last->code != SPerl_OP_C_CODE_RETURN_PROCESS) {
                  
                  SPerl_OP* op_return = SPerl_OP_new_op(sperl, SPerl_OP_C_CODE_RETURN, op_cur->file, op_cur->line);
                  if (sub->op_return_type->code != SPerl_OP_C_CODE_VOID) {
                    SPerl_RESOLVED_TYPE* op_return_resolved_type = SPerl_OP_get_resolved_type(sperl, sub->op_return_type);
                    if (op_return_resolved_type) {
                      if (SPerl_RESOLVED_TYPE_is_core_value(sperl, op_return_resolved_type)) {
                        SPerl_OP* op_constant;
                        if (op_return_resolved_type->id <= SPerl_RESOLVED_TYPE_C_ID_INT) {
                          op_constant = SPerl_OP_new_op_constant_int(sperl, 0, op_cur->file, op_cur->line);
                        }
                        else if (op_return_resolved_type->id == SPerl_RESOLVED_TYPE_C_ID_LONG) {
                          op_constant = SPerl_OP_new_op_constant_long(sperl, 0, op_cur->file, op_cur->line);
                        }
                        else if (op_return_resolved_type->id == SPerl_RESOLVED_TYPE_C_ID_FLOAT) {
                          op_constant = SPerl_OP_new_op_constant_float(sperl, 0, op_cur->file, op_cur->line);
                        }
                        else if (op_return_resolved_type->id == SPerl_RESOLVED_TYPE_C_ID_DOUBLE) {
                          op_constant = SPerl_OP_new_op_constant_double(sperl, 0, op_cur->file, op_cur->line);
                        }
                        
                        SPerl_OP_sibling_splice(sperl, op_return, NULL, 0, op_constant);
                      }
                      // Reference
                      else {
                        // Undef
                        SPerl_OP* op_undef = SPerl_OP_new_op(sperl, SPerl_OP_C_CODE_UNDEF, op_cur->file, op_cur->line);
                        SPerl_OP_sibling_splice(sperl, op_return, NULL, 0, op_undef);
                      }
                    }
                  }
                  
                  SPerl_OP* op_return_process = SPerl_OP_new_op(sperl, SPerl_OP_C_CODE_RETURN_PROCESS, op_return->file, op_return->line);
                  SPerl_OP* op_before_return = SPerl_OP_new_op(sperl, SPerl_OP_C_CODE_BEFORE_RETURN, op_return->file, op_return->line);
                  
                  SPerl_OP_sibling_splice(sperl, op_return_process, op_return_process->last, 0, op_before_return);
                  SPerl_OP_sibling_splice(sperl, op_return_process, op_return_process->last, 0, op_return);
                  
                  SPerl_OP_sibling_splice(sperl, op_statements, op_statements->last, 0, op_return_process);
                }
              }
              
              block_base = op_my_var_stack->length;
              int32_t* block_base_ptr = SPerl_ALLOCATOR_PARSER_alloc_int(sperl, parser->allocator);
              *block_base_ptr = block_base;
              SPerl_ARRAY_push(sperl, block_base_stack, block_base_ptr);
              
              break;
            }
            case SPerl_OP_C_CODE_ASSIGN: {
              op_cur->first->lvalue = 1;
              break;
            }
          }
          
          // [END]Preorder traversal position
          if (op_cur->first) {
            op_cur = op_cur->first;
          }
          else {
            while (1) {
              // [START]Postorder traversal position
              switch (op_cur->code) {
                case SPerl_OP_C_CODE_CONSTANT: {
                  SPerl_CONSTANT* constant = op_cur->uv.constant;
                  
                  SPerl_CONSTANT_POOL* constant_pool = sperl->constant_pool;
                  
                  constant->constant_pool_address = constant_pool->length;
                  
                  switch (constant->code) {
                    case SPerl_CONSTANT_C_CODE_INT: {
                      int32_t value = constant->uv.int_value;
                      if (value >= -32768 && value <= 32767) {
                        constant->constant_pool_address = -1;
                        break;
                      }
                      
                      SPerl_CONSTANT_POOL_push_int(sperl, constant_pool, value);
                      break;
                    }
                    case SPerl_CONSTANT_C_CODE_LONG: {
                      int64_t value = constant->uv.long_value;
                      
                      if (value >= -32768 && value <= 32767) {
                        constant->constant_pool_address = -1;
                        break;
                      }
                      
                      SPerl_CONSTANT_POOL_push_long(sperl, constant_pool, value);
                      break;
                    }
                    case SPerl_CONSTANT_C_CODE_FLOAT: {
                      float value = constant->uv.float_value;
                      
                      if (value == 0 || value == 1 || value == 2) {
                        constant->constant_pool_address = -1;
                        break;
                      }
                      
                      SPerl_CONSTANT_POOL_push_float(sperl, constant_pool, value);
                      break;
                    }
                    case SPerl_CONSTANT_C_CODE_DOUBLE: {
                      double value = constant->uv.double_value;
                      
                      if (value == 0 || value == 1) {
                        constant->constant_pool_address = -1;
                        break;
                      }
                      
                      SPerl_CONSTANT_POOL_push_double(sperl, constant_pool, value);
                      break;
                    }
                    case SPerl_CONSTANT_C_CODE_STRING: {
                      const char* value = constant->uv.string_value;
                      
                      SPerl_CONSTANT_POOL_push_string(sperl, constant_pool, value);
                      break;
                    }
                  }
                  
                  break;
                }
                case SPerl_OP_C_CODE_POP: {
                  if (op_cur->first->code == SPerl_OP_C_CODE_CALL_SUB) {
                    SPerl_OP* op_call_sub = op_cur->first;
                    
                    const char* sub_name = op_call_sub->uv.name_info->resolved_name;
                    
                    SPerl_OP* op_sub= SPerl_HASH_search(sperl, 
                      parser->op_sub_symtable,
                      sub_name,
                      strlen(sub_name)
                    );
                    SPerl_SUB* sub = op_sub->uv.sub;
                    
                    if (sub->op_return_type->code == SPerl_OP_C_CODE_VOID) {
                      op_cur->code = SPerl_OP_C_CODE_NULL;
                    }
                  }
                  break;
                }
                case SPerl_OP_C_CODE_DEFAULT: {
                  if (cur_default_op) {
                    SPerl_yyerror_format(sperl, "multiple default is forbidden at %s line %d\n", op_cur->file, op_cur->line);
                    parser->fatal_error = 1;
                    break;
                  }
                  else {
                    cur_default_op = op_cur;
                  }
                  break;
                }
                case SPerl_OP_C_CODE_CASE: {
                  
                  SPerl_OP* op_term = op_cur->first;
                  
                  if (op_term->code != SPerl_OP_C_CODE_CONSTANT) {
                    SPerl_yyerror_format(sperl, "case need constant at %s line %d\n", op_cur->file, op_cur->line);
                    break;
                  }
                  
                  SPerl_RESOLVED_TYPE* op_term_resolved_type = SPerl_OP_get_resolved_type(sperl, op_term);
                  if (op_term_resolved_type->id != SPerl_RESOLVED_TYPE_C_ID_LONG) {
                    SPerl_yyerror_format(sperl, "case need long at %s line %d\n", op_cur->file, op_cur->line);
                    break;
                  }
                  
                  if (!cur_case_ops) {
                    cur_case_ops = SPerl_ALLOCATOR_PARSER_alloc_array(sperl, parser->allocator, 0);
                  }
                  SPerl_ARRAY_push(sperl, cur_case_ops, op_cur);
                  
                  break;
                }
                case SPerl_OP_C_CODE_SWITCH: {
                  
                  SPerl_OP* op_switch_condition = op_cur->first;
                  
                  SPerl_RESOLVED_TYPE* term_resolved_type = SPerl_OP_get_resolved_type(sperl, op_switch_condition->first);
                  
                  if (term_resolved_type->id != SPerl_RESOLVED_TYPE_C_ID_LONG) {
                    SPerl_yyerror_format(sperl, "switch need long at %s line %d\n", op_cur->file, op_cur->line);
                    break;
                  }
                  
                  in_switch = 0;
                  cur_default_op = NULL;
                  cur_case_ops = NULL;
                  
                  // tableswitch if the following. SWITCHRTIO is 1.5 by default
                  // 4 + range <= (3 + 2 * length) * SWITCHRTIO
                  
                  SPerl_SWITCH_INFO* switch_info = op_cur->uv.switch_info;
                  SPerl_ARRAY* op_cases = switch_info->op_cases;
                  int32_t const length = op_cases->length;
                  if (length > SPerl_OP_LIMIT_CASES) {
                    SPerl_yyerror_format(sperl, "too many case statements in switch at %s line %d\n", op_cur->file, op_cur->line);
                    break;
                  }

                  int64_t min = INT64_MIN;
                  int64_t max = INT64_MAX;
                  
                  for (int32_t i = 0; i < length; i++) {
                    SPerl_OP* op_case = SPerl_ARRAY_fetch(sperl, op_cases, i);
                    SPerl_OP* op_constant = op_case->first;
                    int64_t value = op_constant->uv.constant->uv.long_value;
                    if (value < min) {
                      min = value;
                    }
                    if (value > max) {
                      max = value;
                    }
                  }
                  
                  double range = (double) max - (double) min;
                  
                  int32_t code;
                  if (4.0 + range <= (3.0 + 2.0 * (double) length) * 1.5) {
                    code = SPerl_SWITCH_INFO_C_CODE_TABLESWITCH;
                  }
                  else {
                    code = SPerl_SWITCH_INFO_C_CODE_LOOKUPSWITCH;
                  }
                  
                  switch_info->code = code;
                  switch_info->min = min;
                  switch_info->max = max;
                  
                  break;
                }
                
                case SPerl_OP_C_CODE_EQ: {
                  
                  SPerl_RESOLVED_TYPE* first_resolved_type = SPerl_OP_get_resolved_type(sperl, op_cur->first);
                  SPerl_RESOLVED_TYPE* last_resolved_type = SPerl_OP_get_resolved_type(sperl, op_cur->last);
                  
                  // TERM == TERM
                  if (first_resolved_type && last_resolved_type) {
                    // core == core
                    if (SPerl_RESOLVED_TYPE_is_core_value(sperl, first_resolved_type) && SPerl_RESOLVED_TYPE_is_core_value(sperl, last_resolved_type)) {
                      if (first_resolved_type->id != last_resolved_type->id) {
                        SPerl_yyerror_format(sperl, "== operator two operands must be same type at %s line %d\n", op_cur->file, op_cur->line);
                        break;
                      }
                    }
                    // core == OBJ
                    else if (SPerl_RESOLVED_TYPE_is_core_value(sperl, first_resolved_type)) {
                      SPerl_yyerror_format(sperl, "== left value must be object at %s line %d\n", op_cur->file, op_cur->line);
                      break;
                    }
                    // OBJ == core
                    else if (SPerl_RESOLVED_TYPE_is_core_value(sperl, last_resolved_type)) {
                      SPerl_yyerror_format(sperl, "== right value must be object at %s line %d\n", op_cur->file, op_cur->line);
                      break;
                    }
                  }
                  // undef == TERM
                  else if (!first_resolved_type) {
                    if (SPerl_RESOLVED_TYPE_is_core_value(sperl, last_resolved_type)) {
                      SPerl_yyerror_format(sperl, "== right value must be object at %s line %d\n", op_cur->file, op_cur->line);
                      break;
                    }
                  }
                  // TERM == undef
                  else if (!last_resolved_type) {
                    if (SPerl_RESOLVED_TYPE_is_core_value(sperl, first_resolved_type)) {
                      SPerl_yyerror_format(sperl, "== left value must be object at %s line %d\n", op_cur->file, op_cur->line);
                      break;
                    }
                  }
                  
                  break;
                }
                case SPerl_OP_C_CODE_NE: {

                  SPerl_RESOLVED_TYPE* first_resolved_type = SPerl_OP_get_resolved_type(sperl, op_cur->first);
                  SPerl_RESOLVED_TYPE* last_resolved_type = SPerl_OP_get_resolved_type(sperl, op_cur->last);

                  // TERM == TERM
                  if (first_resolved_type && last_resolved_type) {
                    // core == core
                    if (SPerl_RESOLVED_TYPE_is_core_value(sperl, first_resolved_type) && SPerl_RESOLVED_TYPE_is_core_value(sperl, last_resolved_type)) {
                      if (first_resolved_type->id != last_resolved_type->id) {
                        SPerl_yyerror_format(sperl, "!= operator two operands must be same type at %s line %d\n", op_cur->file, op_cur->line);
                        break;
                      }
                    }
                    // core == OBJ
                    else if (SPerl_RESOLVED_TYPE_is_core_value(sperl, first_resolved_type)) {
                      SPerl_yyerror_format(sperl, "!= left value must be object at %s line %d\n", op_cur->file, op_cur->line);
                      break;
                    }
                    // OBJ == core
                    else if (SPerl_RESOLVED_TYPE_is_core_value(sperl, last_resolved_type)) {
                      SPerl_yyerror_format(sperl, "!= right value must be object at %s line %d\n", op_cur->file, op_cur->line);
                      break;
                    }
                  }
                  // undef == TERM
                  else if (!first_resolved_type) {
                    if (SPerl_RESOLVED_TYPE_is_core_value(sperl, last_resolved_type)) {
                      SPerl_yyerror_format(sperl, "!= right value must be object at %s line %d\n", op_cur->file, op_cur->line);
                      break;
                    }
                  }
                  // TERM == undef
                  else if (!last_resolved_type) {
                    if (SPerl_RESOLVED_TYPE_is_core_value(sperl, first_resolved_type)) {
                      SPerl_yyerror_format(sperl, "!= left value must be object at %s line %d\n", op_cur->file, op_cur->line);
                      break;
                    }
                  }
                  
                  break;
                }
                case SPerl_OP_C_CODE_LT: {

                  SPerl_RESOLVED_TYPE* first_resolved_type = SPerl_OP_get_resolved_type(sperl, op_cur->first);
                  SPerl_RESOLVED_TYPE* last_resolved_type = SPerl_OP_get_resolved_type(sperl, op_cur->last);
                  
                  // undef check
                  if (!first_resolved_type) {
                    SPerl_yyerror_format(sperl, "< left value must be not undef at %s line %d\n", op_cur->file, op_cur->line);
                    break;
                  }
                  if (!last_resolved_type) {
                    SPerl_yyerror_format(sperl, "< right value must be not undef at %s line %d\n", op_cur->file, op_cur->line);
                    break;
                  }
                  
                  // Can receive only core type
                  if (!SPerl_RESOLVED_TYPE_is_core_value(sperl, first_resolved_type)) {
                    SPerl_yyerror_format(sperl, "< left value must be core type at %s line %d\n", op_cur->file, op_cur->line);
                    break;
                  }
                  if (!SPerl_RESOLVED_TYPE_is_core_value(sperl, last_resolved_type)) {
                    SPerl_yyerror_format(sperl, "< right value must be core type at %s line %d\n", op_cur->file, op_cur->line);
                    break;
                  }

                  if (first_resolved_type->id != last_resolved_type->id) {
                    SPerl_yyerror_format(sperl, "< operator two operands must be same type at %s line %d\n", op_cur->file, op_cur->line);
                    break;
                  }

                  break;
                }
                case SPerl_OP_C_CODE_LE: {

                  SPerl_RESOLVED_TYPE* first_resolved_type = SPerl_OP_get_resolved_type(sperl, op_cur->first);
                  SPerl_RESOLVED_TYPE* last_resolved_type = SPerl_OP_get_resolved_type(sperl, op_cur->last);

                  // undef check
                  if (!first_resolved_type) {
                    SPerl_yyerror_format(sperl, "<= left value must be not undef at %s line %d\n", op_cur->file, op_cur->line);
                    break;
                  }
                  if (!last_resolved_type) {
                    SPerl_yyerror_format(sperl, "<= right value must be not undef at %s line %d\n", op_cur->file, op_cur->line);
                    break;
                  }
                                  
                  // Can receive only core type
                  if (!SPerl_RESOLVED_TYPE_is_core_value(sperl, first_resolved_type)) {
                    SPerl_yyerror_format(sperl, "<= left value must be core type at %s line %d\n", op_cur->file, op_cur->line);
                    break;
                  }
                  if (!SPerl_RESOLVED_TYPE_is_core_value(sperl, last_resolved_type)) {
                    SPerl_yyerror_format(sperl, "<= right value must be core type at %s line %d\n", op_cur->file, op_cur->line);
                    break;
                  }

                  if (first_resolved_type->id != last_resolved_type->id) {
                    SPerl_yyerror_format(sperl, "<= operator two operands must be same type at %s line %d\n", op_cur->file, op_cur->line);
                    break;
                  }
                  
                  break;
                }
                case SPerl_OP_C_CODE_GT: {

                  SPerl_RESOLVED_TYPE* first_resolved_type = SPerl_OP_get_resolved_type(sperl, op_cur->first);
                  SPerl_RESOLVED_TYPE* last_resolved_type = SPerl_OP_get_resolved_type(sperl, op_cur->last);

                  // undef check
                  if (!first_resolved_type) {
                    SPerl_yyerror_format(sperl, "> left value must be not undef at %s line %d\n", op_cur->file, op_cur->line);
                    break;
                  }
                  if (!last_resolved_type) {
                    SPerl_yyerror_format(sperl, "> right value must be not undef at %s line %d\n", op_cur->file, op_cur->line);
                    break;
                  }
                  
                  // Can receive only core type
                  if (!SPerl_RESOLVED_TYPE_is_core_value(sperl, first_resolved_type)) {
                    SPerl_yyerror_format(sperl, "> left value must be core type at %s line %d\n", op_cur->file, op_cur->line);
                    break;
                  }
                  if (!SPerl_RESOLVED_TYPE_is_core_value(sperl, last_resolved_type)) {
                    SPerl_yyerror_format(sperl, "> right value must be core type at %s line %d\n", op_cur->file, op_cur->line);
                    break;
                  }

                  if (first_resolved_type->id != last_resolved_type->id) {
                    SPerl_yyerror_format(sperl, "> operator two operands must be same type at %s line %d\n", op_cur->file, op_cur->line);
                    break;
                  }
                  
                  break;
                }
                case SPerl_OP_C_CODE_GE: {

                  SPerl_RESOLVED_TYPE* first_resolved_type = SPerl_OP_get_resolved_type(sperl, op_cur->first);
                  SPerl_RESOLVED_TYPE* last_resolved_type = SPerl_OP_get_resolved_type(sperl, op_cur->last);

                  // undef check
                  if (!first_resolved_type) {
                    SPerl_yyerror_format(sperl, "<= left value must be not undef at %s line %d\n", op_cur->file, op_cur->line);
                    break;
                  }
                  if (!last_resolved_type) {
                    SPerl_yyerror_format(sperl, "<= right value must be not undef at %s line %d\n", op_cur->file, op_cur->line);
                    break;
                  }
                  
                  // Can receive only core type
                  if (SPerl_RESOLVED_TYPE_is_core_value(sperl, first_resolved_type) && !SPerl_RESOLVED_TYPE_is_core_value(sperl, last_resolved_type)) {
                    SPerl_yyerror_format(sperl, ">= left value must be core type at %s line %d\n", op_cur->file, op_cur->line);
                    break;
                  }
                  if (!SPerl_RESOLVED_TYPE_is_core_value(sperl, first_resolved_type) && SPerl_RESOLVED_TYPE_is_core_value(sperl, last_resolved_type)) {
                    SPerl_yyerror_format(sperl, ">= right value must be core type at %s line %d\n", op_cur->file, op_cur->line);
                    break;
                  }

                  if (first_resolved_type->id != last_resolved_type->id) {
                    SPerl_yyerror_format(sperl, ">= operator two operands must be same type at %s line %d\n", op_cur->file, op_cur->line);
                    break;
                  }
                  
                  break;
                }
                case SPerl_OP_C_CODE_LEFT_SHIFT: {
                  SPerl_RESOLVED_TYPE* first_resolved_type = SPerl_OP_get_resolved_type(sperl, op_cur->first);
                  SPerl_RESOLVED_TYPE* last_resolved_type = SPerl_OP_get_resolved_type(sperl, op_cur->last);
                  
                  // Can receive only core type
                  if (!SPerl_RESOLVED_TYPE_is_integral(sperl, first_resolved_type)) {
                    SPerl_yyerror_format(sperl, "<< operator left value must be integral at %s line %d\n", op_cur->file, op_cur->line);
                    break;
                  }
                  if (last_resolved_type->id != SPerl_RESOLVED_TYPE_C_ID_LONG) {
                    SPerl_yyerror_format(sperl, "<< operator right value must be long at %s line %d\n", op_cur->file, op_cur->line);
                    break;
                  }
                  
                  break;
                }
                case SPerl_OP_C_CODE_RIGHT_SHIFT: {
                  SPerl_RESOLVED_TYPE* first_resolved_type = SPerl_OP_get_resolved_type(sperl, op_cur->first);
                  SPerl_RESOLVED_TYPE* last_resolved_type = SPerl_OP_get_resolved_type(sperl, op_cur->last);
                  
                  // Can receive only core type
                  if (!SPerl_RESOLVED_TYPE_is_integral(sperl, first_resolved_type)) {
                    SPerl_yyerror_format(sperl, ">> operator left value must be integral at %s line %d\n", op_cur->file, op_cur->line);
                    break;
                  }
                  if (last_resolved_type->id != SPerl_RESOLVED_TYPE_C_ID_LONG) {
                    SPerl_yyerror_format(sperl, ">> operator right value must be long at %s line %d\n", op_cur->file, op_cur->line);
                    break;
                  }
                  
                  break;
                }
                case SPerl_OP_C_CODE_RIGHT_SHIFT_UNSIGNED: {
                  SPerl_RESOLVED_TYPE* first_resolved_type = SPerl_OP_get_resolved_type(sperl, op_cur->first);
                  SPerl_RESOLVED_TYPE* last_resolved_type = SPerl_OP_get_resolved_type(sperl, op_cur->last);
                  
                  // Can receive only core type
                  if (!SPerl_RESOLVED_TYPE_is_integral(sperl, first_resolved_type)) {
                    SPerl_yyerror_format(sperl, ">>> operator left value must be integral at %s line %d\n", op_cur->file, op_cur->line);
                    break;
                  }
                  if (last_resolved_type->id > SPerl_RESOLVED_TYPE_C_ID_LONG) {
                    SPerl_yyerror_format(sperl, ">>> operator right value must be long at %s line %d\n", op_cur->file, op_cur->line);
                    break;
                  }
                  
                  break;
                }
                case SPerl_OP_C_CODE_MALLOC: {
                  SPerl_OP* op_type = op_cur->first;
                  SPerl_RESOLVED_TYPE* resolved_type = op_type->uv.type->resolved_type;
                  
                  if (SPerl_RESOLVED_TYPE_is_core_value_array(sperl, resolved_type)) {
                    SPerl_OP* op_index_term = op_type->last;
                    SPerl_RESOLVED_TYPE* index_resolved_type = SPerl_OP_get_resolved_type(sperl, op_index_term);
                    
                    if (!index_resolved_type) {
                      SPerl_yyerror_format(sperl, "new operator can't create array which don't have length \"%s\" at %s line %d\n", resolved_type->name, op_cur->file, op_cur->line);
                      break;
                    }
                    else if (index_resolved_type->id == SPerl_RESOLVED_TYPE_C_ID_LONG) {
                      // OK
                    }
                    else {
                      SPerl_yyerror_format(sperl, "new operator can't create array which don't have long length \"%s\" at %s line %d\n", resolved_type->name, op_cur->file, op_cur->line);
                      break;
                    }
                  }
                  else if (SPerl_RESOLVED_TYPE_is_array(sperl, resolved_type)) {
                    SPerl_yyerror_format(sperl,
                      "new operator can't receive object array at %s line %d\n", op_cur->file, op_cur->line);
                    break;
                  }
                  else {
                    if (SPerl_RESOLVED_TYPE_contain_sub(sperl, resolved_type)) {
                      SPerl_yyerror_format(sperl,
                        "new operator can't receive sub type at %s line %d\n", op_cur->file, op_cur->line);
                      break;
                    }
                    
                    if (SPerl_RESOLVED_TYPE_is_core_value(sperl, resolved_type)) {
                      SPerl_yyerror_format(sperl,
                        "new operator can't receive core type at %s line %d\n", op_cur->file, op_cur->line);
                      break;
                    }
                    
                    SPerl_OP* op_package = SPerl_HASH_search(sperl, parser->op_package_symtable, resolved_type->name, strlen(resolved_type->name));
                    
                    if (!op_package) {
                      SPerl_yyerror_format(sperl, "new operator can't receive non package name \"%s\" at %s line %d\n", resolved_type->name, op_cur->file, op_cur->line);
                      break;
                    }
                    else {
                      SPerl_PACKAGE* package = op_package->uv.package;
                      if (!package->op_fields->length) {
                        SPerl_yyerror_format(sperl, "new operator can't receive package which don't have fields \"%s\" at %s line %d\n", resolved_type->name, op_cur->file, op_cur->line);
                        break;
                      }
                    }
                  }
                  
                  break;
                }
                case SPerl_OP_C_CODE_BIT_XOR: {
                  SPerl_RESOLVED_TYPE* first_resolved_type = SPerl_OP_get_resolved_type(sperl, op_cur->first);
                  SPerl_RESOLVED_TYPE* last_resolved_type = SPerl_OP_get_resolved_type(sperl, op_cur->last);
                  
                  // Can receive only core type
                  if (first_resolved_type->id >= SPerl_RESOLVED_TYPE_C_ID_FLOAT || last_resolved_type->id >= SPerl_RESOLVED_TYPE_C_ID_FLOAT) {
                    SPerl_yyerror_format(sperl,
                      "& operator can receive only integral type at %s line %d\n", op_cur->file, op_cur->line);
                    break;
                  }
                  
                  break;
                }
                case SPerl_OP_C_CODE_BIT_OR: {
                  SPerl_RESOLVED_TYPE* first_resolved_type = SPerl_OP_get_resolved_type(sperl, op_cur->first);
                  SPerl_RESOLVED_TYPE* last_resolved_type = SPerl_OP_get_resolved_type(sperl, op_cur->last);
                  
                  // Can receive only core type
                  if (first_resolved_type->id >= SPerl_RESOLVED_TYPE_C_ID_FLOAT || last_resolved_type->id >= SPerl_RESOLVED_TYPE_C_ID_FLOAT) {
                    SPerl_yyerror_format(sperl,
                      "& operator can receive only integral type at %s line %d\n", op_cur->file, op_cur->line);
                    break;
                  }
                  
                  break;
                }
                case SPerl_OP_C_CODE_BIT_AND: {
                  SPerl_RESOLVED_TYPE* first_resolved_type = SPerl_OP_get_resolved_type(sperl, op_cur->first);
                  SPerl_RESOLVED_TYPE* last_resolved_type = SPerl_OP_get_resolved_type(sperl, op_cur->last);
                  
                  // Can receive only core type
                  if (first_resolved_type->id >= SPerl_RESOLVED_TYPE_C_ID_FLOAT || last_resolved_type->id >= SPerl_RESOLVED_TYPE_C_ID_FLOAT) {
                    SPerl_yyerror_format(sperl,
                      "& operator can receive only integral type at %s line %d\n", op_cur->file, op_cur->line);
                    break;
                  }
                  
                  break;
                }
                case SPerl_OP_C_CODE_ARRAY_LENGTH: {
                  SPerl_RESOLVED_TYPE* first_resolved_type = SPerl_OP_get_resolved_type(sperl, op_cur->first);
                  
                  // First value must be array
                  _Bool first_resolved_type_is_array = SPerl_RESOLVED_TYPE_is_array(sperl, first_resolved_type);
                  if (!first_resolved_type_is_array) {
                    SPerl_yyerror_format(sperl, "right of @ must be array at %s line %d\n", op_cur->file, op_cur->line);
                    break;
                  }
                  
                  break;
                }
                case SPerl_OP_C_CODE_ARRAY_ELEM: {
                  SPerl_RESOLVED_TYPE* first_resolved_type = SPerl_OP_get_resolved_type(sperl, op_cur->first);
                  SPerl_RESOLVED_TYPE* last_resolved_type = SPerl_OP_get_resolved_type(sperl, op_cur->last);
                  
                  // First value must be array
                  _Bool first_resolved_type_is_array = SPerl_RESOLVED_TYPE_is_array(sperl, first_resolved_type);
                  if (!first_resolved_type_is_array) {
                    SPerl_yyerror_format(sperl, "left value must be array at %s line %d\n", op_cur->file, op_cur->line);
                    break;
                  }
                  
                  // Last value must be integer
                  if (last_resolved_type->id != SPerl_RESOLVED_TYPE_C_ID_LONG) {
                    SPerl_yyerror_format(sperl, "array index must be long at %s line %d\n", op_cur->file, op_cur->line);
                    break;
                  }
                  
                  break;
                }
                case SPerl_OP_C_CODE_ASSIGN: {
                  
                  SPerl_RESOLVED_TYPE* first_resolved_type = SPerl_OP_get_resolved_type(sperl, op_cur->first);
                  SPerl_RESOLVED_TYPE* last_resolved_type = SPerl_OP_get_resolved_type(sperl, op_cur->last);
                  
                  // Type assumption
                  if (!first_resolved_type) {
                    SPerl_OP* op_var = op_cur->first;
                    SPerl_MY_VAR* my_var = op_var->uv.var->op_my_var->uv.my_var;
                    first_resolved_type = SPerl_OP_get_resolved_type(sperl, my_var->op_term_assumption);
                    
                    if (first_resolved_type) {
                      SPerl_OP* op_type = SPerl_OP_new_op(sperl, SPerl_OP_C_CODE_TYPE, op_cur->file, op_cur->line);
                      SPerl_TYPE* type = SPerl_TYPE_new(sperl);
                      type->resolved_type = first_resolved_type;
                      op_type->uv.type = type;
                      my_var->op_type = op_type;
                    }
                    else {
                      SPerl_yyerror_format(sperl, "Type can't be detected at %s line %d\n", op_cur->first->file, op_cur->first->line);
                      parser->fatal_error = 1;
                      return;
                    }
                  }
                  
                  // It is OK that left type is object and right is undef
                  if (!SPerl_RESOLVED_TYPE_is_core_value(sperl, first_resolved_type) && !last_resolved_type) {
                    // OK
                  }
                  // Invalid type
                  else if (first_resolved_type->id != last_resolved_type->id) {
                    SPerl_yyerror_format(sperl, "Invalid type at %s line %d\n", op_cur->file, op_cur->line);
                    parser->fatal_error = 1;
                    return;
                  }
                  
                  // Insert var op
                  if (op_cur->last->code == SPerl_OP_C_CODE_ASSIGN) {
                    SPerl_OP* op_var = SPerl_OP_new_op(sperl, SPerl_OP_C_CODE_VAR, op_cur->file, op_cur->line);
                    op_var->uv.var = op_cur->last->first->uv.var;
                    
                    SPerl_OP* op_last_old = op_cur->last;
                    
                    op_last_old->sibparent = op_var;
                    
                    op_var->first = op_last_old;
                    op_var->sibparent = op_cur;
                    
                    op_cur->last = op_var;
                    
                    op_cur->first->sibparent = op_var;
                  }
                  
                  break;
                }
                case SPerl_OP_C_CODE_RETURN: {
                  
                  SPerl_OP* op_term = op_cur->first;
                  
                  if (op_term) {
                    SPerl_RESOLVED_TYPE* first_resolved_type = SPerl_OP_get_resolved_type(sperl, op_term);
                    SPerl_RESOLVED_TYPE* sub_return_resolved_type = SPerl_OP_get_resolved_type(sperl, sub->op_return_type);
                    
                    _Bool is_invalid = 0;
                    
                    // Undef
                    if (op_term->code == SPerl_OP_C_CODE_UNDEF) {
                      if (sub->op_return_type->code == SPerl_OP_C_CODE_VOID) {
                        is_invalid = 1;
                      }
                      else {
                        if (SPerl_RESOLVED_TYPE_is_core_value(sperl, sub_return_resolved_type)) {
                          is_invalid = 1;
                        }
                      }
                    }
                    // Normal
                    else if (op_term) {
                      if (first_resolved_type->id != sub_return_resolved_type->id) {
                        is_invalid = 1;
                      }
                    }
                    // Empty
                    else {
                      if (sub->op_return_type->code != SPerl_OP_C_CODE_VOID) {
                        is_invalid = 1;
                      }
                    }
                    
                    if (is_invalid) {
                      SPerl_yyerror_format(sperl, "Invalid return type at %s line %d\n", op_cur->file, op_cur->line);
                      break;
                    }
                  }
                  
                  // Add before return process
                  SPerl_OP* op_return_process = op_cur->sibparent;
                  SPerl_OP* op_before_return = op_return_process->first;
                  
                  for (int32_t j = op_my_var_stack->length - 1; j >= 0; j--) {
                    SPerl_OP* op_my_var = SPerl_ARRAY_fetch(sperl, op_my_var_stack, j);
                    
                    SPerl_RESOLVED_TYPE* resolved_type = SPerl_OP_get_resolved_type(sperl, op_my_var);
                    
                    // Decrement reference count at before return
                    if (!SPerl_RESOLVED_TYPE_is_core_value(sperl, resolved_type)) {
                      // If return target is variable, don't decrement reference count
                      _Bool do_dec_ref_count = 0;
                      if (op_term) {
                        if (op_term->code == SPerl_OP_C_CODE_VAR) {
                          if (op_term->uv.var->op_my_var->uv.my_var->address != op_my_var->uv.my_var->address) {
                            do_dec_ref_count = 1;
                          }
                        }
                        else {
                          do_dec_ref_count = 1;
                        }
                      }
                      else {
                        do_dec_ref_count = 1;
                      }
                      
                      if (do_dec_ref_count) {
                        SPerl_OP* op_decrefcount = SPerl_OP_new_op(sperl, SPerl_OP_C_CODE_DECREFCOUNT, op_cur->file, op_cur->line);
                        SPerl_OP* op_var = SPerl_OP_new_op_var_from_op_my_var(sperl, op_my_var);
                        SPerl_OP_sibling_splice(sperl, op_decrefcount, NULL, 0, op_var);
                        SPerl_OP_sibling_splice(sperl, op_before_return, NULL, 0, op_decrefcount);
                      }
                    }
                    assert(op_my_var);
                  }
                  
                  break;
                }
                case SPerl_OP_C_CODE_NEGATE: {
                  SPerl_RESOLVED_TYPE* first_resolved_type = SPerl_OP_get_resolved_type(sperl, op_cur->first);
                  
                  // Must be int, long, float, double
                  if (!SPerl_RESOLVED_TYPE_is_core_value(sperl, first_resolved_type)) {
                    SPerl_yyerror_format(sperl, "Type of - operator right value must be int, long, float, double at %s line %d\n", op_cur->file, op_cur->line);
                    break;
                  }
                  
                  break;
                }
                case SPerl_OP_C_CODE_PLUS: {
                  SPerl_RESOLVED_TYPE* first_resolved_type = SPerl_OP_get_resolved_type(sperl, op_cur->first);
                  
                  // Must be int, long, float, double
                  if (!SPerl_RESOLVED_TYPE_is_core_value(sperl, first_resolved_type)) {
                    SPerl_yyerror_format(sperl, "Type of + operator right value must be int, long, float, double at %s line %d\n", op_cur->file, op_cur->line);
                    break;
                  }
                  
                  break;
                }
                case SPerl_OP_C_CODE_ADD: {
                  SPerl_RESOLVED_TYPE* first_resolved_type = SPerl_OP_get_resolved_type(sperl, op_cur->first);
                  SPerl_RESOLVED_TYPE* last_resolved_type = SPerl_OP_get_resolved_type(sperl, op_cur->last);
                  
                  // Left value must not be undef
                  if (!first_resolved_type) {
                    SPerl_yyerror_format(sperl, "+ operator left value must be not undef at %s line %d\n", op_cur->file, op_cur->line);
                    break;
                  }
                  
                  // Right value Must not be undef
                  if (!last_resolved_type) {
                    SPerl_yyerror_format(sperl, "+ operator right value must be not undef at %s line %d\n", op_cur->file, op_cur->line);
                    break;
                  }
                  
                  // Must be same type
                  if (first_resolved_type->id != last_resolved_type->id) {
                    SPerl_yyerror_format(sperl, "Type of + operator left and right value must be same at %s line %d\n", op_cur->file, op_cur->line);
                    break;
                  }
                                                  
                  // Value must be int, long, float, double
                  if (!SPerl_RESOLVED_TYPE_is_core_value(sperl, first_resolved_type)) {
                    SPerl_yyerror_format(sperl, "Type of + operator left and right value must be core type at %s line %d\n", op_cur->file, op_cur->line);
                    break;
                  }
                  
                  break;
                }
                case SPerl_OP_C_CODE_SUBTRACT: {
                  SPerl_RESOLVED_TYPE* first_resolved_type = SPerl_OP_get_resolved_type(sperl, op_cur->first);
                  SPerl_RESOLVED_TYPE* last_resolved_type = SPerl_OP_get_resolved_type(sperl, op_cur->last);
                  
                  // Left value must not be undef
                  if (!first_resolved_type) {
                    SPerl_yyerror_format(sperl, "- operator left value must be not undef at %s line %d\n", op_cur->file, op_cur->line);
                    break;
                  }
                  
                  // Right value Must not be undef
                  if (!last_resolved_type) {
                    SPerl_yyerror_format(sperl, "- operator right value must be not undef at %s line %d\n", op_cur->file, op_cur->line);
                    break;
                  }
                  
                  // Must be same type
                  if (first_resolved_type->id != last_resolved_type->id) {
                    SPerl_yyerror_format(sperl, "Type of - operator left and right value must be same at %s line %d\n", op_cur->file, op_cur->line);
                    break;
                  }
                                                  
                  // Value must be int, long, float, double
                  if (!SPerl_RESOLVED_TYPE_is_core_value(sperl, first_resolved_type)) {
                    SPerl_yyerror_format(sperl, "Type of - operator left and right value must be core type at %s line %d\n", op_cur->file, op_cur->line);
                    break;
                  }
                  
                  break;
                }
                case SPerl_OP_C_CODE_MULTIPLY: {
                  SPerl_RESOLVED_TYPE* first_resolved_type = SPerl_OP_get_resolved_type(sperl, op_cur->first);
                  SPerl_RESOLVED_TYPE* last_resolved_type = SPerl_OP_get_resolved_type(sperl, op_cur->last);
                  
                  // Left value must not be undef
                  if (!first_resolved_type) {
                    SPerl_yyerror_format(sperl, "* operator left value must be not undef at %s line %d\n", op_cur->file, op_cur->line);
                    break;
                  }
                  
                  // Right value Must not be undef
                  if (!last_resolved_type) {
                    SPerl_yyerror_format(sperl, "* operator right value must be not undef at %s line %d\n", op_cur->file, op_cur->line);
                    break;
                  }
                  
                  // Must be same type
                  if (first_resolved_type->id != last_resolved_type->id) {
                    SPerl_yyerror_format(sperl, "Type of * operator left and right value must be same at %s line %d\n", op_cur->file, op_cur->line);
                    break;
                  }
                                                  
                  // Value must be int, long, float, double
                  if (!SPerl_RESOLVED_TYPE_is_core_value(sperl, first_resolved_type)) {
                    SPerl_yyerror_format(sperl, "Type of * operator left and right value must be core type at %s line %d\n", op_cur->file, op_cur->line);
                    break;
                  }
                  
                  break;
                }
                case SPerl_OP_C_CODE_DIVIDE: {
                  SPerl_RESOLVED_TYPE* first_resolved_type = SPerl_OP_get_resolved_type(sperl, op_cur->first);
                  SPerl_RESOLVED_TYPE* last_resolved_type = SPerl_OP_get_resolved_type(sperl, op_cur->last);
                  
                  // Left value must not be undef
                  if (!first_resolved_type) {
                    SPerl_yyerror_format(sperl, "/ operator left value must be not undef at %s line %d\n", op_cur->file, op_cur->line);
                    break;
                  }
                  
                  // Right value Must not be undef
                  if (!last_resolved_type) {
                    SPerl_yyerror_format(sperl, "/ operator right value must be not undef at %s line %d\n", op_cur->file, op_cur->line);
                    break;
                  }
                  
                  // Must be same type
                  if (first_resolved_type->id != last_resolved_type->id) {
                    SPerl_yyerror_format(sperl, "Type of / operator left and right value must be same at %s line %d\n", op_cur->file, op_cur->line);
                    break;
                  }
                                                  
                  // Value must be int, long, float, double
                  if (!SPerl_RESOLVED_TYPE_is_core_value(sperl, first_resolved_type)) {
                    SPerl_yyerror_format(sperl, "Type of / operator left and right value must be core type at %s line %d\n", op_cur->file, op_cur->line);
                    break;
                  }
                  
                  break;
                }
                case SPerl_OP_C_CODE_REMAINDER: {
                  SPerl_RESOLVED_TYPE* first_resolved_type = SPerl_OP_get_resolved_type(sperl, op_cur->first);
                  SPerl_RESOLVED_TYPE* last_resolved_type = SPerl_OP_get_resolved_type(sperl, op_cur->last);
                  
                  // Left value must not be undef
                  if (!first_resolved_type) {
                    SPerl_yyerror_format(sperl, "% operator left value must be not undef at %s line %d\n", op_cur->file, op_cur->line);
                    break;
                  }
                  
                  // Right value Must not be undef
                  if (!last_resolved_type) {
                    SPerl_yyerror_format(sperl, "% operator right value must be not undef at %s line %d\n", op_cur->file, op_cur->line);
                    break;
                  }
                  
                  // Must be same type
                  if (first_resolved_type->id != last_resolved_type->id) {
                    SPerl_yyerror_format(sperl, "Type of % operator left and right value must be same at %s line %d\n", op_cur->file, op_cur->line);
                    break;
                  }
                                                  
                  // Value must be int, long, float, double
                  if (!SPerl_RESOLVED_TYPE_is_core_value(sperl, first_resolved_type)) {
                    SPerl_yyerror_format(sperl, "Type of % operator left and right value must be core type at %s line %d\n", op_cur->file, op_cur->line);
                    break;
                  }
                  
                  break;
                }
                case SPerl_OP_C_CODE_DIE: {
                  SPerl_RESOLVED_TYPE* first_resolved_type = SPerl_OP_get_resolved_type(sperl, op_cur->first);
                  
                  if (!first_resolved_type || strcmp(first_resolved_type->name, "byte[]") != 0) {
                    SPerl_yyerror_format(sperl, "die can only receive byte array at %s line %d\n", op_cur->file, op_cur->line);
                    break;
                  }
                  break;
                }
                case SPerl_OP_C_CODE_PRE_INC:
                case SPerl_OP_C_CODE_POST_INC:
                case SPerl_OP_C_CODE_PRE_DEC:
                case SPerl_OP_C_CODE_POST_DEC: {
                  SPerl_OP* op_first = op_cur->first;
                  if (op_first->code != SPerl_OP_C_CODE_VAR) {
                    SPerl_yyerror_format(sperl, "invalid lvalue in increment at %s line %d\n", op_cur->file, op_cur->line);
                    break;
                  }
                  SPerl_RESOLVED_TYPE* first_resolved_type = SPerl_OP_get_resolved_type(sperl, op_first);
                  
                  // Only int or long
                  if (first_resolved_type->id > SPerl_RESOLVED_TYPE_C_ID_LONG) {
                    SPerl_yyerror_format(sperl, "Type of increment or decrement target must be integral at %s line %d\n", op_cur->file, op_cur->line);
                    break;
                  }
                  
                  op_cur->first->lvalue = 1;
                  
                  break;
                }
                // End of scope
                case SPerl_OP_C_CODE_BLOCK: {
                  
                  SPerl_OP* op_list_statement = op_cur->first;
                  
                  if (block_base_stack->length > 0) {
                    int32_t* block_base_ptr = SPerl_ARRAY_pop(sperl, block_base_stack);
                    block_base = *block_base_ptr;
                  }
                  
                  // Free my variables at end of block
                  SPerl_OP* op_block_end = SPerl_OP_new_op(sperl, SPerl_OP_C_CODE_BLOCK_END, op_cur->file, op_cur->line);
                  
                  int32_t pop_count = op_my_var_stack->length - block_base;
                  for (int32_t j = 0; j < pop_count; j++) {
                    SPerl_OP* op_my_var = SPerl_ARRAY_pop(sperl, op_my_var_stack);
                    
                    SPerl_RESOLVED_TYPE* resolved_type = SPerl_OP_get_resolved_type(sperl, op_my_var);
                    
                    // Decrement reference count at end of scope
                    if (!SPerl_RESOLVED_TYPE_is_core_value(sperl, resolved_type)) {
                      SPerl_OP* op_decrefcount = SPerl_OP_new_op(sperl, SPerl_OP_C_CODE_DECREFCOUNT, op_cur->file, op_cur->line);
                      SPerl_OP* op_var = SPerl_OP_new_op_var_from_op_my_var(sperl, op_my_var);
                      SPerl_OP_sibling_splice(sperl, op_decrefcount, NULL, 0, op_var);
                      SPerl_OP_sibling_splice(sperl, op_block_end, NULL, 0, op_decrefcount);
                    }
                    
                    assert(op_my_var);
                  }
                  
                  if (!(op_cur->flag & SPerl_OP_C_FLAG_BLOCK_SUB)) {
                    SPerl_OP_sibling_splice(sperl, op_list_statement, op_list_statement->last, 0, op_block_end);
                  }
                  
                  if (block_base_stack->length > 0) {
                    int32_t* before_block_base_ptr = SPerl_ARRAY_fetch(sperl, block_base_stack, block_base_stack->length - 1);
                    int32_t before_block_base = *before_block_base_ptr;
                    block_base = before_block_base;
                  }
                  else {
                    block_base = 0;
                  }
                  
                  break;
                }
                // Add my var
                case SPerl_OP_C_CODE_VAR: {
                  
                  SPerl_VAR* var = op_cur->uv.var;
                  
                  // Search same name variable
                  SPerl_OP* op_my_var = NULL;
                  for (int32_t i = op_my_var_stack->length; i-- > 0; ) {
                    SPerl_OP* op_my_var_tmp = SPerl_ARRAY_fetch(sperl, op_my_var_stack, i);
                    SPerl_MY_VAR* my_var_tmp = op_my_var_tmp->uv.my_var;
                    if (strcmp(var->op_name->uv.name, my_var_tmp->op_name->uv.name) == 0) {
                      op_my_var = op_my_var_tmp;
                      break;
                    }
                  }
                  
                  if (op_my_var) {
                    // Add my var information to var
                    var->op_my_var = op_my_var;
                  }
                  else {
                    // Error
                    SPerl_yyerror_format(sperl, "%s is undeclared in this scope at %s line %d\n", var->op_name->uv.name, op_cur->file, op_cur->line);
                    parser->fatal_error = 1;
                    return;
                  }
                  break;
                }
                case SPerl_OP_C_CODE_MY_VAR_PARENT: {
                  
                  SPerl_OP* op_my_var = op_cur->first;
                  SPerl_MY_VAR* my_var = op_my_var->uv.my_var;
                  
                  // If argument my var is object, increment reference count
                  if (my_var->address < sub->op_args->length) {
                    SPerl_RESOLVED_TYPE* resolved_type = SPerl_OP_get_resolved_type(sperl, op_my_var);
                    if (!SPerl_RESOLVED_TYPE_is_core_value(sperl, resolved_type)) {
                      SPerl_OP* op_var = SPerl_OP_new_op_var_from_op_my_var(sperl, op_my_var);
                      
                      SPerl_OP* op_increfcount = SPerl_OP_new_op(sperl, SPerl_OP_C_CODE_INCREFCOUNT, op_my_var->file, op_my_var->line);
                      SPerl_OP_sibling_splice(sperl, op_increfcount, NULL, 0, op_var);
                      SPerl_OP_sibling_splice(sperl, op_cur, op_cur->last, 0, op_increfcount);
                    }
                  }
                  
                  break;
                }
                case SPerl_OP_C_CODE_MY_VAR: {
                  SPerl_MY_VAR* my_var = op_cur->uv.my_var;
                  
                  if (my_var_address == SPerl_LIMIT_C_MY_VARS) {
                    SPerl_yyerror_format(sperl, "too many lexical variables, my \"%s\" ignored at %s line %d\n", my_var->op_name->uv.name, op_cur->file, op_cur->line);
                    parser->fatal_error = 1;
                    break;
                  }
                  
                  // Search same name variable
                  _Bool found = 0;
                  
                  for (int32_t i = op_my_var_stack->length; i-- > block_base; ) {
                    SPerl_OP* op_bef_my_var = SPerl_ARRAY_fetch(sperl, op_my_var_stack, i);
                    SPerl_MY_VAR* bef_my_var = op_bef_my_var->uv.my_var;
                    if (strcmp(my_var->op_name->uv.name, bef_my_var->op_name->uv.name) == 0) {
                      found = 1;
                      break;
                    }
                  }
                  
                  if (found) {
                    SPerl_yyerror_format(sperl, "redeclaration of my \"%s\" at %s line %d\n", my_var->op_name->uv.name, op_cur->file, op_cur->line);
                    break;
                  }
                  else {
                    my_var->address = ++my_var_address;
                    SPerl_ARRAY_push(sperl, op_my_vars, op_cur);
                    SPerl_ARRAY_push(sperl, op_my_var_stack, op_cur);
                  }
                  
                  // If left is object type and right is not exists, append "= undef" code
                  SPerl_RESOLVED_TYPE* first_resolved_type = SPerl_OP_get_resolved_type(sperl, op_cur);
                  
                  // Assign undef if left value is object and right value is nothing
                  if (first_resolved_type && !SPerl_RESOLVED_TYPE_is_core_value(sperl, first_resolved_type) && !SPerl_OP_sibling(sperl, op_cur)) {
                    // Only my declarations after subroutine arguments
                    if (my_var->address >= sub->op_args->length) {
                      SPerl_OP* op_assign = SPerl_OP_new_op(sperl, SPerl_OP_C_CODE_ASSIGN, op_cur->file, op_cur->line);
                      
                      SPerl_VAR* var = SPerl_VAR_new(sperl);
                      SPerl_OP* op_name_var = SPerl_OP_new_op(sperl, SPerl_OP_C_CODE_NAME, op_cur->file, op_cur->line);
                      op_name_var->uv.name = op_cur->uv.my_var->op_name->uv.name;
                      var->op_name = op_name_var;
                      var->op_my_var = op_cur;
                      
                      SPerl_OP* op_var = SPerl_OP_new_op(sperl, SPerl_OP_C_CODE_VAR, op_cur->file, op_cur->line);
                      op_var->uv.var = var;
                      
                      SPerl_OP* op_undef = SPerl_OP_new_op(sperl, SPerl_OP_C_CODE_UNDEF, op_cur->file, op_cur->line);
                      
                      SPerl_OP_sibling_splice(sperl, op_assign, op_assign->last, 0, op_var);
                      SPerl_OP_sibling_splice(sperl, op_assign, op_assign->last, 0, op_undef);
                      
                      SPerl_OP_sibling_splice(sperl, op_cur->sibparent, op_cur->sibparent->last, 0, op_assign);
                    }
                  }
                  
                  break;
                }
                case SPerl_OP_C_CODE_CALL_SUB: {
                  
                  // Check sub name
                  SPerl_OP_resolve_sub_name(sperl, op_package, op_cur);
                  
                  const char* sub_abs_name = op_cur->uv.name_info->resolved_name;
                  
                  SPerl_OP* found_op_sub= SPerl_HASH_search(sperl, 
                    parser->op_sub_symtable,
                    sub_abs_name,
                    strlen(sub_abs_name)
                  );
                  if (!found_op_sub) {
                    SPerl_yyerror_format(sperl, "unknown sub \"%s\" at %s line %d\n",
                      sub_abs_name, op_cur->file, op_cur->line);
                    break;
                  }
                  
                  // Constant
                  SPerl_SUB* found_sub = found_op_sub->uv.sub;

                  int32_t sub_args_count = found_sub->op_args->length;
                  SPerl_OP* op_list_args = op_cur->last;
                  SPerl_OP* op_term = op_list_args->first;
                  int32_t call_sub_args_count = 0;
                  while ((op_term = SPerl_OP_sibling(sperl, op_term))) {
                    call_sub_args_count++;
                    if (call_sub_args_count > sub_args_count) {
                      SPerl_yyerror_format(sperl, "Too may arguments. sub \"%s\" at %s line %d\n", sub_abs_name, op_cur->file, op_cur->line);
                      return;
                    }
                    
                    _Bool is_invalid = 0;
                    
                    SPerl_OP* op_sub_arg_my_var = SPerl_ARRAY_fetch(sperl, found_sub->op_args, call_sub_args_count - 1);
                    
                    SPerl_RESOLVED_TYPE* sub_arg_resolved_type = SPerl_OP_get_resolved_type(sperl, op_sub_arg_my_var);
                    
                    // Undef
                    if (op_term->code == SPerl_OP_C_CODE_UNDEF) {
                      if (SPerl_RESOLVED_TYPE_is_core_value(sperl, sub_arg_resolved_type)) {
                        is_invalid = 1;
                      }
                    }
                    // Normal
                    else if (op_term) {
                      SPerl_RESOLVED_TYPE* op_term_resolved_type = SPerl_OP_get_resolved_type(sperl, op_term);
                      
                      if (op_term_resolved_type->id !=  sub_arg_resolved_type->id) {
                        is_invalid = 1;
                      }
                    }
                    if (is_invalid) {
                      SPerl_yyerror_format(sperl, "Argument %d type is invalid. sub \"%s\" at %s line %d\n", (int) call_sub_args_count, sub_abs_name, op_cur->file, op_cur->line);
                      return;
                    }
                  }
                  
                  if (call_sub_args_count < sub_args_count) {
                    SPerl_yyerror_format(sperl, "Too few argument. sub \"%s\" at %s line %d\n", sub_abs_name, op_cur->file, op_cur->line);
                    return;
                  }
                  
                  // Constant subroutine
                  if (found_sub->is_constant) {
                    // Replace sub to constant
                    op_cur->code = SPerl_OP_C_CODE_CONSTANT;
                    op_cur->uv.constant = found_sub->op_block->uv.constant;
                    
                    op_cur->first = NULL;
                    op_cur->last = NULL;
                  }
                  
                  break;
                }
                case SPerl_OP_C_CODE_CALL_FIELD: {
                  SPerl_OP* op_term = op_cur->first;
                  SPerl_OP* op_name = op_cur->last;
                  
                  if (op_term->code != SPerl_OP_C_CODE_VAR
                    && op_term->code != SPerl_OP_C_CODE_ARRAY_ELEM
                    && op_term->code != SPerl_OP_C_CODE_CALL_FIELD
                    && op_term->code != SPerl_OP_C_CODE_CALL_SUB)
                  {
                    SPerl_yyerror_format(sperl, "field invoker is invalid \"%s\" at %s line %d\n",
                      op_name->uv.name, op_cur->file, op_cur->line);
                    parser->fatal_error = 1;
                    break;
                  }
                  
                  // Check field name
                  SPerl_OP_resolve_field_name(sperl, op_cur);
                  
                  const char* field_abs_name = op_cur->uv.name_info->resolved_name;
                  
                  SPerl_OP* found_op_field= SPerl_HASH_search(sperl, 
                    parser->op_field_symtable,
                    field_abs_name,
                    strlen(field_abs_name)
                  );
                  if (!found_op_field) {
                    SPerl_yyerror_format(sperl, "unknown field \"%s\" at %s line %d\n",
                      field_abs_name, op_cur->file, op_cur->line);
                    parser->fatal_error = 1;
                    break;
                  }
                  
                  break;
                }
                case SPerl_OP_C_CODE_CONVERT: {
                  
                  SPerl_OP* op_term = op_cur->first;
                  SPerl_OP* op_type = op_cur->last;
                  
                  SPerl_RESOLVED_TYPE* op_term_resolved_type = SPerl_OP_get_resolved_type(sperl, op_term);
                  SPerl_RESOLVED_TYPE* op_type_resolved_type = SPerl_OP_get_resolved_type(sperl, op_type);;
                  
                  // Can receive only core type
                  if (!SPerl_RESOLVED_TYPE_is_core_value(sperl, op_term_resolved_type) || !SPerl_RESOLVED_TYPE_is_core_value(sperl, op_type_resolved_type)) {
                    SPerl_yyerror_format(sperl, "can't convert type %s to %s at %s line %d\n",
                      op_term_resolved_type->name, op_type_resolved_type->name, op_cur->file, op_cur->line);
                    break;
                  }
                }
                break;
              }
              
              // [END]Postorder traversal position
              
              if (op_cur == op_base) {
                
                SPerl_OP* op_statements = op_cur->first;
                
                // Finish
                finish = 1;
                
                break;
              }
              
              // Next sibling
              if (op_cur->moresib) {
                op_cur = SPerl_OP_sibling(sperl, op_cur);
                break;
              }
              // Next is parent
              else {
                op_cur = op_cur->sibparent;
              }
            }
            if (finish) {
              break;
            }
          }
        }
        // Set my var information
        sub->op_my_vars = op_my_vars;
        
        // Operand stack max
        sub->operand_stack_max = op_count * 2;
      }
      
      // Push sub name to constant pool
      sub->abs_name_constant_pool_address = sperl->constant_pool->length;
      SPerl_CONSTANT_POOL_push_string(sperl, sperl->constant_pool, sub->abs_name);
      
      // Push sub information to constant pool
      sub->constant_pool_address = sperl->constant_pool->length;
      SPerl_CONSTANT_POOL_push_sub(sperl, sperl->constant_pool, sub);
      
      // Add constant pool sub to symbol table
      const char* constant_pool_sub_name = (char*)&sperl->constant_pool->values[sub->abs_name_constant_pool_address + 1];
      SPerl_HASH_insert(sperl, sperl->constant_pool_sub_symtable, constant_pool_sub_name, strlen(constant_pool_sub_name), (void*)(intptr_t)sub->constant_pool_address);
    }
  }
}
