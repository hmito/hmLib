#ifndef INC_HMLIB_FILTERBUF
#define INC_HMLIB_FILTERBUF 100
#
#include<streambuf>
#include<iostream>
namespace hmLib{
	template<class _Elem,class _Traits=std::char_traits<_Elem>>
	class basic_filterbuf{
	private:
		typedef std::basic_istream<_Elem,_Traits> in_type;
		typedef std::basic_ostream<_Elem,_Traits> out_type;
		typedef std::basic_iostream<_Elem,_Traits> io_type;
		typedef basic_filterbuf<_Elem,_Traits> my_type;
		typedef std::ios::pos_type pos_type;
		typedef std::ios::off_type off_type;
	private:
		out_type* out_ptr;
		in_type* in_ptr;
	public://open/closeån
		basic_filterbuf()
			:out_ptr(/*C++0x_nullptr/*/NULL/**/)
			,in_ptr(/*C++0x_nullptr/*/NULL/**/){
			return;
		}
		basic_filterbuf(io_type& io_)
			:out_ptr(&io_)
			,in_ptr(&io_){
			return;
		}
		basic_filterbuf(out_type& out_)
			:out_ptr(&out_)
			,in_ptr(/*C++0x_nullptr/*/NULL/**/){
			return;
		}
		basic_filterbuf(in_type& in_)
			:out_ptr(/*C++0x_nullptr/*/NULL/**/)
			,in_ptr(&in_){
			return;
		}
		basic_filterbuf(const my_type& My_);
//		basic_filterbuf(const basic_filterbuf& My_)=delete;
		basic_filterbuf& operator=(const my_type& My_);
//		basic_filterbuf& operator=(const basic_filter_buf& My_)=delete;
	public:
		bool is_in()const{return in_ptr!=/*C++0x_nullptr/*/NULL/**/;}
		bool is_out()const{return out_ptr!=/*C++0x_nullptr/*/NULL/**/;}
		bool is_io()const{return is_in() && is_out();}
		bool is_open()const{return is_in() || is_out();}
		bool open_io(io_type& io_){
			if(is_open())return true;
			in_ptr=&io_;
			out_ptr=&io_;
			return false;
		}
		bool open_io(out_type& out_){
			if(is_open())return true;
			out_ptr=&out_;
			return false;
		}
		bool open_io(in_type& in_){
			if(is_open())return true;
			in_ptr=&in_;
			return false;
		}
		bool close_io(){
			in_ptr=/*C++0x_nullptr/*/NULL/**/;
			out_ptr=/*C++0x_nullptr/*/NULL/**/;
			return false;
		}
	public://ioÉAÉNÉZÉXån
		out_type& ref_out(){return *out_ptr;}
		in_type& ref_in(){return *in_ptr;}
		const out_type& ref_out()const{return *out_ptr;}
		const in_type& ref_in()const{return *in_ptr;}
		pos_type tell_io(){
			if(is_out())return ref_out().tellp();
			else return ref_in().tellg();
		}
		void seek_io(pos_type pos){
			if(is_out())ref_out().seekp(pos);
			else ref_in().seekg(pos);
		}
		void seek_io(off_type off,std::ios_base::seekdir way){
			if(is_out())ref_out().seekp(off,way);
			else ref_in().seekg(off,way);
		}
		bool eof_io()const{
			if(is_out())return ref_out().eof();
			else return ref_in().eof();
		}
	};
	typedef basic_filterbuf<char,std::char_traits<char>> filterbuf;
}
#
#endif
