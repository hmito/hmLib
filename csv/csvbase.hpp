#ifndef HMLIB_CSV_CSVBASE_INC
#define HMLIB_CSV_CSVBASE_INC 100
#
#include<string>
#include<string_view>
#include<type_traits>
#include<iostream>
#include"../exceptions.hpp"
#include"../utility.hpp"
#include"../iterators/range.hpp"
namespace hmLib {
	//exceptions
	namespace csv {
		namespace detail {
			struct iocsv_identifier {};
			struct invalid_field_identifer {};
		}
		using exception = hmLib::exceptions::exception_pattern<csv::detail::iocsv_identifier>;
		using invalid_field = hmLib::exceptions::exception_pattern<csv::detail::invalid_field_identifer, exception>;
	}

	//csv trait definition
	namespace csv {
		template<typename elem_type_, typename trait_type_ = std::char_traits<elem_type_>>
		struct basic_csv_traits {
			using elem_type = elem_type_;
			using trait_type = trait_type_;
			using string_type = std::basic_string<elem_type, trait_type>;
			using istream_type = std::basic_istream<elem_type, trait_type>;
			using ostream_type = std::basic_ostream<elem_type, trait_type>;
			using iostream_type = std::basic_iostream<elem_type, trait_type>;
			constexpr elem_type sep()const { return ','; }
			constexpr elem_type end()const { return '\n'; }
			constexpr elem_type esc()const { return '"'; }
		};
		template<typename elem_type_, typename trait_type_ = std::char_traits<elem_type_>>
		struct basic_tsv_traits {
			using elem_type = elem_type_;
			using trait_type = trait_type_;
			using string_type = std::basic_string<elem_type, trait_type>;
			using istream_type = std::basic_istream<elem_type, trait_type>;
			using ostream_type = std::basic_ostream<elem_type, trait_type>;
			using iostream_type = std::basic_iostream<elem_type, trait_type>;
			constexpr elem_type sep()const{ return '\t'; }
			constexpr elem_type end()const { return '\n'; }
			constexpr elem_type esc()const { return '"'; }
		};
		template<typename elem_type_, typename trait_type_ = std::char_traits<elem_type_>>
		struct basic_ssv_traits {
			using elem_type = elem_type_;
			using trait_type = trait_type_;
			using string_type = std::basic_string<elem_type, trait_type>;
			using istream_type = std::basic_istream<elem_type, trait_type>;
			using ostream_type = std::basic_ostream<elem_type, trait_type>;
			using iostream_type = std::basic_iostream<elem_type, trait_type>;
			constexpr elem_type sep()const { return ' '; }
			constexpr elem_type end()const { return '\n'; }
			constexpr elem_type esc()const { return '"'; }
		};
		using csv_traits = basic_csv_traits<char>;
		using tsv_traits = basic_tsv_traits<char>;
		using ssv_traits = basic_ssv_traits<char>;

		template<class Elem = char, class Traits = std::char_traits<Elem> >
		struct dynamic_csv_traits {
			using elem_type = Elem;
			using trait_type = Traits;
			using string_type = std::basic_string<elem_type, trait_type>;
			using istream_type = std::basic_istream<elem_type, trait_type>;
			using ostream_type = std::basic_ostream<elem_type, trait_type>;
			using iostream_type = std::basic_iostream<elem_type, trait_type>;
		public:
			Elem Sep;
			Elem End;
			Elem Esc;
		public:
			dynamic_csv_traits() = default;
			constexpr dynamic_csv_traits(Elem Sep_, Elem End_, Elem Esc_):Sep(Sep_), End(End_), Esc(Esc_) {}
			constexpr elem_type sep()const { return Sep; }
			constexpr elem_type end()const { return End; }
			constexpr elem_type esc()const { return Esc; }
		};
		template<typename csv_traits_>
		dynamic_csv_traits<typename csv_traits_::elem_type, typename csv_traits_::trait_type> make_csv_traits() {
			return any_csv_traits(csv_traits_::sep(), csv_traits_::end(), csv_traits_::esc());
		}
	}

	//encode & decode
	namespace csv {
		enum class encode_mode { none, simple, full};
		struct encode_state {
			encode_mode Mode;
			bool NeedEncode;
			encode_state()noexcept :Mode(encode_mode::none), NeedEncode(false) {}
			encode_state(encode_mode Mode_, bool NeedEncode_)noexcept :Mode(Mode_), NeedEncode(NeedEncode_) {}
			bool invalid()const { return Mode == encode_mode::none && NeedEncode; }
		};
		template<typename forward_iterator, typename csv_traits>
		encode_state cell_state(forward_iterator Beg, forward_iterator End, csv_traits Traits, bool NotEncoded = false){			
			if (!NotEncoded && *(Beg) == Traits.esc()) {
				//escape mode: control character can be used with esc.
				++Beg;
				bool IsEsc = false;
				bool FindEsc = false;
				bool FindCtr = false;
				for (; Beg != End; ++Beg) {
					if (IsEsc) {
						if (*Beg == Traits.esc()) {
							IsEsc = false;
							FindEsc = true;
							FindCtr = true;
						}else{
							//ERROR: single escape found inside of the field
							return encode_state{ encode_mode::none, true};
						}
					} else {
						if (*Beg == Traits.esc()) {
							IsEsc = true;
						} else if (*Beg == Traits.sep() || *Beg == Traits.end()) {
							FindCtr = true;
						}
					}
				}
				if (!IsEsc) {
					//ERROR: closing escape is not found
					return encode_state{ encode_mode::none, true };
				}
				//finish without error
				return encode_state{ (FindEsc? encode_mode::full: encode_mode::simple), FindCtr };
			} else {
				//non-escape mode: control character cannot be used.
				for (; Beg != End; ++Beg) {
					if (*Beg == Traits.end() || *Beg == Traits.sep() || *Beg == Traits.esc())return encode_state{ encode_mode::none, true };
				}
				return encode_state{ encode_mode::none, false };
			}
		}
		template<typename forward_iterator, typename csv_traits, typename std::enable_if<std::is_convertible<typename std::iterator_traits<forward_iterator>::iterator_category,std::forward_iterator_tag>::value>::type*& = hmLib::utility::enabler>
		std::pair<forward_iterator, encode_state> cell_next(forward_iterator Beg, forward_iterator End, csv_traits Traits, bool NotEncoded = false) {
			if (!NotEncoded && *(Beg) == Traits.esc()) {
				//escape mode: control character can be used with esc.
				++Beg;
				bool IsEsc = false;
				bool FindEsc = false;
				bool FindCtr = false;
				for (; Beg != End; ++Beg) {
					if (IsEsc) {
						if (*Beg == Traits.esc()) {
							IsEsc = false;
							FindEsc = true;
							FindCtr = true;
						} else if (*Beg == Traits.sep() || *Beg == Traits.end()) {
							//Find End of Cell
							return std::make_pair(Beg, encode_state{ (FindEsc ? encode_mode::full : encode_mode::simple), FindCtr });
						} else {
							//ERROR: single escape found inside of the field
							return std::make_pair(Beg, encode_state{ encode_mode::none, true });
						}
					} else {
						if (*Beg == Traits.esc()) {
							IsEsc = true;
						} else if (*Beg == Traits.sep() || *Beg == Traits.end()) {
							FindCtr = true;
						}
					}
				}
				if (!IsEsc) {
					//ERROR: closing escape is not found
					return std::make_pair(Beg, encode_state{ encode_mode::none, true });
				}
				//finish without error
				return std::make_pair(Beg, encode_state{ (FindEsc ? encode_mode::full : encode_mode::simple), FindCtr });
			} else {
				//non-escape mode: control character cannot be used.
				for (; Beg != End; ++Beg) {
					if (*Beg == Traits.esc())return std::make_pair(Beg, encode_state{ encode_mode::none, true });
					if (*Beg == Traits.end() || *Beg == Traits.sep())break;
				}
				return std::make_pair(Beg, encode_state{ encode_mode::none, false });
			}
		}
		template<typename input_iterator, typename output_iterator, typename csv_traits>
		std::tuple<input_iterator, output_iterator, encode_state> cell_advance(input_iterator Beg, input_iterator End, output_iterator Out, csv_traits Traits, bool NotEncoded = false) {
			if (!NotEncoded && *(Beg) == Traits.esc()) {
				//escape mode: control character can be used with esc.
				++Beg;
				bool IsEsc = false;
				bool FindEsc = false;
				bool FindCtr = false;
				for (; Beg != End; ++Beg) {
					if (IsEsc) {
						if (*Beg == Traits.esc()) {
							IsEsc = false;
							FindEsc = true;
							FindCtr = true;
							*(Out++) = *Beg;
						} else if (*Beg == Traits.sep() || *Beg == Traits.end()) {
							//Find End of Cell
							return std::make_tuple(Beg, Out, encode_state{ (FindEsc ? encode_mode::full : encode_mode::simple), FindCtr });
						} else {
							//ERROR: single escape found inside of the field
							return std::make_tuple(Beg, Out, encode_state{ encode_mode::none, true });
						}
					} else {
						if (*Beg == Traits.esc()) {
							IsEsc = true;
						} else {
							if (*Beg == Traits.sep() || *Beg == Traits.end()) {
								FindCtr = true;
							}
							*(Out++) = *Beg;
						}
					}
				}
				if (!IsEsc) {
					//ERROR: closing escape is not found
					return std::make_tuple(Beg, Out, encode_state{ encode_mode::none, true });
				}
				//finish without error
				return std::make_tuple(Beg, Out, encode_state{ (FindEsc ? encode_mode::full : encode_mode::simple), FindCtr });
			} else {
				//non-escape mode: control character cannot be used.
				for (; Beg != End; ++Beg) {
					if (*Beg == Traits.esc())return std::make_tuple(Beg, Out, encode_state{ encode_mode::none, true });
					if (*Beg == Traits.end() || *Beg == Traits.sep())break;
					*(Out++) = *Beg;
				}
				return std::make_tuple(Beg, Out, encode_state{ encode_mode::none, false });
			}
		}
		template<typename input_iterator, typename output_iterator, typename csv_traits>
		output_iterator cell_decode(input_iterator Beg, input_iterator End, output_iterator Out, csv_traits Traits) {
			if (*Beg == Traits.esc()) {
				//escape mode
				++Beg;
				bool IsEsc = false;
				for (; Beg != End; ++Beg) {
					if (IsEsc) {
						if (*Beg == Traits.esc()) {
							IsEsc = false;
							*(Out++) = Traits.esc();
						} else {
							//ERROR: single escape found inside of the field
							hmLib_throw(csv::invalid_field, "single escape found inside of the field.");
						}
					} else {
						if (*Beg == Traits.esc()) {
							IsEsc = true;
						} else {
							*(Out++) = *Beg;
						}
					}
				}
				if (!IsEsc) {
					//ERROR: closing escape is not found
					hmLib_throw(csv::invalid_field, "the filed is not closed by escape character.");
				}
			} else {
				//non-escape mode
				for (; Beg != End; ++Beg) {
					*(Out++) = *Beg;
				}
			}
			return Out;
		}
		template<typename input_iterator, typename output_iterator, typename csv_traits>
		output_iterator cell_decode(input_iterator Beg, input_iterator End, output_iterator Out, csv_traits Traits, encode_state Hint) {
			if (Hint.Mode == encode_mode::full) {
				//escape (full) mode
				//	full decoding is required
				++Beg;
				bool IsEsc = false;
				for (; Beg != End; ++Beg) {
					if (IsEsc) {
						//next character of IsEsc should be esc
						*(Out++) = Traits.esc();
						IsEsc = false;
					} else if (*Beg == Traits.esc()) {
						IsEsc = true;
					} else {
						*(Out++) = *Beg;
					}
				}
			} else if (Hint.Mode == encode_mode::simple) {
				//escape (simple) mode
				//	decode is compleded by removing the edges
				++Beg;
				auto Elem = *Beg;
				++Beg;
				for (; Beg != End; ++Beg) {
					*(Out++) = Elem;
					Elem =  *Beg;
				}
			} else {
				//non-escape mode
				for (; Beg != End; ++Beg) {
					*(Out++) = *Beg;
				}
			}
			return Out;
		}
		template<typename forward_iterator, typename csv_traits>
		hmLib::range<forward_iterator> cell_decode(forward_iterator Beg, forward_iterator End, csv_traits Traits) {
			if (*Beg == Traits.esc()) {
				auto Out = Beg;
				auto Itr = Beg;
				//escape mode
				++Itr;
				bool IsEsc = false;
				for (; Itr != End; ++Itr) {
					if (IsEsc) {
						if (*Itr == Traits.esc()) {
							IsEsc = false;
							*(Out++) = Traits.esc();
						} else {
							//ERROR: single escape found inside of the field
							hmLib_throw(csv::invalid_field, "single escape found inside of the field.");
						}
					} else {
						if (*Itr == Traits.esc()) {
							IsEsc = true;
						} else {
							*(Out++) = *Itr;
						}
					}
				}
				if (!IsEsc) {
					//ERROR: closing escape is not found
					hmLib_throw(csv::invalid_field, "the filed is not closed by escape character.");
				}
				return hmLib::make_range(Beg, Out);
			} else {
				//non-escape mode
				return hmLib::make_range(Beg, End);
			}
		}
		template<typename forward_iterator, typename csv_traits>
		hmLib::range<forward_iterator> cell_decode(forward_iterator Beg, forward_iterator End, csv_traits Traits, encode_state Hint) {
			if (Hint.Mode == encode_mode::full) {
				//escape (full) mode
				//	full decoding is required
				auto Itr = Beg;
				auto Out = Beg;
				++Itr;
				bool IsEsc = false;
				for (; Itr != End; ++Itr) {
					if (IsEsc) {
						//next character of IsEsc should be esc
						*(Out++) = Traits.esc();
						IsEsc = false;
					} else if (*Itr == Traits.esc()) {
						IsEsc = true;
					} else {
						*(Out++) = *Itr;
					}
				}
				return hmLib::make_range(Beg, Out);
			} else if (Hint.Mode == encode_mode::simple) {
				//escape (simple) mode
				//	decode is compleded by removing the edges
				return hmLib::make_range(std::next(Beg), std::next(Beg, std::distance(Beg, End) - 1));
			} else {
				return hmLib::make_range(Beg, End);
			}
		}
		template<typename input_iterator, typename output_iterator, typename csv_traits>
		output_iterator cell_encode(input_iterator Beg, input_iterator End, output_iterator Out, csv_traits Traits) {
			*(Out++) = Traits.esc();
			for (; Beg != End; ++Beg) {
				if (*Beg == Traits.esc()) {
					*(Out++) = Traits.esc();
					*(Out++) = Traits.esc();
				} else {
					*(Out++) = *Beg;
				}
			}
			*(Out++) = Traits.esc();
			return Out;
		}
	}
}
#
#endif
