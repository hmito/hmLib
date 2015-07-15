#include<iterator>
#include<iostream>
#include<sstream>
namespace hmLib{
	template<class Elem = char, class Traits = std::char_traits<Elem> >
	class ocsv_iterator : public std::iterator < std::output_iterator_tag, void, void, void, void > {
	private:
		using my_type = ocsv_iterator < Elem, Traits > ;
		using my_ostream = std::basic_ostream < Elem, Traits > ;
		using my_string = std::basic_string < Elem, Traits > ;
	private:
		struct output_proxy{
		private:
			my_type& My;
		public:
			output_proxy(my_type& My_) :My(My_){}
			void operator=(const my_string& Str){
				if(My.IsCellHead && !My.IsLineHead){
					*(My.pstream) << My.Sep;
					My.IsCellHead = false;
				}

				*(My.pstream) << Str;
			}
			template<typename T>
			output_proxy& operator<<(T Val){
				if(My.IsCellHead && !My.IsLineHead){
					*(My.pstream) << My.Sep;
					My.IsCellHead = false;
				}

				*(My.pstream) << Str;
				return *this;
			}
		};
	private:
		my_ostream* pstream;
		Elem Sep;
		Elem End;
		bool IsCellHead;
		bool IsLineHead;
	public:
		ocsv_iterator()
			: pstream(nullptr){
		}
		ocsv_iterator(my_ostream& Stream_, Elem Sep_ = ',', Elem End_ = '\n')
			: pstream(&Stream_)
			, Sep(Sep_)
			, End(End_)
			, IsCellHead(false)
			, IsLineHead(true){
		}
		ocsv_iterator(const my_type& x) = default;
	public:
		operator bool()const{ return pstream; }
		output_proxy operator*(){ return output_proxy(*this); }
		my_type& operator++(){
			IsCellHead = true;
			IsLineHead = false;
			return *this;
		}
		my_type operator++(int){
			my_type ans(*this);
			operator++();
			return ans;
		}
		bool eol()const{ return IsLineHead; }
		void endl(){
			(*pstream) << End;
			IsLineHead = true;
		}
	};
	template<class Elem = char, class Traits = std::char_traits<Elem> >
	class icsv_iterator : public std::iterator < std::input_iterator_tag, void, void, void, void > {
	private:
		using my_type = icsv_iterator < Elem, Traits > ;
		using my_istream = std::basic_istream < Elem, Traits > ;
		using my_pos = my_istream::pos_type;
		using my_string = std::basic_string < Elem, Traits > ;
		using my_sstream = std::basic_stringstream < Elem, Traits > ;
	private:
		my_istream* pstream;
		Elem Sep;
		Elem End;
		my_pos Pos;
		my_sstream sstream;
		bool HasRead;
		my_pos ReadPos;
		bool IsLineHead;
	private:
		void next(){
			if(HasRead){
				Pos = ReadPos;
			} else{
				Elem c;
				if(pstream->tellg() != Pos) pstream->seekg(Pos);

				while(true){
					c = pstream->get();
					++Pos;

					if(c == Sep){
						IsLineHead = false;
						break;
					}
					if(c == End || c == 0){
						IsLineHead = true;
						break;
					}
				}
			}
		}
		void read(){
			Elem c;
			sstream.str(my_string());
			sstream.clear();

			if(pstream->tellg() != Pos) pstream->seekg(Pos);

			ReadPos = Pos;

			while(true){
				c = pstream->get();
				++ReadPos;

				if(c == Sep){
					IsLineHead = false;
					break;
				}
				if(c == End || c == 0){
					IsLineHead = true;
					break;
				}

				sstream << c;
			}

			HasRead = true;
		}
	private:
		struct input_proxy{
		private:
			my_type& My;
		public:
			input_proxy(my_type& My_) :My(My_){}
			friend void operator=(my_string& Str, input_proxy& Proxy_){
				Str = Proxy_.My.sstream.str();
			}
			template<typename T>
			input_proxy& operator>>(T& Val){
				Proxy_.My.sstream >> Val;
				return *this;
			}
		};
	public:
		icsv_iterator() :pstream(nullptr){}
		icsv_iterator(my_istream& Stream_, Elem Sep_ = ',', Elem End_ = '\n')
			: pstream(&in_)
			, Sep(Sep_)
			, End(End_)
			, Pos(in.tellg()){
		}
		icsv_iterator(const my_type& My_) = default;
		my_type& operator=(const my_type& My_) = default;
		input_proxy operator*(){
			if(!HasRead)read();
			return input_proxy(*this);
		}
		my_type& operator++(){
			if(!HasRead)skip();
			else HasRead = false;
			return *this;
		}
		my_type operator++(int){
			icsv_iterator ans(*this);
			operator++();
			return ans;
		}
		bool eol()const{ return IsLineHead; }
		friend bool operator==(const my_type& my1, const my_type& my2){
			if(&my1 == &my2)return true;

			if(my1.pstream != my2.pstream) return false;

			return my1.Pos == my2.Pos;
		}
		friend bool operator!=(const my_type& my1, const my_type& my2){ return !(my1 == my2); }
	public:
		static my_type line_end(const my_type& My_){
			my_type ans = My_;

			advance_line(ans, 1);

			return ans;
		}
		static my_type file_end(const my_type& My_){
			my_type ans = My_;
			ans.pstream->seekg(0, std::ios_base::end);
			ans.Pos = ans.pstream->tellg();

			return ans;
		}
	};
	template<class Elem = char, class Traits = std::char_traits<Elem> >
	inline icsv_iterator<Elem, Traits> line_end(const icsv_iterator<Elem, Traits>& itr){
		return icsv_iterator<Elem, Traits>::line_end(itr);
	}
	template<class Elem = char, class Traits = std::char_traits<Elem> >
	inline icsv_iterator<Elem, Traits> file_end(const icsv_iterator<Elem, Traits>& itr){
		return icsv_iterator<Elem, Traits>::file_end(itr);
	}
	template<class Elem = char, class Traits = std::char_traits<Elem> >
	inline void advance_line(icsv_iterator<Elem, Traits>& itr, unsigned int num = 1){
		unsigned int no = 0;
		do{
			++itr;
			if(itr.eol())++no;
		} while(no < num);
	}
}
