#ifndef INC_HMLIB_HMRAND
#define INC_HMLIB_HMRAND 100
#ifndef SFMT_H
#define SFMT_H

#include <stdio.h>
#include <cmath>

#if defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 199901L)
  #include <inttypes.h>
#elif defined(_MSC_VER) || defined(__BORLANDC__)
  typedef unsigned int uint32_t;
  typedef unsigned __int64 uint64_t;
  #define inline __inline
#else
  #include <inttypes.h>
  #if defined(__GNUC__)
	#define inline __inline__
  #endif
#endif

#ifndef PRIu64
  #if defined(_MSC_VER) || defined(__BORLANDC__)
	#define SFMT_PRIu64 "I64u"
	#define SFMT_PRIx64 "I64x"
  #else
	#define SFMT_PRIu64 "llu"
	#define SFMT_PRIx64 "llx"
  #endif
#endif

#if defined(__GNUC__)
#define SFMT_ALWAYSINLINE __attribute__((always_inline))
#else
#define SFMT_ALWAYSINLINE
#endif

#if defined(_MSC_VER)
  #if _MSC_VER >= 1200
	#define SFMT_PRE_ALWAYS __forceinline
  #else
	#define SFMT_PRE_ALWAYS inline
  #endif
#else
  #define SFMT_PRE_ALWAYS inline
#endif
/** generates a random number on [0,1]-real-interval */
inline static double to_real1(uint32_t v)
{
	return v * (1.0/4294967295.0); 
	/* divided by 2^32-1 */ 
}

/** generates a random number on [0,1)-real-interval */
inline static double to_real2(uint32_t v)
{
	return v * (1.0/4294967296.0); 
	/* divided by 2^32 */
}

/** generates a random number on (0,1)-real-interval */
inline static double to_real3(uint32_t v)
{
	return (((double)v) + 0.5)*(1.0/4294967296.0); 
	/* divided by 2^32 */
}

/** generates a random number on [0,1) with 53-bit resolution*/
inline static double to_res53(uint64_t v) 
{ 
	return v * (1.0/18446744073709551616.0L);
}

/** generates a random number on [0,1) with 53-bit resolution from two * 32 bit integers */
inline static double to_res53_mix(uint32_t x, uint32_t y) 
{ 
	return to_res53(x | ((uint64_t)y << 32));
}

#endif
#include <string.h>
#include <assert.h>
#include "hmLib_v2/hmRand/hmSFMT/SFMT-params.h"
#ifndef
#	include "hmLib_v2/hmBasic.h"
#endif
#if defined(__BIG_ENDIAN__) && !defined(__amd64) && !defined(SFMT_BIG_ENDIAN64)
#define SFMT_BIG_ENDIAN64 1
#endif
#if defined(HAVE_ALTIVEC) && !defined(SFMT_BIG_ENDIAN64)
#define SFMT_BIG_ENDIAN64 1
#endif
#if defined(ONLY64) && !defined(SFMT_BIG_ENDIAN64)
  #if defined(__GNUC__)
    #error "-DONLY64 must be specified with -DBIG_ENDIAN64"
  #endif
#undef ONLY64
#endif
#if defined(HAVE_ALTIVEC)
  #if !defined(__APPLE__)
    #include <altivec.h>
  #endif
/** 128-bit data structure */
union W128_T {
    vector unsigned int s;
    uint32_t u[4];
};
/** 128-bit data type */
typedef union W128_T w128_t;

#elif defined(HAVE_SSE2)
  #include <emmintrin.h>

/** 128-bit data structure */
union W128_T {
    __m128i si;
    uint32_t u[4];
};
/** 128-bit data type */
typedef union W128_T w128_t;

#else
struct W128_T {
    uint32_t u[4];
};
typedef struct W128_T w128_t;
#endif
#if defined(HAVE_ALTIVEC)
  #include "hmLib_v2/hmRand/hmSFMT/SFMT-alti.h"
#elif defined(HAVE_SSE2)
  #include "hmLib_v2/hmRand/hmSFMT/SFMT-sse2.h"
#endif
class hmRand{
private:
	w128_t sfmt[SFMT_N];
	uint32_t *psfmt32;
	#if !defined(SFMT_BIG_ENDIAN64) || defined(ONLY64)
	uint64_t *psfmt64;
	#endif
	int idx;
	int initialized;
	uint32_t parity[4];
	int idxof(int i){
		#ifdef ONLY64
		return i ^ 1;
		#else
		return i;
		#endif
	}
	void rshift128(w128_t *out,  w128_t const *in, int shift){
		#ifdef ONLY64
    uint64_t th, tl, oh, ol;

    th = ((uint64_t)in->u[2] << 32) | ((uint64_t)in->u[3]);
    tl = ((uint64_t)in->u[0] << 32) | ((uint64_t)in->u[1]);

    oh = th >> (shift * 8);
    ol = tl >> (shift * 8);
    ol |= th << (64 - shift * 8);
    out->u[0] = (uint32_t)(ol >> 32);
    out->u[1] = (uint32_t)ol;
    out->u[2] = (uint32_t)(oh >> 32);
    out->u[3] = (uint32_t)oh;
		#else
    uint64_t th, tl, oh, ol;

    th = ((uint64_t)in->u[3] << 32) | ((uint64_t)in->u[2]);
    tl = ((uint64_t)in->u[1] << 32) | ((uint64_t)in->u[0]);

    oh = th >> (shift * 8);
    ol = tl >> (shift * 8);
    ol |= th << (64 - shift * 8);
    out->u[1] = (uint32_t)(ol >> 32);
    out->u[0] = (uint32_t)ol;
    out->u[3] = (uint32_t)(oh >> 32);
    out->u[2] = (uint32_t)oh;
		#endif
	}
	void lshift128(w128_t *out,  w128_t const *in, int shift){
		#ifdef ONLY64
		uint64_t th, tl, oh, ol;

		th = ((uint64_t)in->u[2] << 32) | ((uint64_t)in->u[3]);
		tl = ((uint64_t)in->u[0] << 32) | ((uint64_t)in->u[1]);

		oh = th << (shift * 8);
		ol = tl << (shift * 8);
		oh |= tl >> (64 - shift * 8);
		out->u[0] = (uint32_t)(ol >> 32);
		out->u[1] = (uint32_t)ol;
		out->u[2] = (uint32_t)(oh >> 32);
		out->u[3] = (uint32_t)oh;
		#else
		uint64_t th, tl, oh, ol;

		th = ((uint64_t)in->u[3] << 32) | ((uint64_t)in->u[2]);
		tl = ((uint64_t)in->u[1] << 32) | ((uint64_t)in->u[0]);

		oh = th << (shift * 8);
		ol = tl << (shift * 8);
		oh |= tl >> (64 - shift * 8);
		out->u[1] = (uint32_t)(ol >> 32);
		out->u[0] = (uint32_t)ol;
		out->u[3] = (uint32_t)(oh >> 32);
		out->u[2] = (uint32_t)oh;
		#endif
	}
	#if (!defined(HAVE_ALTIVEC)) && (!defined(HAVE_SSE2))
	void gen_rand_all(void) {
		int i;
		w128_t *r1, *r2;

		r1 = &sfmt[SFMT_N - 2];
		r2 = &sfmt[SFMT_N - 1];
		for (i = 0; i < SFMT_N - POS1; i++) {
		do_recursion(&sfmt[i], &sfmt[i], &sfmt[i + POS1], r1, r2);
		r1 = r2;
		r2 = &sfmt[i];
		}
		for (; i < SFMT_N; i++) {
		do_recursion(&sfmt[i], &sfmt[i], &sfmt[i + POS1 - SFMT_N], r1, r2);
		r1 = r2;
		r2 = &sfmt[i];
		}
	}

	void gen_rand_array(w128_t *array, int size) {
		int i, j;
		w128_t *r1, *r2;

		r1 = &sfmt[SFMT_N - 2];
		r2 = &sfmt[SFMT_N - 1];
		for (i = 0; i < SFMT_N - POS1; i++) {
		do_recursion(&array[i], &sfmt[i], &sfmt[i + POS1], r1, r2);
		r1 = r2;
		r2 = &array[i];
		}
		for (; i < SFMT_N; i++) {
		do_recursion(&array[i], &sfmt[i], &array[i + POS1 - SFMT_N], r1, r2);
		r1 = r2;
		r2 = &array[i];
		}
		for (; i < size - SFMT_N; i++) {
		do_recursion(&array[i], &array[i - SFMT_N], &array[i + POS1 - SFMT_N], r1, r2);
		r1 = r2;
		r2 = &array[i];
		}
		for (j = 0; j < 2 * SFMT_N - size; j++) {
		sfmt[j] = array[j + size - SFMT_N];
		}
		for (; i < size; i++, j++) {
		do_recursion(&array[i], &array[i - SFMT_N], &array[i + POS1 - SFMT_N], r1, r2);
		r1 = r2;
		r2 = &array[i];
		sfmt[j] = array[i];
		}
	}
	#endif
	uint32_t func1(uint32_t x){return (x ^ (x >> 27)) * (uint32_t)1664525UL;}
	uint32_t func2(uint32_t x){return (x ^ (x >> 27)) * (uint32_t)1566083941UL;}
	void period_certification(void){
		int inner = 0;
		int i, j;
		uint32_t work;

		for (i = 0; i < 4; i++)
		inner ^= psfmt32[idxof(i)] & parity[i];
		for (i = 16; i > 0; i >>= 1)
		inner ^= inner >> i;
		inner &= 1;
		/* check OK */
		if (inner == 1) {
		return;
		}
		/* check NG, and modification */
		for (i = 0; i < 4; i++) {
		work = 1;
		for (j = 0; j < 32; j++) {
			if ((work & parity[i]) != 0) {
			psfmt32[idxof(i)] ^= work;
			return;
			}
			work = work << 1;
		}
		}
	}
	#if defined(SFMT_BIG_ENDIAN64) && !defined(ONLY64) && !defined(HAVE_ALTIVEC)
	void swap(w128_t *array, int size){
			int i;
			uint32_t x, y;
			for (i = 0; i < size; i++) {
				x = array[i].u[0];
				y = array[i].u[2];
				array[i].u[0] = array[i].u[1];
				array[i].u[2] = array[i].u[3];
				array[i].u[1] = x;
				array[i].u[3] = y;
			}
	}
	#endif
	#if (!defined(HAVE_ALTIVEC)) && (!defined(HAVE_SSE2))
	#ifdef ONLY64
	void do_recursion(w128_t *r, w128_t *a, w128_t *b, w128_t *c,
					w128_t *d) {
		w128_t x;
		w128_t y;

		lshift128(&x, a, SL2);
		rshift128(&y, c, SR2);
		r->u[0] = a->u[0] ^ x.u[0] ^ ((b->u[0] >> SR1) & MSK2) ^ y.u[0] 
		^ (d->u[0] << SL1);
		r->u[1] = a->u[1] ^ x.u[1] ^ ((b->u[1] >> SR1) & MSK1) ^ y.u[1] 
		^ (d->u[1] << SL1);
		r->u[2] = a->u[2] ^ x.u[2] ^ ((b->u[2] >> SR1) & MSK4) ^ y.u[2] 
		^ (d->u[2] << SL1);
		r->u[3] = a->u[3] ^ x.u[3] ^ ((b->u[3] >> SR1) & MSK3) ^ y.u[3] 
		^ (d->u[3] << SL1);
	}
	#else
	void do_recursion(w128_t *r, w128_t *a, w128_t *b, w128_t *c,
					w128_t *d) {
		w128_t x;
		w128_t y;

		lshift128(&x, a, SL2);
		rshift128(&y, c, SR2);
		r->u[0] = a->u[0] ^ x.u[0] ^ ((b->u[0] >> SR1) & MSK1) ^ y.u[0] 
		^ (d->u[0] << SL1);
		r->u[1] = a->u[1] ^ x.u[1] ^ ((b->u[1] >> SR1) & MSK2) ^ y.u[1] 
		^ (d->u[1] << SL1);
		r->u[2] = a->u[2] ^ x.u[2] ^ ((b->u[2] >> SR1) & MSK3) ^ y.u[2] 
		^ (d->u[2] << SL1);
		r->u[3] = a->u[3] ^ x.u[3] ^ ((b->u[3] >> SR1) & MSK4) ^ y.u[3] 
		^ (d->u[3] << SL1);
	}
	#endif
	#endif
public:
	hmRand(){
		psfmt32 = &sfmt[0].u[0];
		#if !defined(SFMT_BIG_ENDIAN64) || defined(ONLY64)
		psfmt64 = (uint64_t *)&sfmt[0].u[0];
		#endif
		initialized = 0;
		parity[0]=PARITY1;
		parity[1]=PARITY2;
		parity[2]=PARITY3;
		parity[3]=PARITY4;

		init_gen_rand((uint)rand());
	}
	hmRand(uint32_t seed){
		psfmt32 = &sfmt[0].u[0];
		#if !defined(SFMT_BIG_ENDIAN64) || defined(ONLY64)
		psfmt64 = (uint64_t *)&sfmt[0].u[0];
		#endif
		initialized = 0;
		parity[0]=PARITY1;
		parity[1]=PARITY2;
		parity[2]=PARITY3;
		parity[3]=PARITY4;

		init_gen_rand(seed);
	}
	uint64_t gen_rand64(void) {
		#if defined(SFMT_BIG_ENDIAN64) && !defined(ONLY64)
		uint32_t r1, r2;
		#else
		uint64_t r;
		#endif

		assert(initialized);
		assert(idx % 2 == 0);

		if (idx >= SFMT_N32) {
			gen_rand_all();
			idx = 0;
		}
		#if defined(SFMT_BIG_ENDIAN64) && !defined(ONLY64)
		r1 = psfmt32[idx];
		r2 = psfmt32[idx + 1];
		idx += 2;
		return ((uint64_t)r2 << 32) | r1;
		#else
		r = psfmt64[idx / 2];
		idx += 2;
		return r;
		#endif
	}

	#ifndef ONLY64
	uint32_t gen_rand32(void) {
		uint32_t r;

		assert(initialized);
		if (idx >= SFMT_N32) {
		gen_rand_all();
		idx = 0;
		}
		r = psfmt32[idx++];
		return r;
	}
	void fill_array32(uint32_t *array, int size) {
		assert(initialized);
		assert(idx == SFMT_N32);
		assert(size % 4 == 0);
		assert(size >= SFMT_N32);

		gen_rand_array((w128_t *)array, size / 4);
		idx = SFMT_N32;
	}
	#endif
	void fill_array64(uint64_t *array, int size){
		assert(initialized);
		assert(idx == SFMT_N32);
		assert(size % 2 == 0);
		assert(size >= SFMT_N64);

		gen_rand_array((w128_t *)array, size / 2);
		idx = SFMT_N32;

		#if defined(SFMT_BIG_ENDIAN64) && !defined(ONLY64)
		swap((w128_t *)array, size /2);
		#endif
	}
	void init_gen_rand(uint32_t seed) {
		int i;

		psfmt32[idxof(0)] = seed;
		for (i = 1; i < SFMT_N32; i++) {
		psfmt32[idxof(i)] = 1812433253UL * (psfmt32[idxof(i - 1)] 
							^ (psfmt32[idxof(i - 1)] >> 30))
			+ i;
		}
		idx = SFMT_N32;
		period_certification();
		initialized = 1;
	}
	void init_by_array(uint32_t *init_key, int key_length) {
		int i, j, count;
		uint32_t r;
		int lag;
		int mid;
		int size = SFMT_N * 4;

		if (size >= 623) {
		lag = 11;
		} else if (size >= 68) {
		lag = 7;
		} else if (size >= 39) {
		lag = 5;
		} else {
		lag = 3;
		}
		mid = (size - lag) / 2;

		memset(sfmt, 0x8b, sizeof(sfmt));
		if (key_length + 1 > SFMT_N32) {
		count = key_length + 1;
		} else {
		count = SFMT_N32;
		}
		r = func1(psfmt32[idxof(0)] ^ psfmt32[idxof(mid)] 
			  ^ psfmt32[idxof(SFMT_N32 - 1)]);
		psfmt32[idxof(mid)] += r;
		r += key_length;
		psfmt32[idxof(mid + lag)] += r;
		psfmt32[idxof(0)] = r;

		count--;
		for (i = 1, j = 0; (j < count) && (j < key_length); j++) {
		r = func1(psfmt32[idxof(i)] ^ psfmt32[idxof((i + mid) % SFMT_N32)] 
			  ^ psfmt32[idxof((i + SFMT_N32 - 1) % SFMT_N32)]);
		psfmt32[idxof((i + mid) % SFMT_N32)] += r;
		r += init_key[j] + i;
		psfmt32[idxof((i + mid + lag) % SFMT_N32)] += r;
		psfmt32[idxof(i)] = r;
		i = (i + 1) % SFMT_N32;
		}
		for (; j < count; j++) {
		r = func1(psfmt32[idxof(i)] ^ psfmt32[idxof((i + mid) % SFMT_N32)] 
			  ^ psfmt32[idxof((i + SFMT_N32 - 1) % SFMT_N32)]);
		psfmt32[idxof((i + mid) % SFMT_N32)] += r;
		r += i;
		psfmt32[idxof((i + mid + lag) % SFMT_N32)] += r;
		psfmt32[idxof(i)] = r;
		i = (i + 1) % SFMT_N32;
		}
		for (j = 0; j < SFMT_N32; j++) {
		r = func2(psfmt32[idxof(i)] + psfmt32[idxof((i + mid) % SFMT_N32)] 
			  + psfmt32[idxof((i + SFMT_N32 - 1) % SFMT_N32)]);
		psfmt32[idxof((i + mid) % SFMT_N32)] ^= r;
		r -= i;
		psfmt32[idxof((i + mid + lag) % SFMT_N32)] ^= r;
		psfmt32[idxof(i)] = r;
		i = (i + 1) % SFMT_N32;
		}

		idx = SFMT_N32;
		period_certification();
		initialized = 1;
	}
	const char *get_idstring(void){return IDSTR;}
	int get_min_array_size32(void){return SFMT_N32;}
	int get_min_array_size64(void){return SFMT_N64;}
	/** generates a random number on [0,1]-real-interval */
	double genrand_real1(void){return to_real1(gen_rand32());}
	/** generates a random number on [0,1)-real-interval */
	double genrand_real2(void){return to_real2(gen_rand32());}
	/** generates a random number on (0,1)-real-interval */
	double genrand_real3(void){return to_real3(gen_rand32());}
	/** generates a random number on [0,1) with 53-bit resolution*/
	double genrand_res53(void) { return to_res53(gen_rand64());} 
	/** generates a random number on [0,1) with 53-bit resolution using 32bit integer.*/
	double genrand_res53_mix(void) 
	{ 
		uint32_t x, y;

		x = gen_rand32();
		y = gen_rand32();
		return to_res53_mix(x, y);
	} 
	double genrand_norm(void){
		double tmp,tmp2;
		tmp=1.-genrand_real2();
		tmp2=1.-genrand_real2();
		return sqrt(log(tmp)*-2.)*sin(2*pi()*tmp2);
	}
};
namespace hm{
	extern hmRand rand;
}
#endif
