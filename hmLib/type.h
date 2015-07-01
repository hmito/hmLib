#ifndef HMLIB_TYPE_INC
#define HMLIB_TYPE_INC 100
#
#ifdef __cplusplus
#	include<cstdint>
using hmLib_uint8 = std::uint8_t;
using hmLib_sint8 = std::int8_t;
using hmLib_uint16 = std::uint16_t;
using hmLib_sint16 = std::int16_t;
using hmLib_uint32 = std::uint32_t;
using hmLib_sint32 = std::int32_t;
using hmLib_uint64 = std::uint64_t;
using hmLib_sint64 = std::int64_t;
using hmLib_float32 = float;
using hmLib_float64 = double;
using hmLib_float128 = long double;
#else
#	include<stdint.h>
#	include<stdbool.h>
typedef uint8_t hmLib_uint8;
typedef int8_t hmLib_sint8;
typedef uint16_t hmLib_uint16;
typedef int16_t hmLib_sint16;
typedef uint32_t hmLib_uint32;
typedef int32_t hmLib_sint32;
typedef uint64_t hmLib_uint64;
typedef int64_t hmLib_sint64;
typedef float hmLib_float32;
typedef double hmLib_float64;
typedef long double hmLib_float128;
#endif
//boolian
typedef unsigned char  hmLib_boolian;
#define hmLib_true 1
#define hmLib_false 0
//pointer
typedef void* hmLib_pointer;
//pi
#define hmLib_pi() (3.141592653589793)

//functions
typedef void(*hmLib_vFp_v)(void);

typedef void(*hmLib_vFp_c)(char);
typedef char(*hmLib_cFp_v)(void);
typedef void(*hmLib_vFp_s)(short);
typedef short(*hmLib_sFp_v)(void);
typedef void(*hmLib_vFp_i)(int);
typedef int(*hmLib_iFp_v)(void);
typedef void(*hmLib_vFp_l)(long);
typedef long(*hmLib_lFp_v)(void);
typedef void(*hmLib_vFp_f)(float);
typedef float(*hmLib_fFp_v)(void);
typedef void(*hmLib_vFp_lf)(double);
typedef double(*hmLib_lfFp_v)(void);

typedef void(*hmLib_vFp_b)(hmLib_boolian);
typedef hmLib_boolian(*hmLib_bFp_v)(void);
typedef void(*hmLib_vFp_p)(hmLib_pointer);
typedef hmLib_pointer(*hmLib_pFp_v)(void);

//hmStd Type
typedef void(*hmLib_vFp_u8)(hmLib_uint8);
typedef void(*hmLib_vFp_s8)(hmLib_sint8);
typedef hmLib_uint8(*hmLib_u8Fp_v)(void);
typedef hmLib_sint8(*hmLib_s8Fp_v)(void);
typedef void(*hmLib_vFp_u16)(hmLib_uint16);
typedef void(*hmLib_vFp_s16)(hmLib_sint16);
typedef hmLib_uint16(*hmLib_u16Fp_v)(void);
typedef hmLib_sint16(*hmLib_s16Fp_v)(void);
typedef void(*hmLib_vFp_u32)(hmLib_uint32);
typedef void(*hmLib_vFp_s32)(hmLib_sint32);
typedef hmLib_uint32(*hmLib_u32Fp_v)(void);
typedef hmLib_sint32(*hmLib_s32Fp_v)(void);
typedef void(*hmLib_vFp_u64)(hmLib_uint64);
typedef void(*hmLib_vFp_s64)(hmLib_sint64);
typedef hmLib_uint64(*hmLib_u64Fp_v)(void);
typedef hmLib_sint64(*hmLib_s64Fp_v)(void);
typedef void(*hmLib_vFp_f32)(hmLib_float32);
typedef hmLib_float32(*hmLib_f32Fp_v)(void);
typedef void(*hmLib_vFp_f64)(hmLib_float64);
typedef hmLib_float64(*hmLib_f64Fp_v)(void);

//namespace_type
#ifdef __cplusplus
namespace hmLib{
	inline decltype(hmLib_pi()) pi(){return hmLib_pi();} 
	namespace type{
		//default type
		typedef signed char schar;
		typedef unsigned char uchar;
		typedef signed short sshort;
		typedef unsigned short ushort;
		typedef signed int sint;
		typedef unsigned int uint;
		typedef signed long slong;
		typedef unsigned long ulong;
		typedef signed long long sdlong;
		typedef unsigned long long udlong;

		typedef hmLib_vFp_v vFp_v;
		typedef hmLib_vFp_c vFp_c;
		typedef hmLib_vFp_s vFp_s;
		typedef hmLib_vFp_i vFp_i;
		typedef hmLib_vFp_l vFp_l;
		typedef hmLib_cFp_v cFp_v;
		typedef hmLib_sFp_v sFp_v;
		typedef hmLib_iFp_v iFp_v;
		typedef hmLib_lFp_v lFp_v;

		typedef hmLib_vFp_f vFp_f;
		typedef hmLib_fFp_v fFp_v;
		typedef hmLib_vFp_lf vFp_lf;
		typedef hmLib_lfFp_v lfFp_v;


		//boolian
		typedef hmLib_boolian boolian;
		typedef hmLib_bFp_v bFp_v;
		typedef hmLib_vFp_b vFp_b;

		typedef hmLib_pointer pointer;
		typedef hmLib_pFp_v pFp_v;
		typedef hmLib_vFp_p vFp_p;

		//hmStd Type
			typedef hmLib_uint8 uint8;
			typedef hmLib_sint8 sint8;

			typedef hmLib_vFp_u8 vFp_u8;
			typedef hmLib_u8Fp_v u8Fp_v;
			typedef hmLib_vFp_s8 vFp_s8;
			typedef hmLib_s8Fp_v s8Fp_v;

			typedef hmLib_uint16 uint16;
			typedef hmLib_sint16 sint16;

			typedef hmLib_vFp_u16 vFp_u16;
			typedef hmLib_vFp_s16 vFp_s16;
			typedef hmLib_u16Fp_v u16Fp_v;
			typedef hmLib_s16Fp_v s16Fp_v;

			typedef hmLib_uint32 uint32;
			typedef hmLib_sint32 sint32;

			typedef hmLib_vFp_u32 vFp_u32;
			typedef hmLib_vFp_s32 vFp_s32;
			typedef hmLib_u32Fp_v u32Fp_v;
			typedef hmLib_s32Fp_v s32Fp_v;

			typedef hmLib_uint64 uint64;
			typedef hmLib_sint64 sint64;

			typedef hmLib_vFp_u64 vFp_u64;
			typedef hmLib_vFp_s64 vFp_s64;
			typedef hmLib_u64Fp_v u64Fp_v;
			typedef hmLib_s64Fp_v s64Fp_v;

			typedef hmLib_float32 float32;

			typedef hmLib_vFp_f32 vFp_f32;
			typedef hmLib_f32Fp_v f32Fp_v;

			typedef hmLib_float64 float64;

			typedef hmLib_vFp_f64 vFp_f64;
			typedef hmLib_f64Fp_v f64Fp_v;
	}
}
#endif

//non_hmLib prefix version
#if defined(HMLIB_TYPE_NOPREFIX) || defined(HMLIB_NOPREFIX)
//default type
typedef signed char schar;
typedef unsigned char uchar;
typedef signed short sshort;
typedef unsigned short ushort;
typedef signed int sint;
typedef unsigned int uint;
typedef signed long slong;
typedef unsigned long ulong;
typedef signed long long sdlong;
typedef unsigned long long udlong;

typedef hmLib_vFp_v vFp_v;
typedef hmLib_vFp_c vFp_c;
typedef hmLib_vFp_s vFp_s;
typedef hmLib_vFp_i vFp_i;
typedef hmLib_vFp_l vFp_l;
typedef hmLib_cFp_v cFp_v;
typedef hmLib_sFp_v sFp_v;
typedef hmLib_iFp_v iFp_v;
typedef hmLib_lFp_v lFp_v;

typedef hmLib_vFp_f vFp_f;
typedef hmLib_fFp_v fFp_v;
typedef hmLib_vFp_lf vFp_lf;
typedef hmLib_lfFp_v lfFp_v;


//boolian
typedef hmLib_boolian boolian;
typedef hmLib_bFp_v bFp_v;
typedef hmLib_vFp_b vFp_b;

typedef hmLib_pointer pointer;
typedef hmLib_pFp_v pFp_v;
typedef hmLib_vFp_p vFp_p;

//hmStd Type
typedef hmLib_uint8 uint8;
typedef hmLib_sint8 sint8;

typedef hmLib_vFp_u8 vFp_u8;
typedef hmLib_u8Fp_v u8Fp_v;
typedef hmLib_vFp_s8 vFp_s8;
typedef hmLib_s8Fp_v s8Fp_v;

typedef hmLib_uint16 uint16;
typedef hmLib_sint16 sint16;

typedef hmLib_vFp_u16 vFp_u16;
typedef hmLib_vFp_s16 vFp_s16;
typedef hmLib_u16Fp_v u16Fp_v;
typedef hmLib_s16Fp_v s16Fp_v;

typedef hmLib_uint32 uint32;
typedef hmLib_sint32 sint32;

typedef hmLib_vFp_u32 vFp_u32;
typedef hmLib_vFp_s32 vFp_s32;
typedef hmLib_u32Fp_v u32Fp_v;
typedef hmLib_s32Fp_v s32Fp_v;

typedef hmLib_uint64 uint64;
typedef hmLib_sint64 sint64;

typedef hmLib_vFp_u64 vFp_u64;
typedef hmLib_vFp_s64 vFp_s64;
typedef hmLib_u64Fp_v u64Fp_v;
typedef hmLib_s64Fp_v s64Fp_v;

typedef hmLib_float32 float32;

typedef hmLib_vFp_f32 vFp_f32;
typedef hmLib_f32Fp_v f32Fp_v;

typedef hmLib_float64 float64;

typedef hmLib_vFp_f64 vFp_f64;
typedef hmLib_f64Fp_v f64Fp_v;

#endif

#
#endif
