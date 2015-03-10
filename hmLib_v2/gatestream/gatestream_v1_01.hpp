#ifndef INC_HMLIB_GATESTREAM
#define INC_HMLIB_GATESTREAM 101
#ifndef INC_HMLIB_HMSTREAMBUF
#	include "hmLib_v2/hmStreamBuf.hpp"
#endif

namespace shm{
class itfGate{
public:
//input functions
	virtual char getc()=0;
	virtual bool pfail(){return false;}
	virtual bool empty(){return false;}
//output functions
	virtual void putc(char c)=0;
	virtual bool gfail(){return false;}
	virtual bool full(){return false;}
};
class gateios{
protected:
	class gatestreambuf:public shm::basic_nonbuf_streambuf{
	protected:
		itfGate* gate;
		char buf;
		bool bufF;
	public:
		gatestreambuf(itfGate& _gate){
			gate=&_gate;
			bufF=false;
		}
		void putc(char c){gate->putc(c);}
		char getc(){
			if(bufF){
				bufF=false;
				return buf;
			}
			return gate->getc();
		}
		void unget(char c){
			buf=c;
			bufF=true;
		}
		bool fail(){return (gate->pfail() && gate->gfail());}
		bool pfail(){return gate->pfail();}
		bool gfail(){return gate->gfail();}
		bool empty(){return gate->empty();}
		bool full(){return gate->full();}
	};
protected:
	gatestreambuf* streambuf;
	std::_Uninitialized uninit;
};
class igatestream:public std::istream,virtual public gateios{
public:
	igatestream():std::istream(uninit){streambuf=0;}
	igatestream(itfGate& _gate):std::istream(uninit){
		streambuf=0;
		open(_gate);
	}
	~igatestream(){close();}
	void open(itfGate& _gate){
		close();
		streambuf= new gatestreambuf(_gate);
		std::istream::init(streambuf);
	}
	void close(){
		if(streambuf==0)return;
		delete streambuf;
		streambuf=0;
	}
	bool gfail(){return streambuf->gfail();}
	bool empty(){return streambuf->empty();}
	bool eof(){return empty();}
};
class ogatestream:public std::ostream,virtual public gateios{
public:
	ogatestream():std::ostream(uninit){streambuf=0;}
	ogatestream(itfGate& _gate):std::ostream(uninit){
		streambuf=0;
		open(_gate);
	}
	~ogatestream(){close();}
	void open(itfGate& _gate){
		close();
		streambuf= new gatestreambuf(_gate);
		std::ostream::init(streambuf);
	}
	void close(){
		if(streambuf==0)return;
		delete streambuf;
		streambuf=0;
	}
	bool pfail(){return streambuf->pfail();}
	bool full(){return streambuf->full();}
};
class gatestream:public igatestream,public ogatestream{
public:
	gatestream():igatestream(),ogatestream(){return;}
	gatestream(itfGate& _gate):igatestream(_gate),ogatestream(_gate){return;}
	void open(itfGate& _gate){
		igatestream::open(_gate);
		ogatestream::open(_gate);
	}
	void close(){
		igatestream::close();
		ogatestream::close();
	}
	bool fail(){return (gfail()&&pfail());}
};
}
#endif
