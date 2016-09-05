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
		template<typename iterator_, typename interface_>
		struct referable_enumbase : public interface_{
			using iterator = iterator_;
			iterator_ Cur;
			void set(iterator_ Cur_){
				Cur = Cur_;
			}
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
