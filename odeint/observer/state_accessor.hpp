#ifndef HMLIB_ODEINT_OBSERVER_STATEACCESSOR_INC
#define HMLIB_ODEINT_OBSERVER_STATEACCESSOR_INC 100
#
#include "../../iterators/range.hpp"
namespace hmLib{
	namespace odeint{
		template<typename container>
		struct range_accessor{
		private:
			const container& ref;
		public:
			range_accessor(const container& ref_):ref(ref_){}
			auto operator()()const{
				return hmLib::make_range(ref.begin(),ref.end());
			}		
		};
		template<typename container>
		auto make_range_accessor(const container& range_){
			return range_accessor<container>(range_);
		}
		template<typename container>
		struct recent_range_accessor{
		private:
			const container& ref;
			std::size_t min_n;
			std::size_t max_n;
		public:
			recent_range_accessor(const container& ref_, std::size_t min_n_, std::size_t max_n_) :ref(ref_), min_n(min_n_), max_n(max_n_) {}
			auto operator()()const{
				if (std::distance(ref.begin(), ref.end()) < static_cast<decltype(std::distance(ref.begin(), ref.end()))>(min_n))return hmLib::make_range(ref.end(), ref.end());
				return hmLib::make_range(std::next(ref.begin(),std::max<int>(0,std::distance(ref.begin(),ref.end())- max_n)),ref.end());
			}		
		};
		template<typename container>
		auto make_recent_range_accessor(const container& range_, std::size_t min_n_, std::size_t max_n_){
			return recent_range_accessor<container>(range_,min_n_,max_n_);
		}

	}
}
#
#endif
