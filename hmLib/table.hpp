#ifndef HMLIB_TABLE_INC
#define HMLIB_TABLE_INC 100
#
#include<string>
#include<vector>
#include<algorithm>
#include<boost/lexical_cast.hpp>
#include<boost/iterator_adaptors.hpp>
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
	public:
		struct element_proxy{
		private:
			string_type& Str;
		public:
			element_proxy(string_type& Ref):Str(Ref){}
			operator string_type&(){ return Str; }
			operator const string_type&()const{ return Str; }
			element_proxy& operator=(const string_type& Str_){
				Str = Str_;
				return *this;
			}
			template<typename type>
			void operator>>(type& Val)const{
				Val = boost::lexical_cast<type>(Str);
			}
			template<typename type>
			void operator<<(type Val){
				Str = boost::lexical_cast<string_type>(Val);
			}
		};
		struct const_element_proxy{
		private:
			const string_type& Str;
		public:
			const_element_proxy(const string_type& Ref) :Str(Ref){}
			operator const string_type&()const{ return Str; }
			template<typename type>
			void operator>>(type& Val)const{
				Val = boost::lexical_cast<type>(Str);
			}
		};
		template<typename type>
		struct casted_element_proxy:private element_proxy{
		public:
			element_proxy(string_type& Ref) :element_proxy(Ref){}
			operator type()const{
				type Val;
				this->operator>>(Val);
				return Val;
			}
			casted_element_proxy<type>& operator=(const type& Val){
				this->operator<<(Val);
				return *this;
			}
		};
		template<typename type>
		struct const_casted_element_proxy:private const_element_proxy{
		public:
			const_casted_element_proxy(const string_type& Ref) :const_element_proxy(Ref){}
			operator type()const{
				type Val;
				this->operator>>(Val);
				return Val;
			}
		};
	public:
		struct column_proxy{
		private:
			data_type& rData;
			using base_iterator = typename data_type::second_type::iterator;
		public:
			struct element_iterator
				: public boost::iterator_adaptor<element_iterator, base_iterator, element_proxy, boost::random_access_traversal_tag, element_proxy>{
				using iterator_adaptor = boost::iterator_adaptor<element_iterator, base_iterator, element_proxy, boost::random_access_traversal_tag, element_proxy>;
			public:
				element_iterator() = default;
				element_iterator(base_iterator bitr) : iterator_adaptor(bitr){}
			private:
				element_proxy dereference()const{
					return element_proxy(*(base_reference()));
				}
			};
			template<typename type>
			struct casted_element_iterator
				: public boost::iterator_adaptor<casted_element_iterator<type>, base_iterator, casted_element_proxy<type>, boost::random_access_traversal_tag, casted_element_proxy<type> >{
				using iterator_adaptor = boost::iterator_adaptor<casted_element_iterator<type>, base_iterator, casted_element_proxy<type>, boost::random_access_traversal_tag, casted_element_proxy<type> >;
			public:
				casted_element_iterator() = default;
				casted_element_iterator(base_iterator) : iterator_adaptor(base_iterator){}
			private:
				casted_element_proxy<type> dereference()const{
					return casted_element_proxy<type>(*(base_reference()));
				}
			};
		public:
			column_proxy(data_type& rData_) :rData(rData_){}
			string_type name()const{ return rData.first; }
			element_proxy operator[](size_type Pos){ return element_proxy(rData.second.at(Pos)); }
			const_element_proxy operator[](size_type Pos)const{ return const_element_proxy(rData.second.at(Pos)); }
			element_proxy at(size_type Pos){ return element_proxy(rData.second.at(Pos)); }
			const_element_proxy at(size_type Pos)const{ return const_element_proxy(rData.second.at(Pos)); }
			size_type size()const{ return rData.second.size(); }
			element_iterator begin(){ return element_iterator(rData.second.begin()); }
			element_iterator end(){ return element_iterator(rData.second.end()); }
			template<typename type>
			casted_element_iterator<type> casted_begin(){ return casted_element_iterator<type>(rData.second.begin()); }
			template<typename type>
			casted_element_iterator<type> casted_end(){ return casted_element_iterator<type>(rData.second.end()); }
		};
		struct row_proxy{
			struct element_iterator{};
			element_proxy operator[](size_type Pos);
			element_proxy at(size_type Pos);
			size_type size()const;
			element_iterator begin();
			element_iterator end();
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
		void insert_column(column_iterator itr, column_type column_ = column_type(), string_type column_name_ = "");
		void erase_column(column_iterator itr);
		void clear();
	public://access column
		column_proxy column(size_type column_pos_){ return column_proxy(Data.at(column_pos_)); }
		const_column_proxy column(size_type column_pos_){ return const_column_proxy(Data.at(column_pos_)); }
		column_iterator column_begin(){ return column_iterator(Data.at(column_no_).second.begin()); }
		column_iterator column_end(){ return column_iterator(Data.at(column_no_).second.end()); }
		const_column_iterator column_begin()const{ return const_column_iterator(Data.at(column_no_).second.begin()); }
		const_column_iterator column_end()const{ return const_column_iterator(Data.at(column_no_).second.end()); }
	public://access row
		row_iterator row_begin(size_type row_no_){ return row_iterator(data_begin(),row_no_); }
		row_iterator row_end(size_type row_no_){ return row_iterator(data_end(), row_no_); }
		const_row_iterator row_begin(size_type row_no_)const{ return const_row_iterator(data_begin(), row_no_); }
		const_row_iterator row_end(size_type row_no_)const{ return const_row_iterator(data_end(), row_no_); }
	public://access directoly
		element_proxy at(size_type row_pos_, size_type column_pos_){ return element_proxy(Data.at(column_pos_).second.at(row_pos_)); }
		const_element_proxy at(size_type row_pos_, size_type column_pos_)const{ return const_element_proxy(Data.at(column_pos_).second.at(row_pos_)); }
		element_proxy operator()(size_type row_pos_, size_type column_pos_){ return element_proxy(Data.at(column_pos_).second.at(row_pos_)); }
		const_element_proxy operator()(size_type row_pos_, size_type column_pos_)const{ return const_element_proxy(Data.at(column_pos_).second.at(row_pos_)); }
		template<typename type>
		casted_element_proxy<type> casted_at(size_type row_pos_, size_type column_pos_){ return casted_element_proxy<type>(Data.at(column_pos_).second.at(row_pos_)); }
		template<typename type>
		const_casted_element_proxy<type> casted_at(size_type row_pos_, size_type column_pos_)const{ return const_casted_element_proxy<type>(Data.at(column_pos_).second.at(row_pos_)); }
	};
	using table = basic_table<char, std::char_traits < char >> ;
}
#
#endif
