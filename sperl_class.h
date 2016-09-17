#ifndef SPERL_CLASS_H
#define SPERL_CLASS_H

#include <stdint.h>

#define SPerl_CLASS_MAX 1024

// Field information
struct SPerl_field_info;
typedef struct SPerl_field_info SPerl_FIELD_INFO;
struct SPerl_field_info {
  uint8_t* name;
  int16_t desc_flag;
  uint8_t* type;
};

// Argument information
struct SPerl_argument_info;
typedef struct SPerl_argument_info SPerl_ARGUMENT_INFO;
struct SPerl_argument_info {
  int16_t desc_flag;
  uint8_t* type;
};

// Local variable
struct SPerl_local_variable;
typedef struct SPerl_local_variable SPerl_LOCAL_VARIABLE;
struct SPerl_local_variable {
  uint8_t* name;
  int32_t offset;
};

// Method information
struct SPerl_method_info;
typedef struct SPerl_method_info SPerl_METHOD_INFO;
struct SPerl_METHOD_INFO {
  uint8_t* name;
  int16_t desc_flag;
  uint8_t* return_type;
  SPerl_ARGUMENT_INFO* argument_infos;
  int32_t argument_count;
};

// Constant pool tag
enum SPerl_constant_tag {
  SPerl_COSNTANT_BOOLEAN,
  SPerl_CONSTANT_CHAR,
  SPerl_CONSTANT_BYTE,
  SPerl_CONSTANT_SHORT,
  SPerl_CONSTANT_INT,
  SPerl_CONSTANT_LONG,
  SPerl_CONSTANT_FLOAT,
  SPerl_CONSTANT_STRING
};

// Class information
struct SPerl_class_info;
typedef struct SPerl_class_info SPerl_CLASS_INFO;
struct SPerl_class_info {
  uint8_t* name;
  int16_t desc_flag;
  uint8_t* super_class_name;
  SPerl_FIELD_INFO* field_infos;
  int32_t field_count;
  SPerl_METHOD_INFO* method_infos;
  int32_t method_count;
};

extern SPerl_CLASS_INFO* class_infos;

#endif
