#ifndef INC_HMLIB_BYTEBASETYPE
#define INC_HMLIB_BYTEBASETYPE 100
#ifndef INC_HMLIB_STANDARDTYPE
#	include "hmLib_v2/standard_type.hpp"
#endif
#
namespace hmLib{
	namespace type{
		namespace{
			#//StdTypeSize
			#if TYPE_SIZE_CHAR==1
			#	ifndef TYPE_SIZE_8
			#		define TYPE_SIZE_8
					typedef unsigned char u8;
					typedef signed char s8;
			#	endif
			#elif TYPE_SIZE_CHAR==2
			#	ifndef TYPE_SIZE_16
			#		define TYPE_SIZE_16
					typedef unsigned char u16;
					typedef signed char s16;
			#	endif
			#endif
			#if TYPE_SIZE_SHORT==1
			#	ifndef TYPE_SIZE_8
			#		define TYPE_SIZE_8
					typedef unsigned short u8;
					typedef signed short s8;
			#	endif
			#elif TYPE_SIZE_SHORT==2
			#	ifndef TYPE_SIZE_16
			#		define TYPE_SIZE_16
					typedef unsigned short u16;
					typedef signed short s16;
			#	endif
			#elif TYPE_SIZE_SHORT==4
			#	ifndef TYPE_SIZE_32
			#		define TYPE_SIZE_32
					typedef unsigned short u32;
					typedef signed short s32;
			#	endif
			#endif
			#if TYPE_SIZE_INT==1
			#	ifndef TYPE_SIZE_8
			#		define TYPE_SIZE_8
					typedef unsigned int u8;
					typedef signed int s8;
			#	endif
			#elif TYPE_SIZE_INT==2
			#	ifndef TYPE_SIZE_16
			#		define TYPE_SIZE_16
					typedef unsigned int u16;
					typedef signed int s16;
			#	endif
			#elif TYPE_SIZE_INT==4
			#	ifndef TYPE_SIZE_32
			#		define TYPE_SIZE_32
					typedef unsigned int u32;
					typedef signed int s32;
			#	endif
			#elif TYPE_SIZE_INT==8
			#	ifndef TYPE_SIZE_64
			#		define TYPE_SIZE_64
					typedef unsigned int u64;
					typedef signed int s64;
			#	endif
			#endif
			#if TYPE_SIZE_LONG==1
			#	ifndef TYPE_SIZE_8
			#		define TYPE_SIZE_8
					typedef unsigned long u8;
					typedef signed long s8;
			#	endif
			#elif TYPE_SIZE_LONG==2
			#	ifndef TYPE_SIZE_16
			#		define TYPE_SIZE_16
					typedef unsigned long u16;
					typedef signed long s16;
			#	endif
			#elif TYPE_SIZE_LONG==4
			#	ifndef TYPE_SIZE_32
			#		define TYPE_SIZE_32
					typedef unsigned long u32;
					typedef signed long s32;
			#	endif
			#elif TYPE_SIZE_LONG==8
			#	ifndef TYPE_SIZE_64
			#		define TYPE_SIZE_64
					typedef unsigned long u64;
					typedef signed long s64;
			#	endif
			#elif TYPE_SIZE_LONG==16
			#	ifndef TYPE_SIZE_128
			#		define TYPE_SIZE_128
					typedef unsigned long u128;
					typedef signed long s128;
			#	endif
			#endif
			#if TYPE_SIZE_LONG_LONG==1
			#	ifndef TYPE_SIZE_8
			#		define TYPE_SIZE_8
					typedef unsigned long long u8;
					typedef signed long long s8;
			#	endif
			#elif TYPE_SIZE_LONG_LONG==2
			#	ifndef TYPE_SIZE_16
			#		define TYPE_SIZE_16
					typedef unsigned long long u16;
					typedef signed long long s16;
			#	endif
			#elif TYPE_SIZE_LONG_LONG==4
			#	ifndef TYPE_SIZE_32
			#		define TYPE_SIZE_32
					typedef unsigned long long u32;
					typedef signed long long s32;
			#	endif
			#elif TYPE_SIZE_LONG_LONG==8
			#	ifndef TYPE_SIZE_64
			#		define TYPE_SIZE_64
					typedef unsigned long long u64;
					typedef signed long long s64;
			#	endif
			#elif TYPE_SIZE_LONG_LONG==16
			#	ifndef TYPE_SIZE_128
			#		define TYPE_SIZE_128
					typedef unsigned long long u128;
					typedef signed long long s128;
			#	endif
			#endif
			#if TYPE_SIZE_FLOAT==1
			#	ifndef TYPE_SIZE_f8
			#		define TYPE_SIZE_f8
					typedef float f8;
			#	endif
			#elif TYPE_SIZE_FLOAT==2
			#	ifndef TYPE_SIZE_f16
			#		define TYPE_SIZE_f16
					typedef float f16;
			#	endif
			#elif TYPE_SIZE_FLOAT==4
			#	ifndef TYPE_SIZE_f32
			#		define TYPE_SIZE_f32
					typedef float f32;
			#	endif
			#elif TYPE_SIZE_FLOAT==8
			#	ifndef TYPE_SIZE_f64
			#		define TYPE_SIZE_f64
					typedef float f64;
			#	endif
			#elif TYPE_SIZE_FLOAT==16
			#	ifndef TYPE_SIZE_f128
			#		define TYPE_SIZE_f128
					typedef float f128;
			#	endif
			#endif
			#if TYPE_SIZE_DOUBLE==1
			#	ifndef TYPE_SIZE_f8
			#		define TYPE_SIZE_f8
					typedef double f8;
			#	endif
			#elif TYPE_SIZE_DOUBLE==2
			#	ifndef TYPE_SIZE_f16
			#		define TYPE_SIZE_f16
					typedef double f16;
			#	endif
			#elif TYPE_SIZE_DOUBLE==4
			#	ifndef TYPE_SIZE_f32
			#		define TYPE_SIZE_f32
					typedef double f32;
			#	endif
			#elif TYPE_SIZE_DOUBLE==8
			#	ifndef TYPE_SIZE_f64
			#		define TYPE_SIZE_f64
					typedef double f64;
			#	endif
			#elif TYPE_SIZE_DOUBLE==16
			#	ifndef TYPE_SIZE_f128
			#		define TYPE_SIZE_f128
					typedef double f128;
			#	endif
			#endif
			#if TYPE_SIZE_LONG_DOUBLE==1
			#	ifndef TYPE_SIZE_f8
			#		define TYPE_SIZE_f8
					typedef long double f8;
			#	endif
			#elif TYPE_SIZE_LONG_DOUBLE==2
			#	ifndef TYPE_SIZE_f16
			#		define TYPE_SIZE_f16
					typedef long double f16;
			#	endif
			#elif TYPE_SIZE_LONG_DOUBLE==4
			#	ifndef TYPE_SIZE_f32
			#		define TYPE_SIZE_f32
					typedef long double f32;
			#	endif
			#elif TYPE_SIZE_LONG_DOUBLE==8
			#	ifndef TYPE_SIZE_f64
			#		define TYPE_SIZE_f64
					typedef long double f64;
			#	endif
			#elif TYPE_SIZE_LONG_DOUBLE==16
			#	ifndef TYPE_SIZE_f128
			#		define TYPE_SIZE_f128
					typedef long double f128;
			#	endif
			#endif
		}
		#//hmStd Type
		#ifdef TYPE_SIZE_8
			typedef u8 uint8;
			typedef s8 sint8;
		#endif
		#ifdef TYPE_SIZE_16
			typedef u16 uint16;
			typedef s16 sint16;
		#endif
		#ifdef TYPE_SIZE_32
			typedef u32 uint32;
			typedef s32 sint32;
		#endif
		#ifdef TYPE_SIZE_64
			typedef u64 uint64;
			typedef s64 sint64;
		#endif
		#ifdef TYPE_SIZE_128
			typedef u128 uint128;
			typedef s128 sint128;
		#endif
		#ifdef TYPE_SIZE_f32
			typedef f32 float32;
		#endif
		#ifdef TYPE_SIZE_f64
			typedef f64 float64;
		#endif
		#ifdef TYPE_SIZE_f128
			typedef f128 float128;
		#endif
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
	}
	#if defined TYPE_SIZE_f128
		const type::float128 PI=3.141592653589793;
		inline type::float128 pi(){return 3.141592653589793;}
	#elif defined TYPE_SIZE_f64
		//‰~Žü—¦pi
		const type::float64 PI=3.141592653589793;
		inline type::float64 pi(){return 3.141592653589793;}
	#elif defined TYPE_SIZE_f32
		const type::float32 PI=3.141592653589793;
		inline type::float32 pi(){return 3.141592653589793;}
	#elif defined TYPE_SIZE_f16
		const type::float16 PI=3.141592653589793;
		inline type::float16 pi(){return 3.141592653589793;}
	#endif
}
#
#endif
