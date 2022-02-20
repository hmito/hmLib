#ifndef HMLIB_ODEINT_OBSERVER_OBSERVECONTAINER_INC
#define HMLIB_ODEINT_OBSERVER_OBSERVECONTAINER_INC 100
#
#include<vector>
#include"../../circular.hpp"
#include"iterator_observer.hpp"
#include"observe_skipper.hpp"
#include"state_accessor.hpp"
namespace hmLib{
	namespace odeint{
		template<typename state_type, typename time_type>
		struct observe_vector{
		public:
			std::vector<state_type> x;
			std::vector<time_type> t;
		public:
			void clear(){
				x.clear();
				t.clear();
			}
			auto begin()const{return x.begin();}
			auto end()const{return x.end();}
			auto time_begin()const{return t.begin();}
			auto time_end()const{return t.end();}
			auto observer(){
				return make_iterator_observer(std::back_inserter(x),std::back_inserter(t));
			}
			auto observer(unsigned int interval){
				return skip_observe(make_iterator_observer(std::back_inserter(x),std::back_inserter(t)),interval);
			}
			auto state_accessor(){
				return make_range_accessor(x);
			}
			auto state_accessor(std::size_t min_n, std::size_t max_n){
				return make_recent_range_accessor(x,min_n,max_n);
			}
		};
		template<typename state_type, typename time_type, std::size_t max_size>
		struct observe_circular{
		public:
			hmLib::circular<state_type,max_size> x;
			hmLib::circular<time_type,max_size> t;
		public:
			void clear(){
				x.clear();
				t.clear();
			}
			auto begin()const{return x.begin();}
			auto end()const{return x.end();}
			auto time_begin()const{return t.begin();}
			auto time_end()const{return t.end();}
			auto observer(){
				return make_iterator_observer(std::back_inserter(x),std::back_inserter(t));
			}
			auto observer(unsigned int interval){
				return skip_observe(make_iterator_observer(std::back_inserter(x),std::back_inserter(t)),interval);
			}
			auto state_accessor(){
				return make_range_accessor(x);
			}
			auto state_accessor(std::size_t min_n, std::size_t max_n){
				return make_recent_range_accessor(x,min_n,max_n);
			}
		};
	}
}
#
#endif
