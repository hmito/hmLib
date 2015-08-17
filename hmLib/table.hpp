#ifndef HMLIB_TABLE_INC
#define HMLIB_TABLE_INC 100
#
#include<string>
#include<vector>
#include<algorithm>
#include<limits>
#include<boost/lexical_cast.hpp>
#include<boost/iterator_adaptors.hpp>
namespace hmLib{
	template<class Elem = char, class Traits = std::char_traits<Elem> >
	class basic_table{
	private:
		using my_type = basic_table < Elem, Traits >;
	public:
		using size_type = unsigned int;
		using diff_type = int;
		using size_pair_type = std::pair<size_type,size_type>;
		using string_type = std::basic_string < Elem, Traits >;
		using column_type = std::vector < string_type >;
		using data_type = std::pair < string_type, column_type >;
	private:
		std::vector<data_type> Data;
	public://element proxy
		struct element_proxy{
			friend struct const_element_proxy;
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
			const_element_proxy(const element_proxy& Prx) :Str(Prx.Str){}
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
			const_casted_element_proxy(const casted_element_proxy<type>& Prx) : const_element_proxy(element_proxy(Prx)){}
			operator type()const{
				type Val;
				this->operator>>(Val);
				return Val;
			}
		};
	public://column proxy
		using basic_column_element_iterator = typename data_type::second_type::iterator;
		using basic_const_column_element_iterator = typename data_type::second_type::const_iterator;
		struct column_element_iterator
			: public boost::iterator_adaptor<column_element_iterator, basic_column_element_iterator, element_proxy, boost::random_access_traversal_tag, element_proxy>{
			using iterator_adaptor = boost::iterator_adaptor<column_element_iterator, basic_column_element_iterator, element_proxy, boost::random_access_traversal_tag, element_proxy>;
		public:
			column_element_iterator() = default;
			column_element_iterator(basic_column_element_iterator  bitr) : iterator_adaptor(bitr){}
		private:
			friend class boost::iterator_core_access;
			element_proxy dereference()const{
				return element_proxy(*(base_reference()));
			}
		};
		struct const_column_element_iterator
			: public boost::iterator_adaptor<const_column_element_iterator, basic_const_column_element_iterator, const_element_proxy, boost::random_access_traversal_tag, const_element_proxy>{
			using iterator_adaptor = boost::iterator_adaptor<const_column_element_iterator, basic_const_column_element_iterator, const_element_proxy, boost::random_access_traversal_tag, const_element_proxy>;
		public:
			const_column_element_iterator() = default;
			const_column_element_iterator(basic_const_column_element_iterator  bitr) : iterator_adaptor(bitr){}
			const_column_element_iterator(column_element_iterator itr) : iterator_adaptor(itr.base_reference()){}
		private:
			friend class boost::iterator_core_access;
			const_element_proxy dereference()const{
				return const_element_proxy(*(base_reference()));
			}
		};
		template<typename type>
		struct casted_column_element_iterator
			: public boost::iterator_adaptor<casted_column_element_iterator<type>, basic_column_element_iterator, casted_element_proxy<type>, boost::random_access_traversal_tag, casted_element_proxy<type> >{
			using iterator_adaptor = boost::iterator_adaptor<casted_element_iterator<type>, basic_column_element_iterator, casted_element_proxy<type>, boost::random_access_traversal_tag, casted_element_proxy<type> >;
		public:
			casted_column_element_iterator() = default;
			casted_column_element_iterator(basic_column_element_iterator bitr) : iterator_adaptor(bitr){}
		private:
			friend class boost::iterator_core_access;
			casted_element_proxy<type> dereference()const{
				return casted_element_proxy<type>(*(base_reference()));
			}
		};
		template<typename type>
		struct const_casted_column_element_iterator
			: public boost::iterator_adaptor<const_casted_column_element_iterator<type>, basic_const_column_element_iterator, const_casted_element_proxy<type>, boost::random_access_traversal_tag, const_casted_element_proxy<type> >{
			using iterator_adaptor = boost::iterator_adaptor<const_casted_column_element_iterator<type>, basic_const_column_element_iterator, const_casted_element_proxy<type>, boost::random_access_traversal_tag, const_casted_element_proxy<type> >;
		public:
			const_casted_column_element_iterator() = default;
			const_casted_column_element_iterator(basic_const_column_element_iterator bitr) : iterator_adaptor(bitr){}
			const_casted_column_element_iterator(casted_column_element_iterator itr) : iterator_adaptor(itr.base_reference()){}
		private:
			friend class boost::iterator_core_access;
			const_casted_element_proxy<type> dereference()const{
				return const_casted_element_proxy<type>(*(base_reference()));
			}
		};
		struct column_proxy{
		private:
			data_type& rData;
		public:
			column_proxy(data_type& rData_) :rData(rData_){}
			string_type name()const{ return rData.first; }
			void rename(string_type Name_){ rData.first = Name_; }
			size_type size()const{ return rData.second.size(); }
			element_proxy operator[](size_type Pos){ return element_proxy(rData.second.at(Pos)); }
			element_proxy at(size_type Pos){ return element_proxy(rData.second.at(Pos)); }
			template<typename type>
			casted_element_proxy<type> casted_at(size_type Pos){ return casted_element_proxy<type>(rData.second.at(Pos)); }
			const_element_proxy operator[](size_type Pos)const{ return const_element_proxy(rData.second.at(Pos)); }
			const_element_proxy at(size_type Pos)const{ return const_element_proxy(rData.second.at(Pos)); }
			template<typename type>
			const_casted_element_proxy<type> casted_at(size_type Pos)const{ return const_casted_element_proxy<type>(rData.second.at(Pos)); }
			column_element_iterator begin(){ return column_element_iterator(rData.second.begin()); }
			column_element_iterator end(){ return column_element_iterator(rData.second.end()); }
			const_column_element_iterator begin()const{ return const_column_element_iterator(rData.second.begin()); }
			const_column_element_iterator end()const{ return const_column_element_iterator(rData.second.end()); }
			template<typename type>
			casted_column_element_iterator<type> casted_begin(){ return casted_column_element_iterator<type>(rData.second.begin()); }
			template<typename type>
			casted_column_element_iterator<type> casted_end(){ return casted_column_element_iterator<type>(rData.second.end()); }
			template<typename type>
			const_casted_column_element_iterator<type> casted_begin()const{ return const_casted_column_element_iterator<type>(rData.second.begin()); }
			template<typename type>
			const_casted_column_element_iterator<type> casted_end()const{ return const_casted_column_element_iterator<type>(rData.second.end()); }
		};
		struct const_column_proxy{
		private:
			const data_type& rData;
		public:
			const_column_proxy(const data_type& rData_) :rData(rData_){}
			string_type name()const{ return rData.first; }
			size_type size()const{ return rData.second.size(); }
			const_element_proxy operator[](size_type Pos)const{ return const_element_proxy(rData.second.at(Pos)); }
			const_element_proxy at(size_type Pos)const{ return const_element_proxy(rData.second.at(Pos)); }
			template<typename type>
			const_casted_element_proxy<type> casted_at(size_type Pos)const{ return const_casted_element_proxy<type>(rData.second.at(Pos)); }
			const_column_element_iterator begin()const{ return const_column_element_iterator(rData.second.begin()); }
			const_column_element_iterator end()const{ return const_column_element_iterator(rData.second.end()); }
			template<typename type>
			const_casted_column_element_iterator<type> casted_begin()const{ return const_casted_column_element_iterator<type>(rData.second.begin()); }
			template<typename type>
			const_casted_column_element_iterator<type> casted_end()const{ return const_casted_column_element_iterator<type>(rData.second.end()); }
		};
	public://row proxy
		using basic_row_element_iterator = typename std::vector<data_type>::iterator;
		using basic_const_row_element_iterator = typename std::vector<data_type>::const_iterator;
		struct row_element_iterator
			: public boost::iterator_adaptor<row_element_iterator, basic_row_element_iterator, element_proxy, boost::random_access_traversal_tag, element_proxy>{
			using iterator_adaptor = boost::iterator_adaptor<row_element_iterator, basic_row_element_iterator, element_proxy, boost::random_access_traversal_tag, element_proxy>;
			friend struct const_row_element_iterator;
		private:
			size_type column_pos;
		public:
			row_element_iterator() = default;
			row_element_iterator(basic_row_element_iterator  bitr, size_type column_pos_) : iterator_adaptor(bitr), column_pos(column_pos_){}
		private:
			friend class boost::iterator_core_access;
			bool equal(row_element_iterator itr_){
				return (column_pos == itr_.column_pos) && (base_reference() == itr_.base_reference());
			}
			element_proxy dereference()const{
				return element_proxy(base_reference()[column_pos]);
			}
		};
		struct const_row_element_iterator
			: public boost::iterator_adaptor<const_row_element_iterator, basic_const_row_element_iterator, const_element_proxy, boost::random_access_traversal_tag, const_element_proxy>{
			using iterator_adaptor = boost::iterator_adaptor<const_row_element_iterator, basic_const_row_element_iterator, const_element_proxy, boost::random_access_traversal_tag, const_element_proxy>;
		private:
			size_type column_pos;
		public:
			const_row_element_iterator() = default;
			const_row_element_iterator(basic_const_row_element_iterator  bitr, size_type column_pos_) : iterator_adaptor(bitr), column_pos(column_pos_){}
			const_row_element_iterator(row_element_iterator itr) : iterator_adaptor(itr.base_reference()), column_pos(itr.column_pos){}
		private:
			friend class boost::iterator_core_access;
			bool equal(row_element_iterator itr_){
				return (column_pos == itr_.column_pos) && (base_reference() == itr_.base_reference());
			}
			const_element_proxy dereference()const{
				return const_element_proxy(base_reference()[column_pos]);
			}
		};
		struct row_proxy{
			friend struct const_row_proxy;
		private:
			my_type& My;
			size_type row_pos;
		public:
			row_proxy(my_type& My_, size_type row_pos_) :My(My_), row_pos(row_pos_){}
			size_type size()const{return My.column_size()}
			size_type pos()const{ return row_pos; }
			element_proxy operator[](size_type Pos){ return My(row_pos, Pos); }
			element_proxy at(size_type Pos){ return My.at(row_pos, Pos); }
			const_element_proxy operator[](size_type Pos)const{ return My(row_pos, Pos); }
			const_element_proxy at(size_type Pos)const{ return My.at(row_pos, Pos); }
			row_element_iterator begin(){ return row_element_iterator(My.Data.begin(), row_pos); }
			row_element_iterator end(){return row_element_iterator(My.Data.end(), row_pos); }
			const_row_element_iterator begin()const{ return const_row_element_iterator(My.Data.begin(), row_pos); }
			const_row_element_iterator end()const{ return const_row_element_iterator(My.Data.end(), row_pos); }
		};
		struct const_row_proxy{
		private:
			const my_type& My;
			size_type row_pos;
		public:
			const_row_proxy(const my_type& My_, size_type row_pos_) :My(My_), row_pos(row_pos_){}
			const_row_proxy(row_proxy Prx) :My(Prx.My), row_pos(Prx.row_pos){}
			size_type size()const{ return My.column_size() }
			size_type pos()const{ return row_pos; }
			const_element_proxy operator[](size_type Pos)const{ return My(row_pos, Pos); }
			const_element_proxy at(size_type Pos)const{ return My.at(row_pos, Pos); }
			const_row_element_iterator begin()const{ return const_row_element_iterator(My.Data.begin(), row_pos); }
			const_row_element_iterator end()const{ return const_row_element_iterator(My.Data.end(), row_pos); }
		};
	public:
		struct row_iterator
			: public boost::iterator_facade<row_iterator, row_proxy, boost::random_access_traversal_tag, row_proxy, diff_type >{
			using iterator_facade = boost::iterator_facade<row_iterator, row_proxy, boost::random_access_traversal_tag, row_proxy, diff_type >;
			friend struct const_row_iterator;
		private:
			my_type& Ref;
			size_type row_pos;
		public:
			row_iterator():row_pos(std:numeric_limits<size_type>::max()){}
			row_iterator(my_type& Ref_,size_type row_pos_) : Ref(Ref_), row_pos(row_pos_){}
			row_iterator(const_row_iterator itr) : Ref(itr.Ref), row_pos(itr.row_pos){}
		private:
			friend class boost::iterator_core_access;
			row_proxy deference(){ return row_proxy(Ref, row_pos); }
			bool equal(row_iterator itr_){ return (row_pos == itr_.row_pos) && (&Ref == &(itr_.Ref)); }
			void increment(){ ++row_pos; }
			void decrement(){ --row_pos; }
			void advance(diff_type n){ row_pos += n; }
			diff_type distance_to(row_iterator itr_){ return itr_.row_pos - row_itr; }
		};
		struct const_row_iterator
			: public boost::iterator_facade<const_row_iterator, const_row_proxy, boost::random_access_traversal_tag, const_row_proxy, diff_type >{
			using iterator_facade = boost::iterator_facade<const_row_iterator, const_row_proxy, boost::random_access_traversal_tag, const_row_proxy, diff_type >;
		private:
			const my_type& Ref;
			size_type row_pos;
		public:
			const_row_iterator() :row_pos(std : numeric_limits<size_type>::max()){}
			const_row_iterator(const my_type& Ref_, size_type row_pos_) : Ref(Ref_), row_pos(row_pos_){}
		private:
			friend class boost::iterator_core_access;
			const_row_proxy deference(){ return row_proxy(Ref, row_pos); }
			bool equal(row_iterator itr_){ return (row_pos == itr_.row_pos) && (&Ref == &(itr_.Ref)); }
			void increment(){ ++row_pos; }
			void decrement(){ --row_pos; }
			void advance(diff_type n){ row_pos += n; }
			diff_type distance_to(row_iterator itr_){ return itr_.row_pos - row_itr; }
		};
		struct column_iterator
			: public boost::iterator_adaptor<column_iterator, basic_row_element_iterator, column_proxy, boost::random_access_traversal_tag, column_proxy>{
			using iterator_adaptor = boost::iterator_adaptor<column_iterator, basic_row_element_iterator, column_proxy, boost::random_access_traversal_tag, column_proxy>;
			friend struct const_column_iterator;
		public:
			column_iterator() = default;
			column_iterator(basic_row_element_iterator  bitr) : iterator_adaptor(bitr){}
			column_iterator(const_column_iterator itr) : iterator_adaptor(itr.base_reference()){}
		private:
			friend class boost::iterator_core_access;
			bool equal(column_iterator itr_){
				return (base_reference() == itr_.base_reference());
			}
			column_proxy dereference()const{
				return column_proxy(base_reference()->second);
			}
		};
		struct const_column_iterator
			: public boost::iterator_adaptor<const_column_iterator, basic_row_element_iterator, const_column_proxy, boost::random_access_traversal_tag, const_column_proxy>{
			using iterator_adaptor = boost::iterator_adaptor<const_column_iterator, basic_row_element_iterator, const_column_proxy, boost::random_access_traversal_tag, const_column_proxy>;
		public:
			const_column_iterator() = default;
			const_column_iterator(basic_row_element_iterator  bitr) : iterator_adaptor(bitr){}
		private:
			friend class boost::iterator_core_access;
			bool equal(const_column_iterator itr_){
				return (base_reference() == itr_.base_reference());
			}
			const_column_proxy dereference()const{
				return const_column_proxy(base_reference()->second);
			}
		};
	public://access directoly
		element_proxy at(size_type row_pos_, size_type column_pos_){ return element_proxy(Data.at(column_pos_).second.at(row_pos_)); }
		const_element_proxy at(size_type row_pos_, size_type column_pos_)const{ return const_element_proxy(Data.at(column_pos_).second.at(row_pos_)); }
		element_proxy operator()(size_type row_pos_, size_type column_pos_){ return element_proxy(Data.at(column_pos_).second.at(row_pos_)); }
		const_element_proxy operator()(size_type row_pos_, size_type column_pos_)const{ return const_element_proxy(Data.at(column_pos_).second.at(row_pos_)); }
		template<typename type>
		casted_element_proxy<type> casted_at(size_type row_pos_, size_type column_pos_){ return casted_element_proxy<type>(Data.at(column_pos_).second.at(row_pos_)); }
		template<typename type>
		const_casted_element_proxy<type> casted_at(size_type row_pos_, size_type column_pos_)const{ return const_casted_element_proxy<type>(Data.at(column_pos_).second.at(row_pos_)); }
		element_proxy at(size_pair_type pos_){ return at(pos_.first,pos_.second); }
		const_element_proxy at(size_pair_type pos_)const{ return at(pos_.first,pos_.second); }
		element_proxy operator[](size_pair_type pos_){ return operator()(pos_.first,pos_.second);}
		const_element_proxy operator[](size_pair_type pos_)const{ return operator()(pos_.first,pos_.second);}
		template<typename type>
		casted_element_proxy<type> casted_at(size_pair_type pos_){return casted_at(pos_.first,pos_.second);}
		template<typename type>
		const_casted_element_proxy<type> casted_at(size_pair_type pos_)const{return casted_at(pos_.first,pos_.second);}
	public://access column
		size_type column_size()const{ return Data.size(); }
		column_proxy column(size_type column_pos_){ return column_proxy(Data.at(column_pos_)); }
		const_column_proxy column(size_type column_pos_){ return const_column_proxy(Data.at(column_pos_)); }
		column_iterator column_begin(){ return column_iterator(Data.at(column_no_).second.begin()); }
		column_iterator column_end(){ return column_iterator(Data.at(column_no_).second.end()); }
		const_column_iterator column_begin()const{ return const_column_iterator(Data.at(column_no_).second.begin()); }
		const_column_iterator column_end()const{ return const_column_iterator(Data.at(column_no_).second.end()); }
	public://access row
		size_type row_size()const{ return Data.at(0).second.size(); }
		row_iterator row_begin(size_type row_pos_){ return row_iterator(Data.at(0).second.begin(),row_pos_); }
		row_iterator row_end(size_type row_pos_){ return row_iterator(Data.at(0).second.end(), row_pos_); }
		const_row_iterator row_begin(size_type row_pos_)const{ return const_row_iterator(data_begin(), row_pos_); }
		const_row_iterator row_end(size_type row_pos_)const{ return const_row_iterator(data_end(), row_pos_); }
	public://change elements
		size_pair_type size(){
			if(Data.empty())return size_pair_type(0, 0);
			return size_pair_type(Data.size(), Data.at(0).second.size());
		}
		void assign(size_type row_size_, size_type col_size_){
			Data.assign(col_size_);
			for(auto& Column : Data)Column.second.assign(row_size_);
		}
		void reserve(size_type row_size_, size_type col_size_){
			Data.reserve(col_size_);
			for(auto& Column : Data)Column.second.reserve(row_size_);
		}
		void clear(){ Data.clear(); }
	/*
		column_iterator find_column(const string_type& column_name_){ return std::find_if(data_begin(), data_end(), [](const data_type& d)->bool{return d.first == column_name; }); }
		void push_back_column(column_type column_ = column_type(), string_type column_name_ = ""){ Data.push_back(std::make_pair(column_name_, column_); }
		void pop_back_column(){ Data.pop_back(); }
		void insert_column(column_iterator itr, column_type column_ = column_type(), string_type column_name_ = "");
		void erase_column(column_iterator itr);
	*/
	};
	using table = basic_table<char, std::char_traits < char >> ;
}
#
#endif
