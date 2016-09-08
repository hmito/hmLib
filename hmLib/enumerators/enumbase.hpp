#ifndef HMLIB_ENUMERATORS_ENUMBASE_INC
#define HMLIB_ENUMERATORS_ENUMBASE_INC 100
#
#include<iterator>
namespace hmLib{
	//iterator_tag;
	namespace enumerators{
		struct cur_t{}constexpr cur;
		struct end_t{}constexpr end;
		struct beg_t{}constexpr beg;
		struct iniend_t{}constexpr iniend;
		struct inibeg_t{}constexpr inibeg;
	}
	//enumerator_base tag
	struct referable_enumerator_tag{};
	struct sentinel_enumerator_tag :public referable_enumerator_tag{};
	struct range_enumerator_tag :public sentinel_enumerator_tag{};
	struct mutable_range_enumerator_tag :public range_enumerator_tag{};

	//enumbase
	namespace enumerators{
		//enumbase_interface
		template<typename enumerator_traits_>
		struct referable_enumbase_interface{
			virtual void advance(enumerators::cur_t, typename enumerator_traits_::difference_type Dif) = 0;
		};
		template<typename enumerator_traits_>
		struct sentinel_enumbase_interface: public referable_enumbase_interface<enumerator_traits_>{
			virtual operator bool()const = 0;
			virtual void advance(enumerators::end_t, typename enumerator_traits_::difference_type Dif) = 0;
			virtual typename enumerator_traits_::difference_type distance(enumerators::cur_t, enumerators::end_t)const = 0;
			typename enumerator_traits_::difference_type distance(enumerators::end_t, enumerators::cur_t)const{ return -std::distance(cur, end); }
		};
		template<typename enumerator_traits_>
		struct range_enumbase_interface : public sentinel_enumbase_interface<enumerator_traits_>{
			virtual void reset_cur() = 0;
			virtual void reset()= 0;
			virtual void advance(enumerators::beg_t, typename enumerator_traits_::difference_type Dif) = 0;
			virtual typename enumerator_traits_::difference_type distance(enumerators::beg_t, enumerators::cur_t)const = 0;
			virtual typename enumerator_traits_::difference_type size()const = 0;
			typename enumerator_traits_::difference_type distance(enumerators::cur_t, enumerators::beg_t)const{ return -std::distance(cur, beg); }
			typename enumerator_traits_::difference_type distance(enumerators::beg_t, enumerators::end_t)const{ return size(); }
			typename enumerator_traits_::difference_type distance(enumerators::end_t, enumerators::beg_t)const{ return -size(); }
		};
		template<typename enumerator_traits_>
		struct mutable_range_enumbase_interface : public range_enumbase_interface<enumerator_traits_>{
			virtual void reset_range() = 0;
			virtual typename enumerator_traits_::difference_type distance(enumerators::inibeg_t, enumerators::beg_t)const = 0;
			virtual typename enumerator_traits_::difference_type distance(enumerators::inibeg_t, enumerators::cur_t)const = 0;
			virtual typename enumerator_traits_::difference_type distance(enumerators::inibeg_t, enumerators::end_t)const = 0;
			virtual typename enumerator_traits_::difference_type distance(enumerators::beg_t, enumerators::iniend_t)const = 0;
			virtual typename enumerator_traits_::difference_type distance(enumerators::cur_t, enumerators::iniend_t)const = 0;
			virtual typename enumerator_traits_::difference_type distance(enumerators::end_t, enumerators::iniend_t)const = 0;
			virtual typename enumerator_traits_::difference_type initial_size()const = 0;
			typename enumerator_traits_::difference_type distance(enumerators::beg_t, enumerators::inibeg_t)const{return -distance(inibeg,beg); }
			typename enumerator_traits_::difference_type distance(enumerators::cur_t, enumerators::inibeg_t)const{return -distance(inibeg,cur);}
			typename enumerator_traits_::difference_type distance(enumerators::end_t, enumerators::inibeg_t)const{return -distance(inibeg,end);}
			typename enumerator_traits_::difference_type distance(enumerators::iniend_t, enumerators::beg_t)const{ return -distance(beg, iniend); }
			typename enumerator_traits_::difference_type distance(enumerators::iniend_t, enumerators::cur_t)const{ return -distance(cur, iniend); }
			typename enumerator_traits_::difference_type distance(enumerators::iniend_t, enumerators::end_t)const{ return -distance(end, iniend); }
			typename enumerator_traits_::difference_type distance(enumerators::inibeg_t, enumerators::iniend_t)const{ return initial_size(); }
			typename enumerator_traits_::difference_type distance(enumerators::iniend_t, enumerators::inibeg_t)const{ return -initial_size(); }
		};
		//enumbase
		template<typename enumerator_traits_, typename iterator_, typename interface_>
		struct referable_enumbase : public interface_{
		public:
			using iterator = iterator_;
		public:
			using enumerator_tag = typename enumerator_traits_::enumerator_tag;
			using value_type = typename enumerator_traits_::value_type;
			using difference_type = typename enumerator_traits_::difference_type;
			using reference = typename enumerator_traits_::reference;
			using pointer = typename enumerator_traits_::pointer;
		public:
			iterator_ Cur;
			void initialize(iterator_ Cur_){
				Cur = Cur_;
			}
		public:
			void advance(enumerators::cur_t, difference_type Dif)override{ std::advance(Cur, Dif); }
		};
		template<typename enumerator_traits_, typename iterator_, typename interface_>
		struct sentinel_enumbase : public referable_enumbase<enumerator_traits_, iterator_, interface_>{
		private:
			using base_type = referable_enumbase<enumerator_traits_, iterator_, interface_>;
		public:
			iterator_ End;
			void initialize(iterator_ Beg_, iterator_ End_){
				End = End_;
				base_type::initialize(Beg_);
			}
			operator bool()const override{ return Cur != End; }
			void advance(enumerators::end_t, difference_type Dif)override{ std::advance(End, Dif); }
			difference_type distance(enumerators::cur_t, enumerators::end_t)const override{ return std::distance(Cur, End); }
		};
		template<typename enumerator_traits_, typename iterator_, typename interface_>
		struct range_enumbase : public sentinel_enumbase<enumerator_traits_, iterator_, interface_>{
		private:
			using base_type = sentinel_enumbase<enumerator_traits_, iterator_, interface_>;
		public:
			iterator_ Beg;
			void initialize(iterator_ Beg_, iterator_ End_){
				Beg = Beg_;
				base_type::initialize(Beg_, End_);
			}
		public:
			void reset_cur()override{ Cur = Beg; }
			void reset()override{ reset_cur(); }
			void advance(enumerators::beg_t, difference_type Dif)override{ std::advance(Beg, Dif); }
			difference_type distance(enumerators::beg_t, enumerators::cur_t)const override{ return std::distance(Beg, Cur); }
			difference_type size()const override{ return std::distance(Beg, End); }
		};
		template<typename enumerator_traits_, typename iterator_, typename interface_>
		struct mutable_range_enumbase : public range_enumbase<enumerator_traits_, iterator_, interface_>{
		private:
			using base_type = range_enumbase<enumerator_traits_, iterator_, interface_>;
		public:
			iterator_ IniBeg;
			iterator_ IniEnd;
			void initialize(iterator_ Beg_, iterator_ End_){
				IniBeg = Beg_;
				IniEnd = End_;
				base_type::initialize(Beg_, End_);
			}
		public:
			void reset_range()override{ Beg = IniBeg; End = IniEnd; }
			void reset()override{ reset_range(); base_type::reset(); }
			difference_type distance(enumerators::inibeg_t, enumerators::beg_t)const override{ return std::distance(IniBeg, Beg); }
			difference_type distance(enumerators::inibeg_t, enumerators::cur_t)const override{ return std::distance(IniBeg, Cur); }
			difference_type distance(enumerators::inibeg_t, enumerators::end_t)const override{ return std::distance(IniBeg, End); }
			difference_type distance(enumerators::beg_t, enumerators::iniend_t)const override{ return std::distance(Beg, IniEnd); }
			difference_type distance(enumerators::cur_t, enumerators::iniend_t)const override{ return std::distance(Cur, IniEnd); }
			difference_type distance(enumerators::end_t, enumerators::iniend_t)const override{ return std::distance(End, IniEnd); }
			difference_type initial_size()const override{ return std::distance(IniBeg, IniEnd); }
		};

		//enumbase_interface selecter
		template<typename enumerator_traits_, typename enumerator_tag_ = typename enumerator_traits_::enumerator_tag>
		struct enumbase_interface_from_tag{};
		template<typename enumerator_traits_>
		struct enumbase_interface_from_tag<enumerator_traits_, referable_enumerator_tag>{
			using type = referable_enumbase_interface<enumerator_traits_>;
		};
		template<typename enumerator_traits_>
		struct enumbase_interface_from_tag<enumerator_traits_, sentinel_enumerator_tag>{
			using type = sentinel_enumbase_interface<enumerator_traits_>;
		};
		template<typename enumerator_traits_>
		struct enumbase_interface_from_tag<enumerator_traits_, range_enumerator_tag>{
			using type = range_enumbase_interface<enumerator_traits_>;
		};
		template<typename enumerator_traits_>
		struct enumbase_interface_from_tag<enumerator_traits_, mutable_range_enumerator_tag>{
			using type = mutable_range_enumbase_interface<enumerator_traits_>;
		};

		//enumbase selecter
		template<typename enumerator_traits_, typename iterator_, typename interface_, typename enumerator_tag_ = typename enumerator_traits_::enumerator_tag>
		struct enumbase_from_tag{};
		template<typename enumerator_traits_, typename iterator_, typename interface_>
		struct enumbase_from_tag<enumerator_traits_, iterator_, interface_, referable_enumerator_tag>{
			using type = referable_enumbase<enumerator_traits_, iterator_, interface_>;
		};
		template<typename enumerator_traits_, typename iterator_, typename interface_>
		struct enumbase_from_tag<enumerator_traits_, iterator_, interface_, sentinel_enumerator_tag>{
			using type = sentinel_enumbase<enumerator_traits_, iterator_, interface_>;
		};
		template<typename enumerator_traits_, typename iterator_, typename interface_>
		struct enumbase_from_tag<enumerator_traits_, iterator_, interface_, range_enumerator_tag>{
			using type = range_enumbase<enumerator_traits_, iterator_, interface_>;
		};
		template<typename enumerator_traits_, typename iterator_, typename interface_>
		struct enumbase_from_tag<enumerator_traits_, iterator_, interface_, mutable_range_enumerator_tag>{
			using type = mutable_range_enumbase<enumerator_traits_, iterator_, interface_>;
		};
	}
}
#
#endif
