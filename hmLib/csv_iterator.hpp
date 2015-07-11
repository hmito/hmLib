#include<iterator>
#include<iostream>
#include<sstream>
namespace hmLib{
	template<class Elem = char, class Traits = std::char_traits<Elem> >
	class ocsv_iterator : public std::iterator<std::output_iterator_tag, void, void, void, void>{
	private:
		using my_type = icsv_iterator <Elem, Traits>;
		using my_ostream = std::basic_ostream< Elem, Traits >;
		using my_pos = my_ostream::pos_type;
		using my_string = std::basic_string < Elem, Traits >;
	private:
		my_ostream* pstream;
		my_pos Pos;
		Elem Sep;
		Elem End;
		bool IsLineHead;
	public:
		ocsv_iterator(my_ostream& s, const CharT* delimiter = ',') :out_ptr(&s), del(delimiter){}
		ocsv_iterator(const my_type& x) = default;
	public:
		my_string& operator*();
		my_string* operator->();
		my_type& operator++();
		my_type operator++(int){
			ocell_iterator ans(*this);
			operator++();
			return ans;
		}
		bool eol()const;
		void endl();
	};
	template<class Elem = char, class Traits = std::char_traits<Elem> >
	class icsv_iterator : public std::iterator<std::input_iterator_tag, void, void, void, void>{
	private:
		using my_type = icsv_iterator <Elem, Traits>;
		using my_istream = std::basic_istream< Elem, Traits > ;
		using my_pos = my_istream::pos_type;
		using my_string = std::basic_string < Elem, Traits > ;
	private:
		my_istream* pstream;
		my_pos Pos;
		Elem Sep;
		Elem End;
		my_string ReadStr;
		bool HasRead;
		bool IsLineHead;
	private:
		void read();
	public:
		icsv_iterator() :pstream(nullptr){}
		icsv_iterator(my_istream& in_, Elem Sep_ = ',', Elem End_ = '\n')
			: pstream(&in_)
			, Sep(Sep_)
			, End(End_){
		}
		icsv_iterator(const my_type& x) = default;
		const my_string& operator*(){
			read();
			return ReadStr;
		}
		const my_string* operator->(){
			read();
			return &ReadStr;
		}
		my_type& operator++();
		my_type operator++(int){
			icell_iterator ans(*this);
			operator++();
			return ans;
		}
		bool eol()const{ return IsLineHead; }
		template<typename T>
		T get()const{
			std::stringstream StrStream(operator*());
			T Ans;
			StrStream >> Ans;
			return Ans;
		}
		friend bool operator==(const my_type& my1, const my_type& my2){
			if(&my1 == &my2)return true;

			if(my1.pstream != my2.pstream) return false;
			
			return my1.Pos == my2.Pos;
		}
		friend bool operator!=(const my_type& my1, const my_type& my2){return !(my1 == my2);}
	};
	template<class Elem = char, class Traits = std::char_traits<Elem> >
	void advance_line(icsv_iterator<Elem,Traits>& itr, unsigned int num = 1);
}
