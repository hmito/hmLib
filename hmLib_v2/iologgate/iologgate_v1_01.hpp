#ifndef INC_HMLIB_IOLOGGATE
#define INC_HMLIB_IOLOGGATE 101
#
#ifndef INC_HMLIB_GATESTREAM
#	include "hmLib_v2/gatestream.hpp"
#endif
#ifndef INC_HMLIB_HMTIME
#	include "hmLib_v2/hmTime.hpp"
#endif
#include <deque>
#include <string>
namespace hm{
	class basic_iologType{
	public:
		bool io;
		basic_iologType(bool _io){io=_io;}
		virtual bool check(bool _io){return io!=_io;}
		virtual void log(char c)=0;
	};
	class iologNorm:public basic_iologType{
	public:
		std::string Str;
		iologNorm(bool _io):basic_iologType(_io){return;}
		virtual void log(char c){Str.push_back(c);}
	};
	class iologPrcTime:public basic_iologType{
	public:
		std::string Str;
		int Start,End;
		iologPrcTime(bool _io):basic_iologType(_io){Start=hmstd::PrcTime();}
		virtual void log(char c){
			Str.push_back(c);
			End=hmstd::PrcTime();
		}
	};
	class iologHmTime:public basic_iologType{
	public:
		std::string Str;
		hmTime Start,End;
		iologHmTime(bool _io):basic_iologType(_io){Start=hm::time();}
		virtual void log(char c){
			Str.push_back(c);
			End=hm::time();
		}
	};
}
namespace hmstd{
	template<class iologType>
	class iologgate:public basic_gate{
	private:
		basic_gate* gatep;
	public:
		std::deque<iologType> Log;
	public:
		iologgate(){gatep=0;}
		iologgate(basic_gate& _gate){open(_gate);}
		void open(basic_gate& _gate){gatep=&_gate;}
		void close(){gatep=0;}
	//input functions
		virtual char getc(){
			if(gatep==0)throw "virtual char iologgate::getc() gate adress error";
			if((Log.size()==0) || Log.back().check(1))Log.push_back(iologType(1));
			char tmp=gatep->getc();
			Log.back().log(tmp);
			return tmp;
		}
		virtual bool pfail(){
			if(gatep==0)throw "virtual char iologgate::getc() gate adress error";
			return gatep->pfail();
		}
		virtual bool empty(){
			if(gatep==0)throw "virtual char iologgate::getc() gate adress error";
			return gatep->empty();
		}
	//output functions
		virtual void putc(char c){
			if(gatep==0)throw "virtual char iologgate::getc() gate adress error";
			if((Log.size()==0) || Log.back().check(0))Log.push_back(iologType(0));
			Log.back().log(c);
			gatep->putc(c);
		}
		virtual bool gfail(){
			if(gatep==0)throw "virtual char iologgate::getc() gate adress error";
			return gatep->gfail();
		}
		virtual bool full(){
			if(gatep==0)throw "virtual char iologgate::getc() gate adress error";
			return gatep->full();
		}
	};
}
#
#endif
