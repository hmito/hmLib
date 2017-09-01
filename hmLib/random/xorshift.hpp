﻿#ifndef HMLIB_RANDOM_XORSHIFT
#define HMLIB_RANDOM_XORSHIFT 100
#
#include<limits>
#include<vector>
#include<iterator>
namespace hmLib{
	/*!
	@brief Random engine class using algorithm XOrShift.
	This random value generating algorithm is faster than mt, and generating better random sequences than linear congruential engines.
	@tparam result_type_ Terget type for generating random values
	@tparam xval 1st default engine parameter
	@tparam yval 2nd default engine parameter
	@tparam zval 3rd default engine parameter
	@tparam default_seedval default seed value*/
	template<typename result_type_, result_type_ xval, result_type_ yval, result_type_ zval, result_type_ default_seedval>
	class xorshift_engine{
	public:
		using result_type = result_type_;
	public:
		//!@brief Return min value generated by this engine
		static constexpr result_type min(){ return std::numeric_limits<result_type>::min(); }
		//!@brief Return max value generated by this engine
		static constexpr result_type max(){ return std::numeric_limits<result_type>::max(); }
	public:
		//!@brief Constructor with set seed value by default_seedval.
		xorshift_engine()
			: x(xval)
			, y(yval)
			, z(zval)
			, w(default_seedval){}
		/*!
		@brief Constructor with set seed value by given value.
		@param[in] s seed value*/
		xorshift_engine(result_type s)
			: x(xval)
			, y(yval)
			, z(zval)
			, w(s){}
		template<class Sseq>
		xorshift_engine(Sseq&& q)
			: x(xval)
			, y(yval)
			, z(zval)
			, w(0){
			seed(q);
		}
		//!@brief Copy contstructor.
		xorshift_engine(const xorshift_engine& e) = default;
		//!@brief Move contstructor.
		xorshift_engine(xorshift_engine&& e) = default;
		/*!
		@brief Generate random value.
		@return Generated random value.*/
		result_type operator()(){
			result_type t = x ^ (x << 11);
			x = y;
			y = z;
			z = w;
			return w = (w ^ (w >> 19)) ^ (t ^ (t >> 8));
		}
		/*!
		@brief Set seed value of the engine.
		@param[in] s seed value*/
		void seed(result_type s = default_seedval){ w = s; }
		template<class Sseq>
		void seed(Sseq&& q){
			// シード列を取得
			std::vector<result_type> result;
			q.param(std::back_inserter(result));
			w = result.front();
		}
		void discard(unsigned long long z){
			if(z == 0)return;
			while(z-->0){
				operator()();
			}
		}
	private:
		result_type x;
		result_type y;
		result_type z;
		result_type w;
	};
	//!@brief Pre-set default xorshift engine for unsigned int.
	using xorshift128 = xorshift_engine<unsigned int, 123456789u, 362436069u, 521288629u, 1u>;
}
#
#endif