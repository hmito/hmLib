#ifndef HMLIB_CSV_CSVLINE_INC
#define HMLIB_CSV_CSVLINE_INC 100
#
#include<vector>
#include<iostream>
#include<sstream>
#include<string>
#include"csvbase.hpp"
#include"../clone_ptrproxy.hpp"
namespace hmLib {
	template<typename elem_type_, typename trait_type_ = std::char_traits<elem_type_> >
	class basic_csvline{
	private:
		using this_type = basic_csvline<elem_type_, trait_type_>;
		using elem_type = elem_type_;
		using trait_type = trait_type_;
		using string_type = std::basic_string < elem_type, trait_type >;
		using ostream_type = std::basic_ostream < elem_type, trait_type >;
		using istream_type = std::basic_istream < elem_type, trait_type >;
		using stringstream_type = std::basic_stringstream<elem_type, trait_type>;
		using csv_mode_type = basic_csv_mode<elem_type, trait_type>;
		using container = std::vector<string_type>;
	public:
		struct const_proxy {
		private:
			const string_type& Str;
		public:
			const_proxy() = delete;
			explicit const_proxy(const string_type& Str_):Str(Str_) {}
		public:
			operator string_type()const { return Str; }
			const string_type& str()const { return Str; }
			template<typename T>
			const_proxy& operator>>(T& Val)const {
				stringstream_type sst(Str);
				sst >> Val;
				return *this;
			}
			template<typename T>
			void read(T& Val)const {
				stringstream_type sst(Str);
				sst >> Val;
			}
			friend ostream_type& operator<<(ostream_type& out, const const_proxy& p) {
				return out << p.str();
			}
		};
		struct proxy {
		private:
			string_type& Str;
		public:
			proxy() = delete;
			explicit proxy(string_type& Str_):Str(Str_) {}
		public:
			operator const_proxy()const { return const_proxy(Str); }
			operator string_type()const { return Str; }
			string_type& str() { return Str; }
			const string_type& str()const { return Str; }
			template<typename T>
			proxy& operator>>(T& Val)const {
				stringstream_type sst(Str);
				sst >> Val;
				return *this;
			}
			template<typename T>
			void read(T& Val)const {
				stringstream_type sst(Str);
				sst >> Val;
			}
			template<typename T>
			proxy& operator<<(T& Val) {
				stringstream_type sst;
				sst << Val;
				Str = sst.str();
				return *this;
			}
			template<typename T>
			void write(const T& Val) {
				stringstream_type sst;
				sst << Val;
				Str = sst.str();
			}
			friend ostream_type& operator<<(ostream_type& out, const proxy& p) {
				return out << p.str();
			}
			friend istream_type& operator>>(istream_type& out, proxy& p) {
				p.Str.clear();
				out >> p.Str;
			}
		};
		struct const_iterator {
		private:
			using base_iterator = container::const_iterator;
		public:
			using value_type = string_type;
			using reference = const_proxy;
			using pointer = clone_ptrproxy<reference>;
			using difference_type = typename std::iterator_traits<base_iterator>::difference_type;
			using iterator_category = std::random_access_iterator_tag;
		private:
			base_iterator bitr;
		public:
			const_iterator() = default;
			const_iterator(base_iterator bitr_)noexcept:bitr(bitr_) {}
			base_iterator base() { return bitr; }
			reference operator*() { return reference(*bitr); }
			pointer operator->() { return pointer(operator*()); }
			reference operator[](difference_type n) { return reference(bitr[n]); }
			const_iterator& operator++() { ++bitr; return *this; }
			const_iterator operator++(int) {
				const_iterator prev = *this;
				operator++();
				return prev;
			}
			const_iterator& operator--() { --bitr; return *this; }
			const_iterator operator--(int) {
				const_iterator prev = *this;
				operator--();
				return prev;
			}
			const_iterator& operator+=(difference_type n) { bitr += n; return *this; }
			const_iterator& operator-=(difference_type n) { bitr -= n; return *this; }
			friend const_iterator operator+(const_iterator itr, difference_type n) {
				return itr += n;
			}
			friend const_iterator operator+(difference_type n, const_iterator itr) {
				return itr += n;
			}
			friend const_iterator operator-(const_iterator itr, difference_type n) {
				return itr -= n;
			}
			friend difference_type operator-(const_iterator itr1, const_iterator itr2) {
				return itr1.bitr-itr2.bitr;
			}
			friend bool operator==(const_iterator itr1, const_iterator itr2) { return itr1.bitr == itr2.bitr; }
			friend bool operator!=(const_iterator itr1, const_iterator itr2) { return itr1.bitr != itr2.bitr; }
			friend bool operator<=(const_iterator itr1, const_iterator itr2) { return itr1.bitr <= itr2.bitr; }
			friend bool operator>=(const_iterator itr1, const_iterator itr2) { return itr1.bitr >= itr2.bitr; }
			friend bool operator<(const_iterator itr1, const_iterator itr2) { return itr1.bitr < itr2.bitr; }
			friend bool operator>(const_iterator itr1, const_iterator itr2) { return itr1.bitr > itr2.bitr; }
		};
		struct iterator {
		private:
			using base_iterator = container::iterator;
		public:
			using value_type = string_type;
			using reference = proxy;
			using pointer = clone_ptrproxy<reference>;
			using difference_type = typename std::iterator_traits<base_iterator>::difference_type;
			using iterator_category = std::random_access_iterator_tag;
		private:
			base_iterator bitr;
		public:
			iterator() = default;
			iterator(base_iterator bitr_)noexcept:bitr(bitr_) {}
			base_iterator base() { return bitr; }
			operator const_iterator()const { return const_iterator(bitr); }
			reference operator*() { return reference(*bitr); }
			pointer operator->() { return pointer(operator*()); }
			reference operator[](difference_type n) { return reference(bitr[n]); }
			iterator& operator++() { ++bitr; return *this; }
			iterator operator++(int) {
				const_iterator prev = *this;
				operator++();
				return prev;
			}
			iterator& operator--() { --bitr; return *this; }
			iterator operator--(int) {
				const_iterator prev = *this;
				operator--();
				return prev;
			}
			iterator& operator+=(difference_type n) { bitr += n; return *this; }
			iterator& operator-=(difference_type n) { bitr -= n; return *this; }
			friend iterator operator+(iterator itr, difference_type n) {return itr += n;}
			friend iterator operator+(difference_type n, iterator itr) {return itr += n;}
			friend iterator operator-(iterator itr, difference_type n) {return itr -= n;}
			friend difference_type operator-(iterator itr1, iterator itr2) {return itr1.bitr-itr2.bitr;}
			friend bool operator==(iterator itr1, iterator itr2) { return itr1.bitr == itr2.bitr; }
			friend bool operator!=(iterator itr1, iterator itr2) { return itr1.bitr != itr2.bitr; }
			friend bool operator<=(iterator itr1, iterator itr2) { return itr1.bitr <= itr2.bitr; }
			friend bool operator>=(iterator itr1, iterator itr2) { return itr1.bitr >= itr2.bitr; }
			friend bool operator<(iterator itr1, iterator itr2) { return itr1.bitr < itr2.bitr; }
			friend bool operator>(iterator itr1, iterator itr2) { return itr1.bitr > itr2.bitr; }
		};
	private:
		std::vector<string_type> Data;
		csv_mode_type Mode;
	public:
		proxy operator[](std::size_t index) {return proxy(Data[index]);}
		const_proxy operator[](std::size_t index) const { return const_proxy(Data[index]); }
		proxy at(std::size_t index) { return proxy(Data.at(index)); }
		const_proxy at(std::size_t index) const{return const_proxy(Data.at(index));}
		proxy front() { return proxy(Data.front()); }
		const_proxy front()const { return const_proxy(Data.front()); }
		proxy back() { return proxy(Data.back()); }
		const_proxy back()const { return const_proxy(Data.back()); }
		void push_back(const string_type& str) {
			Data.push_back(str);
		}
		void push_back(string_type&& str) {
			Data.push_back(std::move(str));
		}
		void pop_back() { return Data.pop_back(); }
		iterator insert(const_iterator itr, const string_type& str){Data.insert(itr, str);}
		template<typename input_iterator>
		iterator insert(const_iterator itr, input_iterator Beg, input_iterator End){Data.insert(itr, Beg,End);}
		iterator erase(const_iterator itr) {
			return iterator(Data.erase(itr.base()));
		}
		iterator erase(const_iterator from, const_iterator to) {
			return iterator(Data.erase(from.base(), to.base()));
		}
		template<typename T>
		void emplace_back(const T& val) {
			stringstream_type sst;
			sst<<val;
			Data.emplace_back(sst.str());
		}
		template<typename T>
		iterator emplace(const_iterator itr,const T& val) {
			stringstream_type sst;
			sst<<val;
			return iterator(Data.emplace(itr,sst.str()));
		}
		std::size_t size()const { return Data.size();}
		bool empty()const { return Data.empty(); }
		void clear() { return Data.clear(); }
		void assign(std::size_t n) { Data.assign(n, ""); }
		template<typename input_iterator>
		void assign(input_iterator Beg, input_iterator End) { Data.assign(Beg, End); }
		iterator begin() { return iterator(Data.begin()); }
		iterator end() { return iterator(Data.end()); }
		const_iterator begin()const { return cbegin(); }
		const_iterator end()const { return cend(); }
		const_iterator cbegin()const { return const_iterator(Data.begin()); }
		const_iterator cend()const { return const_iterator(Data.end()); }
		template<typename csv_traits>
		void read(istream_type& in, csv_traits Traits) {
			string_type Line;
			std::getline(in, Line, Traits.end());

			stringstream_type sin(Line);
			Data.clear();
			string_type Cell;
			while(std::getline(Line, Cell, Traits.sep())){
				Data.push_back(Cell);
			}
			if(!sin && Cell.empty()){
				Data.push_back("");
			}
		}
		template<typename csv_traits>
		void write(ostream_type& out, csv_traits Traits)const {
			if(!Data.empty()) {
				out<<Data[0];

				for(std::size_t i = 1; i<Data.size(); ++i) {
					out<<Traits.sep()<<Data[i];
				}
			}
			out<<Traits.end();
		}
	};
	using csvline = basic_csvline<char>;
}
#
#endif
