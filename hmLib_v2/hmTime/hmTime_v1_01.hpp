#ifndef INC_HMLIB_HMTIME
#define INC_HMLIB_HMTIME 101
#include <ctime>
#include <windows.h>
#ifndef INC_HMLIB_HMBASIC
#	include "hmLib_v2/hmBasic.hpp"
#endif
class hmTime{
private:
	dlong Time;
	int Msec;
private:
	void adj(){
		while(Msec>1000){
			++Time;
			Msec-=1000;
		}
		while(Msec<0){
			--Time;
			Msec+=1000;
		}
	}
public:
	hmTime(){
		Time=0;
		Msec=0;
	}
	hmTime(const hmTime& _hmT){
		Time=_hmT.Time;
		Msec=_hmT.Msec;
	}
	hmTime(const dlong& _Time,const int& _Msec){
		Time=_Time;
		Msec=_Msec;
		adj();
	}
	const hmTime& operator=(const hmTime& _hmT){
		if(this!=&_hmT){
			Time=_hmT.Time;
			Msec=_hmT.Msec;
			adj();
		}
		return *this;
	}
	const hmTime& operator+=(const hmTime& _hmT){
		Time+=_hmT.Time;
		Msec+=_hmT.Msec;
		adj();
		return *this;
	}
	const hmTime& operator-=(const hmTime& _hmT){
		Time-=_hmT.Time;
		Msec-=_hmT.Msec;
		adj();
		return *this;
	}
	hmTime operator+(const hmTime& _hmT)const{
		hmTime ansT(*this);
		return ansT+=_hmT;
	}
	hmTime operator-(const hmTime& _hmT)const{
		hmTime ansT(*this);
		return ansT-=_hmT;
	}
	bool operator>(const hmTime& _hmT)const{
		if(Time!=_hmT.Time)return Time>_hmT.Time;
		else return Msec>_hmT.Msec;
	}
	bool operator>=(const hmTime& _hmT)const{
		if(Time!=_hmT.Time)return Time>=_hmT.Time;
		else return Msec>=_hmT.Msec;
	}
	bool operator<(const hmTime& _hmT)const{
		if(Time!=_hmT.Time)return Time<_hmT.Time;
		else return Msec<_hmT.Msec;
	}
	bool operator<=(const hmTime& _hmT)const{
		if(Time!=_hmT.Time)return Time<=_hmT.Time;
		else return Msec<=_hmT.Msec;
	}
	bool operator==(const hmTime& _hmT)const{
		return (Time==_hmT.Time)&&(Msec==_hmT.Msec);
	}

	bool operator!=(const hmTime& _hmT)const{
		return (Time!=_hmT.Time)||(Msec!=_hmT.Msec);
	}
	dlong time()const{return Time;}
	int msec()const{return Msec;}
	void set(const dlong& _Time,const int& _Msec){
		Time=_Time;
		Msec=_Msec;
		adj();
	}
	friend inline std::ostream& operator<<(std::ostream& _out,const hmTime& _hmT);
	friend inline std::istream& operator>>(std::istream& _in,hmTime& _hmT);
};
inline std::ostream& operator<<(std::ostream& _out,const hmTime& _hmT){
	 _out<<_hmT.Time<<"."<<hmstd::fStr("%03d",_hmT.Msec);
	 return _out;
 }
inline std::istream& operator>>(std::istream& _in,hmTime& _hmT){
	_in>>_hmT.Time;
	_in.ignore(1);
	int Keta=100;
	while(static_cast<int>('0')<=_in.peek() && _in.peek()<=static_cast<int>('9')){
		_hmT.Msec+=Keta*(_in.get()-static_cast<int>('0'));
		Keta/=10;
	}
	return _in;
}
class hmDate{
public:
	int Year;
	int Month;
	int Day;
	int Hour;
	int Minute;
	int Second;
	int mSecond;
public:
	hmDate(){
		Year=1900;
		Month=1;
		Day=1;
		Hour=0;
		Minute=0;
		Second=0;
		mSecond=0;
	}
	hmDate(int _Year,int _Month,int _Day,int _Hour,int _Minute,int _Second,int _mSecond){
		Year=_Year;
		Month=_Month;
		Day=_Day;
		Hour=_Hour;
		Minute=_Minute;
		Second=_Second;
		mSecond=_mSecond;
	}
	hmDate(const hmTime& _Time){
		time_t tit=_Time.time();
		struct tm* tstP;
		tstP=localtime(&tit);
		Year=tstP->tm_year+1900;
		Month=tstP->tm_mon+1;
		Day=tstP->tm_mday;
		Hour=tstP->tm_hour;
		Minute=tstP->tm_min;
		Second=tstP->tm_sec;
		mSecond=_Time.msec();
	}
	void set(int _Year,int _Month,int _Day,int _Hour,int _Minute,int _Second,int _mSecond){
		Year=_Year;
		Month=_Month;
		Day=_Day;
		Hour=_Hour;
		Minute=_Minute;
		Second=_Second;
		mSecond=_mSecond;
	}
	void set(const hmTime& _Time){
		time_t tit=_Time.time();
		struct tm* tstP;
		tstP=localtime(&tit);
		Year=tstP->tm_year+1900;
		Month=tstP->tm_mon+1;
		Day=tstP->tm_mday;
		Hour=tstP->tm_hour;
		Minute=tstP->tm_min;
		Second=tstP->tm_sec;
		mSecond=_Time.msec();
	}
	hmTime time()const{
		struct tm tst;
		tst.tm_hour=Hour;
		tst.tm_mday=Day;
		tst.tm_min=Minute;
		tst.tm_mon=Month-1;
		tst.tm_sec=Second;
		tst.tm_year=Year-1900;
		dlong tit=mktime(&tst);
		return hmTime(tit,mSecond);
	}
	std::string str(char _sep='_')const{
		return hmstd::fStr("%02d%02d%02d%c%02d%02d%02d%c%03d",Year%100,Month,Day,_sep,Hour,Minute,Second,_sep,mSecond);
	}
	friend inline std::ostream& operator<<(std::ostream& _out,const hmDate& _hmD);
//	friend std::istream& operator>>(std::istream& _in,const hmDate& _hmD);
};
inline std::ostream& operator<<(std::ostream& _out,const hmDate& _hmD){
	_out<<_hmD.str();
	return _out;
}
namespace hm{
	inline hmTime time(){
		SYSTEMTIME SystemTime;
		GetLocalTime(&SystemTime);
		time_t t;
		::time(&t);
		return hmTime(t,SystemTime.wMilliseconds);
	}
	inline hmDate date(){
		SYSTEMTIME SystemTime;
		GetLocalTime(&SystemTime);
		return hmDate(SystemTime.wYear,SystemTime.wMonth,SystemTime.wDay,SystemTime.wHour,SystemTime.wMinute,SystemTime.wSecond,SystemTime.wMilliseconds);
	}
}
#endif
