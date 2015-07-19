#ifndef HMLIB_TABLE_INC
#define HMLIB_TABLE_INC 100
#
#include<string>
#include<vector>
#include<algorithm>
#include<hmLib/position.hpp>
namespace hmLib{
	template<class Elem = char, class Traits = std::char_traits<Elem> >
	class basic_table{
	private:
		using my_type = basic_table < Elem, Traits >;
	public:
		using size_type = unsigned int;
		using string_type = std::basic_string < Elem, Traits >;
		using column_type = std::vector < string_type >;
		using data_type = std::pair < string_type, column_type >;
	private:
		std::vector<data_type> Data;
	private:
		struct proxy{
			operator string_type&();
			operator const string_type&()const;
			proxy& operator=(const string_type& Str_);
			template<typename type>
			void operator>>(type Type);
			template<typename type>
			void operator<<(type Type);
		};
		struct const_proxy{
			operator const string_type&()const;
			template<typename type>
			void operator>>(type Type);
		};
		template<typename type>
		struct type_proxy{
			operator type();
			type_proxy<type>& operator=(const type& Val);
		};
		template<typename type>
		struct const_type_proxy{
			operator type();
		};
	private:
		using data_iterator = typename std::vector<data_type>::iterator;
		using const_data_iterator = typename std::vector<data_type>::const_iterator;
		data_iterator data_begin(){ return Data.begin(); }
		data_iterator data_end(){ return Data.end(); }
		const_data_iterator data_begin()const{ return Data.begin(); }
		const_data_iterator data_end()const{ return Data.end(); }
	public:
		struct row_iterator;
		struct const_row_iterator;
		struct column_iterator;
		struct const_column_iterator;
		struct casted_column_iterator;
		struct const_casted_column_iterator;
		struct name_iterator;
		struct const_name_iterator;
	public:
		size_type find_column(const string_type& column_name_)const{ return std::find_if(data_begin(), data_end(), [](const data_type& d)->bool{return d.first == column_name; }); }
		void push_back_column(column_type column_ = column_type(), string_type column_name_ = ""){ Data.push_back(std::make_pair(column_name_, column_); }
		void pop_back_column(){ Data.pop_back(); }
		void insert_column(size_type column_no_, column_type column_ = column_type(), string_type column_name_ = "");
		void erase_column(size_type column_no_);		
		void clear();
	public://access column
		column_iterator column_begin(size_type column_no_){ return column_iterator(Data.at(column_no_).second.begin()); }
		column_iterator column_end(size_type column_no_){ return column_iterator(Data.at(column_no_).second.end()); }
		const_column_iterator column_begin(size_type column_no_)const{ return const_column_iterator(Data.at(column_no_).second.begin()); }
		const_column_iterator column_end(size_type column_no_)const{ return const_column_iterator(Data.at(column_no_).second.end()); }
	public://access column as casted style
		casted_column_iterator casted_column_begin(size_type column_no_);
		casted_column_iterator casted_column_end(size_type column_no_);
		const_casted_column_iterator casted_column_begin(size_type column_no_)const;
		const_casted_column_iterator casted_column_end(size_type column_no_)const;
	public://access name of column
		name_iterator name_begin(){ return name_iterator(data_begin()); }
		name_iterator name_end(){ return name_iterator(data_end()); }
		const_name_iterator name_begin()const{ return const_name_iterator(data_begin()); }
		const_name_iterator name_end()const{ return const_name_iterator(data_end()); }
	public://access row
		row_iterator row_begin(size_type row_no_){ return row_iterator(data_begin(),row_no_); }
		row_iterator row_end(size_type row_no_){ return row_iterator(data_end(), row_no_); }
		const_row_iterator row_begin(size_type row_no_)const{ return const_row_iterator(data_begin(), row_no_); }
		const_row_iterator row_end(size_type row_no_)const{ return const_row_iterator(data_end(), row_no_); }
	public://access directoly
		proxy at(size_type row_pos_, size_type column_pos_){ return proxy(Data.at(column_pos_).second.at(row_pos_)); }
		const_proxy at(size_type row_pos_, size_type column_pos_)const{ return const_proxy(Data.at(column_pos_).second.at(row_pos_)); }
		proxy operator()(size_type row_pos_, size_type column_pos_){ return proxy(Data.at(column_pos_).second.at(row_pos_)); }
		const_proxy operator()(size_type row_pos_, size_type column_pos_)const{ return const_proxy(Data.at(column_pos_).second.at(row_pos_)); }
		template<typename type>
		type_proxy<type> casted_at(size_type row_pos_, size_type column_pos_){ return type_proxy<type>(Data.at(column_pos_).second.at(row_pos_)); }
		template<typename type>
		const_type_proxy<type> casted_at(size_type row_pos_, size_type column_pos_)const{ return const_type_proxy<type>(Data.at(column_pos_).second.at(row_pos_)); }
	};
	using table = basic_table<char, std::char_traits < char >> ;
}
#
#endif
