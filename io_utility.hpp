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
	template<typename time_type>
	class progress_bar {
	private:
		unsigned int cur_cnt;
		time_type prev_t;
		time_type end_t;
		time_type beg_t;
	public:
		progress_bar() = default;
		explicit progress_bar(time_type end_t_) { reset(0, end_t_); }
		progress_bar(time_type beg_t_, time_type end_t_) { reset(beg_t_, end_t_); }
		void reset(time_type beg_t_, time_type end_t_) {
			cur_cnt = 0;
			end_t = end_t_;
			beg_t = beg_t_;
		}
		void operator()(std::ostream& out, time_type t) {
			unsigned int num = static_cast<unsigned int>(std::round(50 * static_cast<double>(t - beg_t) / end_t)) + 1;
			if ((cur_cnt & 1) == 0) {
				if (cur_cnt == 0) {
					out << "0----1----2----3----4----5----6----7----8----9----X" << std::endl;
					prev_t = t;
					cur_cnt = 1;
				} else if (cur_cnt < num * 2) {
					cur_cnt = 1;
					prev_t = t;
				}
			} else {
				out << "\r";

				cur_cnt = 0;
				while (cur_cnt < num * 2) {
					out << "*";
					cur_cnt += 2;
				}
				out << std::flush;
			}
		}
		time_type beg_time()const { return beg_t; }
		time_type end_time()const { return end_t; }
		time_type cur_time()const { return prev_t; }
	};
	template<typename time_type>
	class estimate_progress_bar{
		using clock = std::chrono::system_clock;
		using time_point = clock::time_point;
	private:
		time_point beg_tp;
		time_point prev_tp;
		unsigned int cur_cnt;
		time_type prev_t;
		time_type beg_t;
		time_type end_t;
	public:
		estimate_progress_bar()=default;
		explicit estimate_progress_bar(time_type end_t_){reset(0, end_t_);}
		estimate_progress_bar(time_type beg_t_, time_type end_t_) { reset(beg_t_, end_t_); }
		void reset(time_type beg_t_, time_type end_t_){
			cur_cnt = 0;
			beg_t= beg_t_;
			end_t = end_t_;
		}
		void operator()(std::ostream& out, time_type t){
			unsigned int num = static_cast<unsigned int>(std::round(50* static_cast<double>(t - beg_t) /end_t))+1;
			if((cur_cnt&1)==0){
				if(cur_cnt==0){
					out<<"0----1----2----3----4----5----6----7----8----9----X"<<std::endl;
					beg_tp = clock::now();
					prev_tp = beg_tp;
					prev_t = t;
					cur_cnt = 1;
				}else if(cur_cnt < num*2){
					prev_tp = clock::now();
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
				auto cur_tp = clock::now();
				out<<" "<<duration_format(cur_tp - beg_tp)
				   <<"+"<<duration_format((cur_tp - beg_tp)*static_cast<double>(end_t-t)/(t-beg_t))
				   <<"(+"<<duration_format((cur_tp - prev_tp)* static_cast<double>(end_t-t)/(t - prev_t)) << ")" ;
				out<<std::flush;
			}
		}
		time_type beg_time()const { return beg_t; }
		time_type end_time()const { return end_t; }
		time_type cur_time()const { return prev_t; }
	};
}
#
#endif
