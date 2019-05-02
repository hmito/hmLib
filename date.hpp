#ifndef HMLIB_DATE_INC
#define HMLIB_DATE_INC 100
#
#include<chrono>
#include<ctime>
#include<string>
#include<iostream>
#include<iomanip>
#include<sstream>
namespace hmLib{
	struct date{
		typedef std::chrono::system_clock::time_point time_point;
		unsigned int Year;	//Y
		unsigned int Month;	//M
		unsigned int Day;	//D
		unsigned int Hour;	//h
		unsigned int Min;	//m
		unsigned int Sec;	//s
		unsigned int mSec;	//d
	public:
		date():Year(0),Month(0),Day(0),Hour(0),Min(0),Sec(0),mSec(0){}
		date(unsigned int Year_,unsigned int Month_,unsigned int Day_,unsigned int Hour_,unsigned int Min_,unsigned int Sec_,unsigned int mSec_)
			:Year(Year_),Month(Month_),Day(Day_),Hour(Hour_),Min(Min_),Sec(Sec_),mSec(mSec_){
		}
		time_point to_time()const{
			std::tm st;
			st.tm_year=Year-1900;
			st.tm_mon=Month-1;
			st.tm_mday=Day;
			st.tm_hour=Hour;
			st.tm_min=Min;
			st.tm_sec=Sec;
			return std::chrono::system_clock::from_time_t(std::mktime( &st ))+std::chrono::milliseconds(mSec);
		}
		void from_time(const time_point& Time_){
			std::time_t time = std::chrono::system_clock::to_time_t(Time_);
			std::tm* st = std::localtime(&time);
			Year = st->tm_year+1900;
			Month= st->tm_mon+1;
			Day  = st->tm_mday;
			Hour = st->tm_hour;
			Min  = st->tm_min;
			Sec  = st->tm_sec;
			mSec = (std::chrono::duration_cast<std::chrono::milliseconds>(Time_.time_since_epoch()).count())%1000;
		}
		std::string str(std::string format){
			std::string ans;
			for (auto c : format) {
				std::ostringstream sout;
				switch (c) {
				case 'Y':
					sout << std::setw(4) << std::setfill('0') << std::right << Year;
					ans += sout.str();
					break;
				case 'M':
					sout << std::setw(2) << std::setfill('0') << std::right << Month;
					ans += sout.str();
					break;
				case 'D':
					sout << std::setw(2) << std::setfill('0') << std::right << Day;
					ans += sout.str();
					break;
				case 'h':
					sout << std::setw(2) << std::setfill('0') << std::right << Hour;
					ans += sout.str();
					break;
				case 'm':
					sout << std::setw(2) << std::setfill('0') << std::right << Min;
					ans += sout.str();
					break;
				case 's':
					sout << std::setw(2) << std::setfill('0') << std::right << Sec;
					ans += sout.str();
					break;
				case 'd':
					sout << std::setw(2) << std::setfill('0') << std::right << mSec;
					ans += sout.str();
					break;
				default:
					ans += c;
					break;
				}
			}
			return ans;
		}
	};
	inline date current_date() {
		date Cur;
		Cur.from_time(std::chrono::system_clock::now());
		return Cur;
	}
}
#
#endif
