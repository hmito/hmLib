#ifndef HMLIB_CSV_CSVBASE_INC
#define HMLIB_CSV_CSVBASE_INC 100
#
#include<string>
#include<iostream>
#include"../exceptions.hpp"
namespace hmLib {
	namespace csv {
		namespace detail {
			struct iocsv_identifier {};
		}
	}
	using iocsv_exception = hmLib::exceptions::exception_pattern<csv::detail::iocsv_identifier>;

	namespace csv {
		template<typename elem_type_, typename trait_type_ = std::char_traits<elem_type_>>
		struct basic_csv_traits {
			using elem_type = elem_type_;
			using trait_type = trait_type_;
			static constexpr elem_type sep(){ return ','; }
			static constexpr elem_type end() { return '\n'; }
			static constexpr elem_type esc() { return '"'; }
		};
		template<typename elem_type_, typename trait_type_ = std::char_traits<elem_type_>>
		struct basic_tsv_traits {
			using elem_type = elem_type_;
			using trait_type = trait_type_;
			static constexpr elem_type sep() { return '\t'; }
			static constexpr elem_type end() { return '\n'; }
			static constexpr elem_type esc() { return '"'; }
		};
		template<typename elem_type_, typename trait_type_ = std::char_traits<elem_type_>>
		struct basic_ssv_traits {
			using elem_type = elem_type_;
			using trait_type = trait_type_;
			static constexpr elem_type sep() { return ' '; }
			static constexpr elem_type end() { return '\n'; }
			static constexpr elem_type esc() { return '"'; }
		};
		using csv_traits = basic_csv_traits<char>;
		using tsv_traits = basic_tsv_traits<char>;
		using ssv_traits = basic_ssv_traits<char>;

		template<class Elem = char, class Traits = std::char_traits<Elem> >
		struct any_csv_traits {
			using elem_type = Elem;
			using trait_type = Traits;
		public:
			Elem Sep;
			Elem End;
			Elem Esc;
		public:
			any_csv_traits() = default;
			any_csv_traits(Elem Sep_, Elem End_, Elem Esc_):Sep(Sep_), End(End_), Esc(Esc_) {}
			elem_type sep()const { return Sep; }
			elem_type end()const { return End; }
			elem_type esc()const { return Esc; }
		};
		template<typename csv_traits_>
		any_csv_traits<typename csv_traits_::elem_type, typename csv_traits_::trait_type> make_csv_traits() {
			return any_csv_traits(csv_traits_::sep(), csv_traits_::end(), csv_traits_::esc());
		}

		template<typename csv_traits_>
		void write_cell(std::basic_ostream<typename csv_traits_::elem_type, typename csv_traits_::trait_type>& fout, const std::basic_string < typename csv_traits_::elem_type, typename csv_traits_::trait_type>& Str, csv_traits_ Traits, bool IsEndOfLine, bool DoEndOfLine) {
			if(!IsEndOfLine) {
				fout<<Traits.sep();
			}

			bool NeedEscape = false;

			for(auto itr = Str.begin(); itr!=Str.end(); ++itr) {
				if(*Str == Traits.sep() || *Str == Traits.end()) {
					NeedEscape = true;
					break;
				}
			}

			if(NeedEscape) {
				fout << Traits.esc();
				for(auto itr = Str.begin(); itr != Str.end(); ++itr) {
					if(*itr == Traits.esc()) {
						fout << Traits.esc();
					}
					fout<<*itr;
				}
				fout << Traits.esc();
			} else {
				fout << Str;
			}

			if(DoEndOfLine) {
				fout<<Traits.end();
			}
		}
		template<typename csv_traits_>
		bool read_cell(std::basic_istream<typename csv_traits_::elem_type, typename csv_traits_::trait_type>& fin, std::basic_string <typename csv_traits_::elem_type, typename csv_traits_::trait_type>& Str, csv_traits_ Traits) {
			int c;
			typename csv_traits_::elem_type Elem;

			//first character
			c = fin.get();
			Elem = static_cast<csv_traits_::elem_type>(c);
			if(c == EOF || Elem ==  Traits.end() ||fin.eof()) {
				return true;
			} else if(Elem == Traits.sep() && !EscFlag) {
				return false;
			}

			if(Elem == Traits.esc()) {
				//escaped field
				bool EscFlag = false;

				while(!fin.eof()) {
					c = fin.get();
					Elem = static_cast<csv_traits_::elem_type>(c);

					if(c == EOF || Elem == Traits.end()) {
						break;
					} else if(Elem == Traits.esc()) {
						if(EscFlag) {
							EscFlag = false;
							//double escape
							Str.push_back(Elem);
						} else {
							EscFlag = true;
						}
					} else {
						//closed with escape
						if(EscFlag) {
							if(c == EOF || Elem == Traits.end() || Elem == Traits.sep())break;

							EscFlag = false;
							Str.push_back(Traits.sep());
							Str.push_back(Elem);
						} else {
							Str.push_back(Elem);
						}
					}
				}
			} else {
				//none escaped field
				Str.push_back(Elem);
				while(!fin.eof()) {
					fin.get(Elem);

					if(c == EOF || Elem == csv_traits_::end() || Elem == csv_traits_::sep())break;

					Str.push_back(Elem);
				}
			}

			return (c == EOF || Elem == csv_traits_::end() || fin.eof());
		}
	}
}
#
#endif
