#ifndef INC_HMLIB_HMSTREAMBUF
#define INC_HMLIB_HMSTREAMBUF 100
#include <iostream>
#include <streambuf>
#pragma warning(disable : 4996)
namespace hmstd{
	class hmNBstreambuf :public std::streambuf{
	private:
		std::streamsize xsputn(const char* c_p,std::streamsize size){
			std::string str(c_p,static_cast<unsigned int>(size));
			put(str);
			return 0;
		}
		std::streamsize xsgetn(char* c_p,std::streamsize size){
			std::string str;
			str=get(static_cast<int>(size));
			sprintf(c_p,"%s",str.c_str());
			return 0;
		}
		int overflow(int nCh){
			putc(nCh);
			return 0;
		}
		int uflow(){return getc();}
		int pbackfail(int nCh){
			unget((char)nCh);
			return 0;
		}
		int underflow(){
			return peek();
		}
	public:
		virtual void putc(char c)=0;
		virtual void put(std::string str){
			for(unsigned int i=0;i<str.size();i++)putc(str.at(i));
		}
		virtual char getc()=0;
		virtual std::string get(int size){
			std::string str;
			for(int i=0;i<size;i++)str+=getc();
			return str;
		}
		virtual std::string get(char endchar,int maxsize){
			if(maxsize<0)maxsize=INT_MAX;
			std::string str;
			for(int i=0;i<maxsize;i++){
				str+=getc();
				if(str[str.size()-1]==endchar)break;
			}
			return str;
		}
		virtual void unget(char c)=0;
		virtual char peek(){
			char ans=getc();
			unget(ans);
			return ans;
		}
	};
	class hmBstreambuf :public std::streambuf{
	private:
		bool ioFlag;
		int sync(){
			flush();
			return 0;
		}
		std::streambuf::pos_type seekpos(std::streambuf::pos_type abspos,std::ios_base::openmode which=std::ios_base::in|std::ios_base::out){
			seek(abspos);
			return tell();
		}
		std::streambuf::pos_type seekoff(std::streambuf::off_type relpos,std::ios_base::seekdir dir,std::ios_base::open_mode which = std::ios_base::in | std::ios_base::out){
			seek(relpos,dir);
			return tell();
		}
		int overflow(){
			woverflow();
			return 0;
		}
		int underflow(){
			roverflow();
			return 0;
		}
	public:
		hmBstreambuf(bool _ioFlag){ioFlag=_ioFlag;}
		virtual void flush()=0;
		virtual void woverflow()=0;
		virtual void roverflow()=0;
		virtual void setbuf(char* pstrt,char* pend){
			if(ioFlag)setg(pstrt,pstrt,pend);
			else setp(pstrt,pend);
		}
		virtual char* getbufstrt(){
			if(ioFlag)return eback();
			else return pbase();
		}
		virtual char* getbufend(){
			if(ioFlag)return egptr();
			else return epptr();
		}
		virtual void setptr(char* ptr){
			if(ioFlag)gbump((int)(ptr-getptr()));
			else pbump((int)(ptr-getptr()));
		}
		virtual char* getptr(){
			if(ioFlag)return gptr();
			else return pptr();
		}
		virtual int size(){return (int)(getbufend()-getbufstrt());}
		virtual int tell(){return (int)(getptr()-getbufstrt());}
		virtual void seek(int relpos,std::ios_base::seekdir dir){
			int tmp;

			if(dir==std::ios_base::beg)tmp=0;
			if(dir==std::ios_base::cur)tmp=(int)(getptr()-getbufstrt());
			if(dir==std::ios_base::end)tmp=(int)(getbufend()-getbufstrt());

			setptr(getbufstrt());

			if(ioFlag)gbump(relpos+tmp);
			else pbump(relpos+tmp);
		}
		virtual void seek(int abspos){
			setptr(getbufstrt());

			if(ioFlag)gbump(abspos);
			else pbump(abspos);
		}
	};
};
#endif
