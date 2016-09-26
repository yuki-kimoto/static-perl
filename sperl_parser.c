#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>

#include "sperl_type.h"
#include "sperl_parser.h"
#include "sperl_method_info.h"
#include "sperl_array.h"
#include "sperl_argument_info.h"
#include "sperl_class_info.h"


SPerl_yy_parser* SPerl_new_parser() {
  SPerl_yy_parser* parser = (SPerl_yy_parser*)malloc(sizeof(SPerl_yy_parser));
  memset(parser, 0, sizeof(SPerl_yy_parser));
  
  parser->current_method_infos = SPerl_ARRAY_new(0);
  parser->current_const_ops = SPerl_ARRAY_new(0);
  parser->class_infos = SPerl_ARRAY_new(0);
  parser->line = 1;
  parser->current_const_pool_size = 0;
  
  return parser;
}

void SPerl_PARSER_dump_class_infos(SPerl_yy_parser* parser) {
  SPerl_int i;
  
  if (!parser) {
    return;
  }
  
  SPerl_ARRAY* class_infos = parser->class_infos;
  for (i = 0; i < class_infos->length; i++) {
    SPerl_CLASS_INFO* class_info = (SPerl_CLASS_INFO*)SPerl_ARRAY_fetch(class_infos, i);
    
    printf("class_info[" PRId64 "]\n", i);
    printf("  name => \"%s\"\n", class_info->name);
    printf("  op_block => %x\n", class_info->op_block);
    
    // Dump method informations
    SPerl_ARRAY* method_infos = class_info->method_infos;
    SPerl_int j;
    printf("  method_infos\n");
    for (j = 0; j < method_infos->length; j++) {
      SPerl_METHOD_INFO* method_info = SPerl_ARRAY_fetch(method_infos, j);
      printf("    method_info[%" PRId32 "]\n", j);
      SPerl_PARSER_dump_method_info(method_info);
    }
    
    printf("  const_ops\n");
    SPerl_ARRAY* const_ops = class_info->const_ops;
    for (j = 0; j < const_ops->length; j++) {
      SPerl_OP* const_op = SPerl_ARRAY_fetch(const_ops, j);
      printf("    const_op[%" PRId32 "]\n", j);
      SPerl_PARSER_dump_const_op(const_op);
    }

    printf("  const_pool\n");
    for (j = 0; j < const_ops->length; j++) {
      SPerl_OP* const_op = SPerl_ARRAY_fetch(const_ops, j);
      SPerl_PARSER_dump_const_pool(class_info->const_pool, const_op);
    }
    printf("  const_pool_size %" PRId32 "\n", class_info->const_pool_size);
    printf("  const_pool_next_pos %" PRId32 "\n", class_info->const_pool_next_pos);
  }
}

void SPerl_PARSER_dump_const_pool(SPerl_char* const_pool, SPerl_OP* const_op) {
  SPerl_int type = const_op->type;
  printf("    const_pool[%" PRId32 "]\n", const_op->const_pos);
  printf("      %s", SPerl_op_name[type]);
  if (type == SPerl_OP_CONST) {
    switch(const_op->private) {
      case SPerl_OPp_CONST_BOOLEAN:
        printf("      boolean %" PRId32 "\n", *(SPerl_int*)(const_pool + const_op->const_pos));
        break;
      case SPerl_OPp_CONST_CHAR:
        printf("      char %c\n", *(SPerl_int*)(const_pool + const_op->const_pos));
        break;
      case SPerl_OPp_CONST_BYTE:
        printf("      byte %" PRId32 "\n", *(SPerl_int*)(const_pool + const_op->const_pos));
        break;
      case SPerl_OPp_CONST_SHORT:
        printf("      short %" PRId32 "\n", *(SPerl_int*)(const_pool + const_op->const_pos));
        break;
      case SPerl_OPp_CONST_INT:
        printf("      int %" PRId32 "\n", *(SPerl_int*)(const_pool + const_op->const_pos));
        break;
      case SPerl_OPp_CONST_LONG:
        printf("      long %" PRId64 "\n", *(SPerl_long*)(const_pool + const_op->const_pos));
        break;
      case SPerl_OPp_CONST_FLOAT:
        printf("      float %f\n", *(SPerl_float*)(const_pool + const_op->const_pos));
        break;
      case SPerl_OPp_CONST_DOUBLE:
        printf("      double %f\n", *(SPerl_double*)(const_pool + const_op->const_pos));
        break;
      case SPerl_OPp_CONST_STRING:
        printf("      string \"%s\"\n", const_pool + const_op->const_pos);
        break;
    }
  }
}

void SPerl_PARSER_dump_const_op(SPerl_OP* const_op) {
  SPerl_int type = const_op->type;
  printf("      %s", SPerl_op_name[type]);
  if (type == SPerl_OP_CONST) {
    switch(const_op->private) {
      case SPerl_OPp_CONST_BOOLEAN:
        printf("      boolean %" PRId32 "\n", const_op->uv.boolean_value);
        break;
      case SPerl_OPp_CONST_CHAR:
        printf("      char %c\n", const_op->uv.char_value);
        break;
      case SPerl_OPp_CONST_BYTE:
        printf("      byte %" PRId32 "\n", const_op->uv.byte_value);
        break;
      case SPerl_OPp_CONST_SHORT:
        printf("      short %" PRId32 "\n", const_op->uv.short_value);
        break;
      case SPerl_OPp_CONST_INT:
        printf("      int %" PRId32 "\n", const_op->uv.int_value);
        break;
      case SPerl_OPp_CONST_LONG:
        printf("      long %" PRId64 "\n", const_op->uv.long_value);
        break;
      case SPerl_OPp_CONST_FLOAT:
        printf("      float %f\n", const_op->uv.float_value);
        break;
      case SPerl_OPp_CONST_DOUBLE:
        printf("      double %f\n", const_op->uv.double_value);
        break;
      case SPerl_OPp_CONST_STRING:
        printf("      string \"%s\"\n", const_op->uv.string_value);
        break;
    }
  }
}

void SPerl_PARSER_dump_method_info(SPerl_METHOD_INFO* method_info) {
  if (method_info) {
    SPerl_char* desc_str = (SPerl_char*)malloc(sizeof(SPerl_char) * 100);
    SPerl_DESCRIPTER_to_str(desc_str, method_info->desc_flags);
    
    printf("      name => \"%s\"\n", method_info->name);
    printf("      argument_count => %" PRId32 "\n", method_info->argument_count);
    
    SPerl_int j;
    printf("      argument_infos\n");
    for (j = 0; j < method_info->argument_count; j++) {
      SPerl_ARGUMENT_INFO* argument_info
        = (SPerl_ARGUMENT_INFO*)SPerl_ARRAY_fetch(method_info->argument_infos, j);
      printf("        argument_info[%" PRId32 "]\n", j);
      printf("        type => \"%s\"\n", argument_info->type);
      
      SPerl_char* desc_str = (SPerl_char*)malloc(sizeof(SPerl_char) * 100);
      SPerl_DESCRIPTER_to_str(desc_str, argument_info->desc_flags);
      printf("        desc_flags => \"%s\"\n", desc_str);
      
      free(desc_str);
    }
    
    printf("      desc_flags => \"%s\"\n", desc_str);
    printf("      treturn_type => \"%s\"\n", method_info->return_type);
    printf("      op_block => %x\n", method_info->op_block);
    
    free(desc_str);
  }
  else {
    printf("      None\n");
  }
}
