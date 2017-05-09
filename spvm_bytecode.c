#include "spvm_bytecode.h"







const char* const SPVM_BYTECODE_C_CODE_NAMES[] = {
  "NOP",
  "UNDEF",
  "CONSTANT_BYTE_0",
  "CONSTANT_BYTE_1",
  "CONSTANT_SHORT_0",
  "CONSTANT_SHORT_1",
  "CONSTANT_INT_M1",
  "CONSTANT_INT_0",
  "CONSTANT_INT_1",
  "CONSTANT_INT_2",
  "CONSTANT_INT_3",
  "CONSTANT_INT_4",
  "CONSTANT_INT_5",
  "CONSTANT_LONG_M1",
  "CONSTANT_LONG_0",
  "CONSTANT_LONG_1",
  "CONSTANT_LONG_2",
  "CONSTANT_LONG_3",
  "CONSTANT_LONG_4",
  "CONSTANT_LONG_5",
  "CONSTANT_FLOAT_0",
  "CONSTANT_FLOAT_1",
  "CONSTANT_FLOAT_2",
  "CONSTANT_DOUBLE_0",
  "CONSTANT_DOUBLE_1",
  "PUSH_BYTE",
  "PUSH_BYTE_TO_SHORT",
  "PUSH_SHORT",
  "PUSH_BYTE_TO_INT",
  "PUSH_SHORT_TO_INT",
  "PUSH_BYTE_TO_LONG",
  "PUSH_SHORT_TO_LONG",
  "LOAD_CONSTANT",
  "ADD_BYTE",
  "ADD_SHORT",
  "ADD_INT",
  "ADD_LONG",
  "ADD_FLOAT",
  "ADD_DOUBLE",
  "SUBTRACT_BYTE",
  "SUBTRACT_SHORT",
  "SUBTRACT_INT",
  "SUBTRACT_LONG",
  "SUBTRACT_FLOAT",
  "SUBTRACT_DOUBLE",
  "MULTIPLY_BYTE",
  "MULTIPLY_SHORT",
  "MULTIPLY_INT",
  "MULTIPLY_LONG",
  "MULTIPLY_FLOAT",
  "MULTIPLY_DOUBLE",
  "DIVIDE_BYTE",
  "DIVIDE_SHORT",
  "DIVIDE_INT",
  "DIVIDE_LONG",
  "DIVIDE_FLOAT",
  "DIVIDE_DOUBLE",
  "REMAINDER_BYTE",
  "REMAINDER_SHORT",
  "REMAINDER_INT",
  "REMAINDER_LONG",
  "REMAINDER_FLOAT",
  "REMAINDER_DOUBLE",
  "NEGATE_BYTE",
  "NEGATE_SHORT",
  "NEGATE_INT",
  "NEGATE_LONG",
  "NEGATE_FLOAT",
  "NEGATE_DOUBLE",
  "LEFT_SHIFT_BYTE",
  "LEFT_SHIFT_SHORT",
  "LEFT_SHIFT_INT",
  "LEFT_SHIFT_LONG",
  "RIGHT_SHIFT_BYTE",
  "RIGHT_SHIFT_SHORT",
  "RIGHT_SHIFT_INT",
  "RIGHT_SHIFT_LONG",
  "RIGHT_SHIFT_UNSIGNED_BYTE",
  "RIGHT_SHIFT_UNSIGNED_SHORT",
  "RIGHT_SHIFT_UNSIGNED_INT",
  "RIGHT_SHIFT_UNSIGNED_LONG",
  "BIT_AND_BYTE",
  "BIT_AND_SHORT",
  "BIT_AND_INT",
  "BIT_AND_LONG",
  "BIT_OR_BYTE ",
  "BIT_OR_SHORT ",
  "BIT_OR_INT",
  "BIT_OR_LONG",
  "BIT_XOR_BYTE",
  "BIT_XOR_SHORT",
  "BIT_XOR_INT",
  "BIT_XOR_LONG",
  "CONVERT_BYTE_TO_SHORT",
  "CONVERT_BYTE_TO_INT",
  "CONVERT_BYTE_TO_LONG",
  "CONVERT_BYTE_TO_FLOAT",
  "CONVERT_BYTE_TO_DOUBLE",
  "CONVERT_SHORT_TO_BYTE",
  "CONVERT_SHORT_TO_INT",
  "CONVERT_SHORT_TO_LONG",
  "CONVERT_SHORT_TO_FLOAT",
  "CONVERT_SHORT_TO_DOUBLE",
  "CONVERT_INT_TO_BYTE",
  "CONVERT_INT_TO_SHORT",
  "CONVERT_INT_TO_LONG",
  "CONVERT_INT_TO_FLOAT",
  "CONVERT_INT_TO_DOUBLE",
  "CONVERT_LONG_TO_BYTE",
  "CONVERT_LONG_TO_SHORT",
  "CONVERT_LONG_TO_INT",
  "CONVERT_LONG_TO_FLOAT",
  "CONVERT_LONG_TO_DOUBLE",
  "CONVERT_FLOAT_TO_BYTE",
  "CONVERT_FLOAT_TO_SHORT",
  "CONVERT_FLOAT_TO_INT",
  "CONVERT_FLOAT_TO_LONG",
  "CONVERT_FLOAT_TO_DOUBLE",
  "CONVERT_DOUBLE_TO_BYTE",
  "CONVERT_DOUBLE_TO_SHORT",
  "CONVERT_DOUBLE_TO_INT",
  "CONVERT_DOUBLE_TO_LONG",
  "CONVERT_DOUBLE_TO_FLOAT",
  "INC_BYTE",
  "INC_SHORT",
  "INC_INT",
  "INC_LONG",
  "LOAD",
  "LOAD_0",
  "LOAD_1",
  "LOAD_2",
  "LOAD_3",
  "STORE",
  "STORE_0",
  "STORE_1",
  "STORE_2",
  "STORE_3",
  "STORE_ADDRESS",
  "POP",
  "POP_ADDRESS",
  "MALLOC_OBJECT",
  "MALLOC_ARRAY",
  "MALLOC_STRING",
  "ARRAY_LOAD_INT",
  "ARRAY_LOAD_LONG",
  "ARRAY_LOAD_FLOAT",
  "ARRAY_LOAD_DOUBLE",
  "ARRAY_LOAD_BYTE",
  "ARRAY_LOAD_SHORT",
  "ARRAY_STORE_BYTE",
  "ARRAY_STORE_SHORT",
  "ARRAY_STORE_INT",
  "ARRAY_STORE_LONG",
  "ARRAY_STORE_FLOAT",
  "ARRAY_STORE_DOUBLE",
  "ARRAY_STORE_ADDRESS",
  "ARRAY_LENGTH",
  "GET_FIELD_BYTE",
  "GET_FIELD_SHORT",
  "GET_FIELD_INT",
  "GET_FIELD_LONG",
  "GET_FIELD_FLOAT",
  "GET_FIELD_DOUBLE",
  "SET_FIELD_BYTE",
  "SET_FIELD_SHORT",
  "SET_FIELD_INT",
  "SET_FIELD_LONG",
  "SET_FIELD_FLOAT",
  "SET_FIELD_DOUBLE",
  "SET_FIELD_ADDRESS",
  "CMP_BYTE",
  "CMP_SHORT",
  "CMP_FLOAT_L",
  "CMP_FLOAT_G",
  "CMP_DOUBLE_L",
  "CMP_DOUBLE_G",
  "IF_EQ_ZERO_INT",
  "IF_NE_ZERO_INT",
  "IF_LT_ZERO_INT",
  "IF_GE_ZERO_INT",
  "IF_GT_ZERO_INT",
  "IF_LE_ZERO_INT",
  "IF_EQ_CMP_INT",
  "IF_NE_CMP_INT",
  "IF_LT_CMP_INT",
  "IF_GE_CMP_INT",
  "IF_GT_CMP_INT",
  "IF_LE_CMP_INT",
  "IF_NE_ZERO_LONG",
  "IF_EQ_CMP_LONG",
  "IF_NE_CMP_LONG",
  "IF_LT_CMP_LONG",
  "IF_GE_CMP_LONG",
  "IF_GT_CMP_LONG",
  "IF_LE_CMP_LONG",
  "IF_EQ_CMP_ADDRESS",
  "IF_NE_CMP_ADDRESS",
  "IF_NULL",
  "IF_NON_NULL",
  "TABLE_SWITCH_INT",
  "LOOKUP_SWITCH_INT",
  "TABLE_SWITCH_LONG",
  "LOOKUP_SWITCH_LONG",
  "GOTO",
  "CALL_SUB",
  "RETURN_VOID",
  "RETURN",
  "DIE",
  "INC_REF_COUNT",
  "DEC_REF_COUNT",
  "WIDE",
};
