#ifndef HMLIB_FILTERBUF_INC
#define HMLIB_FILTERBUF_INC 102
#
/*===filterbuf===
filterbuf_v1_02/130101 hmIto
	iostreamに対応
*/
#include<streambuf>
#include<iostream>
namespace hmLib{
	template<class _Elem,class _Traits=std::char_traits<_Elem>>
	class basic_filterbuf{
	protected:
		typedef std::basic_istream<_Elem,_Traits> istream;
		typedef std::basic_ostream<_Elem,_Traits> ostream;
		typedef std::basic_iostream<_Elem,_Traits> iostream;
		typedef basic_filterbuf<_Elem,_Traits> my_type;
		typedef std::ios::pos_type pos_type;
		typedef std::ios::off_type off_type;
	private:
		iostream* ptr_io;
		istream* ptr_i;
		ostream* ptr_o;
	public:
		basic_filterbuf()
			:ptr_io(nullptr)
			,ptr_o(nullptr)
			,ptr_i(nullptr){
		}
		basic_filterbuf(iostream& io_)
			:ptr_io(&io_)
			,ptr_o(&io_)
			,ptr_i(&io_){
		}
		basic_filterbuf(istream& in_)
			:ptr_io(nullptr)
			,ptr_o(nullptr)
			,ptr_i(&in_){
		}
		basic_filterbuf(ostream& out_)
			:ptr_io(nullptr)
			,ptr_o(&out_)
			,ptr_i(nullptr){
		}
	public:
		iostream& ref_io(){return *ptr_io;}
		istream& ref_i(){return *ptr_i;}
		ostream& ref_o(){return *ptr_o;}
		const iostream& ref_io(){return *ptr_io;}
		const istream& cref_i()const{return *ptr_i;}
		const ostream& cref_o()const{return *ptr_o;}
		bool has_i()const{return ptr_i!=nullptr;}
		bool has_o()const{return ptr_o!=nullptr;}
		bool has_io()const{return has_io!=nullptr;}
		bool is_open()const{return has_i() || has_o();}
		bool open(iostream& io_){
			if(is_open())return true;
			ptr_io=&io_;
			ptr_o=&io_;
			ptr_i=&io_;
			return false;
		}
		bool open(ostream& out_){
			if(is_open())return true;
			ptr_io=nullptr;
			ptr_o=&out_;
			ptr_i=nullptr;
			return false;
		}
		bool open(istream& in_){
			if(is_open())return true;
			ptr_io=nullptr;
			ptr_o=nullptr;
			ptr_i=&in_;
			return false;
		}
		bool close(){
			if(!is_open())return true;
			ptr_io=nullptr;
			ptr_o=nullptr;
			ptr_i=nullptr;
			return false;
		}
		pos_type tell_i()const{
			hmLib_assert(has_i(),"There is no istream");
			return ptr_i->tellg();
		}
		pos_type tell_o()const{
			hmLib_assert(has_o(),"There is no ostream");
			return ptr_o->tellp();
		}
		void seek_i(pos_type pos){
			hmLib_assert(has_i(),"There is no istream");
			return ptr_i->seelg(pos);
		}
		void seek_o(pos_type pos){
			hmLib_assert(has_o(),"There is no ostream");
			return ptr_o->seelp(pos);
		}
		void seek_i(off_type off,std::ios_base::seekdir way){
			hmLib_assert(has_i(),"There is no istream");
			return ptr_i->seelg(off,way);
		}
		void seek_o(off_type off,std::ios_base::seekdir way){
			hmLib_assert(has_o(),"There is no ostream");
			return ptr_o->seelg(off,way);
		}
		bool eof_i()const{
			hmLib_assert(has_i(),"There is no istream");
			return ptr_i->eof();
		}
		bool eof_o()const{
			hmLib_assert(has_o(),"There is no ostream");
			return ptr_o->eof();
		}
	};
	typedef basic_filterbuf<char,std::char_traits<char>> filterbuf;
}
#
#endif
