#ifndef H_CTYPES
#define H_CTYPES

#include <stdbool.h>
#include <stdint.h>

// 0 to save ram and optimize for embedded systems
// 1 to gain extra speed by replacing all floats by doubles
// 2 to gain extra speed by using bigger integers depending on arch
// level 2 "optimizations" will definitely eat your ram
#ifndef CTYPES_SPEED
#define CTYPES_SPEED 0
#endif

///////////////////
// regular stuff //
///////////////////

#if CTYPES_SPEED < 2
// 100% standard
typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;
typedef int8_t i8;
typedef int16_t i16;
typedef int32_t i32;
typedef int64_t i64;
#endif

#if CTYPES_SPEED < 1
// the floating point number representation is not defined by the C standard
// however, it strongly suggests using the IEEE 754 norm for that task
// "long double" is not redefined because it is compiler-dependant
typedef float f32;
typedef double f64;

// floating point helpers for tuning constants and helping optimizations propagation
// doesn't affect the code at all, it just concatenates the given numbers as a literal
// f64(3,141592653589793238462643383279502884197169)
// gives 3.141592653589793238462643383279502884197169
// f32(3,1415926535)
// gives 3.1415926535f
#define f64(a,b) (a##.##b)
#define f32(a,b) (a##.##b##f)
#endif

///////////////////////////////
// black magic optimizations //
///////////////////////////////

#if CTYPES_SPEED > 0
// the best optimization out there
// doubles are usually slower than floats for various reasons
// on embedded systems though, it is most likely the opposite
typedef double f64;
typedef f64 f32;

// reduce type conversions if the literal helpers were used
#define f64(a,b) (a##.##b)
#define f32(a,b) f64(a,b)
#endif

#if CTYPES_SPEED > 1
// the following block tries to optimize speed at the cost of ram
// we are testing the architecture in the most portable way possible
// the following macro is not mandatory, obscure systems might not provide it
// on 16 bits systems, 16-bit integer operations can be the fastest
// on 32 bits systems, 32-bit integer operations can be the fastest
// on 64 bits systems, 64-bit integer operations can be the fastest
#if UINTPTR_MAX == 0xffff
typedef uint16_t u8;
typedef int16_t i8;
typedef uint16_t u16;
typedef int16_t i16;
typedef uint32_t u32;
typedef int32_t i32;
typedef uint64_t u64;
typedef int64_t i64;
#elif UINTPTR_MAX == 0xffffffff
typedef uint32_t u8;
typedef int32_t i8;
typedef uint32_t u16;
typedef int32_t i16;
typedef uint32_t u32;
typedef int32_t i32;
typedef uint64_t u64;
typedef int64_t i64;
#elif UINTPTR_MAX == 0xffffffffffffffff
typedef uint64_t u8;
typedef int64_t i8;
typedef uint64_t u16;
typedef int64_t i16;
typedef uint64_t u32;
typedef int64_t i32;
typedef uint64_t u64;
typedef int64_t i64;
#endif
#endif

#endif
