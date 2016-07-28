#ifndef HMLIB_TABLE_INC
#define HMLIB_TABLE_INC 100
#
#include<string>
#include<vector>
#include<algorithm>
#include<limits>
#include<boost/lexical_cast.hpp>
#include<boost/iterator_adaptors.hpp>
#include"exceptions.hpp"
namespace hmLib{
	namespace{
		struct basic_table_identifier{};
	}
	using table_exception=exceptions::exception_pattern<basic_table_identifier>;

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
			string_type& ref(){ return Str; }
			const string_type& cref()const{ return Str; }
			bool empty()const{ return cref().empty(); }
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
			template<typename type>
			type casted_get(){
				type Value;
				(*this) >> Value;
				return Value;
			}
			template<typename type>
			void casted_set(type Value_){
				(*this) << Value;
			}
		public:
			friend std::ostream& operator<<(std::ostream& out, const element_proxy& my){return out << my.cref();}
			friend std::istream& operator>>(std::istream& in, const element_proxy& my){ return in >> my.ref(); }
		};
		struct const_element_proxy{
		private:
			const string_type& Str;
		public:
			const_element_proxy(const string_type& Ref) :Str(Ref){}
			const_element_proxy(const element_proxy& Prx) :Str(Prx.Str){}
			operator const string_type&()const{ return Str; }
			const string_type& cref()const{ return Str; }
			bool empty()const{ return cref().empty(); }
			template<typename type>
			void operator>>(type& Val)const{
				Val = boost::lexical_cast<type>(Str);
			}
			template<typename type>
			type casted_get(){
				type Value;
				(*this) >> Value;
				return Value;
			}
		public:
			friend std::ostream& operator<<(std::ostream& out, const const_element_proxy& my){ return out << my.cref(); }
		};
		template<typename type>
		struct casted_element_proxy:private element_proxy{
		public:
			casted_element_proxy(string_type& Ref) :element_proxy(Ref){}
			operator type()const{
				type Val;
				this->operator>>(Val);
				return Val;
			}
			casted_element_proxy<type>& operator=(const type& Val){
				this->operator<<(Val);
				return *this;
			}
			type get()const{ return casted_get(); }
			void set(const type& value){ casted_set(value); }
			string_type& raw(){ return ref(); }
			const string_type& craw()const{ return cref(); }
			bool empty()const{ return cref().empty(); }
		public:
			friend std::ostream& operator<<(std::ostream& out, const casted_element_proxy<type>& my){ return out << my.get(); }
			friend std::istream& operator>>(std::istream& in, const casted_element_proxy<type>& my){ type value; in >> value; my.set(value); }
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
			type get()const{ return casted_get(); }
			const string_type& craw()const{ return cref(); }
			bool empty()const{ return cref().empty(); }
		public:
			friend std::ostream& operator<<(std::ostream& out, const const_casted_element_proxy<type>& my){ return out << my.get(); }
		};
	private://basic types for column/row iterators
		using basic_row_element_iterator = typename std::vector<data_type>::iterator;
		using basic_const_row_element_iterator = typename std::vector<data_type>::const_iterator;
		using basic_column_element_iterator = typename data_type::second_type::iterator;
		using basic_const_column_element_iterator = typename data_type::second_type::const_iterator;
	public://column proxy
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
			using iterator_adaptor = boost::iterator_adaptor<casted_column_element_iterator<type>, basic_column_element_iterator, casted_element_proxy<type>, boost::random_access_traversal_tag, casted_element_proxy<type> >;
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
			const_casted_column_element_iterator(casted_column_element_iterator<type> itr) : iterator_adaptor(itr.base_reference()){}		private:
		private:
			friend class boost::iterator_core_access;
			const_casted_element_proxy<type> dereference()const{
				return const_casted_element_proxy<type>(*(base_reference()));
			}
		};
		struct column_proxy{
			friend class my_type;
		private:
			data_type& rData;
		public:
			column_proxy(data_type& rData_) :rData(rData_){}
			string_type name()const{ return rData.first; }
			void rename(string_type Name_){ rData.first = Name_; }
			const column_type& cref()const{ return rData.second; }
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
			const column_type& cref()const{ return rData.second; }
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
			bool equal(row_element_iterator itr_)const{
				return (column_pos == itr_.column_pos) && (base_reference() == itr_.base_reference());
			}
			element_proxy dereference()const{
				return element_proxy(base_reference()->second[column_pos]);
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
			bool equal(const_row_element_iterator itr_)const{
				return (column_pos == itr_.column_pos) && (base_reference() == itr_.base_reference());
			}
			const_element_proxy dereference()const{
				return const_element_proxy(base_reference()->second[column_pos]);
			}
		};
		struct row_proxy{
			friend struct const_row_proxy;
		private:
			my_type& My;
			size_type row_pos;
		public:
			row_proxy(my_type& My_, size_type row_pos_) :My(My_), row_pos(row_pos_){}
			size_type size()const{ return My.column_size(); }
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
			my_type* Ptr;
			size_type RowPos;
		public:
			row_iterator():Ptr(nullptr),RowPos(std:numeric_limits<size_type>::max()){}
			row_iterator(my_type& Ref_,size_type row_pos_) : Ptr(&Ref_), RowPos(row_pos_){}
		public:
			size_type row_pos()const{ return RowPos; }
		private:
			friend class boost::iterator_core_access;
			row_proxy dereference()const{ return row_proxy(*Ptr, RowPos); }
			bool equal(row_iterator itr_)const{ return (RowPos == itr_.RowPos) && (Ptr == itr_.Ptr); }
			void increment(){ ++RowPos; }
			void decrement(){ --RowPos; }
			void advance(diff_type n){ RowPos += n; }
			diff_type distance_to(row_iterator itr_)const{ return itr_.RowPos - row_itr.RowPos; }
		};
		struct const_row_iterator
			: public boost::iterator_facade<const_row_iterator, const_row_proxy, boost::random_access_traversal_tag, const_row_proxy, diff_type >{
			using iterator_facade = boost::iterator_facade<const_row_iterator, const_row_proxy, boost::random_access_traversal_tag, const_row_proxy, diff_type >;
		private:
			const my_type* Ptr;
			size_type RowPos;
		public:
			const_row_iterator() :Ptr(nullptr), RowPos(std : numeric_limits<size_type>::max()){}
			const_row_iterator(const my_type& Ref_, size_type row_pos_) : Ptr(&Ref_), RowPos(row_pos_){}
			const_row_iterator(row_iterator itr) : Ptr(itr.Ptr), RowPos(itr.RowPos){}
		public:
			size_type row_pos()const{ return RowPos; }
		private:
			friend class boost::iterator_core_access;
			const_row_proxy dereference()const{ return const_row_proxy(*Ptr, RowPos); }
			bool equal(const_row_iterator itr_)const{ return (RowPos == itr_.RowPos) && (Ptr == itr_.Ptr); }
			void increment(){ ++RowPos; }
			void decrement(){ --RowPos; }
			void advance(diff_type n){ RowPos += n; }
			diff_type distance_to(const_row_iterator itr_)const{ return itr_.RowPos - row_itr.RowPos; }
		};
		struct column_iterator
			: public boost::iterator_adaptor<column_iterator, basic_row_element_iterator, column_proxy, boost::random_access_traversal_tag, column_proxy>{
			using iterator_adaptor = boost::iterator_adaptor<column_iterator, basic_row_element_iterator, column_proxy, boost::random_access_traversal_tag, column_proxy>;
			friend struct const_column_iterator;
		public:
			column_iterator() = default;
			column_iterator(basic_row_element_iterator  bitr) : iterator_adaptor(bitr){}
		private:
			friend class boost::iterator_core_access;
			bool equal(column_iterator itr_)const{
				return (base_reference() == itr_.base_reference());
			}
			column_proxy dereference()const{
				return column_proxy(*(base_reference()));
			}
		};
		struct const_column_iterator
			: public boost::iterator_adaptor<const_column_iterator, basic_const_row_element_iterator, const_column_proxy, boost::random_access_traversal_tag, const_column_proxy>{
			using iterator_adaptor = boost::iterator_adaptor<const_column_iterator, basic_const_row_element_iterator, const_column_proxy, boost::random_access_traversal_tag, const_column_proxy>;
		public:
			const_column_iterator() = default;
			const_column_iterator(basic_const_row_element_iterator bitr) : iterator_adaptor(bitr){}
			const_column_iterator(column_iterator itr) : iterator_adaptor(itr.base_reference()){}
		private:
			friend class boost::iterator_core_access;
			bool equal(const_column_iterator itr_)const{
				return (base_reference() == itr_.base_reference());
			}
			const_column_proxy dereference()const{
				return const_column_proxy(*(base_reference()));
			}
		};
		struct name_iterator
			: public boost::iterator_adaptor<name_iterator, basic_row_element_iterator, string_type, boost::random_access_traversal_tag>{
			using iterator_adaptor = boost::iterator_adaptor<name_iterator, basic_row_element_iterator, string_type, boost::random_access_traversal_tag>;
			friend struct const_name_iterator;
		public:
			name_iterator() = default;
			name_iterator(basic_row_element_iterator  bitr) : iterator_adaptor(bitr){}
		private:
			friend class boost::iterator_core_access;
			bool equal(name_iterator itr_)const{
				return (base_reference() == itr_.base_reference());
			}
			string_type& dereference()const{
				return base_reference()->first;
			}
		};
		struct const_name_iterator
			: public boost::iterator_adaptor<const_name_iterator, basic_const_row_element_iterator, const string_type, boost::random_access_traversal_tag>{
			using iterator_adaptor = boost::iterator_adaptor<const_name_iterator, basic_const_row_element_iterator, const string_type, boost::random_access_traversal_tag>;
		public:
			const_name_iterator() = default;
			const_name_iterator(basic_const_row_element_iterator bitr) : iterator_adaptor(bitr){}
			const_name_iterator(name_iterator itr) : iterator_adaptor(itr.base_reference()){}
		private:
			friend class boost::iterator_core_access;
			bool equal(const_name_iterator itr_)const{
				return (base_reference() == itr_.base_reference());
			}
			const string_type& dereference()const{
				return base_reference()->first;
			}
		};
	public:
		basic_table() = default;
		basic_table(const my_type& my_) = default;
		my_type& operator=(const my_type& my_) = default;
		basic_table(my_type&& my_) = default;
		my_type& operator=(my_type&& my_) = default;
		basic_table(size_type row_size_, size_type col_size_) :basic_table(){
			assign(row_size_, col_size_);
		}
		explicit basic_table(size_pair_type size_) :basic_table(){
			assign(size_.first, size_.second);
		}
		void swap(my_type& my_){Data.swap(my_.Data);}
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
		column_iterator column_begin(){ return column_iterator(Data.begin()); }
		column_iterator column_end(){ return column_iterator(Data.end()); }
		const_column_proxy column(size_type column_pos_)const{ return const_column_proxy(Data.at(column_pos_)); }
		const_column_iterator column_begin()const{ return const_column_iterator(Data.begin()); }
		const_column_iterator column_end()const{ return const_column_iterator(Data.end()); }
		column_iterator find_column(const string_type& column_name_){
			return std::find_if(column_begin(), column_end(), [](const column_proxy& col)->bool{return col.name() == column_name; });
		}
		const_column_iterator find_column(const string_type& column_name_)const{
			return std::find_if(column_begin(), column_end(), [](const const_column_proxy& col)->bool{return col.name() == column_name; });
		}
	public://access row
		size_type row_size()const{ return Data.at(0).second.size(); }
		row_proxy row(size_type row_pos_){ return row_proxy(*this, row_pos_); }
		row_iterator row_begin(){ return row_iterator(*this,0); }
		row_iterator row_end(){ return row_iterator(*this,row_size()); }
		const_row_proxy row(size_type row_pos_)const{ return row_proxy(*this, row_pos_); }
		const_row_iterator row_begin()const{ return const_row_iterator(*this, 0);}
		const_row_iterator row_end()const{ return const_row_iterator(*this, row_size()); }
	public://access column_name
		string_type& column_name(size_type column_pos_){return Data.at(column_pos_).first;}
		name_iterator column_name_begin(){ return name_iterator(Data.begin()); }
		name_iterator column_name_end(){ return name_iterator(Data.end()); }
		const string_type& column_name(size_type column_pos_)const{ return Data.at(column_pos_).first; }
		const_name_iterator column_name_begin()const{ return const_name_iterator(Data.begin()); }
		const_name_iterator column_name_end()const{ return const_name_iterator(Data.end()); }
	public://change elements
		size_pair_type size(){
			if(Data.empty())return size_pair_type(0, 0);
			return size_pair_type(Data.size(), Data.at(0).second.size());
		}
		void assign(size_type row_size_, size_type col_size_){
			Data.assign(col_size_, data_type());
			for(auto& Column : Data)Column.second.assign(row_size_,string_type());
		}
		void reserve(size_type row_size_, size_type col_size_){
			Data.reserve(col_size_);
			for(auto& Column : Data)Column.second.reserve(row_size_);
		}
		void clear(){ Data.clear(); }
	public://manage column
		void push_back_column(string_type column_name_, column_type column_){
			//set same rownum with other columns
			if(row_size() > column_.size()){
				column_.insert(column_.end(), row_size() - column_.size(), "");
			} else if(row_size() < column_.size()){
				column_.erase(column_.begin() + row_size(), column_.end());
			}

			//push_back new column
			Data.push_back(std::make_pair(column_name_, column_));
		}
		void push_back_column(string_type column_name_ = ""){
			push_back_column(column_name_, column_type(row_size(),""));
		}
		void pop_back_column(){ Data.pop_back(); }
		column_iterator insert_column(column_iterator itr, size_type n, string_type column_name_, column_type column_){
			if(n == 0)return itr;

			hmLib_assert(itr->size() == column_.size(), table_exception, "column with different size is requested to insert.");

			//Insert new column
			return column_iterator(Data.insert(Data.begin() + (itr-column_begin()), n, std::make_pair(column_name_, std::move(column_))));
		}
		column_iterator insert_column(column_iterator itr, string_type column_name_ = ""){
			return insert_column(itr, 1, std::move(column_name_), column_type(row_size(), ""));
		}
		column_iterator insert_column(column_iterator itr, column_iterator first, column_iterator last){
			for(; first != last; ++first){
				itr = insert_column(itr, first->cref(), first->name());
				++itr;
			}
		}
		column_iterator erase_column(column_iterator itr){
			return column_iterator(Data.erase(Data.begin() + (itr - column_begin())));
		}
		column_iterator erase_column(column_iterator first, column_iterator last){
			return column_iterator(Data.erase(Data.begin() + (first - column_begin()), Data.begin() + (last - column_begin())));
		}
		void swap_column(column_iterator itr1, column_iterator itr2){
			hmLib_assert(itr1->size() == itr2->size(), table_exception, "columns with different size cannot be swaped.");

			std::swap(itr1->rData, itr2->rData);
		}
	public://manage row
		void push_back_row(){
			for(auto& Column : Data){
				Column.second.push_back("");
			}
		}
		void pop_back_row(){
			for(auto& Column : Data){
				Column.second.pop_back();
			}
		}
		row_iterator insert_row(row_iterator itr, size_type n = 1){
			if(n == 0)return itr;

			for(auto& Column : Data){
				Column.second.insert(Column.second.begin()+itr.row_pos(), n, "");
			}
			return itr;
		}
		row_iterator insert_row(row_iterator itr, row_iterator first, row_iterator last){
			hmLib_assert(itr->size() == first.size(), table_exception, "row with different size is requested to insert.");

			itr = insert_row(itr, last-first);

			for(; first != last; ++first){
				for(size_type pos = 0; pos < row_size(); ++pos){
					itr->at(pos) = first->at(pos);
				}
			}

			return itr;
		}
		row_iterator erase_row(row_iterator itr){
			for(auto& Column : Data){
				Column.second.erase(Column.second.begin() + itr.row_pos());
			}
			return itr;
		}
		row_iterator erase_row(row_iterator first, row_iterator last){
			for(auto& Column : Data){
				Column.second.erase(Column.second.begin() + first.row_pos(), Column.second.begin() + last.row_pos());
			}
			return itr;
		}
		void swap_row(row_iterator itr1, row_iterator itr2){
			hmLib_assert(itr1->size() == itr2->size(), table_exception, "rows with different size cannot be swaped.");

			for(auto& Column : Data){
				std::swap(Column.second.at(itr1.row_pos()), Column.second.at(itr2.row_pos()));
			}
		}
	};
	using table = basic_table<char, std::char_traits < char >> ;
}
#
#endif
