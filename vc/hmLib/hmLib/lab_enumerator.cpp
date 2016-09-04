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

template<typename T, typename... traits>
struct basic_sentinel_enumerator :public enumerators::traits_mixin<T, basic_sentinel_enumerator<T, traits...>, traits...>{
	using interface = enumerators::traits_interface<T, traits...>;
	template<typename iterator_>
	struct enumeratable :public enumerators::traits_impl<T, enumeratable<iterator_>, interface, traits...>{
	public:
		iterator_ Cur;
		iterator_ Beg;
		iterator_ End;
	public:
		enumeratable(iterator_ Beg_, iterator_ End_) :Cur(Beg_), Beg(Beg_), End(End_){}
	};
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

*/

struct referable_enumeratable{
	static void out(){ std::cout << "referable" << std::endl; }
	template<typename iterator_, typename interface_>
	struct base:public interface_{
		iterator_ Cur;
	};
};
struct sentinel_enumeratable:public referable_enumeratable{
	static void out(){ std::cout << "sentinel" << std::endl; }
};
struct range_enumeratable:public sentinel_enumeratable{
	static void out(){ std::cout << "range" << std::endl; }
};
struct mutable_range_enumeratable:public range_enumeratable{
	static void out(){ std::cout << "mutable_range" << std::endl; }
};


struct find_trait{
	template<typename enumeratable_, typename enumeratable_type = hmLib::sieve_base_of<enumeratable_, sentinel_enumeratable, mutable_range_enumeratable>::type>
	struct impl{};
	template<typename enumeratable_>
	struct impl<enumeratable_, sentinel_enumeratable>{
		static void str(){ std::cout << "snetinel" << std::endl; }
	};
	template<typename enumeratable_>
	struct impl<enumeratable_, mutable_range_enumeratable>{
		static void str(){ std::cout << "mutable_range" << std::endl; }
	};
};
int main(void){
//	find_trait::impl<referable_enumeratable>::str();
	find_trait::impl<sentinel_enumeratable>::str();
	find_trait::impl<range_enumeratable>::str();
	find_trait::impl<mutable_range_enumeratable>::str();


	system("pause");
	return 0;
}
