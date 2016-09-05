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
	struct abilities_mixin{};
	template<typename type, typename base, typename ability, typename... others>
	struct abilities_mixin <type, base, ability, others...> : public ability::template mixin<type, base>, public abilities_mixin<type, base, others...>{};
	template<typename type, typename... others>
	struct abilities_interface{};
	template<typename type, typename ability, typename... others>
	struct abilities_interface <type, ability, others...> : public ability::template interface<type>, public abilities_interface<type, others...>{};
	template<typename type, typename enumeratable, typename base, typename... others>
	struct abilities_impl : public base{};
	template<typename type, typename enumeratable, typename base, typename ability, typename... others>
	struct abilities_impl <type, enumeratable, base, ability, others...> : public ability::template impl<type, enumeratable, abilities_impl<type, enumeratable, base, others...>>{};
}
namespace enumerators{
	struct forward_traversal_abilities{
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
	struct sort_abilities{
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

template<typename T, typename enumeratable_, typename... abilities>
struct enumerator_base :public enumerators::abilities_mixin<T, enumerator_base<T, enumeratable_, abilities...>, abilities...>{
	using interface = enumerators::abilities_interface<T, enumeratable_, abilities...>;
	template<typename iterator_>
	using enumeratable = enumerators::abilities_impl<T, enumeratable_::impl<iterator_>, interface, abilities...>;
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

//enumerator_base tag
struct referable_enumeratable_tag{};
struct sentinel_enumeratable_tag :public referable_enumeratable_tag{};
struct range_enumeratable_tag :public sentinel_enumeratable_tag{};
struct mutable_range_enumeratable_tag :public range_enumeratable_tag{};



struct find_ability{
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

struct sort_ability{
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
	struct ability_enumerator{
	public:
		void sort(void){ auto& ref = static_cast<base*>(this)->get_enumeratable(); ref.sort(); }
	};
};

struct incrementable_traversal_abilities{
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
	struct ability_enumerator{
		decltype(static_cast<base*>(this)->get_enumeratable()) Ref;
	private:
		auto& ref()->{ return  static_cast<base*>(this)->get_enumeratable(); }
		const auto& ref()const->decltype(static_cast<base*>(this)->get_enumeratable()){ return  static_cast<base*>(this)->get_enumeratable(); }
	public:
		ability_enumerator() :Ref(static_cast<enumeratable&>(*this)){}
	public:
		type& operator*(void){ return *ref(); }
		const type& operator*(void) const{ return *ref(); }
		operator bool()const{ return static_cast<bool>(ref()); }
		void operator++(){ ++ref(); }
		void reset(){ ref().reset(); }
	};
};
struct forward_traversal_abilities{
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
	struct ability_enumerator{
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

//enumerator_base tag
struct referable_enumerator_tag{};
struct sentinel_enumerator_tag :public referable_enumerator_tag{};
struct range_enumerator_tag :public sentinel_enumerator_tag{};
struct mutable_range_enumerator_tag :public range_enumerator_tag{};

//enumeratable
namespace enumerators{
	//enumbase
	template<typename iterator_, typename interface_>
	struct referable_enumbase: public interface_{
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
//ability pack
namespace enumerators{
	template<typename... abilities>
	struct ability_pack{
	private:
		template<typename enumerator_base, typename... others>
		struct enumeratable_interface_pack{};
		template<typename enumerator_base, typename ability, typename... others>
		struct enumeratable_interface_pack<enumerator_base, ability, others...>
			: public ability::template ability_interface<enumerator_base>
			, public enumeratable_interface_pack<enumerator_base, others...>{
		};

		template<typename enumerator_base, typename enumbase, typename base, typename... others>
		struct enumeratable_pack: public base{};
		template<typename enumerator_base, typename enumbase, typename base, typename ability, typename... others>
		struct enumeratable_pack<enumerator_base, enumbase, base, ability, others...>
			: public enumeratable_pack<enumerator_base, enumbase, typename ability::template ability_enumeratable<enumerator_base, enumbase, base>, others...>{
		};

		template<typename enumerator_base, typename base, typename... others>
		struct enumerator_pack : public base{};
		template<typename enumerator_base, typename base, typename ability, typename... others>
		struct enumerator_pack<enumerator_base, base, ability, others...>
			: public enumerator_pack<enumerator_base, typename ability::template ability_enumerator<enumerator_base, base>, others...>{
		};
	public:
		template<typename enumerator_base>
		using enumeratable_interface = enumeratable_interface_pack<enumerator_base, abilities...>;
		template<typename enumerator_base,typename enumbase>
		using enumeratable = enumeratable_pack<enumerator_base, enumbase, enumbase, abilities...>;
		template<typename enumerator_base>
		using enumerator = enumerator_pack<enumerator_base, enumerator_base, abilities...>;
	};

	template<typename enumerator_traits_, typename... abilities>
	struct enumerator_base{
	public:
		using this_type = enumerator_base<enumerator_traits_, abilities...>;
		using enumerator_traits = enumerator_traits_;
		using enumerator_tag = typename enumerator_traits::enumerator_tag;
		using value_type = typename enumerator_traits::value_type;
		using difference_type = typename enumerator_traits::difference_type;
		using reference = typename enumerator_traits::reference;
		using pointer = typename enumerator_traits::pointer;
	private:
		using this_ability_pack = enumerators::ability_pack<abilities...>;
	public:
		using enumeratable_interface = typename this_ability_pack::template enumeratable_interface<this_type>;
	protected:
		template<typename iterator_>
		using enumbase = typename enumerators::enumbase_from_tag<enumerator_tag, iterator_, enumeratable_interface>::type;
	public:
		template<typename iterator_>
		using enumeratable = typename this_ability_pack::template enumeratable<this_type, enumbase<iterator_> >;
	private:
		std::unique_ptr<enumeratable_interface> Ptr;
	protected:
		enumeratable_interface& ref(){ return *Ptr; }
		const enumeratable_interface& ref()const{ return *Ptr; }
		const enumeratable_interface& cref()const{ return *Ptr; }
	};

	template<typename enumerator_traits_, typename... abilities>
	using ability_enumerator = typename ability_pack<abilities...>::template enumerator< enumerators::enumerator_base<enumerator_traits_, abilities...> >;

	template<typename enumerator_tag_, template<typename iterator_> typename  enumeratable_, template<typename iterator_> typename enumbase_>
	struct make_enumeratable_mixin{
		template<typename iterator_>
		static enumeratable_<iterator_> make_enumeratable(iterator_ Beg_, iterator_ End_){
			enumeratable_<iterator_> Enumeratable;
			static_cast<enumbase_<iterator_>&>(Enumeratable).set(Beg_, End_);
			return Enumeratable;
		}
	};
	template<template<typename iterator_> typename  enumeratable_, template<typename iterator_> typename enumbase_>
	struct make_enumeratable_mixin<referable_enumerator_tag, enumeratable_, enumbase_>{
		template<typename iterator_>
		static enumeratable_<iterator_> make_enumeratable(iterator_ Cur_){
			enumeratable_<iterator_> Enumeratable;
			static_cast<enumbase_<iterator_>&>(Enumeratable).set(Cur_);
			return Enumeratable;
		}
	};
}
struct ability1{
	template<typename enumerator_base>
	struct ability_interface{
		virtual typename enumerator_base::value_type func1() = 0;
	};
	template<typename enumerator_base, typename enumbase, typename base>
	struct ability_enumeratable : public base{
	public:
		typename enumerator_base::value_type func1()override{
			return *enumbase::Cur;
		}
	};
	template<typename enumerator_base, typename base>
	struct ability_enumerator : public base{
	public:
		typename enumerator_base::value_type func1(){
			return enumerator_base::ref().func1();
		}
	};
};
struct ability2{
	template<typename enumerator_base>
	struct ability_interface{
		virtual typename enumerator_base::value_type func2() = 0;
	};
	template<typename enumerator_base, typename enumbase, typename base>
	struct ability_enumeratable : public base{
	public:
		typename enumerator_base::value_type func2()override{
			return *enumbase::Beg;
		}
	};
	template<typename enumerator_base, typename base>
	struct ability_enumerator:public base{
	public:
		typename enumerator_base::value_type func2(){
			return ref().func2();
		}
	};
};


template<typename enumerator_tag_, typename value_type_, typename difference_type_ = int, typename reference_ = value_type_&, typename pointer_ = const value_type_&>
struct enumerator_traits{
public:
	using enumerator_tag = enumerator_tag_;
	using value_type = value_type_;
	using difference_type = difference_type_;
	using reference = reference_;
	using pointer = pointer_;
};
template<typename enumerator_traits_, typename... abilities>
struct enumerator 
	: public enumerators::ability_enumerator<enumerator_traits_, abilities...>
	, public enumerators::make_enumeratable_mixin<
		typename enumerator_traits_::enumerator_tag
		,typename enumerators::ability_enumerator<enumerator_traits_, abilities...>::enumeratable
		,typename enumerators::ability_enumerator<enumerator_traits_, abilities...>::enumbase
	>{
};

int main(void){
	int Array[10];
	using this_enumerator = enumerator<enumerator_traits<referable_enumerator_tag, int>, ability1>;
	this_enumerator Enum;
	this_enumerator::make_enumeratable(Array);
//	Enum.func2();
	Enum.func1();
	return 0;
}
