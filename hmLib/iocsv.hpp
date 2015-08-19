#ifndef HMLIB_IOCSV_INC
#define HMLIB_IOCSV_INC 100
#
#include<string>
#include<hmLib/table.hpp>
#include<hmLib/csv_iterator.hpp>
namespace hmLib{
	template<typename Elem=char, typename Traits=std::char_traits<Elem> >
	basic_table<Elem, Traits> read_csv(std::string FileName, Elem SepChar=',', Elem EndChar='\n', bool HasNameRow = false, bool IsHorizontalColumn = false){
		using my_ifstream = std::basic_ifstream<Elem, Traits>;
		using my_table = basic_table<Elem, Traits>;
		using my_icsv_iterator = basic_icsv_iterator<Elem, Traits>;

		//open CSV file, or return null data
		my_ifstream fin(FileNmae);
		if(!fin.is_open())return my_table();
	
		//get iterator
		my_icsv_iterator beg(fin, SepChar, EndChar);
		my_icsv_iterator end = itr.get_file_end();

		//count row and column
		unsigned int RowNum = 0;
		unsigned int ColumnNum = 0;
		unsigned int ColumnCnt = 0;
		for(my_icsv_iterator itr = beg; itr != end; ++itr){			
			if(itr.eol()){
				ColumnNum = std::max(ColumnNum, ColumnCnt);
				ColumnCnt = 0;
				++RowNum;
			}else{
				++ColumnCnt;
			}
		}
		ColumnNum = std::max(ColumnNum, ColumnCnt);
		++RowNum;

		//create table
		my_table Table(RowNum,ColumnNum);
		unsigned int RowCnt = 0;
		ColumnCnt = 0;
		for(my_icsv_iterator itr = beg; itr != end;){
			Table(RowCnt, ColCnt) = *itr;
			++itr;

			if(itr.eol()){
				ColumnCnt = 0;
				++RowCnt;
			}else{
				++ColumnCnt;
			}
		}

		return std::move(Table);
	}

	//template<typename Elem=char, typename Traits=std::char_traits<Elem>>
	//virtualなstringを返すtableがほしい
	//基本的な動作は同じ　違いは読みだされるまでメモリに文字を格納しない点のみ
	//istream_stringみたいなものを作って全部解決しないか？	
	//	書き込まれたところだけ、stringで保持し直す
	//	optionalが必要か eitherでも可
	//読み書きが両方できるはず
	//const string& へのcast operatorとstr関数を両方持つべきか
	//empty/size関数なんかは保持していてもいいかも
	//異なるtemplate引数に基づくbasic_table間のconvert機能も欲しい
}
#
#endif
