#include<memory>
#include<algorithm>
#include<iostream>
#include<array>
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
			/*
			const T& operator()(void) const override{ const auto& This = static_cast<const base&>(*this); return *(This.Itr); }
			T& operator()(void) override{ auto& ref = static_cast<base&>(*this); return *(ref.itr); }
			operator bool()const override{ const auto& ref = static_cast<const base&>(*this); return ref.itr != ref.end; }
			void operator++() override{ auto& ref = static_cast<base&>(*this); ++(ref.itr); }
			void reset() override{ auto& ref = static_cast<base&>(*this);  ref.begin = ref.itr; }
			*/
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
int main(void){
	using this_enumerator = basic_sentinel_enumerator<int, enumerators::forward_traversal_traits>;

	std::array<int, 100> Array;
	int val = 0;
	std::generate(Array.begin(), Array.end(), [&](){return ++val; });

	this_enumerator Enum(Array.begin(), Array.end());

	for(; Enum; ++Enum){
		std::cout << *Enum << std::endl;
	}

	system("pause");
	return 0;
}
