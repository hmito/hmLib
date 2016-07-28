#ifndef HMLIB_IOCSV_INC
#define HMLIB_IOCSV_INC 100
#
#include<string>
#include<hmLib/table.hpp>
#include<hmLib/csv_iterator.hpp>
#include<hmLib/exceptions.hpp>
namespace hmLib{
	namespace{
		struct iocsv_identifier{};
	}
	using iocsv_exception = hmLib::exceptions::exception_pattern<iocsv_identifier>;

	template<typename Elem=char, typename Traits=std::char_traits<Elem> >
	basic_table<Elem, Traits> read_csv(std::string FileName, bool HasNameRow = false, bool IsHorizontalColumn = false, Elem SepChar = ',', Elem EndChar = '\n', Elem EscChar ='"'){
		using my_ifstream = std::basic_ifstream<Elem, Traits>;
		using my_table = basic_table<Elem, Traits>;
		using my_icsv_iterator = basic_icsv_iterator<Elem, Traits>;

		//open CSV file, or return null data
		my_ifstream fin(FileName);
		if(!fin.is_open())return my_table();

		//get iterator
		my_icsv_iterator beg(fin, SepChar, EndChar, EscChar);
		my_icsv_iterator end = beg.get_file_end();

		//count row and column
		unsigned int RowNum = 0;
		unsigned int ColumnNum = 0;

		//investigate max size of row/column
		{
			unsigned int ColumnCnt = 0;
			for(my_icsv_iterator itr = beg; itr != end; ++itr){
				if(itr.eol()){
					ColumnNum = std::max(ColumnNum, ColumnCnt);
					ColumnCnt = 0;
					++RowNum;
				} else{
					++ColumnCnt;
				}
			}
			ColumnNum = std::max(ColumnNum, ColumnCnt);
			++RowNum;
		}

		//create table
		my_table Table;

		//read Horizontal Column
		if(IsHorizontalColumn){
			if(HasNameRow){
				Table.assign(ColumnNum-1, RowNum);
			} else{
				Table.assign(ColumnNum, RowNum);
			}

			my_icsv_iterator itr = beg;

			unsigned int RowCnt = 0;
			unsigned int ColumnCnt = 0;
			while(itr != end){
				if(HasNameRow){
					if(ColumnCnt == 0) Table.column(RowCnt).rename(*itr);
					else Table(ColumnCnt-1, RowCnt) = *itr;
				} else{
					Table(ColumnCnt, RowCnt) = *itr;
				}
				++itr;

				if(itr.eol()){
					ColumnCnt = 0;
					++RowCnt;
				} else{
					++ColumnCnt;
				}
			}
		} else {
			if(HasNameRow){
				Table.assign(RowNum - 1, ColumnNum);
			} else{
				Table.assign(RowNum, ColumnNum);
			}

			my_icsv_iterator itr = beg;

			unsigned int RowCnt = 0;
			unsigned int ColumnCnt = 0;

			if(HasNameRow){
				do{
					Table.column(ColumnCnt++).rename(*itr++);
				} while(!itr.eol());
			}

			ColumnCnt = 0;
			while(itr != end){
				Table(RowCnt, ColumnCnt) = *itr;
				++itr;

				if(itr.eol()){
					ColumnCnt = 0;
					++RowCnt;
				} else{
					++ColumnCnt;
				}
			}
		}

		return std::move(Table);
	}
	template<typename Elem = char, typename Traits = std::char_traits<Elem> >
	void write_csv(const basic_table<Elem, Traits>& Table,std::string FileName, bool HasNameRow = false, bool IsHorizontalColumn = false, Elem SepChar = ',', Elem EndChar = '\n', Elem EscChar = '"'){
		using my_ofstream = std::basic_ofstream<Elem, Traits>;
		using my_ocsv_iterator = basic_ocsv_iterator<Elem, Traits>;

		//open CSV file, or return null data
		my_ofstream fout(FileName);
		hmLib_assert(fout.is_open(), iocsv_exception, "fail to open \"" + FileName + "\" for csv file");

		//get iterator
		my_ocsv_iterator itr(fout, SepChar, EndChar, EscChar);

		if(IsHorizontalColumn){
			for(unsigned int ColumnCnt = 0; ColumnCnt < Table.column_size(); ++ColumnCnt){
				if(HasNameRow)*itr++ = Table.column(ColumnCnt).name();
				for(unsigned int RowCnt = 0; RowCnt < Table.row_size(); ++RowCnt){
					*itr++ = Table.at(RowCnt, ColumnCnt);
				}
				itr.endl();
			}
		} else{
			if(HasNameRow){
				for(unsigned int ColumnCnt = 0; ColumnCnt < Table.column_size(); ++ColumnCnt){
					*itr++ = Table.column(ColumnCnt).name();
				}
				itr.endl();
			}

			for(unsigned int RowCnt = 0; RowCnt < Table.row_size(); ++RowCnt){
				for(unsigned int ColumnCnt = 0; ColumnCnt < Table.column_size(); ++ColumnCnt){
					*itr++ = Table.at(RowCnt, ColumnCnt);
				}
				itr.endl();
			}
		}
	}
}
#
#endif
