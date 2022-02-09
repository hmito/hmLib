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
			minima_result() = default;
			minima_result(bool success_, unsigned int count_, value_type value_, fvalue_type fvalue_)
				: success(success_)
				, count(count_)
				, value(value_)
				, fvalue(fvalue_) {
			}
		};
		template<typename value_type,typename fvalue_type>
		auto make_minima_result(bool success, unsigned int count, value_type value, fvalue_type fvalue){
			return minima_result<value_type,fvalue_type>(success, count, value, fvalue);
		}
	}
}
#
#endif
