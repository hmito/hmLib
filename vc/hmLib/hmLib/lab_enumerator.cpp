#include<memory>
#include<algorithm>
#include<iostream>
#include<array>
#include<iostream>
#include<iterator>
#include<type_traits>
#include<hmLib/type_traits.hpp>

/*
namespace enumerators{
	template<typename type, typename base, typename... others>
	struct traits_mixin{};
	template<typename type, typename base, typename trait, typename... others>
	struct traits_mixin <type, base, trait, others...> : public trait::template mixin<type, base>, public traits_mixin<type, base, others...>{};
	template<typename type, typename... others>
	struct traits_interface{};
	template<typename type, typename trait, typename... others>
	struct traits_interface <type, trait, others...> : public trait::template interface<type>, public traits_interface<type, others...>{};
	template<typename type, typename enumeratable, typename base, typename... others>
	struct traits_impl : public base{};
	template<typename type, typename enumeratable, typename base, typename trait, typename... others>
	struct traits_impl <type, enumeratable, base, trait, others...> : public trait::template impl<type, enumeratable, traits_impl<type, enumeratable, base, others...>>{};
}
namespace enumerators{
	struct forward_traversal_traits{
		template<typename type>
		struct interface{
		public:
			virtual const type& operator*(void) const = 0;
			virtual type& operator*(void) = 0;
			virtual operator bool()const = 0;
			virtual void operator++() = 0;
			virtual void reset() = 0;
		};
		template<typename type, typename enumeratable, typename base>
		struct impl : public base{
		public:
			const type& operator*(void) const override{ const auto& ref = static_cast<const enumeratable&>(*this); return *(ref.Cur); }
			type& operator*(void) override{ auto& ref = static_cast<enumeratable&>(*this); return *(ref.Cur); }
			operator bool()const override{ const auto& ref = static_cast<const enumeratable&>(*this); return ref.Cur != ref.End; }
			void operator++() override{ auto& ref = static_cast<enumeratable&>(*this); ++(ref.Cur); }
			void reset() override{ auto& ref = static_cast<enumeratable&>(*this); ref.Cur = ref.Beg; }
		};
		template<typename type, typename base>
		struct mixin{
		public:
			const type& operator*(void) const{ const auto& ref = static_cast<const base*>(this)->get_enumeratable(); return *ref; }
			type& operator*(void){ auto& ref = static_cast<base*>(this)->get_enumeratable(); return *ref; }
			operator bool()const{ const auto& ref = static_cast<const base*>(this)->get_enumeratable(); return static_cast<bool>(ref); }
			void operator++(){ auto& ref = static_cast<base*>(this)->get_enumeratable(); ++ref; }
			void reset(){ auto& ref = static_cast<base*>(this)->get_enumeratable(); ref.reset(); }
		};
	};
	struct sort_traits{
		template<typename type>
		struct interface{
		public:
			virtual void sort(void) = 0;
		};
		template<typename type, typename enumeratable, typename base>
		struct impl : public base{
		public:
			void sort(void)override{ auto& ref = static_cast<enumeratable&>(*this); std::sort(ref.Begin, ref.End); }
		};
		template<typename type, typename base>
		struct mixin{
		public:
			void sort(void){ auto& ref = static_cast<base*>(this)->get_enumeratable(); ref.sort(); }
		};
	};
}

template<typename T, typename enumeratable_, typename... traits>
struct basic_enumerator :public enumerators::traits_mixin<T, basic_enumerator<T, enumeratable_, traits...>, traits...>{
	using interface = enumerators::traits_interface<T, enumeratable_, traits...>;
	template<typename iterator_>
	using enumeratable = enumerators::traits_impl<T, enumeratable_::impl<iterator_>, interface, traits...>;
private:
	std::unique_ptr<interface> Ptr;
public:
	interface& get_enumeratable(){ return *Ptr; }
	const interface& get_enumeratable()const{ return *Ptr; }
public:
	template<typename iterator>
	basic_sentinel_enumerator(iterator Beg, iterator End){
		Ptr = std::make_unique<enumeratable<iterator>>(Beg, End);
	}
};

//enumerator tag
struct referable_enumeratable_tag{};
struct sentinel_enumeratable_tag :public referable_enumeratable_tag{};
struct range_enumeratable_tag :public sentinel_enumeratable_tag{};
struct mutable_range_enumeratable_tag :public range_enumeratable_tag{};



struct find_trait{
	template<
		typename enumeratable_, 
		typename enumeratable_type = hmLib::sieve_base_of<enumeratable_, sentinel_enumeratable_tag, mutable_range_enumeratable_tag>::type
	>
	struct impl{};
	template<typename enumeratable_>
	struct impl<enumeratable_, sentinel_enumeratable_tag>{
		static void str(){ std::cout << "snetinel" << std::endl; }
	};
	template<typename enumeratable_>
	struct impl<enumeratable_, mutable_range_enumeratable_tag>{
		static void str(){ std::cout << "mutable_range" << std::endl; }
	};
};

struct sort_trait{
	template<typename type>
	struct enumeratable_interface{
	public:
		virtual void sort(void) = 0;
	};
	template<typename type, typename enumeratable, typename base>
	struct enumeratable_mixin : public base{
	public:
		void sort(void)override{ auto& ref = static_cast<enumeratable&>(*this); std::sort(ref.Begin, ref.End); }
	};
	template<typename type, typename base>
	struct enumerator_mixin{
	public:
		void sort(void){ auto& ref = static_cast<base*>(this)->get_enumeratable(); ref.sort(); }
	};
};

struct incrementable_traversal_traits{
	template<typename type>
	struct enumeratable_interface{
	public:
		virtual type& operator*(void) = 0;
		virtual const type& operator*(void) const = 0;
		virtual void operator++() = 0;
	};
	template<typename type, typename enumeratable, typename base>
	struct enumeratable_mixin : public base{
	private:
		enumeratable& Ref;
	public:
		enumeratable_mixin():Ref(static_cast<enumeratable&>(*this)){}
	public:
		type& operator*(void) override{ return *(Ref.Cur); }
		const type& operator*(void) const override{ return *(Ref.Cur); }
		void operator++() override{ ++(Ref.Cur); }
	};
	template<typename type, typename base>
	struct enumerator_mixin{
		decltype(static_cast<base*>(this)->get_enumeratable()) Ref;
	private:
		auto& ref()->{ return  static_cast<base*>(this)->get_enumeratable(); }
		const auto& ref()const->decltype(static_cast<base*>(this)->get_enumeratable()){ return  static_cast<base*>(this)->get_enumeratable(); }
	public:
		enumerator_mixin() :Ref(static_cast<enumeratable&>(*this)){}
	public:
		type& operator*(void){ return *ref(); }
		const type& operator*(void) const{ return *ref(); }
		operator bool()const{ return static_cast<bool>(ref()); }
		void operator++(){ ++ref(); }
		void reset(){ ref().reset(); }
	};
};
struct forward_traversal_traits{
	template<typename type>
	struct enumeratable_interface{
	public:
		virtual type& operator*(void) = 0;
		virtual const type& operator*(void) const = 0;
		virtual operator bool()const = 0;
		virtual void operator++() = 0;
		virtual void reset() = 0;
	};
	template<typename type, typename enumeratable, typename base>
	struct enumeratable_mixin : public base{
	private:
		enumeratable& ref(){return static_cast<enumeratable&>(*this);}
		const enumeratable& ref()const{ return static_cast<const enumeratable&>(*this); }
	public:
		type& operator*(void) override{ return *(ref().Cur); }
		const type& operator*(void) const override{ return *(ref().Cur); }
		operator bool()const override{ const auto& Ref = ref(); return Ref.Cur != Ref.End; }
		void operator++() override{ ++(ref().Cur); }
		void reset() override{ auto& Ref = ref(); ref.Cur = ref.Beg; }
	};
	template<typename type, typename base>
	struct enumerator_mixin{
	private:
		auto& ref()->decltype(static_cast<base*>(this)->get_enumeratable()){ return  static_cast<base*>(this)->get_enumeratable(); }
		const auto& ref()const->decltype(static_cast<base*>(this)->get_enumeratable()){ return  static_cast<base*>(this)->get_enumeratable(); }
	public:
		type& operator*(void){ return *ref(); }
		const type& operator*(void) const{return *ref(); }
		operator bool()const{return static_cast<bool>(ref()); }
		void operator++(){ ++ref(); }
		void reset(){ ref().reset(); }
	};
};
//*/
//enumeratable
namespace enumerators{
	//enumerator tag
	struct referable_enumerator_tag{};
	struct sentinel_enumerator_tag :public referable_enumerator_tag{};
	struct range_enumerator_tag :public sentinel_enumerator_tag{};
	struct mutable_range_enumerator_tag :public range_enumerator_tag{};

	//enumeratable
	struct referable_enumeratable{
		template<typename extention_interface_>
		struct enumeratable_interface{};
		template<typename iterator_, typename extention_interface_>
		struct enumeratable : public enumeratable_interface, public extention_interface_{
			iterator_ Cur;
		};
	};
	struct sentinel_enumeratable{
		template<typename extention_interface_>
		struct enumeratable_interface : public referable_enumeratable::enumeratable_interface< extention_interface_>{
			virtual operator bool()const = 0;
		};
		template<typename iterator_, typename extention_interface_>
		struct enumeratable :public enumeratable_interface, public extention_interface_{
			iterator_ Cur;
			iterator_ End;
		public:
			operator bool()const override{
				return Cur != End;
			}
		};
	};
	struct range_enumeratable{
		template<typename extention_interface_>
		struct enumeratable_interface : public sentinel_enumeratable::enumeratable_interface< extention_interface_>{
			virtual void reset() = 0;
		};
		template<typename iterator_, typename extention_interface_>
		struct enumeratable :public enumeratable_interface, public extention_interface_{
			iterator_ Cur;
			iterator_ Beg;
			iterator_ End;
		public:
			operator bool()const override{
				return Cur != End;
			}
			void reset()override{
				Cur = Beg;
			}
		};
	};
	struct mutable_range_enumeratable{
		template<typename extention_interface_>
		struct enumeratable_interface : public range_enumeratable::enumeratable_interface< extention_interface_>{
			virtual void reset_range() = 0;
		};
		template<typename iterator_, typename extention_interface_>
		struct enumeratable :public enumeratable_interface, public extention_interface_{
			iterator_ Cur;
			iterator_ Beg;
			iterator_ End;
			iterator_ IniBeg;
			iterator_ IniEnd;
		public:
			operator bool()const override{
				return Cur != End;
			}
			void reset()override{
				Cur = Beg;
			}
			void reset_range()override{
				Beg = IniBeg;
				End = IniEnd;
			}
		};
	};

	//enumeratable selecter
	template<typename enumerator_tag>
	struct select_enumeratable{};
	template<>
	struct select_enumeratable<referable_enumerator_tag>{
		using type = referable_enumeratable;
	};
	template<>
	struct select_enumeratable<sentinel_enumerator_tag>{
		using type = sentinel_enumeratable;
	};
	template<>
	struct select_enumeratable<range_enumerator_tag>{
		using type = range_enumeratable;
	};
	template<>
	struct select_enumeratable<mutable_range_enumerator_tag>{
		using type = mutable_range_enumeratable;
	};
}
//trait pack
namespace enumerators{
	template<typename... traits>
	struct traits_pack{
	private:
		template<typename enumerator, typename... others>
		struct extention_interface_pack{};
		template<typename enumerator, typename trait, typename... others>
		struct extention_interface_pack<enumerator, trait, others...>
			: public trait::template extention_interface<enumerator>
			, public extention_interface_pack<enumerator, others...>{
		};
		template<typename enumeratable, typename base, typename... others>
		struct extention_pack{};
		template<typename enumeratable, typename base, typename trait, typename... others>
		struct extention_pack<enumeratable, base, trait, others...>
			: public extention_pack<enumeratable, trait::template extention<enumeratable, base>, others...>{};
		template<typename enumerator, typename base, typename... others>
		struct enumerator_extention_pack{};
		template<typename enumerator, typename base, typename trait, typename... others>
		struct enumerator_extention_pack<enumerator, base, trait, others...>
			: public enumerator_extention_pack<enumerator, trait::template enumerator_extention<enumerator, base>, others...>{};
	public:
		template<typename enumerator>
		using extention_interface = extention_interface_pack<enumerator, traits...>;
		template<typename enumeratable, typename base>
		using extention = extention_pack<enumeratable, base, traits...>;
		template<typename enumerator, typename base>
		using enumerator_extention = enumerator_extention_pack<enumerator, base, traits...>;
	};
}
struct trait1{
	template<typename enumerator>
	struct extention_interface{
		virtual enumerator::value_type func1() = 0;
	};
	template<typename enumeratable, typename base>
	struct extention : public base{
		enumeratable::value_type func1()override{
			return *enrable::Cur;
		}
	};
	template<typename enumerator, typename base>
	struct enumerator_extention : public base{
		enumerator::value_type  func1(){
			enr_t::ref().func1();
		}
	};
};
struct trait2{
	template<typename enumerator>
	struct extention_interface{
		virtual enumerator::value_type func2() = 0;
	};
	template<typename enumeratable, typename base>
	struct extention : public base{
		enumeratable::value_type func2()override{
			return *enumeratable::Beg;
		}
	};
	template<typename enumerator, typename base>
	struct enumerator_extention : public base{
		enumerator::value_type func2(){
			return enumerator::ref().func2();
		}
	};
};


template<typename enumerator_tag_, typename traversal_tag_, typename value_type_, typename difference_type_, typename reference_, typename pointer_, typename... traits>
struct basic_enumerator{
public:
	using enumerator_tag = enumerator_tag_;
	using traversal_tag = traversal_tag_;
	using value_type = value_type_;
	using difference_type = difference_type_;
	using reference = reference_;
	using pointer = pointer_;
private:
	using this_trais_pack = enumerators::traits_pack<traits...>;
	using this_enumeratable = typename enumerators::select_enumeratable<enumerator_tag>::type;
public:
	using enumeratable_interface = typename this_enumeratable::template enumeratable_interface<typename this_trais_pack::extention_interface>;
	template<typename iterator_>
	using enumeratable = typename this_trais_pack::extention<enumeratable_, enumeratable_>;
private:
	std::unique_ptr<enumeratable_interface> Ptr;
public:
	template<typename iterator_>
	static enumeratable<iterator_> make_enumeratable(iterator_ Begin, iterator_ End){
	}
};
int main(void){
	return 0;
}
