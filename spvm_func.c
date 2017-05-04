#include <assert.h>
#include <stdio.h>
#include <stdint.h>
#include <inttypes.h>

#include "spvm.h"
#include "spvm_func.h"
#include "spvm_api.h"
#include "spvm_env.h"
#include "spvm_sv.h"

void SPVM_FUNC_std_test_call1(SPVM* spvm, SPVM_ENV* env) {
  (void)spvm;
  (void)env;
  
  int64_t value =  SPVM_API_get_var_long(spvm, env, 0);
  
  int64_t value2 = value * 2;
  
  // Call subroutine
  SPVM_ENV* new_env = SPVM_ENV_new(spvm);
  SPVM_API_push_var_long(spvm, new_env, value2);
  SPVM_API_call_sub(spvm, new_env, "std::test_call2");
  int64_t value3 = SPVM_API_pop_ret_long(spvm, new_env);
  SPVM_ENV_free(spvm, new_env);
  
  SPVM_API_push_ret_long(spvm, env, value3);
}

void SPVM_FUNC_std_test_call2(SPVM* spvm, SPVM_ENV* env) {
  (void)spvm;
  (void)env;
  
  int64_t value =  SPVM_API_get_var_long(spvm, env, 0);
  
  int64_t value2 = value * 3;
  
  SPVM_API_push_ret_long(spvm, env, value2);
}

void SPVM_FUNC_std_suml(SPVM* spvm, SPVM_ENV* env) {
  (void)spvm;
  (void)env;
  
  void* long_array = SPVM_API_get_var_ref(spvm, env, 0);
  
  int64_t length = SPVM_API_get_array_length(spvm, env, long_array);
  
  int64_t* long_array_data = SPVM_API_get_array_long_values(spvm, env, long_array);
  
  int64_t total = 0;
  for (int64_t i = 0; i < length; i++) {
    total += long_array_data[i];
  }
  
  SPVM_API_push_ret_long(spvm, env, total);
}

void SPVM_FUNC_std_println(SPVM* spvm, SPVM_ENV* env) {
  (void)spvm;
  (void)env;
  
  void* string = SPVM_API_get_var_ref(spvm, env, 0);
  
  SPVM_SV* sv = SPVM_API_get_string_sv(spvm, env, string);
  
  char* buffer = sv->buffer;
  int64_t length = sv->length;
  
  for (int64_t i = 0; i < length; i++) {
    putchar((int)buffer[i]);
  }
  
  printf("\n");
}

void SPVM_FUNC_std_printb(SPVM* spvm, SPVM_ENV* env) {
  (void)spvm;
  (void)env;
  
  int8_t value = SPVM_API_get_var_byte(spvm, env, 0);
  
  printf("TEST: %" PRId8 "\n", value);
}

void SPVM_FUNC_std_prints(SPVM* spvm, SPVM_ENV* env) {
  (void)spvm;
  (void)env;
  
  int16_t value = SPVM_API_get_var_short(spvm, env, 0);
  
  printf("TEST: %" PRId16 "\n", value);
}

void SPVM_FUNC_std_printi(SPVM* spvm, SPVM_ENV* env) {
  (void)spvm;
  (void)env;
  
  int32_t value = SPVM_API_get_var_int(spvm, env, 0);
  
  printf("TEST: %" PRId32 "\n", value);
}

void SPVM_FUNC_std_printl(SPVM* spvm, SPVM_ENV* env) {
  (void)spvm;
  (void)env;
  
  int64_t value = SPVM_API_get_var_long(spvm, env, 0);
  
  printf("TEST: %" PRId64 "\n", value);
}

void SPVM_FUNC_std_printf(SPVM* spvm, SPVM_ENV* env) {
  (void)spvm;
  (void)env;
  
  float value = SPVM_API_get_var_float(spvm, env, 0);
  
  printf("TEST: %f\n", value);
}

void SPVM_FUNC_std_printd(SPVM* spvm, SPVM_ENV* env) {
  (void)spvm;
  (void)env;
  
  double value = SPVM_API_get_var_double(spvm, env, 0);
  
  printf("TEST: %f\n", value);
}
