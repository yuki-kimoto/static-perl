#include "sperl_vmcode.h"







SPerl_char* const SPerl_VMCODE_C_CODE_NAMES[] = {
  "POP",
  "POP2",
  "ADD_INT",
  "ADD_LONG",
  "ADD_FLOAT",
  "ADD_DOUBLE",
  "SUBTRACT_INT",
  "SUBTRACT_LONG",
  "SUBTRACT_FLOAT",
  "SUBTRACT_DOUBLE",
  "MULTIPLY_INT",
  "MULTIPLY_LONG",
  "MULTIPLY_FLOAT",
  "MULTIPLY_DOUBLE",
  "DIVIDE_INT",
  "DIVIDE_LONG",
  "DIVIDE_FLOAT",
  "DIVIDE_DOUBLE",
  "CONVERT_DOUBLE_TO_FLOAT",
  "CONVERT_DOUBLE_TO_INT",
  "CONVERT_DDOUBLE_TO_LONG",
  "CONVERT_FLOAT_TO_DOUBLE",
  "CONVERT_FLOAT_TO_INT",
  "CONVERT_FLOAT_TO_LONG",
  "CONVERT_INT_TO_BYTE",
  "I2B",
  "CONVERT_INT_TO_DOUBLE",
  "CONVERT_INT_TO_FLOAT",
  "CONVERT_INT_TO_LONG",
  "CONVERT_INT_TO_SHORT",
  "CONVERT_LONG_TO_DOUBLE",
  "CONVERT_LONG_TO_FLOAT",
  "CONVERT_LONG_TO_INT",
  "CONSTANT_DOUBLE_0",
  "CONSTANT_DOUBLE_1",
  "CONSTANT_FLOAT_0",
  "CONSTANT_FLOAT_1",
  "CONSTANT_FLOAT_2",
  "CONSTANT_INT_M1",
  "CONSTANT_INT_0",
  "CONSTANT_INT_1",
  "CONSTANT_INT_2",
  "CONSTANT_INT_3",
  "CONSTANT_INT_4",
  "CONSTANT_INT_5",
  "CONSTANT_LONG_0",
  "CONSTANT_LONG_1",
  "LOAD_CONSTANT",
  "LOAD_CONSTANT_W",
  "LOAD_CONSTANT2_W",
  "NEGATE_INT",
  "NEGATE_LONG",
  "NEGATE_FLOAT",
  "NEGATE_DOUBLE",
  "REMAINDER_INT",
  "REMAINDER_LONG",
  "REMAINDER_FLOAT",
  "REMAINDER_DOUBLE",
  "RETURN_INT",
  "RETURN_LONG",
  "FETURN_FLOAT",
  "RETURN_DOUBLE",
  "RETURN_REF",
  "RETURN",
  "LOAD_INT",
  "LOAD_INT_0",
  "LOAD_INT_1",
  "LOAD_INT_2",
  "LOAD_INT_3",
  "LOAD_LONG",
  "LOAD_LONG_0",
  "LOAD_LONG_1",
  "LOAD_LONG_2",
  "LOAD_LONG_3",
  "LOAD_FLOAT",
  "LOAD_FLOAT_0",
  "LOAD_FLOAT_1",
  "LOAD_FLOAT_2",
  "LOAD_FLOAT_3",
  "LOAD_DOUBLE",
  "LOAD_DOUBLE_0",
  "LOAD_DOUBLE_1",
  "LOAD_DOUBLE_2",
  "LOAD_DOUBLE_3",
  "LOAD_REF",
  "LOAD_REF_0",
  "LOAD_REF_1",
  "LOAD_REF_2",
  "LOAD_REF_3",
  "STORE_INT",
  "STORE_INT_0",
  "STORE_INT_1",
  "STORE_INT_2",
  "STORE_INT_3",
  "STORE_LONG",
  "STORE_LONG_0",
  "STORE_LONG_1",
  "STORE_LONG_2",
  "STORE_LONG_3",
  "STORE_FLOAT",
  "STORE_FLOAT_0",
  "STORE_FLOAT_1",
  "STORE_FLOAT_2",
  "STORE_FLOAT_3",
  "STORE_DOUBLE",
  "STORE_DOUBLE_0",
  "STORE_DOUBLE_1",
  "STORE_DOUBLE_2",
  "STORE_DOUBLE_3",
  "STORE_REF",
  "STORE_REF_0",
  "STORE_REF_1",
  "STORE_REF_2",
  "STORE_REF_3",
  "LOAD_ARRAY_CHAR",
  "LOAD_ARRAY_BYTE",
  "LOAD_ARRAY_SHORT",
  "LOAD_ARRAY_INT",
  "LOAD_ARRAY_LONG",
  "LOAD_ARRAY_FLOAT",
  "LOAD_ARRAY_DOUBLE",
  "LOAD_ARRAY_REF",
  "STORE_ARRAY_CHAR",
  "STORE_ARRAY_BYTE",
  "STORE_ARRAY_SHORT",
  "STORE_ARRAY_INT",
  "STORE_ARRAY_LONG",
  "STORE_ARRAY_FLOAT",
  "STORE_ARRAY_DOUBLE",
  "STORE_ARRAY_REF",
  "BIT_AND_INT",
  "BIT_AND_LONG",
  "BIT_OR_INT",
  "BIT_OR_LONG",
  "BIT_XOR_INT",
  "BIT_XOR_LONG",
  "ACONST_NULL",
  "ANEWARRAY",
  "ARRAYLENGTH",
  "BIPUSH",
  "DCMPG",
  "DCMPL",
  "DUP",
  "DUP_X1",
  "DUP_X2",
  "DUP2",
  "DUP2_X1",
  "DUP2_X2",
  "FCMPG",
  "FCMPL",
  "GETFIELD",
  "GOTO",
  "GOTO_W",
  "IF_ACMPEQ",
  "IF_ACMPNE",
  "IF_ICMPEQ",
  "IF_ICMPNE",
  "IF_ICMPLT",
  "IF_ICMPGE",
  "IF_ICMPGT",
  "IF_ICMPLE",
  "IFEQ",
  "IFNE",
  "IFLT",
  "IFGE",
  "IFGT",
  "IFLE",
  "IFNONNULL",
  "IFNULL",
  "IINC",
  "LINC",
  "INSTANCEOF",
  "INVOKESTATIC",
  "ISHL",
  "ISHR",
  "IUSHR",
  "JSR",
  "JSR_W",
  "LCMP",
  "LSHL",
  "LSHR",
  "LUSHR",
  "MONITORENTER",
  "MONITOREXIT",
  "MULTIANEWARRAY",
  "NEW",
  "NEWARRAY",
  "NOP",
  "PUTFIELD",
  "RET",
  "SIPUSH",
  "SWAP",
  "WIDE",
  // "CHECKCAST",
  // "ATHROW",
  // "LOOKUPSWITCH",
  // "TABLESWITCH",
  // "PUTSTATIC",
  // "GETSTATIC",
  // "INVOKEDYNAMIC",
  // "INVOKEINTERFACE",
  // "INVOKESPECIAL",
  // "INVOKEVIRTUAL",
};
