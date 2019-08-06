#ifndef HMLIB_IOUTILITY_INC
#define HMLIB_IOUTILITY_INC 100
#
#include<iostream>
#include<iomanip>
#include<ios>
#include<cmath>
#include<string>
#include<sstream>
#include<chrono>
namespace hmLib{
	/*
		105 ms     0-999
		20.4 s  1.00-59.9
		10.2 m  1.00-59.0
		20.5 h  1.00-23.9
		14.5 d  1.00-99.9
		none
	*/
	namespace detail{
		template<typename ostream_>
		ostream_& apply_ostream(ostream_& out){return out;}
		template<typename ostream_, typename manipulator, typename... others>
		ostream_& apply_ostream(ostream_& out, manipulator m, others... oth){
			out<<m;
			return apply_ostream(out, oth...);	
		}
	}
	template<typename T, typename... manipulator>
	std::string to_string(const T& v, manipulator... Manipulator){
		std::stringstream sout;
		(detail::apply_ostream(sout,Manipulator...,v));
		return sout.str();
	}
	
	template<typename duration>
	std::string duration_format(duration od){
		//5 char
		auto d = std::chrono::duration_cast<std::chrono::milliseconds>(od);

		if(d < std::chrono::hours(1)){
			//ms,s,m
			if(d<std::chrono::seconds(1)){
				//ms
				return to_string(d.count(),std::setw(3),std::right)+"ms";
			}else if(d<std::chrono::minutes(1)){
				//s
				return to_string(d.count()/1000.0,std::defaultfloat,std::setprecision(3))+"s";
			}else{
				//m
				return to_string(d.count()/1000.0/60.0,std::defaultfloat,std::setprecision(3))+"m";
			}
		}else{
			//h,d,y
			double dval = d.count() / (1000.0*60.0*60.0);
			if(dval < 24){
				return to_string(dval,std::defaultfloat,std::setprecision(3))+"h";
			}else if(dval<100*24){
				return to_string(dval/24,std::defaultfloat,std::setprecision(3))+"d";
			}else if(dval<100*24*365){
				return to_string(dval/24/365,std::defaultfloat,std::setprecision(3))+"y";
			}else{
				return " inf ";
			}
		}
	}
	class progress_bar{
		using clock = std::chrono::system_clock;
		using time_point = clock::time_point;
	private:
		time_point strt_time;
		time_point prev_time;
		unsigned int cur_cnt;
		double prev_t;
		double strt_t;
		double end_t;
	public:
		progress_bar()=default;
		progress_bar(double end_t_,double strt_t_ = 0.0){reset(end_t_, strt_t_);}
		void reset(double end_t_,double strt_t_){
			cur_cnt = 0;
			strt_t= strt_t_;
			end_t = end_t_;
		}
		void operator()(std::ostream& out, double t){
			unsigned int num = static_cast<unsigned int>(std::round(50*t/end_t))+1;
			if((cur_cnt&1)==0){
				if(cur_cnt==0){
					out<<"0----1----2----3----4----5----6----7----8----9----X"<<std::endl;
					strt_time = clock::now();
					prev_time = strt_time;
					prev_t = t;
					cur_cnt = 1;
				}else if(cur_cnt < num*2){
					prev_time = clock::now();
					cur_cnt = 1;
					prev_t = t;
				}
			}else{
				out<<"\r";

				cur_cnt = 0;
				while(cur_cnt < num*2){
					out<<"*";
					cur_cnt+=2;
				}
				auto cur_time = clock::now();
				out<<" "<<duration_format(cur_time-strt_time)
				   <<"+"<<duration_format((cur_time-strt_time)*(end_t-t)/(t-strt_t)) 
				   <<"(+"<<duration_format((cur_time-prev_time)*(end_t-t)/(t - prev_t)) << ")" ;
				out<<std::flush;
			}
		}
	};
}
#
#endif
