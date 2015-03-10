#ifndef INC_HMLIB_WINTIME
#define INC_HMLIB_WINTIME 100
#
#include <ctime>
#include <windows.h>
#ifndef INC_HMLIB_HMBASIC
#	include "hmLib_v2/hmBasic.hpp"
#endif
namespace win{
	inline double getTime(){
		SYSTEMTIME SystemTime;
		GetLocalTime(&SystemTime);
		time_t t;
		time(&t);
		return (double)t+SystemTime.wMilliseconds/1000.;
	}
	inline std::string TimeToStr(const double& _time,char _sep='_'){
		time_t t=(int)_time;
		struct tm* t_st;
		t_st=localtime(&t);
		if(_sep>=0)
			return hmstd::fStr("%02d%02d%02d%c%02d%02d%02d%c%03d",
				t_st->tm_year-100,
				t_st->tm_mon+1,
				t_st->tm_mday,
				_sep,
				t_st->tm_hour,
				t_st->tm_min,
				t_st->tm_sec,
				_sep,
				(int)(_time*1000-t*1000)
			);
		else 
			return hmstd::fStr("%02d%02d%02d%02d%02d%02d%03d",
				t_st->tm_year-100,
				t_st->tm_mon+1,
				t_st->tm_mday,
				t_st->tm_hour,
				t_st->tm_min,
				t_st->tm_sec,
				(int)(_time*1000-t*1000)
			);
	}
	inline void TimeToDate(const double& _time,int& _YMD,int& _HMS,int& _mS){
		time_t t=(int)_time;
		struct tm* t_st;
		t_st=localtime(&t);
		_YMD=(t_st->tm_year-100)*10000+(t_st->tm_mon+1)*100+(t_st->tm_mday);
		_HMS=t_st->tm_hour*10000+t_st->tm_min*100+t_st->tm_sec;
		_mS=(int)(_time*1000.-t);
	}
	inline std::string TimeToYMD(const double& _time,bool _adjFlag=false){
		time_t t=(int)_time;
		struct tm* t_st;
		t_st=localtime(&t);
		if(t_st==0)return "Error";
		if(_adjFlag)return hmstd::fStr("%02d/%02d/%02d",t_st->tm_year-100,t_st->tm_mon+1,t_st->tm_mday);
		return hmstd::fStr("%02d%02d%02d",t_st->tm_year-100,t_st->tm_mon+1,t_st->tm_mday);
	}
	inline std::string TimeToHMS(const double& _time,bool _adjFlag=false){
		time_t t=(int)_time;
		struct tm* t_st;
		t_st=localtime(&t);
		if(t_st==0)return "Error";
		if(_adjFlag)return hmstd::fStr("%02d:%02d:%02d",t_st->tm_hour,t_st->tm_min,t_st->tm_sec);
		return hmstd::fStr("%02d%02d%02d",t_st->tm_hour,t_st->tm_min,t_st->tm_sec);
	}
	inline std::string TimeToHMSmS(const double& _time){
		time_t t=(int)_time;
		struct tm* t_st;
		t_st=localtime(&t);
		return hmstd::fStr("%02d%02d%02d.%03d",t_st->tm_hour,t_st->tm_min,t_st->tm_sec,(int)(_time*1000-t*1000));
	}
}
#
#endif
