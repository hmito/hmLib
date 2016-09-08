#include<memory>
#include<algorithm>
#include<iostream>
#include<array>
#include<iostream>
#include<iterator>
#include<type_traits>
#include<hmLib/enumerators.hpp>

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
	struct enumerator_impl : public base{
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
	struct enumerator_impl :public base{
	public:
		typename enumerator_base::value_type func2(){
			return ref().func2();
		}
	};
};

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

int main(void){
	int Array[5] = {9,7,5,3,1};
	using this_enumerator = hmLib::enumerator<hmLib::enumerator_traits<hmLib::range_enumerator_tag, int>, hmLib::enumerators::forward_traversal_abilities>;
	auto EnumeratorImpl = this_enumerator::make_enumerator<int*>(std::begin(Array),std::end(Array));

	this_enumerator& Ref = EnumeratorImpl;


	using enum_traits = hmLib::enumerator_traits<hmLib::range_enumerator_tag, int>;
	using enumbase = hmLib::enumerators::range_enumbase<enum_traits,int*, hmLib::enumerators::enumerator_pack<enum_traits,hmLib::enumerators::forward_traversal_abilities>>;
	hmLib::enumerators::forward_traversal_abilities::ability_impl<enumbase,enumbase> Enum;


	system("pause");
	return 0;
}
