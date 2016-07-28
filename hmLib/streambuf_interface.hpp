#ifndef HMLIB_STREAMBUFINTERFACE_INC
#define HMLIB_STREAMBUFINTERFACE_INC 102
/*===streambuf_interface===
streambuf作成支援用のクラスです。

streambuf_interface:v1_02/130328 hmIto
	一部関数名が変更しきれていなかった問題を修正
streambuf_interface:v1_01/130104 hmIto
	nonbuf_streambuf_interfaceとbuf_streambuf_interfaceを廃止
	streambuf_interfaceを追加
	関数名を統一的に整理
		obuf/ibuf制御用インターフェースをobuf_/ibuf_に統一
*/
#include <iostream>
#include <streambuf>
#pragma warning(disable : 4996)
/*===std::streambuf functions===
public:
	pos_type pubseekoff(off_type _Off, ios_base::seekdir _Way, ios_base::openmode _Mode = ios_base::in | ios_base::out);
	pos_type pubseekoff(off_type _Off, ios_base::seek_dir _Way, ios_base::open_mode _Mode);
		point位置を相対位置で切り替える
		ただのseekoff
	pos_type pubseekpos(pos_type _Pos, ios_base::openmode _Mode = ios_base::in | ios_base::out);
	pos_type pubseekpos(pos_type _Pos, ios_base::open_mode _Mode);
		point位置を絶対位置で切り替える
		ただのseekpos
	_Myt* pubsetbuf(_Elem *_Buffer, streamsize _Count);
		不明
		setbufを呼び出している
	locale pubimbue(const locale &_Newlocale);
		不明
		imbueを呼び出している
	locale getloc() const;
		不明
		localeを取得する関数?
	streamsize in_avail();
		ibuf中に文字があればその数、なければshowmanyc()の値を返す
	int pubsync();
		obufの中身を外部へ反映
		sync()を呼び出す
	int_type sbumpc();
		1文字読み出し、pointを進める
	int_type sgetc();
		1文字読み出すが、pointを進めない
	streamsize sgetn(_Elem *_Ptr, streamsize _Count);
		n文字読み出し、pointを進める
		xsgetnを呼び出しているだけなので、overflowしても何もしない模様
	int_type snextc();
		現在のpointの次の1文字を読み出し、pointを進めない
	int_type sputbackc(_Elem _Ch);
		1文字を読み出す前に戻す
	void stossc();
		読み出し用のpointを進める
	int_type sungetc();
		読み出し用のpointを戻す?
	int_type sputc(_Elem _Ch);
		1文字書き込む
	streamsize sputn(const _Elem *_Ptr, streamsize _Count);
		n文字書き込む
		xsputnを呼び出しているだけなので、overflowしても何もしない模様
	virtual void _Lock();
	virtual void _Unlock();
		thread関連
protected:
	int_type overflow(int_type = _Traits::eof());
		obufがあふれたときに呼ばれる。
		obufが設定されていない場合、常に呼ばれる。
	int_type pbackfail(int_type = _Traits::eof());
		ibufの先頭に一文字追加する。
		失敗した場合はeofを返す。
	streamsize showmanyc();
		ibufへの入力文字数を返す?
	int_type underflow();
		ibufから読み出すが、pointは動かさない。
	int_type uflow();
		ibufから読み出し、pointを動かす。
	streamsize xsgetn(_Elem * _Ptr, streamsize _Count);
		ibufからCount文字だけ受け取り、pointを動かす。
	streamsize xsputn(const _Elem *_Ptr, streamsize _Count);
		obufへCount文字だけ書き込み、pointを動かす。
	pos_type seekoff(off_type, ios_base::seekdir, ios_base::openmode = ios_base::in | ios_base::out);
		obuf/ibufのpointをoffで変更する。
	pos_type seekpos(pos_type, ios_base::openmode = ios_base::in | ios_base::out);
		obuf/ibufのpointをposで変更する。
	_Myt *setbuf(_Elem *, streamsize);
		外部agentから提供されるバッファを利用する 内部で
	int sync();
		外部agentと同期?
		flushに相当。
	void imbue(const locale&);
		localeを設定?
*/
namespace hmLib{
	template<class _Elem,class _Traits=std::char_traits<_Elem> >
	class basic_streambuf_interface:public virtual std::basic_streambuf<_Elem, _Traits>{
/*===for virtual functions===
	protected://common virtual functions
		pos_type iobuf_seekpos(pos_type abspos,std::ios_base::openmode which=std::ios_base::in|std::ios_base::out)override=0;
		pos_type iobuf_seekoff(off_type relpos,std::ios_base::seekdir dir,std::ios_base::openmode which=std::ios_base::in|std::ios_base::out)override=0;
	protected://streambuf_interface ibuf
		_Elem ibuf_overflow()override=0;
		streamsize ibuf_gets(_Elem* str,streamsize size)override=0;
		bool ibuf_unget(_Elem c)override=0;
		_Elem ibuf_peek()override=0;
	protected://streambuf_interface obuf
		bool obuf_overflow(_Elem c)override=0;
		streamsize obuf_puts(const _Elem* str,streamsize size)override=0;
		bool obuf_flush()override=0;
*/
	private://my typedef
		typedef basic_streambuf_interface<_Elem,_Traits> my_type;
		typedef std::basic_streambuf<_Elem,_Traits> my_streambuf;
	protected://common typedef
		typedef std::streamsize streamsize;
		typedef typename my_streambuf::pos_type pos_type;
		typedef typename my_streambuf::off_type off_type;
		typedef typename my_streambuf::int_type int_type;
	protected://common virtual functions
		//戻り値は移動後の絶対位置
		virtual pos_type iobuf_seekpos(pos_type abspos,std::ios_base::openmode which=std::ios_base::in|std::ios_base::out)=0;
		//戻り値は移動後の絶対位置
		virtual pos_type iobuf_seekoff(off_type relpos,std::ios_base::seekdir dir,std::ios_base::openmode which=std::ios_base::in|std::ios_base::out)=0;
	protected://ibuf virtual functions
		//戻り値は次の文字
		virtual _Elem ibuf_overflow()=0;
		//戻り値は成功した文字数
		virtual streamsize ibuf_gets(_Elem* str,streamsize size)=0;
		//戻り値は失敗した場合はtrue
		virtual bool ibuf_unget(_Elem c)=0;
		//戻り値は次の文字
		virtual _Elem ibuf_peek()=0;
	protected://obuf virtual functions
		//戻り値は失敗した場合はtrue
		virtual bool obuf_overflow(_Elem c)=0;
		//戻り値は成功した文字数
		virtual streamsize obuf_puts(const _Elem* str,streamsize size)=0;
		//戻り値は失敗した場合はtrue
		virtual bool obuf_flush()=0;
	protected://ibuf functions
		void ibuf_set(_Elem* pstrt,_Elem* pend){setg(pstrt,pstrt,pend);}
		_Elem* ibuf_begin(){return this->eback();}
		_Elem* ibuf_end(){return this->egptr();}
		_Elem* ibuf_getpos(){return this->gptr();}
		void ibuf_setpos(_Elem* ptr){this->gbump((int)(ptr-ibuf_pos()));}
		int ibuf_used(){return static_cast<int>(ibuf_pos()-ibuf_begin());}
		int ibuf_size(){return static_cast<int>(ibuf_end()-ibuf_begin());}
	protected://obuf functions
		void obuf_set(_Elem* pstrt,_Elem* pend){setp(pstrt,pend);}
		_Elem* obuf_begin(){return this->pbase();}
		_Elem* obuf_end(){return this->epptr();}
		_Elem* obuf_getpos(){return this->pptr();}
		void obuf_setpos(_Elem* ptr){this->pbump((int)(ptr-obuf_pos()));}
		int obuf_used(){return static_cast<int>(obuf_pos()-obuf_begin());}
		int obuf_size(){return static_cast<int>(obuf_end()-obuf_begin());}
	private://streambuf override functions
		streamsize xsputn(const _Elem* c_p,std::streamsize size)override{return obuf_puts(c_p,size);}
		streamsize xsgetn(_Elem* c_p,std::streamsize size)override{return ibuf_gets(c_p,size);}
		pos_type seekpos(pos_type abspos_,std::ios_base::openmode which=std::ios_base::in|std::ios_base::out)override{return iobuf_seekpos(abspos_,which);}
		pos_type seekoff(off_type relpos_,std::ios_base::seekdir dir_,std::ios_base::openmode which_ = std::ios_base::in | std::ios_base::out)override{return iobuf_seekoff(relpos_,dir_,which_);}
		int_type overflow(int_type c_= _Traits::eof())override{return static_cast<int_type>(!obuf_overflow(c_));}
		int_type uflow()override{return _Traits::to_int_type(ibuf_overflow());}
		int_type underflow()override{return _Traits::to_int_type(ibuf_peek());}
		int sync()override{return -static_cast<int>(obuf_flush());}
		int_type pbackfail(int_type nCh)override{return _Traits::to_int_type(!ibuf_unget((_Elem)nCh));}
	};
	typedef basic_streambuf_interface<char,std::char_traits<char>> streambuf_interface;
}
#endif
