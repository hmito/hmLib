#ifndef HMLIB_CSV_CSVITERATOR_INC
#define HMLIB_CSV_CSVITERATOR_INC 100
#
#include<iterator>
#include<iostream>
#include<sstream>
namespace hmLib{
	namespace csv{
		template<typename csv_traits_>
		std::basic_string <typename csv_traits_::elem_type, typename csv_traits_::trait_type> cell_encode(std::basic_string <typename csv_traits_::elem_type, typename csv_traits_::trait_type> Str, csv_traits_ Traits){
			if(Str.find(Traits.sep()) < Str.size() || Str.find(Traits.esc()) < Str.size()){
				for(auto itr = Str.begin(); itr != Str.end(); ++itr){
					if(*itr == Traits.esc()){
						itr = Str.insert(++itr, Traits.esc());
					}
				}

				Str.insert(Str.begin(), Traits.esc());
				Str.push_back(Traits.esc());
			}

			return std::move(Str);
		}
		template<class elem_type = char, class Traits = std::char_traits<elem_type> >
		std::basic_string <typename csv_traits_::elem_type, typename csv_traits_::trait_type> cell_decode(std::basic_string <typename csv_traits_::elem_type, typename csv_traits_::trait_type> Str, csv_traits_ Traits){
			if(Str.size()>1 && Str.front()== Traits.esc() && Str.back()== Traits.esc()){
				Str.erase(Str.begin());
				Str.pop_back();

				bool EscFlag=false;
				for(auto itr = Str.begin(); itr != Str.end(); ++itr){
					if(*itr == Traits.esc()){
						if(EscFlag){
							itr = Str.erase(itr);
							--itr;
							EscFlag = false;
						} else{
							EscFlag = true;
						}
					} else{
						EscFlag = false;
					}
				}
			}

			return std::move(Str);
		}
	}
	template<typename csv_traits_>
	class ocsv_iterator {
	private:
		using this_type = icsv_iterator < csv_traits_ >;
	public:
		using csv_traits = csv_traits_;
		using elem_type = typename csv_traits::elem_type;
		using traits_type = typename csv_traits::traits_type;
		using sstream_type = std::basic_stringstream < elem_type, traits_type >;
		using ostream_type = std::basic_ostream < elem_type, traits_type >;
		using pos_type = typename ostream_type::pos_type;
		using string_type = std::basic_string < elem_type, traits_type >;
	public:
		using iterator_category = std::output_iterator_tag;
		using value_type = void;
		using reference = void;
		using pointer = void;
		using difference_type = int;
	private:
		struct output_proxy{
		private:
			this_type& My;
		private:
			void cell_head_treat(){
				if(My.IsCellHead && !My.IsLineHead){
					*(My.pstream) << My.Traits.sep();
				}
				My.IsCellHead = false;
			}
		public:
			output_proxy(this_type& My_) :My(My_){}
			void operator=(const string_type& Str){
				cell_head_treat();
				*(My.pstream) << csv::cell_encode(Str,My.Traits);
			}
			void raw_str(const string_type& Str){
				cell_head_treat();
				*(My.pstream) << Str;
			}
			template<typename T>
			output_proxy& operator<<(T Val){
				cell_head_treat();
				*(My.pstream) << Val;
				return *this;
			}
			template<typename T, typename U>
			output_proxy& write(U Val) {
				cell_head_treat();
				T TmpVal = Val;
				*(My.pstream) << TmpVal;
				return *this;
			}
			friend std::basic_istream<elem_type, Traits>& operator>>(std::basic_istream<elem_type, Traits>& in, output_proxy p){
				string_type Str;
				in >> Str;
				p = Str;
				return in;
			}
		};
	private:
		ostream_type* pstream;
		csv_traits Traits;
		bool IsCellHead;
		bool IsLineHead;
	public:
		basic_ocsv_iterator()
			: pstream(nullptr){
		}
		basic_ocsv_iterator(ostream_type& Stream_, csv_traits CSVMode_)
			: pstream(&Stream_)
			, Traits(CSVMode_)
			, IsCellHead(false)
			, IsLineHead(true){
		}
		basic_ocsv_iterator(const this_type& My_) = default;
		this_type& operator=(const this_type& My_) = default;
	public:
		operator bool()const{ return pstream; }
		output_proxy operator*(){ return output_proxy(*this); }
		this_type& operator++(){
			IsCellHead = true;
			IsLineHead = false;
			return *this;
		}
		this_type operator++(int){
			this_type ans(*this);
			operator++();
			return ans;
		}
		bool eol()const{ return IsLineHead; }
		void endl(){
			(*pstream) << Traits.end();
			IsLineHead = true;
			IsCellHead = false;
		}
	};

	template<typename csv_traits_>
	class icsv_iterator{
	private:
		using this_type = icsv_iterator < csv_traits_ > ;
	public:
		using csv_traits = csv_traits_;
		using elem_type = typename csv_traits::elem_type;
		using traits_type = typename csv_traits::traits_type;
		using sstream_type = std::basic_stringstream < elem_type, traits_type >;
		using istream_type = std::basic_istream < elem_type, traits_type > ;
		using pos_type = typename istream_type::pos_type;
		using string_type = std::basic_string < elem_type, traits_type > ;
	private:
		struct input_proxy {
		private:
			this_type& My;
		public:
			input_proxy(this_type& My_):My(My_) {}
			operator string_type() { return csv::cell_decode(My.read().str(), My.Traits); }
			string_type raw_str() { return My.read().str(); }
			template<typename T>
			input_proxy& operator>>(T& Val) {
				My.read() >> Val;
				return *this;
			}
			template<typename T, typename U>
			input_proxy& read(U& Val) {
				T TmpVal;
				My.read() >> TmpVal;
				Val = TmpVal;
				return *this;
			}
			friend std::basic_ostream<elem_type, Traits>& operator<<(std::basic_ostream<elem_type, Traits>& out, input_proxy p) {
				return out << static_cast<string_type>(p);
			}
		};
	public:
		using iterator_category = std::input_iterator_tag;
		using value_type = string_type;
		using reference = input_proxy;
		using pointer = void;
		using difference_type = int;
	private:
		istream_type* pstream;
		csv_traits Traits;
		pos_type Pos;
		sstream_type sstream;
		bool HasRead;
		pos_type ReadPos;
		bool IsLineHead;
	private:
		void next(){
			if(HasRead){
				HasRead = false;
				Pos = ReadPos;
			} else{
				elem_type c;
				pstream->seekg(0, std::ios::end);
				auto EndPos = pstream->tellg();
				pstream->seekg(Pos);

				bool EscFlag = false;
				while(pstream->tellg() != EndPos){
					c = pstream->get();

					if(c == Traits.end() || c == EOF){
						IsLineHead = true;
						break;
					}

					if(c == Traits.sep() && !EscFlag){
						IsLineHead = false;
						break;
					}

					if(c == Traits.esc()){
						EscFlag = !EscFlag;
					}
				}

				Pos = pstream->tellg();
			}			
		}
		sstream_type& read(){
			if(!HasRead){
				elem_type c;
				sstream.str(string_type());
				sstream.clear();

				pstream->seekg(0, std::ios::end);
				auto EndPos = pstream->tellg();
				pstream->seekg(Pos);

				bool EscFlag=false;
				while(pstream->tellg() != EndPos){
					c = pstream->get();

					if(c == Traits.end() || c == EOF){
						IsLineHead = true;
						break;
					}

					if(c == Traits.sep() && !EscFlag){
						IsLineHead = false;
						break;
					}

					if(c == Traits.esc()){
						EscFlag = !EscFlag;
					}
					
					sstream << c;
				}

				ReadPos = pstream->tellg();
				HasRead = true;
			}
			return sstream;
		}
	public:
		icsv_iterator() :pstream(nullptr){}
		icsv_iterator(istream_type& Stream_, pos_type Pos_, csv_traits CSVMode_,bool IsLineHead_)
			: pstream(&Stream_)
			, Traits(CSVMode_)
			, Pos(Pos_)
			, sstream()
			, HasRead(false)
			, IsLineHead(IsLineHead_){
		}
		icsv_iterator(const this_type& My_)
			: pstream(My_.pstream)
			, Traits(My_.Traits)
			, Pos(My_.Pos)
			, sstream(My_.sstream.str())
			, HasRead(My_.HasRead)
			, ReadPos(My_.ReadPos)
			, IsLineHead(My_.IsLineHead){
		}
		this_type& operator=(const this_type& My_){
			if(this != &My_){
				pstream = My_.pstream;
				Traits = My_.Traits;
				Pos = My_.Pos;
				sstream = My_.sstream.str();
				HasRead = My_.HasRead;
				ReadPos = My_.ReadPos;
				IsLineHead = My_.IsLineHead;
			}
		}
		input_proxy operator*(){return input_proxy(*this);}
		this_type& operator++(){
			next();
			return *this;
		}
		this_type operator++(int){
			basic_icsv_iterator ans(*this);
			operator++();
			return ans;
		}
		bool eol()const{ return IsLineHead; }
		bool eof(){
			pstream->seekg(0, std::ios::end);
			return Pos == pstream->tellg();
		}
		pos_type pos()const{ return Pos; }
		friend bool operator==(const this_type& my1, const this_type& my2){
			if(&my1 == &my2)return true;

			if(my1.pstream != my2.pstream) return false;

			return my1.Pos == my2.Pos;
		}
		friend bool operator!=(const this_type& my1, const this_type& my2){ return !(my1 == my2); }
	};

	template<typename csv_traits_>
	inline icsv_iterator<csv_traits_> icsv_begin(std::basic_istream <typename csv_traits_::elem_type, typename csv_traits_::trait_type>& Stream_, csv_traits_ Traits){
		Stream_.seekg(0, std::ios::beg);
		auto Pos = Stream_.tellg();
		return  icsv_iterator<csv_traits_>(Stream_, Pos,Traits, true);
	}
	template<typename csv_traits_>
	inline icsv_iterator<csv_traits_> icsv_current(std::basic_istream <typename csv_traits_::elem_type, typename csv_traits_::trait_type >& Stream_, csv_traits_ Traits){
		auto Pos = Stream_.tellg();
		return  icsv_iterator<csv_traits_>(Stream_, Pos, Traits, false);
	}
	template<typename csv_traits_>
	inline icsv_iterator<csv_traits_> icsv_end(std::basic_istream <typename csv_traits_::elem_type, typename csv_traits_::trait_type >& Stream_, csv_traits_ Traits){
		Stream_.seekg(0, std::ios::end);
		auto Pos = Stream_.tellg();
		return  icsv_iterator<csv_traits_>(Stream_, Pos, Traits, true);
	}

	template<typename csv_traits_>
	inline void advance_line(icsv_iterator<csv_traits_>& itr, unsigned int num = 1){
		unsigned int no = 0;
		do{
			++itr;
			if(itr.eol())++no;
		} while(no < num);
	}
	template<typename csv_traits_>
	inline icsv_iterator<csv_traits_> next_line(const icsv_iterator<csv_traits_>& itr, unsigned int num = 1){
		icsv_iterator<csv_traits_> next_itr(itr);
		advance_line(next_itr);
		return next_itr;
	}
}
#
#endif
