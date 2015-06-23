#ifndef HMLIB_STREAMBUFPATTERN_INC
#define HMLIB_STREAMBUFPATTERN_INC 100
#include <iostream>
#include <streambuf>
#pragma warning(disable : 4996)
namespace hmLib{
	namespace pattern{
		template<class _Elem,class _Traits=std::char_traits<_Elem> >
		class basic_nonbuf_streambuf_pattern:public virtual std::basic_streambuf<_Elem,_Traits>{
		private:
			typedef basic_nonbuf_streambuf_pattern<_Elem,_Traits> my_type;
			typedef std::basic_streambuf<_Elem,_Traits> my_streambuf;
		protected:
			typedef std::streamsize streamsize;
			typedef typename my_streambuf::pos_type pos_type;
			typedef typename my_streambuf::off_type off_type;
			typedef typename my_streambuf::int_type int_type;
		private:
			streamsize xsputn(const _Elem* c_p,std::streamsize size)/*C++0x_override/**/{
				puts(c_p,size);
				return size;
			}
			streamsize xsgetn(_Elem* c_p,std::streamsize size)/*C++0x_override/**/{
				return gets(c_p,size);
			}
			pos_type seekpos(pos_type abspos_,std::ios_base::openmode which=std::ios_base::in|std::ios_base::out)/*C++0x_override/**/{
				return seek(abspos_,which);
			}
			pos_type seekoff(off_type relpos_,std::ios_base::seekdir dir_,std::ios_base::openmode which_ = std::ios_base::in | std::ios_base::out)/*C++0x_override/**/{
				return seek(relpos_,dir_,which_);
			}
			int_type overflow(int_type c_= _Traits::eof())/*C++0x_override/**/{
				putc(c_);
				return 0;
			}
			int_type uflow()/*C++0x_override/**/{return getc();}
			int_type underflow()/*C++0x_override/**/{return peek();}
			int sync()/*C++0x_override/**/{
				flush();
				return 0;
			}
			int_type pbackfail(int_type nCh)/*C++0x_override/**/{
				unget((_Elem)nCh);
				return 0;
			}
		public://nonbuf_streambuf_pattern
			virtual void putc(_Elem c)=0;
			virtual void puts(const _Elem* str,streamsize size){
				for(std::streamsize i=0;i<size;i++)putc(str[i]);
			}
			virtual void flush()=0;
			virtual _Elem getc()=0;
			virtual streamsize gets(_Elem* str,streamsize size){
				for(std::streamsize i=0;i<size;i++)str[i]=getc();
				return size;
			}
			virtual streamsize gets(_Elem* str,streamsize maxsize,_Elem end){
				for(std::streamsize i=0;i<maxsize;i++){
					str[i]=getc();
					if(str[i]==end)return i+1;
				}
				return maxsize;
			}
			virtual _Elem peek(){
				_Elem ans=getc();
				unget(ans);
				return ans;
			}
			virtual void unget(_Elem c)=0;
			virtual pos_type seek(pos_type abspos,std::ios_base::openmode which=std::ios_base::in|std::ios_base::out)=0;
			virtual pos_type seek(off_type relpos,std::ios_base::seekdir dir,std::ios_base::openmode which=std::ios_base::in|std::ios_base::out)=0;
		};
		template<class _Elem,class _Traits=std::char_traits<_Elem> >
		class basic_buf_streambuf_pattern :public virtual std::basic_streambuf<_Elem,_Traits>{
		private:
			typedef basic_buf_streambuf_pattern<_Elem,_Traits> my_type;
			typedef std::basic_streambuf<_Elem,_Traits> my_streambuf;
		protected:
			typedef std::streamsize streamsize;
			typedef typename my_streambuf::pos_type pos_type;
			typedef typename my_streambuf::off_type off_type;
			typedef typename my_streambuf::int_type int_type;
		private:
			int sync()/*C++0x_override/**/{
				flush();
				return 0;
			}
			pos_type seekpos(pos_type abspos_,std::ios_base::openmode which=std::ios_base::in|std::ios_base::out)/*C++0x_override/**/{
				return seek(abspos_);
			}
			pos_type seekoff(off_type relpos_,std::ios_base::seekdir dir_,std::ios_base::openmode which_ = std::ios_base::in | std::ios_base::out)/*C++0x_override/**/{
				return seek(relpos_,dir_);
			}
			int_type overflow(int_type c_= _Traits::eof())/*C++0x_override/**/{
				woverflow(static_cast<_Elem>(c_));
				return 0;
			}
			int_type underflow()/*C++0x_override/**/{
				return roverflow();
			}
		protected:
			void set_ibuf(_Elem* pstrt,_Elem* pend){
				setg(pstrt,pstrt,pend);
			}
			_Elem* ibuf_begin(){
				return this->eback();
			}
			_Elem* ibuf_end(){
				return this->egptr();
			}
			_Elem* ibuf_pos(){
				return this->gptr();
			}
			void set_ipos(_Elem* ptr){
				this->gbump((int)(ptr-ibuf_pos()));
			}
			int_type size_iuse(){return static_cast<int_type>(ibuf_pos()-ibuf_begin());}
			int_type size_ibuf(){return static_cast<int_type>(ibuf_end()-ibuf_begin());}
			void set_obuf(_Elem* pstrt,_Elem* pend){
				setg(pstrt,pstrt,pend);
				setp(pstrt,pend);
			}
			_Elem* obuf_begin(){
				return this->pbase();
			}
			_Elem* obuf_end(){
				return this->epptr();
			}
			_Elem* obuf_pos(){
				return this->pptr();
			}
			void set_opos(_Elem* ptr){
				this->pbump((int)(ptr-obuf_pos()));
			}
			int_type size_ouse(){return static_cast<int_type>(obuf_pos()-obuf_begin());}
			int_type size_obuf(){return static_cast<int_type>(obuf_end()-obuf_begin());}
		public://buf_streambuf_pattern
			virtual void woverflow(_Elem c_)=0;
			virtual void flush()=0;
			virtual _Elem roverflow()=0;
			virtual std::streambuf::pos_type seek(off_type relpos,std::ios_base::seekdir dir)=0;
			virtual std::streambuf::pos_type seek(pos_type abspos)=0;
		};
		typedef basic_nonbuf_streambuf_pattern<char, std::char_traits<char> > nonbuf_streambuf_pattern;
		typedef basic_nonbuf_streambuf_pattern<char, std::char_traits<char> > buf_streambuf_pattern;
	}
}
#endif
