#ifndef HMLIB_CSVSTREAM_INC
#define HMLIB_CSVSTREAM_INC 101
#
#include<vector>
#include<sstream>
#include<fstream>
#ifndef HMLIB_FILTERBUF_INC
#	include <hmLib_v3_04/filterbuf.hpp>
#endif
#ifndef HMLIB_STREAMBUFPATTERN_INC
#	include<hmLib_v3_04/streambuf_pattern.hpp>
#endif
#
#ifndef HMLIB_CONFIG_INC
#	include "hmLib_config.h"
#endif
#ifndef HMLIB_NOLIB
#	ifdef _DEBUG
#		pragma comment(lib,"hmLib_v3_04/lib/Debug/csvstream.lib")
#	else
#		pragma comment(lib,"hmLib_v3_04/lib/Release/csvstream.lib")
#	endif
#endif

namespace hmLib{
	namespace csv{
		class manipu_endl{};
		class manipu_endc{};
		class manipu_assert_endc{};
		class manipu_assert_endl{};
		class manipu_enclosure{};
		class manipu_noenclosure{};
		extern manipu_endl endl;
		extern manipu_endc endc;
		extern manipu_assert_endc assert_endc;
		extern manipu_assert_endl assert_endl;
		extern manipu_enclosure enclosure;
		extern manipu_noenclosure noenclosure;
	}
	class csvstreambuf:public filterbuf,public pattern::nonbuf_streambuf_pattern{
	public:
		typedef pattern::nonbuf_streambuf_pattern::pos_type pos_type;
		typedef pattern::nonbuf_streambuf_pattern::off_type off_type;
		typedef pattern::nonbuf_streambuf_pattern::int_type int_type;
	public:
		struct cellpos{
		public:
			unsigned int row;
			unsigned int col;
		public:
			cellpos():row(0),col(0){return;}
			cellpos(unsigned int row_,unsigned int col_):row(row_),col(col_){return;}
			bool operator<(const cellpos& my_)const{return (row<my_.row) || (row==my_.row && col<my_.col);}
			bool operator==(const cellpos& my_)const{return row==my_.row && col==my_.col;}
			void increment_cell(){col+=1;}
			void increment_line(){
				row+=1;
				col=0;
			}
			cellpos get_next_cell()const{
				cellpos ans(*this);
				ans.increment_cell();
				return ans;
			}
			cellpos get_next_line()const{
				cellpos ans(*this);
				ans.increment_line();
				return ans;
			}
		};
		typedef std::pair<pos_type,cellpos> cellpair;
	private:
		std::vector<cellpair> Cell;
		std::vector<cellpair>::iterator CellItr;
		char SepChar;		//セル区切り文字
		bool IsEnclosure;	//セルの「"」囲み指定
		bool IsInDQ;		//「"」内にいるかどうか
		bool IsDQNext;		//「"」の次の文字かどうか
		bool IsEndCell;		//セル末端まで読み込んだか　assert_endCellしないと、次にはいけない
		bool IsEndLine;		//行末端まで読み込んだか	assert_endLineしないと、次にはいけない
		bool IsCellErr;		//assertに対するエラーでCellErr
	private:
		void ini_cellstatus(){
			IsInDQ=false;
			IsDQNext=false;
			IsEndCell=false;
			IsEndLine=false;
		}
	public:
		void ini(char SepChar_){
			SepChar=SepChar_;
			IsEnclosure=false;

			ini_cellstatus();

			clear_cellerr();

			Cell.assign(1,cellpair(0,cellpos(0,0)));
			CellItr=Cell.begin();
		}
	private:
		//バーの次の文字を入れる
		void step_front(char c){
			//DQ文字取得
			if(c=='"'){
				//ChangeDQMode
				IsInDQ=!IsInDQ;

				//DQ_Escape
				IsDQNext=true;

			}//DQ以外取得
			else{
				//DQNextgを外す
				IsDQNext=false;

				//IsInDQgでない場合
				if(!IsInDQ){
					//区切り文字取得
					if(c==SepChar){
						++CellItr;
						IsEndCell=true;
						//読んだとこより遠くなら、新規追加
						if(CellItr==Cell.end()){
							Cell.push_back(cellpair(tell_io(),Cell.back().second.get_next_cell()));
							CellItr=Cell.end()-1;
						}
					}//改行文字取得
					else if(c=='\n' || c=='\0'){
						++CellItr;
						IsEndCell=true;
						IsEndLine=true;
						//読んだとこより遠くなら、新規追加
						if(CellItr==Cell.end()){
							Cell.push_back(cellpair(tell_io(),Cell.back().second.get_next_line()));
							CellItr=Cell.end()-1;
						}
					}
				}
			}
		}
	public://const系
		bool eof()const{return ref_in().eof();}
		bool cellerr()const{return IsCellErr;}
		bool is_enclosed()const{return IsInDQ;}
		bool is_escapeEnclosure()const{return IsDQNext;}
	public://フラグセット系
		void clear_cellerr(){IsCellErr=false;}
		void withEnclosure(){IsEnclosure=true;}
		void withoutEnclosure(){IsEnclosure=false;}
	public:
		bool is_newCell(){return (CellItr->first==tell_io());}
		bool is_newLine(){return (is_newCell() && CellItr->second.col==0);}
		bool assert_newCell(){
			if(!is_newCell())IsCellErr=true;
			IsEndCell=false;
			return IsCellErr;
		}
		bool assert_newLine(){
			if(!is_newLine())IsCellErr=true;
			IsEndCell=false;
			IsEndLine=false;
			return IsCellErr;
		}
		void endCell(){
			if(!is_newCell() && IsInDQ)putc('"');	
		}
		void newLine(){
			endCell();
			putc(static_cast<char>('\n'));
			assert_newLine();
		}
		void newCell(){
			endCell();
			putc(SepChar);
			assert_newCell();
		}
		pos_type posCell()const{
			return CellItr->first;
		}
		pos_type posLine()const{
			auto itr=CellItr;
			while(itr->second.col!=0)--itr;
			return itr->first;
		}
		off_type sizeLine(){
			pos_type now=tell_io();
			ignoreLine();
			pos_type nextline=CellItr->first;
			seek(now);
			return nextline-posLine();
		}
		off_type sizeCell(){
			pos_type now=ref_in().tellg();
			ignoreCell();
			pos_type nextcell=CellItr->first;
			seek(now);
			return nextcell-posCell();
		}
		bool ignoreLine(unsigned int num=1){
			while(num--){
				//現在の行以降を探索済み
				if(CellItr->second.row!=Cell.back().second.row){
					unsigned int row=CellItr->second.row;
					while(CellItr->second.row==row)++CellItr;
					seek_io(CellItr->first);
					ini_cellstatus();
				}//現在の行以降を未探索
				else{
					//最終セルに移動
					if(CellItr!=Cell.end()-1){
						CellItr=Cell.end()-1;
						seek_io(CellItr->first);
						ini_cellstatus();
					}
					do{
						getc();
						if(is_newCell())assert_newCell();
					}while(!is_newLine());
				}
				//末尾まで読み込んだことを確認
				assert_newLine();
			}
			return false;
		}
		bool ignoreCell(unsigned int num=1){
			while(num--){
				//現在のセルを探索済み
				if(CellItr!=Cell.end()-1){
					++CellItr;
				}//現在のセルを未探索
				else{
					do{
						getc();
					}while(is_newCell());
				}
				//末尾まで読み込んだことを確認
				assert_newCell();
			}
			return false;
		}
		unsigned int tellRow()const{return CellItr->second.row;}
		unsigned int tellCol()const{return CellItr->second.col;}
		unsigned int sizeRow(){
			pos_type pos=tell_io();
			seek(0,std::ios_base::end);
			unsigned int ans=(CellItr-1)->second.row+1;
			seek(pos);
			return ans;
		}
		unsigned int sizeCol(){
			pos_type pos=tell_io();
			ignoreLine(1);
			unsigned int ans=(CellItr-1)->second.col+1;
			seek(pos);
			return ans;
		}
		unsigned int seekRow(unsigned int abspos_){
			seek(0,std::ios_base::beg);
			ignoreLine(abspos_);
			return tellRow();
		}
		unsigned int seekRow(int relpos_,std::ios_base::seekdir dir_){
			//先頭からの移動
			if(dir_==std::ios_base::beg){
				return seekRow(0+relpos_);
			}else if(dir_==std::ios_base::end){
				return seekRow(sizeRow()-1-relpos_);
			}else{
				return seekRow(tellCol()+relpos_);
			}
		}
		unsigned int seekCol(unsigned int abspos_){
			for(auto itr=Cell.begin();itr!=Cell.end();++itr){
				if(itr->second.row==tellRow()){
					CellItr=itr;
					seek_io(CellItr->first);
					ini_cellstatus();

					break;
				}

				if(itr==Cell.end()-1){
					IsCellErr=true;
					return tellCol();
				}
			}

			ignoreCell(abspos_);

			return tellCol();
		}
		unsigned int seekCol(int relpos_,std::ios_base::seekdir dir_){
			//先頭からの移動
			if(dir_==std::ios_base::beg){
				return seekCol(0+relpos_);
			}else if(dir_==std::ios_base::end){
				return seekCol(sizeCol()-1-relpos_);
			}else{
				return seekCol(tellCol()+relpos_);
			}
		}
	public://nonbuf_streambuf_pattern
		virtual void putc(char c){
			if(IsEndCell || IsEndLine)IsCellErr=true;
			if(IsCellErr)return;
			if(IsEnclosure && is_newCell()){
				ref_out().put('"');
				step_front('"');
			}
			ref_out().put(c);
			//最終セルへの追加以外の場合
			if(CellItr+1!=Cell.end()){
				CellItr=Cell.erase(CellItr+1,Cell.end())-1;
			}
			step_front(c);
		}
		virtual void puts(const char* str,std::streamsize size){
			if(IsEndCell || IsEndLine)IsCellErr=true;
			if(IsCellErr)return;
			for(std::streamsize i=0;i<size;i++)putc(str[i]);
		}
		virtual void flush(){ref_out().flush();}
		virtual char getc(){
			if(IsEndCell || IsEndLine)IsCellErr=true;
			if(IsCellErr)return 0;
			bool tmpIsNewCell=is_newCell();
			char c=static_cast<char>(ref_in().get());
			step_front(c);
			if(tmpIsNewCell && c=='"'){
				c=static_cast<char>(ref_in().get());
				step_front(c);
			}
			return c;
		}
		virtual std::streamsize gets(char* str,std::streamsize size){
			if(IsEndCell || IsEndLine)IsCellErr=true;
			if(IsCellErr)return 0;
			for(std::streamsize i=0;i<size;i++)str[i]=getc();
			return size;
		}
		virtual std::streamsize gets(char* str,std::streamsize maxsize,char endchar){
			if(IsEndCell || IsEndLine)IsCellErr=true;
			if(IsCellErr)return 0;
			for(std::streamsize i=0;i<maxsize;i++){
				str[i]=getc();
				if(str[i]==endchar)return i+1;
			}
			return maxsize;
		}
		virtual char peek(){return static_cast<char>(ref_in().peek());}
		virtual void unget(char c){ref_in().unget();}
		virtual std::streambuf::pos_type seek(off_type relpos_,std::ios_base::seekdir dir_,std::ios_base::openmode which=std::ios_base::in|std::ios_base::out){
			//先頭からの移動
			if(dir_==std::ios_base::beg){
				seek(0+relpos_);
			}else if(dir_==std::ios_base::end){
				pos_type here=tell_io();
				seek_io(0,std::ios_base::end);
				pos_type target=tell_io()+relpos_;
				seek_io(here);
				seek(target);
			}else{
				seek(ref_in().tellg()+relpos_);
			}

			return ref_in().tellg();
		}
		virtual std::streambuf::pos_type seek(pos_type abspos_,std::ios_base::openmode which=std::ios_base::in|std::ios_base::out){
			if(abspos_<0)return tell_io();

			//ターゲットの位置を把握
			auto TargetItr=Cell.begin();
			for(;TargetItr!=Cell.end();++TargetItr){
				if(abspos_<TargetItr->first)break;
			}
			--TargetItr;

			//現在セルと違ったり、同一セルでも現在地より手前の場合
			if(TargetItr!=CellItr || abspos_<tell_io()){
				CellItr=TargetItr;
				seek_io(CellItr->first);
				ini_cellstatus();
			}

			//ターゲットにたどり着くまで、ひたすらstep_front
			if(tell_io()<abspos_){
				if(!is_in())throw "ostream can't check";
				while(tell_io()<abspos_ && tell_io()>=0)step_front(ref_in().get());
			}

			return tell_io();
		}	private:
	public:
		csvstreambuf():filterbuf(){
			return;
		}
		csvstreambuf(std::iostream& io_,char SepChar_=','):filterbuf(io_){
			ini(SepChar_);
		}
		csvstreambuf(std::istream& in_,char SepChar_=','):filterbuf(in_){
			ini(SepChar_);
		}
		csvstreambuf(std::ostream& out_,char SepChar_=','):filterbuf(out_){
			ini(SepChar_);
		}
		~csvstreambuf(){return;}
		bool is_open()const{return filterbuf::is_open();}
		void open(std::iostream& io_,char SepChar_=','){
			if(is_open())close();
			filterbuf::open_io(io_);
			ini(SepChar_);
		}
		void open(std::istream& io_,char SepChar_=','){
			if(is_open())close();
			filterbuf::open_io(io_);
			ini(SepChar_);
		}
		void open(std::ostream& io_,char SepChar_=','){
			if(is_open())close();
			filterbuf::open_io(io_);
			ini(SepChar_);
		}
		void close(){
			filterbuf::close_io();
		}
	};
	class icsvfilter:public std::istream{
	private:
		csvstreambuf* streambuf;
	public:
		class cell{
		private:
			bool IsEscapeEnclosure;
			csvstreambuf* streambuf;
		protected:
			cell(csvstreambuf* streambuf_,bool IsEscapeEnclosure_=false):streambuf(streambuf_),IsEscapeEnclosure(IsEscapeEnclosure_){return;}
		public:
			cell(icsvfilter& filter_,bool IsEscapeEnclosure_=false):streambuf(filter_.streambuf),IsEscapeEnclosure(IsEscapeEnclosure_){return;}
			cell& ignoreCell(unsigned int num_=1){
				streambuf->ignoreCell(num_);
				return *this;
			}
			cell& ignoreLine(unsigned int num_=1){
				streambuf->ignoreLine(num_);
				return *this;
			}
			cell& assert_newLine(){
				streambuf->assert_newLine();
				return *this;
			}
			template<typename T>
			cell& get(T& t_){
				pos_type pos=streambuf->ref_in().tellg();
				streambuf->ref_in().seekg(0,std::ios_base::end);
				pos_type end=streambuf->ref_in().tellg();
				streambuf->ref_in().seekg(pos);

				std::stringstream strin("");
				while(true){
					char c=static_cast<char>(streambuf->getc());	//一回目はOK 二回目の時点ですでにCellErr
					if(streambuf->is_newCell())break;
					if( !(IsEscapeEnclosure && streambuf->is_escapeEnclosure()) ) strin.put(c);
					if(streambuf->ref_in().tellg()>=end || streambuf->eof() || streambuf->cellerr())break;
				}
				streambuf->assert_newCell();

				strin>>t_;

				return *this;
			}
			template<class T>
			cell& operator>>(T& t_){return get(t_);}
			bool eof(){return streambuf->eof();}
			bool cellerr(){return streambuf->cellerr();}
			bool is_newLine(){return streambuf->is_newLine();}
			pos_type posLine(){return streambuf->posLine();}
			pos_type sizeLine(){return streambuf->sizeLine();}
			unsigned int tellRow()const{return streambuf->tellRow();}
			unsigned int tellCol()const{return streambuf->tellCol();}
			unsigned int sizeRow(){return streambuf->sizeRow();}
			unsigned int seekRow(unsigned int abspos_){return streambuf->seekRow(abspos_);}
			unsigned int seekRow(int relpos_,std::ios_base::seekdir dir_){return streambuf->seekRow(relpos_,dir_);}
			unsigned int sizeCol(){return streambuf->sizeCol();}
			unsigned int seekCol(unsigned int abspos_){return streambuf->seekCol(abspos_);}
			unsigned int seekCol(int relpos_,std::ios_base::seekdir dir_){return streambuf->seekCol(relpos_,dir_);}
			cell& operator>>(csv::manipu_endl endl_){
				if(!is_newLine())ignoreLine(1);
				return assert_newLine();
			}
			cell& operator>>(csv::manipu_assert_endl assert_endl_){return assert_newLine();}
		};
	protected:
		bool is_escapeEnclosure()const{return streambuf->is_escapeEnclosure();}
	public:
		icsvfilter():std::istream(streambuf=new csvstreambuf()){
		}
		icsvfilter(std::istream& in_,char SepChar_=','):std::istream(streambuf=new csvstreambuf(in_,SepChar_)){}
		virtual ~icsvfilter(){
			close();
			delete streambuf;
		}
		void open(std::istream& in_,char SepChar_=','){
			if(is_open())close();
			streambuf->open(in_,SepChar_);
		}
		bool is_open()const{return streambuf->is_open();}
		void close(){if(is_open())streambuf->close();}
		bool is_newCell(){return streambuf->is_newCell();}
		bool is_newLine(){return streambuf->is_newLine();}
		bool is_enclosed(){return streambuf->is_enclosed();}
		bool eof(){return streambuf->eof();}
		bool cellerr()const{return streambuf->cellerr();}
		pos_type posCell(){return streambuf->posCell();}
		pos_type posLine(){return streambuf->posLine();}
		pos_type sizeCell(){return streambuf->sizeCell();}
		pos_type sizeLine(){return streambuf->sizeLine();}
		icsvfilter& ignoreCell(unsigned int num_=1){
			streambuf->ignoreCell(num_);
			return *this;
		}
		icsvfilter& ignoreLine(unsigned int num_=1){
			while(num_--)streambuf->ignoreLine();
			return *this;
		}	
		icsvfilter& assert_newCell(){
			streambuf->assert_newCell();
			return *this;
		}
		icsvfilter& assert_newLine(){
			streambuf->assert_newLine();
			return *this;
		}
		unsigned int tellRow()const{return streambuf->tellRow();}
		unsigned int tellCol()const{return streambuf->tellCol();}
		unsigned int sizeRow(){return streambuf->sizeRow();}
		unsigned int seekRow(unsigned int abspos_){return streambuf->seekRow(abspos_);}
		unsigned int seekRow(int relpos_,std::ios_base::seekdir dir_){return streambuf->seekRow(relpos_,dir_);}
		unsigned int sizeCol(){return streambuf->sizeCol();}
		unsigned int seekCol(unsigned int abspos_){return streambuf->seekCol(abspos_);}
		unsigned int seekCol(int relpos_,std::ios_base::seekdir dir_){return streambuf->seekCol(relpos_,dir_);}
		void clear(){
			std::istream::clear();
			streambuf->clear_cellerr();
		}
		template<class T>
		icsvfilter& operator>>(T& t){
			(*static_cast<std::istream*>(this))>>t;
			return *this;
		}
		icsvfilter& operator>>(csv::manipu_endc endc_){
			if(!is_newCell())ignoreCell(1);
			return assert_newCell();
		}
		icsvfilter& operator>>(csv::manipu_endl endl_){
			if(!is_newLine())ignoreLine(1);
			return assert_newLine();
		}
		icsvfilter& operator>>(csv::manipu_assert_endc assert_endc_){return assert_newCell();}
		icsvfilter& operator>>(csv::manipu_assert_endl assert_endl_){return assert_newLine();}
	};
	class ocsvfilter:public std::ostream{
	private:
		csvstreambuf* streambuf;
	public:
		class cell{
		private:
			bool IsEscapeEnclosure;
			csvstreambuf* streambuf;
		protected:
			cell(csvstreambuf* streambuf_,bool IsEscapeEnclosure_=false):streambuf(streambuf_),IsEscapeEnclosure(IsEscapeEnclosure_){return;}
		public:
			cell(ocsvfilter& filter_,bool IsEscapeEnclosure_=false):streambuf(filter_.streambuf),IsEscapeEnclosure(IsEscapeEnclosure_){return;}
			cell& enclosure(){
				streambuf->withEnclosure();
				return *this;
			}
			cell& noenclosure(){
				streambuf->withoutEnclosure();
				return *this;
			}
			cell& newLine(){
				streambuf->newLine();
				return *this;
			}
			bool is_newLine(){return streambuf->is_newLine();}
			unsigned int tellRow()const{return streambuf->tellRow();}
			unsigned int tellCol()const{return streambuf->tellCol();}
			template<typename T>
			cell& put(const T& t_){
				//stringstreamに読込み
				std::stringstream strout;
				strout<<t_;

				//新たなセルを準備
				if(!streambuf->is_newCell())streambuf->newCell();

				//全て読みきるまで
				while(true){
					char c=static_cast<char>(strout.get());

					if(strout.eof())break;

					streambuf->putc(c);
					if(IsEscapeEnclosure && c=='"')streambuf->putc('"');
				}

				return *this;
			}
			template<typename T>
			cell& operator<<(const T& t_){return put(t_);}
			cell& operator<<(csv::manipu_endl endl_){return newLine();}
			cell& operator<<(csv::manipu_enclosure enclosure_){return enclosure();}
			cell& operator<<(csv::manipu_noenclosure noenclosure_){return noenclosure();}
		};
	protected:
		bool is_escapeEnclosure()const{return streambuf->is_escapeEnclosure();}
	public:
		ocsvfilter():std::ostream(streambuf=new csvstreambuf()){return;}
		ocsvfilter(std::ostream& out_,char SepChar_=','):std::ostream(streambuf=new csvstreambuf(out_,SepChar_)){return;}
		virtual ~ocsvfilter(){
			close();
			delete streambuf;
		}
		void open(std::ostream& out_,char SepChar_=','){
			if(is_open())close();
			streambuf->open(out_,SepChar_);
		}
		bool is_open()const{return streambuf->is_open();}
		void close(){if(is_open())streambuf->close();}
		bool is_newCell(){return streambuf->is_newCell();}
		bool is_newLine(){return streambuf->is_newLine();}
		bool is_enclosed(){return streambuf->is_enclosed();}
		bool cellerr()const{return streambuf->cellerr();}
		ocsvfilter& endCell(){
			streambuf->endCell();
			return *this;
		}
		ocsvfilter& newCell(){
			streambuf->newCell();
			return *this;
		}
		ocsvfilter& newLine(){
			streambuf->newLine();
			return *this;
		}
		ocsvfilter& enclosure(){
			streambuf->withEnclosure();
			return *this;
		}
		ocsvfilter& noenclosure(){
			streambuf->withoutEnclosure();
			return *this;
		}
		unsigned int tellRow()const{return streambuf->tellRow();}
		unsigned int tellCol()const{return streambuf->tellCol();}
		void clear(){
			std::ostream::clear();
			streambuf->clear_cellerr();
		}
		template<class T>
		ocsvfilter& operator<<(const T& t){
			(*static_cast<std::ostream*>(this))<<t;
			return *this;
		}
		ocsvfilter& operator<<(csv::manipu_endc endc_){return newCell();}
		ocsvfilter& operator<<(csv::manipu_endl endl_){return newLine();}
		ocsvfilter& operator<<(csv::manipu_enclosure enclosure_){return enclosure();}
		ocsvfilter& operator<<(csv::manipu_noenclosure noenclosure_){return noenclosure();}
	};
	class csvfilter:public std::iostream{
	private:
		csvstreambuf* streambuf;
	public:
		class cell{
		private:
			bool IsEscapeEnclosure;
			csvstreambuf* streambuf;
		protected:
			cell(csvstreambuf* streambuf_,bool IsEscapeEnclosure_=false):streambuf(streambuf_),IsEscapeEnclosure(IsEscapeEnclosure_){return;}
		public:
			cell(csvfilter& filter_,bool IsEscapeEnclosure_=false):streambuf(filter_.streambuf),IsEscapeEnclosure(IsEscapeEnclosure_){return;}
			cell& ignoreCell(unsigned int num_=1){
				streambuf->ignoreCell(num_);
				return *this;
			}
			cell& ignoreLine(unsigned int num_=1){
				streambuf->ignoreLine(num_);
				return *this;
			}
			cell& assert_newLine(){
				streambuf->assert_newLine();
				return *this;
			}
			bool eof(){return streambuf->eof();}
			bool cellerr(){return streambuf->cellerr();}
			bool is_newLine(){return streambuf->is_newLine();}
			pos_type posLine(){return streambuf->posLine();}
			pos_type sizeLine(){return streambuf->sizeLine();}
			cell& enclosure(){
				streambuf->withEnclosure();
				return *this;
			}
			cell& noenclosure(){
				streambuf->withoutEnclosure();
				return *this;
			}
			cell& newLine(){
				streambuf->newLine();
				return *this;
			}
			unsigned int tellRow()const{return streambuf->tellRow();}
			unsigned int tellCol()const{return streambuf->tellCol();}
			unsigned int sizeRow(){return streambuf->sizeRow();}
			unsigned int seekRow(unsigned int abspos_){return streambuf->seekRow(abspos_);}
			unsigned int seekRow(int relpos_,std::ios_base::seekdir dir_){return streambuf->seekRow(relpos_,dir_);}
			unsigned int sizeCol(){return streambuf->sizeCol();}
			unsigned int seekCol(unsigned int abspos_){return streambuf->seekCol(abspos_);}
			unsigned int seekCol(int relpos_,std::ios_base::seekdir dir_){return streambuf->seekCol(relpos_,dir_);}
			template<typename T>
			cell& put(const T& t_){
				//stringstreamに読込み
				std::stringstream strout;
				strout<<t_;

				//新たなセルを準備
				if(!streambuf->is_newCell())streambuf->newCell();

				//全て読みきるまで
				while(true){
					char c=static_cast<char>(strout.get());

					if(strout.eof())break;

					streambuf->putc(c);
					if(IsEscapeEnclosure && c=='"')streambuf->putc('"');
				}

				return *this;
			}
			template<typename T>
			cell& operator<<(const T& t_){return put(t_);}
			cell& operator<<(csv::manipu_endl endl_){return newLine();}
			cell& operator<<(csv::manipu_enclosure enclosure_){return enclosure();}
			cell& operator<<(csv::manipu_noenclosure noenclosure_){return noenclosure();}
			template<typename T>
			cell& get(T& t_){
				std::stringstream strin;

				pos_type pos=streambuf->ref_in().tellg();
				streambuf->ref_in().seekg(0,std::ios_base::end);
				pos_type end=streambuf->ref_in().tellg();
				streambuf->ref_in().seekg(pos);

				while(true){
					char c=static_cast<char>(streambuf->getc());
					if(streambuf->is_newCell())break;
					if( !(IsEscapeEnclosure && streambuf->is_escapeEnclosure()) ) strin.put(c);
					if(streambuf->ref_in().tellg()>=end || streambuf->eof())break;
				}
				streambuf->assert_newCell();

				strin>>t_;

				return *this;
			}
			template<class T>
			cell& operator>>(T& t_){return get(t_);}
			cell& operator>>(csv::manipu_endl endl_){
				if(!is_newLine())ignoreLine(1);
				return assert_newLine();
			}
			cell& operator>>(csv::manipu_assert_endl assert_endl_){return assert_newLine();}
		};
	public:
		csvfilter():std::iostream(streambuf=new csvstreambuf()){return;}
		csvfilter(std::iostream& io_,char SepChar_=','):std::iostream(streambuf=new csvstreambuf(io_,SepChar_)){return;}
		csvfilter(std::istream& in_,char SepChar_=','):std::iostream(streambuf=new csvstreambuf(in_,SepChar_)){return;}
		csvfilter(std::ostream& out_,char SepChar_=','):std::iostream(streambuf=new csvstreambuf(out_,SepChar_)){return;}
		virtual ~csvfilter(){
			delete streambuf;
			close();
		}
		void open(std::iostream& io_,char SepChar_=','){
			if(is_open())close();
			streambuf->open(io_,SepChar_);
		}
		void open(std::istream& in_,char SepChar_=','){
			if(is_open())close();
			streambuf->open(in_,SepChar_);
		}
		void open(std::ostream& out_,char SepChar_=','){
			if(is_open())close();
			streambuf->open(out_,SepChar_);
		}
		bool is_open()const{return streambuf->is_open();}
		void close(){if(is_open())streambuf->close();}
		bool is_newCell(){return streambuf->is_newCell();}
		bool is_newLine(){return streambuf->is_newLine();}
		bool is_enclosed(){return streambuf->is_enclosed();}
		bool eof(){return streambuf->eof();}
		bool cellerr()const{return streambuf->cellerr();}
		pos_type posCell(){return streambuf->posCell();}
		pos_type posLine(){return streambuf->posLine();}
		pos_type sizeCell(){return streambuf->sizeCell();}
		pos_type sizeLine(){return streambuf->sizeLine();}
		csvfilter& ignoreCell(unsigned int num_=1){
			streambuf->ignoreCell(num_);
			return *this;
		}
		csvfilter& ignoreLine(unsigned int num_=1){
			streambuf->ignoreLine(num_);
			return *this;
		}	
		csvfilter& endCell(){
			streambuf->endCell();
			return *this;
		}
		csvfilter& newCell(){
			streambuf->newCell();
			return *this;
		}
		csvfilter& newLine(){
			streambuf->newLine();
			return *this;
		}
		csvfilter& assert_newCell(){
			streambuf->assert_newCell();
			return *this;
		}
		csvfilter& assert_newLine(){
			streambuf->assert_newLine();
			return *this;
		}
		csvfilter& enclosure(){
			streambuf->withEnclosure();
			return *this;
		}
		csvfilter& noenclosure(){
			streambuf->withoutEnclosure();
			return *this;
		}
		unsigned int tellRow()const{return streambuf->tellRow();}
		unsigned int tellCol()const{return streambuf->tellCol();}
		unsigned int sizeRow(){return streambuf->sizeRow();}
		unsigned int seekRow(unsigned int abspos_){return streambuf->seekRow(abspos_);}
		unsigned int seekRow(int relpos_,std::ios_base::seekdir dir_){return streambuf->seekRow(relpos_,dir_);}
		unsigned int sizeCol(){return streambuf->sizeCol();}
		unsigned int seekCol(unsigned int abspos_){return streambuf->seekCol(abspos_);}
		unsigned int seekCol(int relpos_,std::ios_base::seekdir dir_){return streambuf->seekCol(relpos_,dir_);}
		void clear(){
			std::iostream::clear();
			streambuf->clear_cellerr();
		}
		template<class T>
		csvfilter& operator<<(const T& t){
			(*static_cast<std::iostream*>(this))<<t;
			return *this;
		}
		csvfilter& operator<<(csv::manipu_endc endc_){return newCell();}
		csvfilter& operator<<(csv::manipu_endl endl_){return newLine();}
		csvfilter& operator<<(csv::manipu_enclosure enclosure_){return enclosure();}
		csvfilter& operator<<(csv::manipu_noenclosure noenclosure_){return noenclosure();}
		template<class T>
		csvfilter& operator>>(T& t){
			(*static_cast<std::iostream*>(this))>>t;
			return *this;
		}
		csvfilter& operator>>(csv::manipu_endc endc_){
			if(!is_newCell())ignoreCell(1);
			return assert_newCell();
		}
		csvfilter& operator>>(csv::manipu_endl endl_){
			if(!is_newLine())ignoreLine(1);
			return assert_newLine();
		}
		csvfilter& operator>>(csv::manipu_assert_endc assert_endc_){return assert_newCell();}
		csvfilter& operator>>(csv::manipu_assert_endl assert_endl_){return assert_newLine();}
	};
	class icsvstream:public icsvfilter{
	private:
		std::ifstream fin;
	public:
		icsvstream():icsvfilter(){}
		icsvstream(const char *FileName_,char SepChar_=',',std::ios_base::openmode Mode_ = std::ios_base::in):icsvfilter(){
			open(FileName_,SepChar_,Mode_);
		}
		icsvstream(const std::string& FileName_,char SepChar_=',',std::ios_base::openmode Mode_ = std::ios_base::in):icsvfilter(){
			open(FileName_,SepChar_,Mode_);
		}
		virtual ~icsvstream(){
			close();
		}
		void open(const char *FileName_,char SepChar_=',',std::ios_base::openmode Mode_ = std::ios_base::in){
			fin.open(FileName_,Mode_);
			if(!fin.is_open()){
				return;
			}
			icsvfilter::open(fin,SepChar_);
		}
		void open(const std::string& FileName_,char SepChar_=',',std::ios_base::openmode Mode_ = std::ios_base::in){
			open(FileName_.c_str(),SepChar_,Mode_);
		}
		void close(){
			if(!is_open())icsvfilter::close();
			if(fin.is_open())fin.close();
		}
	};
	class ocsvstream:public ocsvfilter{
	private:
		std::ofstream fout;
	public:
		ocsvstream():ocsvfilter(){return;}
		ocsvstream(const char *FileName_,char SepChar_=',',std::ios_base::openmode Mode_ = std::ios_base::out | std::ios::trunc):ocsvfilter(){
			open(FileName_,SepChar_,Mode_);
		}
		ocsvstream(const std::string& FileName_,char SepChar_=',',std::ios_base::openmode Mode_ = std::ios_base::out | std::ios::trunc):ocsvfilter(){
			open(FileName_,SepChar_,Mode_);
		}
		virtual ~ocsvstream(){close();}
		void open(const char *FileName_,char SepChar_=',',std::ios_base::openmode Mode_ = std::ios_base::out | std::ios::trunc){
			fout.open(FileName_,Mode_);
			if(!fout.is_open())return;
			ocsvfilter::open(fout,SepChar_);
		}
		void open(const std::string& FileName_,char SepChar_=',',std::ios_base::openmode Mode_ = std::ios_base::out | std::ios::trunc){
			fout.open(FileName_,Mode_);
			if(!fout.is_open())return;
			ocsvfilter::open(fout,SepChar_);
		}
		void close(){
			if(!is_open())return;
			fout.close();
			if(fout.is_open())return;
			ocsvfilter::close();
		}
	};
	class csvstream:public csvfilter{
	private:
		std::fstream fio;
	public:
		csvstream():csvfilter(){return;}
		csvstream(const char *FileName_,char SepChar_=',',std::ios_base::openmode Mode_ = std::ios_base::in|std::ios_base::out|std::ios::trunc):csvfilter(){
			open(FileName_,SepChar_,Mode_);
		}
		csvstream(const std::string& FileName_,char SepChar_=',',std::ios_base::openmode Mode_ = std::ios_base::in|std::ios_base::out|std::ios::trunc):csvfilter(){
			open(FileName_,SepChar_,Mode_);
		}
		virtual ~csvstream(){close();}
		void open(const char *FileName_,char SepChar_=',',std::ios_base::openmode Mode_ = std::ios_base::in|std::ios_base::out|std::ios::trunc){
			fio.open(FileName_,Mode_);
			if(!fio.is_open())return;
			csvfilter::open(fio,SepChar_);
		}
		void open(const std::string& FileName_,char SepChar_=',',std::ios_base::openmode Mode_ = std::ios_base::in|std::ios_base::out|std::ios::trunc){
			fio.open(FileName_,Mode_);
			if(!fio.is_open())return;
			csvfilter::open(fio,SepChar_);
		}
		void close(){
			if(!is_open())return;
			fio.close();
			if(fio.is_open())return;
			csvfilter::close();
		}
	};
}
#
#endif
