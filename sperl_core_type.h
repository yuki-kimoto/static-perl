#ifndef SPERL_TYPE_H
#define SPERL_TYPE_H

#include <stdint.h>

typedef _Bool SPerl_boolean;
typedef uint8_t SPerl_char;
typedef int8_t SPerl_byte;
typedef int16_t SPerl_short;
typedef int32_t SPerl_int;
typedef int64_t SPerl_long;
typedef float SPerl_float;
typedef double SPerl_double;

/* sperl_type_info_info.h */
enum SPerl_type_info_type;
typedef enum SPerl_type_info_type SPerl_TYPE_INFO_TYPE;

struct SPerl_type_info;
typedef struct SPerl_type_info SPerl_TYPE_INFO;

/* sperl_subtype_info_info.h */
struct SPerl_subtype_info;
typedef struct SPerl_subtype_info SPerl_SUBTYPE_INFO;

/* sperl_enum_value_info.h */
struct SPerl_enum_value_info;
typedef struct SPerl_enum_value_info SPerl_ENUM_VALUE_INFO;

/* sperl_use_info.h */
struct SPerl_use_info;
typedef struct SPerl_use_info SPerl_USE_INFO;

/* sperl_memory_pool_page.h */
struct SPerl_memory_pool_page;
typedef struct SPerl_memory_pool_page SPerl_MEMORY_POOL_PAGE;

/* sperl_memory_pool.h */
struct SPerl_memory_pool;
typedef struct SPerl_memory_pool SPerl_MEMORY_POOL;

/* sperl_var_info.h */
struct SPerl_var_info;
typedef struct SPerl_var_info SPerl_VAR_INFO;

/* sperl_my_var_info.h */
struct SPerl_my_var_info;
typedef struct SPerl_my_var_info SPerl_MY_VAR_INFO;

/* sperl_const_value.h */
struct SPerl_const_value;
typedef struct SPerl_const_value SPerl_CONST_VALUE;

/* sperl_op.h */
struct SPerl_op;
typedef struct SPerl_op SPerl_OP;

enum SPerl_OP_type;
typedef enum SPerl_OP_type SPerl_OP_TYPE;

/* sperl_word_info.h */
struct SPerl_word_info;
typedef struct SPerl_word_info SPerl_WORD_INFO;

/* sperl_field_info.h */
struct SPerl_field_info;
typedef struct SPerl_field_info SPerl_FIELD_INFO;

/* sperl_descripter_info.h */
enum SPerl_descripter_info_type;
typedef enum SPerl_descripter_info_type SPerl_DESCRIPTER_INFO_TYPE;
struct SPerl_descripter_info;
typedef struct SPerl_descripter_info SPerl_DESCRIPTER_INFO;

/* sperl_method_info.h */
struct SPerl_method_info;
typedef struct SPerl_method_info SPerl_METHOD_INFO;

/* sperl_class_info.h */
struct SPerl_class_info;
typedef struct SPerl_class_info SPerl_CLASS_INFO;

enum SPerl_class_info_type;
typedef enum SPerl_class_info_type SPerl_CLASS_INFO_TYPE;

/* sperl_array.h */
struct SPerl_array;
typedef struct SPerl_array SPerl_ARRAY;

/* sperl_hash.h */
struct SPerl_hash_entry;
typedef struct SPerl_hash_entry SPerl_HASH_ENTRY;

struct SPerl_hash;
typedef struct SPerl_hash SPerl_HASH;

/* sperl_parser.h */
struct SPerl_yy_parser_;
typedef struct SPerl_yy_parser_ SPerl_yy_parser;
typedef SPerl_yy_parser SPerl_PARSER;

union SPerl_yystype;
typedef union SPerl_yystype SPerl_YYSTYPE;

enum SPerl_op_expect;
typedef enum SPerl_op_expect SPerl_OP_EXPECT;

#endif