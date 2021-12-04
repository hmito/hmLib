#ifndef HMLIB_MINIMA_MINIMARISULT_INC
#define HMLIB_MINIMA_MINIMARISULT_INC 100
#
namespace hmLib{
	namespace minima{
		template<typename value_type,typename fvalue_type>
		struct minima_result{
			bool success;
			unsigned int count;
			value_type value;
			fvalue_type fvalue;
		};
		template<typename value_type,typename fvalue_type>
		auto make_minima_result(bool success, unsigned int count, value_type value, fvalue_type fvalue){
			return minima_result(success, count, value, fvalue);
		}
	}
}
#
#endif
