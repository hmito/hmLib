#ifndef INC_HMLIB_HMBASIC
#define INC_HMLIB_HMBASIC 103
#include <iostream>
#include <iomanip>
#include <string>
#include <deque>
#pragma warning(disable : 4996)
#define D_FormatStrAdressSize 1024
//unsigndŒn—ñ
typedef unsigned int uint;
typedef unsigned char uchar;
typedef long long dlong;
typedef unsigned long long udlong;
//dequeŒn—ñ
typedef std::deque<std::string> strings;
//mathŒn—ñ
inline double pi(){return 3.141592653589793;}
//•ÏŠ·Œn—ñ
inline uchar itouc(int x){
		if(x<0)return 0;
		else if(x>255)return 255;
		else return (uchar)x;
}
namespace hmstd{
	//FStr
	const char* fCharp(const char *String, ...);
	std::string fStr(const char *String, ...);
}
//Sel
namespace hm{
	template <class T>
	inline const T& sel(bool cnd,const T& trueAns,const T& falseAns){
		if(cnd)return trueAns;
		else return falseAns;
	}
	char* getTmpStrAdress();
	inline int getTmpStrAdressSize(){return D_FormatStrAdressSize;}
};
//mTimeŠÖ˜A
namespace hmstd{
	uint uPrcTime();
	int PrcTime();
	uint uStopwatch();
	int Stopwatch();
};
//bitŠÖ˜A
namespace hmstd{
	template<class T>
	inline bool bittest(T c,char num){
		if(num>=sizeof(T) || num<0)return false;
		return (c&(1<<num))==0;
	}
	template<class T>
	inline void bitput(T c,char num,bool value){
		if(num>=sizeof(T) || num<0)return;
		if(value)c|=(1<<num);
		else c&=(!(1<<num));
	}
	template<class T>
	inline void biton(T c,char num){
		if(num>=sizeof(T) || num<0)return;
		c|=(1<<num);
	}
	template<class T>
	inline void bitoff(T c,char num){
		if(num>=sizeof(T) || num<0)return;
		c&=(!(1<<num));
	}
	template<class T>
	inline void bitxor(T c,char num){
		if(num>=sizeof(T) || num<0)return;
		c^=(!(1<<num));
	}
}
//maxminŠÖ˜A
namespace hmstd{
	template<class T>
	inline T med3(T val1,T val2,T val3){
		if(val1>val2){
			if(val2>val3)return val2;
			else if(val1>val3)return val3;
			else return val1;
		}else{
			if(val3>val2)return val2;
			else if(val1>val3)return val1;
			else return val3;
		}
	}
	template<class T>
	inline T min3(T val1,T val2,T val3){return min(val1,min(val2,val3));}
	template<class T>
	inline T max3(T val1,T val2,T val3){return max(val1,max(val2,val3));}
}
#endif
