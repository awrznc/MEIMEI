/**
  * type.h
  * ===
  *
  * - 型のエイリアス
  * - エイリアスの最大値・最小値の定義
  *
  * ```c
  * #include "type.h"
  *
  * int main() {
  *     i8 i8_max_value = MAX_I8;
  *     if(i8_max_value != MAX_I8) return 1;
  *
  *     i8 i8_min_value = MIN_I8;
  *     if(i8_min_value != MIN_I8) return 1;
  *
  *     u8 u8_max_value = MAX_U8;
  *     if(u8_max_value != MAX_U8) return 1;
  *
  *     i16 i16_max_value = MAX_I16;
  *     if(i16_max_value != MAX_I16) return 1;
  *
  *     i16 i16_min_value = MIN_I16;
  *     if(i16_min_value != MIN_I16) return 1;
  *
  *     u16 u16_max_value = MAX_U16;
  *     if(u16_max_value != MAX_U16) return 1;
  *
  *     i32 i32_max_value = MAX_I32;
  *     if(i32_max_value != MAX_I32) return 1;
  *
  *     i32 i32_min_value = MIN_I32;
  *     if(i32_min_value != MIN_I32) return 1;
  *
  *     u32 u32_max_value = MAX_U32;
  *     if(u32_max_value != MAX_U32) return 1;
  *
  *     // i64 i64_max_value = MAX_I64;
  *     // if(i64_max_value != MAX_I64) return 1;
  *
  *     // i64 i64_min_value = MIN_I64;
  *     // if(i64_min_value != MIN_I64) return 1;
  *
  *     // u64 u64_max_value = MAX_U64;
  *     // if(u64_max_value != MAX_U64) return 1;
  *
  *     return 0;
  * }
  * ```
  **/

#ifndef INCLUDED_TYPE
#define INCLUDED_TYPE

typedef signed char i8;
typedef unsigned char u8;
typedef signed short int i16;
typedef unsigned short int u16;
#if defined(_WIN32) || defined(_WIN64)
typedef signed long int i32;
typedef unsigned long int u32;
#else
typedef signed int i32;
typedef unsigned int u32;
#endif

// typedef signed long long int i64;
// typedef unsigned long long int u64;

#define MAX_I8  127
#define MIN_I8  -128
#define MAX_U8  255
#define MAX_I16 32767
#define MIN_I16 -32768
#define MAX_U16 65535
#define MAX_I32 2147483647
#define MIN_I32 -2147483648
#define MAX_U32 4294967295
// #define MAX_I64 9223372036854775807
// #define MIN_I64 -9223372036854775808
// #define MAX_U64 18446744073709551615

#endif
