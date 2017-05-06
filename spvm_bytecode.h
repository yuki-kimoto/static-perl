#ifndef SPVM_BYTECODE_H
#define SPVM_BYTECODE_H

#include "spvm_base.h"



// SPVM bytecode is same as Java bytecode
enum {
  SPVM_BYTECODE_C_CODE_NOP,
  SPVM_BYTECODE_C_CODE_UNDEF,
  SPVM_BYTECODE_C_CODE_CONSTANT_BYTE_0,
  SPVM_BYTECODE_C_CODE_CONSTANT_BYTE_1,
  SPVM_BYTECODE_C_CODE_CONSTANT_SHORT_0,
  SPVM_BYTECODE_C_CODE_CONSTANT_SHORT_1,
  SPVM_BYTECODE_C_CODE_CONSTANT_INT_M1,
  SPVM_BYTECODE_C_CODE_CONSTANT_INT_0,
  SPVM_BYTECODE_C_CODE_CONSTANT_INT_1,
  SPVM_BYTECODE_C_CODE_CONSTANT_INT_2,
  SPVM_BYTECODE_C_CODE_CONSTANT_INT_3,
  SPVM_BYTECODE_C_CODE_CONSTANT_INT_4,
  SPVM_BYTECODE_C_CODE_CONSTANT_INT_5,
  SPVM_BYTECODE_C_CODE_CONSTANT_LONG_M1,
  SPVM_BYTECODE_C_CODE_CONSTANT_LONG_0,
  SPVM_BYTECODE_C_CODE_CONSTANT_LONG_1,
  SPVM_BYTECODE_C_CODE_CONSTANT_LONG_2,
  SPVM_BYTECODE_C_CODE_CONSTANT_LONG_3,
  SPVM_BYTECODE_C_CODE_CONSTANT_LONG_4,
  SPVM_BYTECODE_C_CODE_CONSTANT_LONG_5,
  SPVM_BYTECODE_C_CODE_CONSTANT_FLOAT_0,
  SPVM_BYTECODE_C_CODE_CONSTANT_FLOAT_1,
  SPVM_BYTECODE_C_CODE_CONSTANT_FLOAT_2,
  SPVM_BYTECODE_C_CODE_CONSTANT_DOUBLE_0,
  SPVM_BYTECODE_C_CODE_CONSTANT_DOUBLE_1,
  SPVM_BYTECODE_C_CODE_PUSH_BYTE,
  SPVM_BYTECODE_C_CODE_PUSH_BYTE_TO_SHORT,
  SPVM_BYTECODE_C_CODE_PUSH_SHORT,
  SPVM_BYTECODE_C_CODE_PUSH_BYTE_TO_INT,
  SPVM_BYTECODE_C_CODE_PUSH_SHORT_TO_INT,
  SPVM_BYTECODE_C_CODE_PUSH_BYTE_TO_LONG,
  SPVM_BYTECODE_C_CODE_PUSH_SHORT_TO_LONG,
  SPVM_BYTECODE_C_CODE_LOAD_CONSTANT,
  SPVM_BYTECODE_C_CODE_LOAD_CONSTANT_W,
  SPVM_BYTECODE_C_CODE_ARRAY_LOAD_INT,
  SPVM_BYTECODE_C_CODE_ARRAY_LOAD_LONG,
  SPVM_BYTECODE_C_CODE_ARRAY_LOAD_FLOAT,
  SPVM_BYTECODE_C_CODE_ARRAY_LOAD_DOUBLE,
  SPVM_BYTECODE_C_CODE_ARRAY_LOAD_BYTE,
  SPVM_BYTECODE_C_CODE_ARRAY_LOAD_SHORT,
  SPVM_BYTECODE_C_CODE_ARRAY_STORE_BYTE,
  SPVM_BYTECODE_C_CODE_ARRAY_STORE_SHORT,
  SPVM_BYTECODE_C_CODE_ARRAY_STORE_INT,
  SPVM_BYTECODE_C_CODE_ARRAY_STORE_LONG,
  SPVM_BYTECODE_C_CODE_ARRAY_STORE_FLOAT,
  SPVM_BYTECODE_C_CODE_ARRAY_STORE_DOUBLE,
  SPVM_BYTECODE_C_CODE_POP,
  SPVM_BYTECODE_C_CODE_APOP,
  SPVM_BYTECODE_C_CODE_ADD_BYTE,
  SPVM_BYTECODE_C_CODE_ADD_SHORT,
  SPVM_BYTECODE_C_CODE_ADD_INT,
  SPVM_BYTECODE_C_CODE_ADD_LONG,
  SPVM_BYTECODE_C_CODE_ADD_FLOAT,
  SPVM_BYTECODE_C_CODE_ADD_DOUBLE,
  SPVM_BYTECODE_C_CODE_SUBTRACT_BYTE,
  SPVM_BYTECODE_C_CODE_SUBTRACT_SHORT,
  SPVM_BYTECODE_C_CODE_SUBTRACT_INT,
  SPVM_BYTECODE_C_CODE_SUBTRACT_LONG,
  SPVM_BYTECODE_C_CODE_SUBTRACT_FLOAT,
  SPVM_BYTECODE_C_CODE_SUBTRACT_DOUBLE,
  SPVM_BYTECODE_C_CODE_MULTIPLY_BYTE,
  SPVM_BYTECODE_C_CODE_MULTIPLY_SHORT,
  SPVM_BYTECODE_C_CODE_MULTIPLY_INT,
  SPVM_BYTECODE_C_CODE_MULTIPLY_LONG,
  SPVM_BYTECODE_C_CODE_MULTIPLY_FLOAT,
  SPVM_BYTECODE_C_CODE_MULTIPLY_DOUBLE,
  SPVM_BYTECODE_C_CODE_DIVIDE_INT,
  SPVM_BYTECODE_C_CODE_DIVIDE_LONG,
  SPVM_BYTECODE_C_CODE_DIVIDE_FLOAT,
  SPVM_BYTECODE_C_CODE_DIVIDE_DOUBLE,
  SPVM_BYTECODE_C_CODE_REMAINDER_INT,
  SPVM_BYTECODE_C_CODE_REMAINDER_LONG,
  SPVM_BYTECODE_C_CODE_REMAINDER_FLOAT,
  SPVM_BYTECODE_C_CODE_REMAINDER_DOUBLE,
  SPVM_BYTECODE_C_CODE_NEGATE_INT,
  SPVM_BYTECODE_C_CODE_NEGATE_LONG,
  SPVM_BYTECODE_C_CODE_NEGATE_FLOAT,
  SPVM_BYTECODE_C_CODE_NEGATE_DOUBLE,
  SPVM_BYTECODE_C_CODE_LEFT_SHIFT_INT,
  SPVM_BYTECODE_C_CODE_LEFT_SHIFT_LONG,
  SPVM_BYTECODE_C_CODE_RIGHT_SHIFT_INT,
  SPVM_BYTECODE_C_CODE_RIGHT_SHIFT_LONG,
  SPVM_BYTECODE_C_CODE_RIGHT_SHIFT_UNSIGNED_INT,
  SPVM_BYTECODE_C_CODE_RIGHT_SHIFT_UNSIGNED_LONG,
  SPVM_BYTECODE_C_CODE_BIT_AND_INT,
  SPVM_BYTECODE_C_CODE_BIT_AND_LONG,
  SPVM_BYTECODE_C_CODE_BIT_OR_INT,
  SPVM_BYTECODE_C_CODE_BIT_OR_LEFT,
  SPVM_BYTECODE_C_CODE_BIT_XOR_INT,
  SPVM_BYTECODE_C_CODE_BIT_XOR_LONG,
  SPVM_BYTECODE_C_CODE_CONVERT_INT_TO_LONG,
  SPVM_BYTECODE_C_CODE_CONVERT_INT_TO_FLOAT,
  SPVM_BYTECODE_C_CODE_CONVERT_INT_TO_DOUBLE,
  SPVM_BYTECODE_C_CODE_CONVERT_LONG_TO_INT,
  SPVM_BYTECODE_C_CODE_CONVERT_LONG_TO_FLOAT,
  SPVM_BYTECODE_C_CODE_CONVERT_LONG_TO_DOUBLE,
  SPVM_BYTECODE_C_CODE_CONVERT_FLOAT_TO_INT,
  SPVM_BYTECODE_C_CODE_CONVERT_FLOAT_TO_LONG,
  SPVM_BYTECODE_C_CODE_CONVERT_FLOAT_TO_DOUBLE,
  SPVM_BYTECODE_C_CODE_CONVERT_DOUBLE_TO_INT,
  SPVM_BYTECODE_C_CODE_CONVERT_DOUBLE_TO_LONG,
  SPVM_BYTECODE_C_CODE_CONVERT_DOUBLE_TO_FLOAT,
  SPVM_BYTECODE_C_CODE_CONVERT_INT_TO_BYTE,
  SPVM_BYTECODE_C_CODE_CONVERT_INT_TO_SHORT,
  SPVM_BYTECODE_C_CODE_ICMP,
  SPVM_BYTECODE_C_CODE_FCMPL,
  SPVM_BYTECODE_C_CODE_FCMPG,
  SPVM_BYTECODE_C_CODE_DCMPL,
  SPVM_BYTECODE_C_CODE_DCMPG,
  SPVM_BYTECODE_C_CODE_IFEQ,
  SPVM_BYTECODE_C_CODE_IFNE,
  SPVM_BYTECODE_C_CODE_IFLT,
  SPVM_BYTECODE_C_CODE_IFGE,
  SPVM_BYTECODE_C_CODE_IFGT,
  SPVM_BYTECODE_C_CODE_IFLE,
  SPVM_BYTECODE_C_CODE_IF_LCMPEQ,
  SPVM_BYTECODE_C_CODE_IF_LCMPNE,
  SPVM_BYTECODE_C_CODE_IF_LCMPLT,
  SPVM_BYTECODE_C_CODE_IF_LCMPGE,
  SPVM_BYTECODE_C_CODE_IF_LCMPGT,
  SPVM_BYTECODE_C_CODE_IF_LCMPLE,
  SPVM_BYTECODE_C_CODE_IF_ACMPEQ,
  SPVM_BYTECODE_C_CODE_IF_ACMPNE,
  SPVM_BYTECODE_C_CODE_GOTO,
  SPVM_BYTECODE_C_CODE_TABLESWITCH,
  SPVM_BYTECODE_C_CODE_LOOKUPSWITCH,
  SPVM_BYTECODE_C_CODE_ARRAYLENGTH,
  SPVM_BYTECODE_C_CODE_ATHROW,
  SPVM_BYTECODE_C_CODE_WIDE,
  SPVM_BYTECODE_C_CODE_IFNULL,
  SPVM_BYTECODE_C_CODE_IFNONNULL,
  SPVM_BYTECODE_C_CODE_INVOKESTATIC_WW,
  SPVM_BYTECODE_C_CODE_LOAD_CONSTANT_WW,
  SPVM_BYTECODE_C_CODE_BGETFIELD,
  SPVM_BYTECODE_C_CODE_SGETFIELD,
  SPVM_BYTECODE_C_CODE_IGETFIELD,
  SPVM_BYTECODE_C_CODE_LGETFIELD,
  SPVM_BYTECODE_C_CODE_FGETFIELD,
  SPVM_BYTECODE_C_CODE_DGETFIELD,
  SPVM_BYTECODE_C_CODE_BPUTFIELD,
  SPVM_BYTECODE_C_CODE_SPUTFIELD,
  SPVM_BYTECODE_C_CODE_IPUTFIELD,
  SPVM_BYTECODE_C_CODE_LPUTFIELD,
  SPVM_BYTECODE_C_CODE_FPUTFIELD,
  SPVM_BYTECODE_C_CODE_DPUTFIELD,
  SPVM_BYTECODE_C_CODE_INC_BYTE,
  SPVM_BYTECODE_C_CODE_INC_SHORT,
  SPVM_BYTECODE_C_CODE_INC_INT,
  SPVM_BYTECODE_C_CODE_INC_LONG,
  SPVM_BYTECODE_C_CODE_CONVERT_BYTE_TO_INT,
  SPVM_BYTECODE_C_CODE_CONVERT_SHORT_TO_INT,
  SPVM_BYTECODE_C_CODE_CONVERT_BYTE_TO_LONG,
  SPVM_BYTECODE_C_CODE_CONVERT_BYTE_TO_FLOAT,
  SPVM_BYTECODE_C_CODE_CONVERT_BYTE_TO_DOUBLE,
  SPVM_BYTECODE_C_CODE_CONVERT_SHORT_TO_BYTE,
  SPVM_BYTECODE_C_CODE_CONVERT_SHORT_TO_LONG,
  SPVM_BYTECODE_C_CODE_CONVERT_SHORT_TO_FLOAT,
  SPVM_BYTECODE_C_CODE_CONVERT_SHORT_TO_DOUBLE,
  SPVM_BYTECODE_C_CODE_CONVERT_LONG_TO_BYTE,
  SPVM_BYTECODE_C_CODE_CONVERT_LONG_TO_SHORT,
  SPVM_BYTECODE_C_CODE_CONVERT_FLOAT_TO_BYTE,
  SPVM_BYTECODE_C_CODE_CONVERT_FLOAT_TO_SHORT,
  SPVM_BYTECODE_C_CODE_CONVERT_DOUBLE_TO_BYTE,
  SPVM_BYTECODE_C_CODE_CONVERT_DOUBLE_TO_SHORT,
  SPVM_BYTECODE_C_CODE_CONVERT_BYTE_TO_SHORT,
  SPVM_BYTECODE_C_CODE_MALLOC_OBJECT,
  SPVM_BYTECODE_C_CODE_MALLOC_ARRAY,
  SPVM_BYTECODE_C_CODE_MALLOC_STRING,
  SPVM_BYTECODE_C_CODE_RETURN_VOID,
  SPVM_BYTECODE_C_CODE_RETURN_VALUE,
  SPVM_BYTECODE_C_CODE_LOAD,
  SPVM_BYTECODE_C_CODE_LOAD_0,
  SPVM_BYTECODE_C_CODE_LOAD_1,
  SPVM_BYTECODE_C_CODE_LOAD_2,
  SPVM_BYTECODE_C_CODE_LOAD_3,
  SPVM_BYTECODE_C_CODE_STORE,
  SPVM_BYTECODE_C_CODE_STORE_0,
  SPVM_BYTECODE_C_CODE_STORE_1,
  SPVM_BYTECODE_C_CODE_STORE_2,
  SPVM_BYTECODE_C_CODE_STORE_3,
  SPVM_BYTECODE_C_CODE_DIVIDE_BYTE,
  SPVM_BYTECODE_C_CODE_REMAINDER_BYTE,
  SPVM_BYTECODE_C_CODE_NEGATE_BYTE,
  SPVM_BYTECODE_C_CODE_LEFT_SHIFT_BYTE,
  SPVM_BYTECODE_C_CODE_RIGHT_SHIFT_BYTE,
  SPVM_BYTECODE_C_CODE_RIGHT_SHIFT_UNSIGNED_BYTE,
  SPVM_BYTECODE_C_CODE_BIT_AND_BYTE,
  SPVM_BYTECODE_C_CODE_BIT_OR_BYTE ,
  SPVM_BYTECODE_C_CODE_BIT_XOR_BYTE,
  SPVM_BYTECODE_C_CODE_BCMP,
  SPVM_BYTECODE_C_CODE_DIVIDE_SHORT,
  SPVM_BYTECODE_C_CODE_REMAINDER_SHORT,
  SPVM_BYTECODE_C_CODE_NEGATE_SHORT,
  SPVM_BYTECODE_C_CODE_LEFT_SHIFT_SHORT,
  SPVM_BYTECODE_C_CODE_RIGHT_SHIFT_SHORT,
  SPVM_BYTECODE_C_CODE_RIGHT_SHIFT_UNSIGNED_SHORT,
  SPVM_BYTECODE_C_CODE_BIT_AND_SHORT,
  SPVM_BYTECODE_C_CODE_BIT_OR_SHORT ,
  SPVM_BYTECODE_C_CODE_BIT_XOR_SHORT,
  SPVM_BYTECODE_C_CODE_CMP_SHORT,
  SPVM_BYTECODE_C_CODE_ASTORE,
  SPVM_BYTECODE_C_CODE_AASTORE,
  SPVM_BYTECODE_C_CODE_APUTFIELD,
  SPVM_BYTECODE_C_CODE_INC_REF_COUNT,
  SPVM_BYTECODE_C_CODE_DEC_REF_COUNT,
};

extern const char* const SPVM_BYTECODE_C_CODE_NAMES[];

#endif
