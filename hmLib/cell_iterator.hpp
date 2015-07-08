#include<iterator>
#include<iostream>
#include<sstream>
namespace hmLib{
	template<typename T, class CharT = char,class Traits = std::char_traits<CharT> >
	class ocell_iterator : public std::iterator<std::output_iterator_tag, void, void, void, void>{
	private:
		using ostream_type = std::basic_ostream < CharT, Traits >;
		using my_type = ocell_iterator < T, CharT, Traits >;
	public:
		struct iterator : public std::iterator<std::output_iterator_tag, void, void, void, void>{
			const T& operator*()&;
			const T* operator->()&;
			T&& operator*() && ;
			T* operator->() && ;
			my_type& operator++();
			my_type operator++(int){
				iterator ans(*this);
				operator++();
				return ans;
			}
		};
	private:
		struct cellset{
		private:
		public:
			iterator itr();
		};
	private:
		ostream_type* out_ptr;
		const CharT* del;
		std::stringstream stream;
	public:
		ocell_iterator(ostream_type& s, const CharT* delimiter=',') :out_ptr(&s), del(delimiter){}
		ocell_iterator(const my_type& x) = default;
	public:
		cellset operator*();
		cellset* operator->();
		my_type& operator++();
		my_type operator++(int){
			ocell_iterator ans(*this);
			operator++();
			return ans;
		}
	};
	void use(){
		ocell_iterator<std::string> ocell(std::cout);

		auto Line = *ocell;
		Line.itr();
		*(itr++) = "test";

	}

	template<typename T, class CharT = char, CharT sep, class Traits = std::char_traits<CharT> >
	class icell_iterator{
	private:
		using my_type = icell_iterator < T, sep >;
	private:
		std::istream* pin;
	public:
		icell_iterator() :pin(nullptr){}
		icell_iterator(std::istream& in_) :pin(&in_){}
		icell_iterator(const icell_iterator& x) = default;
		T operator*();
		T* operator->();
		my_type& operator++();
		my_type operator++(int){
			icell_iterator ans(*this);
			operator++();
			return ans;
		}
	};
	template<typename T>
	using icsv_iterator = icell_iterator <T, ',' >;
	template<typename T>
	using itsv_iterator = icell_iterator <T, '\t' >;
}