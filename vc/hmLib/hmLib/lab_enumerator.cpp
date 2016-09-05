#include<memory>
#include<algorithm>
#include<iostream>
#include<array>
#include<iostream>
#include<iterator>
#include<type_traits>
#include<hmLib/type_traits.hpp>

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
	struct enumerator_impl{
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
	struct enumerator_impl{
		decltype(static_cast<base*>(this)->get_enumeratable()) Ref;
	private:
		auto& ref()->{ return  static_cast<base*>(this)->get_enumeratable(); }
		const auto& ref()const->decltype(static_cast<base*>(this)->get_enumeratable()){ return  static_cast<base*>(this)->get_enumeratable(); }
	public:
		enumerator_impl() :Ref(static_cast<enumeratable&>(*this)){}
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
	struct enumerator_impl{
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
int main(void){
	int Array[5] = {9,7,5,3,1};
	using this_enumerator = enumerator<enumerator_traits<range_enumerator_tag, int>, ability1, ability2>;
	auto Enumeratable = this_enumerator::make_enumeratable(Array,Array+5);
	this_enumerator Enum(Enumeratable);
//	Enum.func2();
	std::cout << Enum.func1() << std::endl;
	std::cout << Enum.func2() << std::endl;

	system("pause");
	return 0;
}
