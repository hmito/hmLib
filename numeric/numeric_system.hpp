#ifndef HMLIB_NUMERIC_NUMERICSYSTEM_INC
#define HMLIB_NUMERIC_NUMERICSYSTEM_INC 100
#
#include<type_traits>
namespace hmLib{
    namespace numeric{
		template<typename fn_type, typename precision_type>
		struct precision_system{
			fn_type fn;
			precision_type precision;
			precision_system(fn_type fn_, precision_type precision_):fn(fn_),precision(precision_){}
		};
		template<typename fn_type, typename precision_type>
		auto make_precision_system(fn_type fn_, precision_type precision_){
			return precision_system<std::decay_t<fn_type>,std::decay_t<precision_type>>(std::forward<fn_type>(fn_),std::forward<precision_type>(precision_));
		}
		template<typename fn_type, typename generator_type>
		struct generator_system{
			fn_type fn;
			generator_type generator;
			generator_system(fn_type fn_, generator_type generator_):fn(fn_),generator(generator_){}
		};
		template<typename fn_type, typename generator_type>
		auto make_generator_system(fn_type fn_, generator_type generator_){
			return generator_system<std::decay_t<fn_type>,std::decay_t<generator_type>>(std::forward<fn_type>(fn_),std::forward<generator_type>(generator_));
		}
	}
}
#
#endif