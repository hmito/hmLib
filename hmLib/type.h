#ifndef HMLIB_TYPE_INC
#define HMLIB_TYPE_INC 100
#
#ifndef HMLIB_CONFIG_INC
#	include "hmLib_config.h"
#endif
#//StdTypeSize
#if HMLIB_TYPE_SIZE_CHAR==1
#	ifndef HMLIB_TYPE_SIZE_8
#		define HMLIB_TYPE_SIZE_8
		typedef unsigned char hmLib_uint8;
		typedef signed char hmLib_sint8;
#	endif
#elif HMLIB_TYPE_SIZE_CHAR==2
#	ifndef HMLIB_TYPE_SIZE_16
#		define HMLIB_TYPE_SIZE_16
		typedef unsigned char hmLib_uint16;
		typedef signed char hmLib_sint16;
#	endif
#endif
#if HMLIB_TYPE_SIZE_SHORT==1
#	ifndef HMLIB_TYPE_SIZE_8
#		define HMLIB_TYPE_SIZE_8
		typedef unsigned short hmLib_uint8;
		typedef signed short hmLib_sint8;
#	endif
#elif HMLIB_TYPE_SIZE_SHORT==2
#	ifndef HMLIB_TYPE_SIZE_16
#		define HMLIB_TYPE_SIZE_16
		typedef unsigned short hmLib_uint16;
		typedef signed short hmLib_sint16;
#	endif
#elif HMLIB_TYPE_SIZE_SHORT==4
#	ifndef HMLIB_TYPE_SIZE_32
#		define HMLIB_TYPE_SIZE_32
		typedef unsigned short hmLib_uint32;
		typedef signed short hmLib_sint32;
#	endif
#endif
#if HMLIB_TYPE_SIZE_INT==1
#	ifndef HMLIB_TYPE_SIZE_8
#		define HMLIB_TYPE_SIZE_8
		typedef unsigned int hmLib_uint8;
		typedef signed int hmLib_sint8;
#	endif
#elif HMLIB_TYPE_SIZE_INT==2
#	ifndef HMLIB_TYPE_SIZE_16
#		define HMLIB_TYPE_SIZE_16
		typedef unsigned int hmLib_uint16;
		typedef signed int hmLib_sint16;
#	endif
#elif HMLIB_TYPE_SIZE_INT==4
#	ifndef HMLIB_TYPE_SIZE_32
#		define HMLIB_TYPE_SIZE_32
		typedef unsigned int hmLib_uint32;
		typedef signed int hmLib_sint32;
#	endif
#elif HMLIB_TYPE_SIZE_INT==8
#	ifndef HMLIB_TYPE_SIZE_64
#		define HMLIB_TYPE_SIZE_64
		typedef unsigned int hmLib_uint64;
		typedef signed int hmLib_sint64;
#	endif
#endif
#if HMLIB_TYPE_SIZE_LONG==1
#	ifndef HMLIB_TYPE_SIZE_8
#		define HMLIB_TYPE_SIZE_8
		typedef unsigned long hmLib_uint8;
		typedef signed long hmLib_sint8;
#	endif
#elif HMLIB_TYPE_SIZE_LONG==2
#	ifndef HMLIB_TYPE_SIZE_16
#		define HMLIB_TYPE_SIZE_16
		typedef unsigned long hmLib_uint16;
		typedef signed long hmLib_sint16;
#	endif
#elif HMLIB_TYPE_SIZE_LONG==4
#	ifndef HMLIB_TYPE_SIZE_32
#		define HMLIB_TYPE_SIZE_32
		typedef unsigned long hmLib_uint32;
		typedef signed long hmLib_sint32;
#	endif
#elif HMLIB_TYPE_SIZE_LONG==8
#	ifndef HMLIB_TYPE_SIZE_64
#		define HMLIB_TYPE_SIZE_64
		typedef unsigned long hmLib_uint64;
		typedef signed long hmLib_sint64;
#	endif
#elif HMLIB_TYPE_SIZE_LONG==16
#	ifndef HMLIB_TYPE_SIZE_128
#		define HMLIB_TYPE_SIZE_128
		typedef unsigned long hmLib_uint128;
		typedef signed long hmLib_sint128;
#	endif
#endif
#if HMLIB_TYPE_SIZE_LONG_LONG==1
#	ifndef HMLIB_TYPE_SIZE_8
#		define HMLIB_TYPE_SIZE_8
		typedef unsigned long long hmLib_uint8;
		typedef signed long long hmLib_sint8;
#	endif
#elif HMLIB_TYPE_SIZE_LONG_LONG==2
#	ifndef HMLIB_TYPE_SIZE_16
#		define HMLIB_TYPE_SIZE_16
		typedef unsigned long long hmLib_uint16;
		typedef signed long long hmLib_sint16;
#	endif
#elif HMLIB_TYPE_SIZE_LONG_LONG==4
#	ifndef HMLIB_TYPE_SIZE_32
#		define HMLIB_TYPE_SIZE_32
		typedef unsigned long long hmLib_uint32;
		typedef signed long long hmLib_sint32;
#	endif
#elif HMLIB_TYPE_SIZE_LONG_LONG==8
#	ifndef HMLIB_TYPE_SIZE_64
#		define HMLIB_TYPE_SIZE_64
		typedef unsigned long long hmLib_uint64;
		typedef signed long long hmLib_sint64;
#	endif
#elif HMLIB_TYPE_SIZE_LONG_LONG==16
#	ifndef HMLIB_TYPE_SIZE_128
#		define HMLIB_TYPE_SIZE_128
		typedef unsigned long long hmLib_uint128;
		typedef signed long long hmLib_sint128;
#	endif
#endif
#if HMLIB_TYPE_SIZE_FLOAT==1
#	ifndef HMLIB_TYPE_SIZE_f8
#		define HMLIB_TYPE_SIZE_f8
		typedef float hmLib_float8;
#	endif
#elif HMLIB_TYPE_SIZE_FLOAT==2
#	ifndef HMLIB_TYPE_SIZE_f16
#		define HMLIB_TYPE_SIZE_f16
		typedef float hmLib_float16;
#	endif
#elif HMLIB_TYPE_SIZE_FLOAT==4
#	ifndef HMLIB_TYPE_SIZE_f32
#		define HMLIB_TYPE_SIZE_f32
		typedef float hmLib_float32;
#	endif
#elif HMLIB_TYPE_SIZE_FLOAT==8
#	ifndef HMLIB_TYPE_SIZE_f64
#		define HMLIB_TYPE_SIZE_f64
		typedef float hmLib_float64;
#	endif
#elif HMLIB_TYPE_SIZE_FLOAT==16
#	ifndef HMLIB_TYPE_SIZE_f128
#		define HMLIB_TYPE_SIZE_f128
		typedef float hmLib_f128;
#	endif
#endif
#if HMLIB_TYPE_SIZE_DOUBLE==1
#	ifndef HMLIB_TYPE_SIZE_f8
#		define HMLIB_TYPE_SIZE_f8
		typedef double hmLib_float8;
#	endif
#elif HMLIB_TYPE_SIZE_DOUBLE==2
#	ifndef HMLIB_TYPE_SIZE_f16
#		define HMLIB_TYPE_SIZE_f16
		typedef double hmLib_float16;
#	endif
#elif HMLIB_TYPE_SIZE_DOUBLE==4
#	ifndef HMLIB_TYPE_SIZE_f32
#		define HMLIB_TYPE_SIZE_f32
		typedef double hmLib_float32;
#	endif
#elif HMLIB_TYPE_SIZE_DOUBLE==8
#	ifndef HMLIB_TYPE_SIZE_f64
#		define HMLIB_TYPE_SIZE_f64
		typedef double hmLib_float64;
#	endif
#elif HMLIB_TYPE_SIZE_DOUBLE==16
#	ifndef HMLIB_TYPE_SIZE_f128
#		define HMLIB_TYPE_SIZE_f128
		typedef double hmLib_f128;
#	endif
#endif
#if HMLIB_TYPE_SIZE_LONG_DOUBLE==1
#	ifndef HMLIB_TYPE_SIZE_f8
#		define HMLIB_TYPE_SIZE_f8
		typedef long double hmLib_float8;
#	endif
#elif HMLIB_TYPE_SIZE_LONG_DOUBLE==2
#	ifndef HMLIB_TYPE_SIZE_f16
#		define HMLIB_TYPE_SIZE_f16
		typedef long double hmLib_float16;
#	endif
#elif HMLIB_TYPE_SIZE_LONG_DOUBLE==4
#	ifndef HMLIB_TYPE_SIZE_f32
#		define HMLIB_TYPE_SIZE_f32
		typedef long double hmLib_float32;
#	endif
#elif HMLIB_TYPE_SIZE_LONG_DOUBLE==8
#	ifndef HMLIB_TYPE_SIZE_f64
#		define HMLIB_TYPE_SIZE_f64
		typedef long double hmLib_float64;
#	endif
#elif HMLIB_TYPE_SIZE_LONG_DOUBLE==16
#	ifndef HMLIB_TYPE_SIZE_f128
#		define HMLIB_TYPE_SIZE_f128
		typedef long double hmLib_float128;
#	endif
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
#ifdef HMLIB_TYPE_SIZE_8
typedef void(*hmLib_vFp_u8)(hmLib_uint8);
typedef void(*hmLib_vFp_s8)(hmLib_sint8);
typedef hmLib_uint8(*hmLib_u8Fp_v)(void);
typedef hmLib_sint8(*hmLib_s8Fp_v)(void);
#endif
#ifdef HMLIB_TYPE_SIZE_16
typedef void(*hmLib_vFp_u16)(hmLib_uint16);
typedef void(*hmLib_vFp_s16)(hmLib_sint16);
typedef hmLib_uint16(*hmLib_u16Fp_v)(void);
typedef hmLib_sint16(*hmLib_s16Fp_v)(void);
#endif
#ifdef HMLIB_TYPE_SIZE_32
typedef void(*hmLib_vFp_u32)(hmLib_uint32);
typedef void(*hmLib_vFp_s32)(hmLib_sint32);
typedef hmLib_uint32(*hmLib_u32Fp_v)(void);
typedef hmLib_sint32(*hmLib_s32Fp_v)(void);
#endif
#ifdef HMLIB_TYPE_SIZE_64
typedef void(*hmLib_vFp_u64)(hmLib_uint64);
typedef void(*hmLib_vFp_s64)(hmLib_sint64);
typedef hmLib_uint64(*hmLib_u64Fp_v)(void);
typedef hmLib_sint64(*hmLib_s64Fp_v)(void);
#endif
#ifdef HMLIB_TYPE_SIZE_128
typedef void(*hmLib_vFp_u128)(hmLib_uint128);
typedef void(*hmLib_vFp_s128)(hmLib_sint128);
typedef hmLib_uint128(*hmLib_u128Fp_v)(void);
typedef hmLib_sint128(*hmLib_s128Fp_v)(void);
#endif
#ifdef HMLIB_TYPE_SIZE_f32
typedef void(*hmLib_vFp_f32)(hmLib_float32);
typedef hmLib_float32(*hmLib_f32Fp_v)(void);
#endif
#ifdef HMLIB_TYPE_SIZE_f64
typedef void(*hmLib_vFp_f64)(hmLib_float64);
typedef hmLib_float64(*hmLib_f64Fp_v)(void);
#endif
#ifdef HMLIB_TYPE_SIZE_f128
typedef hmLib_f128 float128;
#endif

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
		#ifdef HMLIB_TYPE_SIZE_8
			typedef hmLib_uint8 uint8;
			typedef hmLib_sint8 sint8;

			typedef hmLib_vFp_u8 vFp_u8;
			typedef hmLib_u8Fp_v u8Fp_v;
			typedef hmLib_vFp_s8 vFp_s8;
			typedef hmLib_s8Fp_v s8Fp_v;
		#endif
		#ifdef HMLIB_TYPE_SIZE_16
			typedef hmLib_uint16 uint16;
			typedef hmLib_sint16 sint16;

			typedef hmLib_vFp_u16 vFp_u16;
			typedef hmLib_vFp_s16 vFp_s16;
			typedef hmLib_u16Fp_v u16Fp_v;
			typedef hmLib_s16Fp_v s16Fp_v;
		#endif
		#ifdef HMLIB_TYPE_SIZE_32
			typedef hmLib_uint32 uint32;
			typedef hmLib_sint32 sint32;

			typedef hmLib_vFp_u32 vFp_u32;
			typedef hmLib_vFp_s32 vFp_s32;
			typedef hmLib_u32Fp_v u32Fp_v;
			typedef hmLib_s32Fp_v s32Fp_v;
		#endif
		#ifdef HMLIB_TYPE_SIZE_64
			typedef hmLib_uint64 uint64;
			typedef hmLib_sint64 sint64;

			typedef hmLib_vFp_u64 vFp_u64;
			typedef hmLib_vFp_s64 vFp_s64;
			typedef hmLib_u64Fp_v u64Fp_v;
			typedef hmLib_s64Fp_v s64Fp_v;
		#endif
		#ifdef HMLIB_TYPE_SIZE_128
			typedef hmLib_uint128 uint128;
			typedef hmLib_sint128 sint128;

			typedef hmLib_vFp_u128 vFp_u128;
			typedef hmLib_vFp_s128 vFp_s128;
			typedef hmLib_u128Fp_v u128Fp_v;
			typedef hmLib_s128Fp_v s128Fp_v;
		#endif
		#ifdef HMLIB_TYPE_SIZE_f32
			typedef hmLib_float32 float32;

			typedef hmLib_vFp_f32 vFp_f32;
			typedef hmLib_f32Fp_v f32Fp_v;
		#endif
		#ifdef HMLIB_TYPE_SIZE_f64
			typedef hmLib_float64 float64;

			typedef hmLib_vFp_f64 vFp_f64;
			typedef hmLib_f64Fp_v f64Fp_v;
		#endif
		#ifdef HMLIB_TYPE_SIZE_f128
			typedef hmLib_f128 float128;
		#endif

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
#ifdef HMLIB_TYPE_SIZE_8
typedef hmLib_uint8 uint8;
typedef hmLib_sint8 sint8;

typedef hmLib_vFp_u8 vFp_u8;
typedef hmLib_u8Fp_v u8Fp_v;
typedef hmLib_vFp_s8 vFp_s8;
typedef hmLib_s8Fp_v s8Fp_v;
#endif
#ifdef HMLIB_TYPE_SIZE_16
typedef hmLib_uint16 uint16;
typedef hmLib_sint16 sint16;

typedef hmLib_vFp_u16 vFp_u16;
typedef hmLib_vFp_s16 vFp_s16;
typedef hmLib_u16Fp_v u16Fp_v;
typedef hmLib_s16Fp_v s16Fp_v;
#endif
#ifdef HMLIB_TYPE_SIZE_32
typedef hmLib_uint32 uint32;
typedef hmLib_sint32 sint32;

typedef hmLib_vFp_u32 vFp_u32;
typedef hmLib_vFp_s32 vFp_s32;
typedef hmLib_u32Fp_v u32Fp_v;
typedef hmLib_s32Fp_v s32Fp_v;
#endif
#ifdef HMLIB_TYPE_SIZE_64
typedef hmLib_uint64 uint64;
typedef hmLib_sint64 sint64;

typedef hmLib_vFp_u64 vFp_u64;
typedef hmLib_vFp_s64 vFp_s64;
typedef hmLib_u64Fp_v u64Fp_v;
typedef hmLib_s64Fp_v s64Fp_v;
#endif
#ifdef HMLIB_TYPE_SIZE_128
typedef hmLib_uint128 uint128;
typedef hmLib_sint128 sint128;

typedef hmLib_vFp_u128 vFp_u128;
typedef hmLib_vFp_s128 vFp_s128;
typedef hmLib_u128Fp_v u128Fp_v;
typedef hmLib_s128Fp_v s128Fp_v;
#endif
#ifdef HMLIB_TYPE_SIZE_f32
typedef hmLib_float32 float32;

typedef hmLib_vFp_f32 vFp_f32;
typedef hmLib_f32Fp_v f32Fp_v;
#endif
#ifdef HMLIB_TYPE_SIZE_f64
typedef hmLib_float64 float64;

typedef hmLib_vFp_f64 vFp_f64;
typedef hmLib_f64Fp_v f64Fp_v;
#endif
#ifdef HMLIB_TYPE_SIZE_f128
typedef hmLib_f128 float128;
#endif
#endif

#
#endif
