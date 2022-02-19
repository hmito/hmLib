#ifndef HMLIB_ODEINT_OBSERVER_CONTAINEROBSERVER_INC
#define HMLIB_ODEINT_OBSERVER_CONTAINEROBSERVER_INC 100
#
#include<vector>
#include"../../circular.hpp"
namespace hmLib{
	namespace odeint{
		template<typename state_type, typename time_type>
		struct vector_observer{
		private:
			std::vector<state_type> xset;
			std::vector<time_type> tset;
		public:
			void clear(){
				xset.clear();
				tset.clear();
			}
			auto begin()const{return xset.begin();}
			auto end()const{return xset.end();}
			auto time_begin()const{return tset.begin();}
			auto time_end()const{return tset.end();}
			void operator()(const state_type& x, time_type t){
				xset.push_back(x);
				tset.push_back(t);
			}
		};
		template<typename state_type, typename time_type, std::size_t max_size>
		struct circular_observer{
		private:
			hmLib::circular<state_type,max_size> xset;
			hmLib::circular<time_type,max_size> tset;
		public:
			void clear(){
				xset.clear();
				tset.clear();
			}
			auto begin()const{return xset.begin();}
			auto end()const{return xset.end();}
			auto time_begin()const{return tset.begin();}
			auto time_end()const{return tset.end();}
			void operator()(const state_type& x, time_type t){
				xset.rotate_back(x);
				tset.rotate_back(t);
			}
		};
	}
}
#
#endif
