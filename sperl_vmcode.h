#ifndef SPERL_VMCODE_H
#define SPERL_VMCODE_H

#include "sperl_base.h"




enum {
  SPerl_VMCODE_C_CODE_POP,
  SPerl_VMCODE_C_CODE_POP2,
  SPerl_VMCODE_C_CODE_ADD_INT,
  SPerl_VMCODE_C_CODE_ADD_LONG,
  SPerl_VMCODE_C_CODE_ADD_FLOAT,
  SPerl_VMCODE_C_CODE_ADD_DOUBLE,
  SPerl_VMCODE_C_CODE_SUBTRACT_INT,
  SPerl_VMCODE_C_CODE_SUBTRACT_LONG,
  SPerl_VMCODE_C_CODE_SUBTRACT_FLOAT,
  SPerl_VMCODE_C_CODE_SUBTRACT_DOUBLE,
  SPerl_VMCODE_C_CODE_MULTIPLY_INT,
  SPerl_VMCODE_C_CODE_MULTIPLY_LONG,
  SPerl_VMCODE_C_CODE_MULTIPLY_FLOAT,
  SPerl_VMCODE_C_CODE_MULTIPLY_DOUBLE,
  SPerl_VMCODE_C_CODE_DIVIDE_INT,
  SPerl_VMCODE_C_CODE_DIVIDE_LONG,
  SPerl_VMCODE_C_CODE_DIVIDE_FLOAT,
  SPerl_VMCODE_C_CODE_DIVIDE_DOUBLE,
  SPerl_VMCODE_C_CODE_CONVERT_DOUBLE_TO_FLOAT,
  SPerl_VMCODE_C_CODE_CONVERT_DOUBLE_TO_INT,
  SPerl_VMCODE_C_CODE_CONVERT_DDOUBLE_TO_LONG,
  SPerl_VMCODE_C_CODE_CONVERT_FLOAT_TO_DOUBLE,
  SPerl_VMCODE_C_CODE_CONVERT_FLOAT_TO_INT,
  SPerl_VMCODE_C_CODE_CONVERT_FLOAT_TO_LONG,
  SPerl_VMCODE_C_CODE_CONVERT_INT_TO_BYTE,
  SPerl_VMCODE_C_CODE_CONVERT_INT_TO_CHAR,
  SPerl_VMCODE_C_CODE_CONVERT_INT_TO_DOUBLE,
  SPerl_VMCODE_C_CODE_CONVERT_INT_TO_FLOAT,
  SPerl_VMCODE_C_CODE_CONVERT_INT_TO_LONG,
  SPerl_VMCODE_C_CODE_CONVERT_INT_TO_SHORT,
  SPerl_VMCODE_C_CODE_CONVERT_LONG_TO_DOUBLE,
  SPerl_VMCODE_C_CODE_CONVERT_LONG_TO_FLOAT,
  SPerl_VMCODE_C_CODE_CONVERT_LONG_TO_INT,
  SPerl_VMCODE_C_CODE_CONSTANT_DOUBLE_0,
  SPerl_VMCODE_C_CODE_CONSTANT_DOUBLE_1,
  SPerl_VMCODE_C_CODE_CONSTANT_FLOAT_0,
  SPerl_VMCODE_C_CODE_CONSTANT_FLOAT_1,
  SPerl_VMCODE_C_CODE_CONSTANT_FLOAT_2,
  SPerl_VMCODE_C_CODE_CONSTANT_INT_M1,
  SPerl_VMCODE_C_CODE_CONSTANT_INT_0,
  SPerl_VMCODE_C_CODE_CONSTANT_INT_1,
  SPerl_VMCODE_C_CODE_CONSTANT_INT_2,
  SPerl_VMCODE_C_CODE_CONSTANT_INT_3,
  SPerl_VMCODE_C_CODE_CONSTANT_INT_4,
  SPerl_VMCODE_C_CODE_CONSTANT_INT_5,
  SPerl_VMCODE_C_CODE_CONSTANT_LONG_0,
  SPerl_VMCODE_C_CODE_CONSTANT_LONG_1,
  SPerl_VMCODE_C_CODE_LOAD_CONSTANT,
  SPerl_VMCODE_C_CODE_LOAD_CONSTANT_W,
  SPerl_VMCODE_C_CODE_LOAD_CONSTANT2_W,
  SPerl_VMCODE_C_CODE_NEGATE_INT,
  SPerl_VMCODE_C_CODE_NEGATE_LONG,
  SPerl_VMCODE_C_CODE_NEGATE_FLOAT,
  SPerl_VMCODE_C_CODE_NEGATE_DOUBLE,
  SPerl_VMCODE_C_CODE_REMAINDER_INT,
  SPerl_VMCODE_C_CODE_REMAINDER_LONG,
  SPerl_VMCODE_C_CODE_REMAINDER_FLOAT,
  SPerl_VMCODE_C_CODE_REMAINDER_DOUBLE,
  SPerl_VMCODE_C_CODE_RETURN_INT,
  SPerl_VMCODE_C_CODE_RETURN_LONG,
  SPerl_VMCODE_C_CODE_FETURN_FLOAT,
  SPerl_VMCODE_C_CODE_RETURN_DOUBLE,
  SPerl_VMCODE_C_CODE_RETURN_REF,
  SPerl_VMCODE_C_CODE_RETURN,
  SPerl_VMCODE_C_CODE_LOAD_INT,
  SPerl_VMCODE_C_CODE_LOAD_INT_0,
  SPerl_VMCODE_C_CODE_LOAD_INT_1,
  SPerl_VMCODE_C_CODE_LOAD_INT_2,
  SPerl_VMCODE_C_CODE_LOAD_INT_3,
  SPerl_VMCODE_C_CODE_LOAD_LONG,
  SPerl_VMCODE_C_CODE_LOAD_LONG_0,
  SPerl_VMCODE_C_CODE_LOAD_LONG_1,
  SPerl_VMCODE_C_CODE_LOAD_LONG_2,
  SPerl_VMCODE_C_CODE_LOAD_LONG_3,
  SPerl_VMCODE_C_CODE_LOAD_FLOAT,
  SPerl_VMCODE_C_CODE_LOAD_FLOAT_0,
  SPerl_VMCODE_C_CODE_LOAD_FLOAT_1,
  SPerl_VMCODE_C_CODE_LOAD_FLOAT_2,
  SPerl_VMCODE_C_CODE_LOAD_FLOAT_3,
  SPerl_VMCODE_C_CODE_LOAD_DOUBLE,
  SPerl_VMCODE_C_CODE_LOAD_DOUBLE_0,
  SPerl_VMCODE_C_CODE_LOAD_DOUBLE_1,
  SPerl_VMCODE_C_CODE_LOAD_DOUBLE_2,
  SPerl_VMCODE_C_CODE_LOAD_DOUBLE_3,
  SPerl_VMCODE_C_CODE_LOAD_REF,
  SPerl_VMCODE_C_CODE_LOAD_REF_0,
  SPerl_VMCODE_C_CODE_LOAD_REF_1,
  SPerl_VMCODE_C_CODE_LOAD_REF_2,
  SPerl_VMCODE_C_CODE_LOAD_REF_3,
  SPerl_VMCODE_C_CODE_STORE_INT,
  SPerl_VMCODE_C_CODE_STORE_INT_0,
  SPerl_VMCODE_C_CODE_STORE_INT_1,
  SPerl_VMCODE_C_CODE_STORE_INT_2,
  SPerl_VMCODE_C_CODE_STORE_INT_3,
  SPerl_VMCODE_C_CODE_STORE_LONG,
  SPerl_VMCODE_C_CODE_STORE_LONG_0,
  SPerl_VMCODE_C_CODE_STORE_LONG_1,
  SPerl_VMCODE_C_CODE_STORE_LONG_2,
  SPerl_VMCODE_C_CODE_STORE_LONG_3,
  SPerl_VMCODE_C_CODE_STORE_FLOAT,
  SPerl_VMCODE_C_CODE_STORE_FLOAT_0,
  SPerl_VMCODE_C_CODE_STORE_FLOAT_1,
  SPerl_VMCODE_C_CODE_STORE_FLOAT_2,
  SPerl_VMCODE_C_CODE_STORE_FLOAT_3,
  SPerl_VMCODE_C_CODE_STORE_DOUBLE,
  SPerl_VMCODE_C_CODE_STORE_DOUBLE_0,
  SPerl_VMCODE_C_CODE_STORE_DOUBLE_1,
  SPerl_VMCODE_C_CODE_STORE_DOUBLE_2,
  SPerl_VMCODE_C_CODE_STORE_DOUBLE_3,
  SPerl_VMCODE_C_CODE_STORE_REF,
  SPerl_VMCODE_C_CODE_STORE_REF_0,
  SPerl_VMCODE_C_CODE_STORE_REF_1,
  SPerl_VMCODE_C_CODE_STORE_REF_2,
  SPerl_VMCODE_C_CODE_STORE_REF_3,
  SPerl_VMCODE_C_CODE_LOAD_ARRAY_CHAR,
  SPerl_VMCODE_C_CODE_LOAD_ARRAY_BYTE,
  SPerl_VMCODE_C_CODE_LOAD_ARRAY_SHORT,
  SPerl_VMCODE_C_CODE_LOAD_ARRAY_INT,
  SPerl_VMCODE_C_CODE_LOAD_ARRAY_LONG,
  SPerl_VMCODE_C_CODE_LOAD_ARRAY_FLOAT,
  SPerl_VMCODE_C_CODE_LOAD_ARRAY_DOUBLE,
  SPerl_VMCODE_C_CODE_LOAD_ARRAY_REF,
  SPerl_VMCODE_C_CODE_STORE_ARRAY_CHAR,
  SPerl_VMCODE_C_CODE_STORE_ARRAY_BYTE,
  SPerl_VMCODE_C_CODE_STORE_ARRAY_SHORT,
  SPerl_VMCODE_C_CODE_STORE_ARRAY_INT,
  SPerl_VMCODE_C_CODE_STORE_ARRAY_LONG,
  SPerl_VMCODE_C_CODE_STORE_ARRAY_FLOAT,
  SPerl_VMCODE_C_CODE_STORE_ARRAY_DOUBLE,
  SPerl_VMCODE_C_CODE_STORE_ARRAY_REF,
  SPerl_VMCODE_C_CODE_BIT_OR_INT,
  SPerl_VMCODE_C_CODE_BIT_OR_LONG,
  SPerl_VMCODE_C_CODE_BIT_AND_INT,
  SPerl_VMCODE_C_CODE_BIT_AND_LONG,
  SPerl_VMCODE_C_CODE_BIT_XOR_INT,
  SPerl_VMCODE_C_CODE_BIT_XOR_LONG,
  SPerl_VMCODE_C_CODE_ACONST_NULL,
  SPerl_VMCODE_C_CODE_ANEWARRAY,
  SPerl_VMCODE_C_CODE_ARRAYLENGTH,
  SPerl_VMCODE_C_CODE_ATHROW,
  SPerl_VMCODE_C_CODE_BIPUSH,
  SPerl_VMCODE_C_CODE_CHECKCAST,
  SPerl_VMCODE_C_CODE_DCMPG,
  SPerl_VMCODE_C_CODE_DCMPL,
  SPerl_VMCODE_C_CODE_DUP,
  SPerl_VMCODE_C_CODE_DUP_X1,
  SPerl_VMCODE_C_CODE_DUP_X2,
  SPerl_VMCODE_C_CODE_DUP2,
  SPerl_VMCODE_C_CODE_DUP2_X1,
  SPerl_VMCODE_C_CODE_DUP2_X2,
  SPerl_VMCODE_C_CODE_FCMPG,
  SPerl_VMCODE_C_CODE_FCMPL,
  SPerl_VMCODE_C_CODE_GETFIELD,
  SPerl_VMCODE_C_CODE_GOTO,
  SPerl_VMCODE_C_CODE_GOTO_W,
  SPerl_VMCODE_C_CODE_IF_ACMPEQ,
  SPerl_VMCODE_C_CODE_IF_ACMPNE,
  SPerl_VMCODE_C_CODE_IF_ICMPEQ,
  SPerl_VMCODE_C_CODE_IF_ICMPNE,
  SPerl_VMCODE_C_CODE_IF_ICMPLT,
  SPerl_VMCODE_C_CODE_IF_ICMPGE,
  SPerl_VMCODE_C_CODE_IF_ICMPGT,
  SPerl_VMCODE_C_CODE_IF_ICMPLE,
  SPerl_VMCODE_C_CODE_IFEQ,
  SPerl_VMCODE_C_CODE_IFNE,
  SPerl_VMCODE_C_CODE_IFLT,
  SPerl_VMCODE_C_CODE_IFGE,
  SPerl_VMCODE_C_CODE_IFGT,
  SPerl_VMCODE_C_CODE_IFLE,
  SPerl_VMCODE_C_CODE_IFNONNULL,
  SPerl_VMCODE_C_CODE_IFNULL,
  SPerl_VMCODE_C_CODE_IINC,
  SPerl_VMCODE_C_CODE_LINC,
  SPerl_VMCODE_C_CODE_INSTANCEOF,
  SPerl_VMCODE_C_CODE_INVOKESTATIC,
  SPerl_VMCODE_C_CODE_ISHL,
  SPerl_VMCODE_C_CODE_ISHR,
  SPerl_VMCODE_C_CODE_IUSHR,
  SPerl_VMCODE_C_CODE_JSR,
  SPerl_VMCODE_C_CODE_JSR_W,
  SPerl_VMCODE_C_CODE_LCMP,
  SPerl_VMCODE_C_CODE_LSHL,
  SPerl_VMCODE_C_CODE_LSHR,
  SPerl_VMCODE_C_CODE_LUSHR,
  SPerl_VMCODE_C_CODE_MONITORENTER,
  SPerl_VMCODE_C_CODE_MONITOREXIT,
  SPerl_VMCODE_C_CODE_MULTIANEWARRAY,
  SPerl_VMCODE_C_CODE_NEW,
  SPerl_VMCODE_C_CODE_NEWARRAY,
  SPerl_VMCODE_C_CODE_NOP,
  SPerl_VMCODE_C_CODE_PUTFIELD,
  SPerl_VMCODE_C_CODE_RET,
  SPerl_VMCODE_C_CODE_SIPUSH,
  SPerl_VMCODE_C_CODE_SWAP,
  SPerl_VMCODE_C_CODE_WIDE,
  // SPerl_VMCODE_C_CODE_LOOKUPSWITCH,
  // SPerl_VMCODE_C_CODE_TABLESWITCH,
  // SPerl_VMCODE_C_CODE_PUTSTATIC,
  // SPerl_VMCODE_C_CODE_GETSTATIC,
  // SPerl_VMCODE_C_CODE_INVOKEDYNAMIC,
  // SPerl_VMCODE_C_CODE_INVOKEINTERFACE,
  // SPerl_VMCODE_C_CODE_INVOKESPECIAL,
  // SPerl_VMCODE_C_CODE_INVOKEVIRTUAL,
};

extern SPerl_char* const SPerl_VMCODE_C_CODE_NAMES[];

/* Binary operation */
struct SPerl_vmcode {
  SPerl_char code;
  SPerl_char operand1;
  SPerl_char operand2;
  SPerl_char operand3;
};

#endif
