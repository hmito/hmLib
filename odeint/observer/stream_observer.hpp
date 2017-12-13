#ifndef HMLIB_ODEINT_STREAMOBSERVER_INC
#define HMLIB_ODEINT_STREAMOBSERVER_INC 100
#
#include<iostream>
#include<string>
#include"../../utility.hpp"
#include"../../type_traits.hpp"
namespace hmLib{
	namespace odeint{
		struct stream_observer{
		private:
			std::ostream& out;
			std::string delm;
			std::string end;
		public:
			stream_observer(std::ostream& out_, std::string delm_=", ", std::string end_ ="\n") :out(out_), delm(delm_), end(end_){}
			template<typename state, typename time, hmLib_static_restrict(has_begin_and_end<state>::value)>
			void operator()(const state& x, time t){
				out << t;
				for(const auto& val : x){
					out << delm << val;
				}
				out << end;
			}
			template<typename state, typename time, hmLib_static_restrict(!has_begin_and_end<state>::value)>
			void operator()(const state& x, time t){
				out << t;
				out << delm << x;
				out << end;
			}
		};
	}
}
#
#endif
