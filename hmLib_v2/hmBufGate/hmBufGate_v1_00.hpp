#ifndef INC_HMLIB_HMBUFGATE
#define INC_HMLIB_HMBUFGATE 100
#
#ifndef INC_HMLIB_GATESTREAM
#	include "hmLib_v2/gatestream.hpp"
#endif
#include <deque>
namespace hmstd{
class hmBuffer:public hmstd::basic_gate{
private:
	hmstd::basic_gate* gatep;
	std::deque<char> ibuf;
	std::deque<char> obuf;
public:
	hmBuffer(){gatep=0;}
	hmBuffer(hmstd::basic_gate& _gate){open(_gate);}
	void open(hmstd::basic_gate& _gate){
		gatep=&_gate;
	}
	void close(){
		gatep=0;
	}
	virtual char getc(){
		char tmp=0;
		if(ibuf.size()){
			tmp=ibuf.back();
			ibuf.pop_back();
		}
		return tmp;
	}
	virtual bool pfail(){return gatep->pfail();}
	virtual bool empty(){return ibuf.size()==0;}
	virtual void putc(char c){obuf.push_back(c);}
	virtual bool gfail(){return gatep->gfail();}
	virtual bool full(){return false;}
	void clearOBuf(){obuf.clear();}
	void clearIBuf(){ibuf.clear();}
	void work(){
		if(gatep==0)return;

		if(!gatep->empty()){
			while(!gatep->empty())ibuf.push_front(gatep->getc());
		}else if(obuf.size()){
			while(obuf.size()){
				gatep->putc(obuf.front());
				obuf.pop_front();
			}
		}
	}
};
}
#endif
