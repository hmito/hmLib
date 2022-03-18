#ifndef HMLIB_MATH_BOUNDARY_INC
#define HMLIB_MATH_BOUNDARY_INC 100
#
#include<functional>
#include<cmath>
#include"constants.hpp"
namespace hmLib{
	template<typename value_type_, typename fvalue_type_ = value_type_>
	struct lower_sine_boundary{
		using value_type = value_type_;
		using fvalue_type = fvalue_type_;
	private:
		std::function<fvalue_type(value_type)> f;
		value_type lower;
		value_type inner_width;
		value_type outer_width;
		fvalue_type outer_strength;
	public:
		template<typenma func>
		lower_sine_boundary(func&& F, value_type lower_, value_type inner_width_, value_type outer_width_, fvalue_type outer_strength_)
			: f(std::forward<func>(F))
			, lower(lower_)
			, inner_width(inner_width_)
			, outer_width(outer_width_)
			, outer_strength(outer_strength_){
		}
		template<typenma func>
		lower_sine_boundary(func&& F, value_type lower_, value_type width_, fvalue_type outer_strength_)
			: f(std::forward<func>(F))
			, lower(lower_)
			, inner_width(width_)
			, outer_width(width_)
			, outer_strength(outer_strength_){
		}
		fvalue_type operator()(const value_type& x)const{
			if(x<=lower+inner_width){
				if(x>lower){
					return (1-cos((x-lower)/inner_width*math::pi<value_type>()))/2*f(x);
				}else if(x>lower-outer_width){
					return (1-cos((x-lower)/outer_width*math::pi<value_type>()))/2*outer_strength;
				}else{
					return outer_strength;
				}
			}
			return(f(x));
		}
	};
	template<typename value_type_, typename fvalue_type_ = value_type_>
	struct upper_sine_boundary{
		using value_type = value_type_;
		using fvalue_type = fvalue_type_;
	private:
		std::function<fvalue_type(value_type)> f;
		value_type upper;
		value_type inner_width;
		value_type outer_width;
		fvalue_type outer_strength;
	public:
		template<typenma func>
		upper_sine_boundary(func&& F, value_type upper_, value_type inner_width_, value_type outer_width_, fvalue_type outer_strength_)
			: f(std::forward<func>(F))
			, upper(upper_)
			, inner_width(inner_width_)
			, outer_width(outer_width_)
			, outer_strength(outer_strength_){
		}
		template<typenma func>
		upper_sine_boundary(func&& F, value_type upper_, value_type width_, fvalue_type outer_strength_)
			: f(std::forward<func>(F))
			, upper(upper_)
			, inner_width(width_)
			, outer_width(width_)
			, outer_strength(outer_strength_){
		}
		fvalue_type operator()(const value_type& x)const{
			if(x>=upper-inner_width){
				if(x<upper){
					return (1-cos((x-upper)/inner_width*math::pi<value_type>()))/2*f(x);
				}else if(x<upper+outer_width){
					return -(1-cos((x-upper)/outer_width*math::pi<value_type>()))/2*outer_strength;
				}else{
					return -outer_strength;
				}
			}

			return(f(x));

		}
	};
	template<typename value_type_, typename fvalue_type_ = value_type_>
	struct range_sine_boundary{
		using value_type = value_type_;
		using fvalue_type = fvalue_type_;
	private:
		std::function<fvalue_type(value_type)> f;
		value_type lower;
		value_type upper;
		value_type inner_width;
		value_type outer_width;
		fvalue_type outer_strength;
	public:
		template<typenma func>
		range_sine_boundary(func&& F, value_type lower_, value_type upper_, value_type inner_width_, value_type outer_width_, fvalue_type outer_strength_)
			: f(std::forward<func>(F))
			, lower(lower_)
			, upper(upper_)
			, inner_width(inner_width_)
			, outer_width(outer_width_)
			, outer_strength(outer_strength_){
		}
		template<typenma func>
		range_sine_boundary(func&& F, value_type lower_, value_type upper_, value_type width_, fvalue_type outer_strength_)
			: f(std::forward<func>(F))
			, lower(lower_)
			, upper(upper_)
			, inner_width(width_)
			, outer_width(width_)
			, outer_strength(outer_strength_){
		}
		fvalue_type operator()(const value_type& x)const{
			if(x>=upper-inner_width){
				if(x<upper){
					return (1-cos((x-upper)/inner_width*math::pi<value_type>()))/2*f(x);
				}else if(x<upper+outer_width){
					return -(1-cos((x-upper)/outer_width*math::pi<value_type>()))/2*outer_strength;
				}else{
					return -outer_strength;
				}
			}

			if(x<=lower+inner_width){
				if(x>lower){
					return (1-cos((x-lower)/inner_width*math::pi<value_type>()))/2*f(x);
				}else if(x>lower-outer_width){
					return (1-cos((x-lower)/outer_width*math::pi<value_type>()))/2*outer_strength;
				}else{
					return outer_strength;
				}
			}

			return(f(x));
		}
	};
	template<typename func, typename value_type,typename fvalue_type_>
	auto make_lower_sine_boundary(func&& F, value_type lower_, value_type width_, fvalue_type_ outer_strength_){
		using fvalue_type = decltype(std::declval<func>()(std::declval<value_type>()));
		return lower_sine_boundary<value_type,fvalue_type>(std::forward<func>(F),lower_,width_,outer_strength_);
	}
	template<typename func, typename value_type,typename fvalue_type_>
	auto make_lower_sine_boundary(func&& F, value_type lower_,value_type inner_width_, value_type outer_width_, fvalue_type_ outer_strength_){
		using fvalue_type = decltype(std::declval<func>()(std::declval<value_type>()));
		return lower_sine_boundary<value_type,fvalue_type>(std::forward<func>(F),lower_,inner_width_,outer_width_,outer_strength_);
	}
	template<typename func, typename value_type,typename fvalue_type_>
	auto make_upper_sine_boundary(func&& F, value_type upper_, value_type width_, fvalue_type_ outer_strength_){
		using fvalue_type = decltype(std::declval<func>()(std::declval<value_type>()));
		return upper_sine_boundary<value_type,fvalue_type>(std::forward<func>(F),upper_,width_,outer_strength_);
	}
	template<typename func, typename value_type,typename fvalue_type_>
	auto make_upper_sine_boundary(func&& F, value_type upper_,value_type inner_width_, value_type outer_width_, fvalue_type_ outer_strength_){
		using fvalue_type = decltype(std::declval<func>()(std::declval<value_type>()));
		return upper_sine_boundary<value_type,fvalue_type>(std::forward<func>(F),upper_,inner_width_,outer_width_,outer_strength_);
	}
	template<typename func, typename value_type,typename fvalue_type_>
	auto make_range_sine_boundary(func&& F, value_type lower_,value_type upper_, value_type width_, fvalue_type_ outer_strength_){
		using fvalue_type = decltype(std::declval<func>()(std::declval<value_type>()));
		return range_sine_boundary<value_type,fvalue_type>(std::forward<func>(F),lower_,upper_,width_,outer_strength_);
	}
	template<typename func, typename value_type,typename fvalue_type_>
	auto make_range_sine_boundary(func&& F, value_type lower_,value_type upper_,value_type inner_width_, value_type outer_width_, fvalue_type_ outer_strength_){
		using fvalue_type = decltype(std::declval<func>()(std::declval<value_type>()));
		return range_sine_boundary<value_type,fvalue_type>(std::forward<func>(F),lower_,upper_,inner_width_,outer_width_,outer_strength_);
	}
}
#
#endif
