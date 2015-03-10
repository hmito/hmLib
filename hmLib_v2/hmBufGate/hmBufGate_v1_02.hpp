#ifndef INC_HMLIB_HMBUFGATE
#define INC_HMLIB_HMBUFGATE 102
#
#ifndef INC_HMLIB_GATESTREAM
#	include "hmLib_v2/gatestream.hpp"
#endif
#include <deque>
namespace hmstd{
class bufgate:public hmstd::basic_gate{
public:
	class ibufctrlgate:public hmstd::basic_gate{
	private:
		bufgate* pgate;
	public:
		ibufctrlgate(bufgate& _gate){pgate=&_gate;}
		virtual char getc(){return 0;}
		virtual bool pfail(){return false;}
		virtual bool empty(){return false;}
		virtual void putc(char c){pgate->ibuf.push_back(c);}
		virtual bool gfail(){return false;}
		virtual bool full(){return false;}
	};
private:
	hmstd::basic_gate* gatep;
	std::deque<char> ibuf;
	std::deque<char> obuf;
public:
	bufgate(){gatep=0;}
	bufgate(hmstd::basic_gate& _gate){open(_gate);}
	void open(hmstd::basic_gate& _gate){
		gatep=&_gate;
	}
	void close(){
		gatep=0;
	}
	virtual char getc(){
		char tmp=0;
		if(ibuf.size()){
			tmp=ibuf.front();
			ibuf.pop_front();
		}else{
			while(gatep->empty());
			tmp=gatep->getc();
		}
		return tmp;
	}
	virtual bool pfail(){return gatep->pfail();}
	virtual bool empty(){return ibuf.size()==0;}
	virtual void putc(char c){obuf.push_back(c);}
	virtual bool gfail(){return gatep->gfail();}
	virtual bool full(){return false;}
	unsigned int sizeOBuf(){return obuf.size();}
	unsigned int sizeIBuf(){return ibuf.size();}
	void clearOBuf(){obuf.clear();}
	void clearIBuf(){ibuf.clear();}
	void work(){
		if(gatep==0)return;

		if(!gatep->empty()){
			while(!gatep->empty())ibuf.push_back(gatep->getc());
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
