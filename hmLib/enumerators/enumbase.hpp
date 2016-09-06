#ifndef HMLIB_ENUMERATORS_ENUMBASE_INC
#define HMLIB_ENUMERATORS_ENUMBASE_INC 100
#
namespace hmLib{
	//enumerator_base tag
	struct referable_enumerator_tag{};
	struct sentinel_enumerator_tag :public referable_enumerator_tag{};
	struct range_enumerator_tag :public sentinel_enumerator_tag{};
	struct mutable_range_enumerator_tag :public range_enumerator_tag{};

	//enumbase
	namespace enumerators{
		//enumbase
		struct referable_enumbase_interface{};
		template<typename iterator_, typename interface_>
		struct referable_enumbase : public interface_{
		public:
			using iterator = iterator_;
		public:
			using enumerator_tag = typename interface_::enumerator_tag;
			using value_type = typename interface_::value_type;
			using difference_type = typename interface_::difference_type;
			using reference = typename interface_::reference;
			using pointer = typename interface_::pointer;
		public:
			iterator_ Cur;
			void set(iterator_ Cur_){
				Cur = Cur_;
			}
		};
		struct sentinel_enumbase_interface{
			virtual operator bool()const = 0;
		};
		template<typename iterator_, typename interface_>
		struct sentinel_enumbase : public referable_enumbase<iterator_, interface_>{
		private:
			using base_type = referable_enumbase<iterator_, interface_>;
		public:
			iterator_ End;
			void set(iterator_ Beg_, iterator_ End_){
				End = End_;
				base_type::set(Beg_);
			}
		public:
			operator bool()const override{ return Cur != End; }
		};
		struct range_enumbase_interface{
		public:
			virtual void reset() = 0;
		};
		template<typename iterator_, typename interface_>
		struct range_enumbase : public sentinel_enumbase<iterator_, interface_>{
		private:
			using base_type = sentinel_enumbase<iterator_, interface_>;
		public:
			iterator_ Beg;
			void set(iterator_ Beg_, iterator_ End_){
				Beg = Beg_;
				base_type::set(Beg_, End_);
			}
		public:
			operator bool()const override{ return Cur != End; }
		};
		template<typename iterator_, typename interface_>
		struct mutable_range_enumbase : public range_enumbase<iterator_, interface_>{
		private:
			using base_type = range_enumbase<iterator_, interface_>;
		public:
			iterator_ IniBeg;
			iterator_ IniEnd;
			void set(iterator_ Beg_, iterator_ End_){
				IniBeg = Beg_;
				IniEnd = End_;
				base_type::set(Beg_, End_);
			}
		};

		//enumbase selecter
		template<typename enumerator_tag, typename iterator_, typename interface_>
		struct enumbase_from_tag{};
		template<typename iterator_, typename interface_>
		struct enumbase_from_tag<referable_enumerator_tag, typename iterator_, typename interface_>{
			using type = referable_enumbase<iterator_, interface_>;
		};
		template<typename iterator_, typename interface_>
		struct enumbase_from_tag<sentinel_enumerator_tag, typename iterator_, typename interface_>{
			using type = sentinel_enumbase<iterator_, interface_>;
		};
		template<typename iterator_, typename interface_>
		struct enumbase_from_tag<range_enumerator_tag, typename iterator_, typename interface_>{
			using type = range_enumbase<iterator_, interface_>;
		};
		template<typename iterator_, typename interface_>
		struct enumbase_from_tag<mutable_range_enumerator_tag, typename iterator_, typename interface_>{
			using type = mutable_range_enumbase<iterator_, interface_>;
		};
	}
}
#
#endif
