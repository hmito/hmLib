#include<utility>
#include<deque>
#include<initializer_list>
#include<functional>
#include<vector>
#include<future>
#include<type_traits>
#include<iostream>
#include<boost/optional.hpp>
#include<cmath>

#include"../../../monad.hpp"
#include"../../../monad/just.hpp"
#include"../../../monad/lazy.hpp"
#include"../../../monad/optional.hpp"

//vector monad and others...
namespace hmLib{
	namespace monad{
		template<typename T>
		struct vector :public monad_mixin<vector<T>, T, monad_categories::flattenable_monad_tag> {
		private:
			using this_type = vector<T>;
			using container = std::vector<T>;
			using iterator = typename container::iterator;
			using const_iterator = typename container::const_iterator;
		private:
			std::vector<T> val;
		public:
			vector() = default;
			vector(const this_type&) = default;
			this_type& operator=(const this_type&) = default;
			vector(this_type&&) = default;
			this_type& operator=(this_type&&) = default;
			explicit vector(std::vector<T> val_) :val(std::move(val_)) {}
			template<typename input_iterator>
			vector(input_iterator Beg, input_iterator End) : val(Beg, End) {}
			vector(std::initializer_list<T> IL) :vector(std::begin(IL), std::end(IL)) {}
		public:
			//for monad
			template<typename U>
			struct rebind { using type = vector<U>; };
			explicit vector(T val_) :val(1, std::move(val_)) {}
			template<typename fn>
			auto apply(fn&& Func)const {
				using ans_type = decltype(hmLib::monad::fmap(std::declval<T>(), Func));
				std::vector<ans_type> Applied(val.size());

				auto Out = std::begin(Applied);
				for (const auto& v : val) *(Out++) = hmLib::monad::fmap(v, Func);
				return vector<ans_type>(std::move(Applied));
			}
		public:
			//for omitable_monad/flattenable_monad
			explicit vector(vector<vector<T>> m_) {
				for (auto&& v : m_.val) {
					val.insert(v.end(), std::begin(v), std::end(v));
				}
			}
		public:
			std::size_t size()const { return val.size(); }
			iterator begin() { return val.begin(); }
			iterator end() { return val.end(); }
			const_iterator cbegin()const { return val.cbegin(); }
			const_iterator cend()const { return val.cend(); }
			const_iterator begin()const { return cbegin(); }
			const_iterator end()const { return cend(); }
		public:
			void push_back(const T& e) { val.push_back(e); }
			void emplace_back(T&& e) { val.emplace_back(e); }
		};
		template<typename T>
		auto make_vector(T&& val) { return vector<typename std::decay<T>::type>(std::forward<T>(val)); }
		template<typename input_iterator>
		auto make_vector(input_iterator Begin, input_iterator End) { return vector<typename std::decay<decltype(*Begin)>::type>(Begin, End); }

		template<typename T>
		struct future {
			std::future<T> Future;
		};

		template<typename T,typename TransformRule>
		struct transform{};
	}
}

int main(void) {
	{
		hmLib::monad::just<hmLib::monad::just<hmLib::monad::just<double>>> v1;

		auto v2 = hmLib::monad::fmap(v1, hmLib::monad::do_omit());

		std::cout << "omit" << std::endl;
		std::cout << "before:" << typeid(v1).name() << std::endl;
		std::cout << "after :" << typeid(v2).name() << std::endl;
	}

	{
		hmLib::monad::just<hmLib::monad::just<hmLib::monad::just<double>>> e1;
		hmLib::monad::vector<hmLib::monad::just<hmLib::monad::just<hmLib::monad::just<double>>>> v1;
		v1.push_back(e1);
		v1.push_back(e1);
		v1.push_back(e1);

		auto v2 = hmLib::monad::fmap(v1, hmLib::monad::do_omit());

		std::cout << "omit" << std::endl;
		std::cout << "before:" << typeid(v1).name() << std::endl;
		std::cout << "after :" << typeid(v2).name() << std::endl;
	}


	{
		auto Fn = [](double v) {return v; };
		auto ForMonad = hmLib::monad::for_context<hmLib::monad::is_monad>(Fn);
		auto ForOmmitable = hmLib::monad::for_context<hmLib::monad::is_omittable>(Fn);
	}

	{
		hmLib::monad::do_omit Do;

		auto Check = hmLib::monad::fmap_target<decltype(Do)>();
		std::cout << "for_omit" << std::endl;
		std::cout << "double:" << Check.check(5.5) << std::endl;
		std::cout << "vector:" << Check.check(hmLib::monad::vector<double>()) << std::endl;
		std::cout << "maybe:" << Check.check(hmLib::monad::just<double>(5.5)) << std::endl;
	}

	{
		hmLib::monad::do_flatten Do;

		auto Check = hmLib::monad::fmap_target<decltype(Do)>();
		std::cout << "for_flatten" << std::endl;
		std::cout << "double:" << Check.check(5.5) << std::endl;
		std::cout << "vector:" << Check.check(hmLib::monad::vector<double>()) << std::endl;
		std::cout << "maybe:" << Check.check(hmLib::monad::just<double>(5.5)) << std::endl;
	}

	{
		auto Do = [](double v) {return v * 2; };

		auto Check = hmLib::monad::fmap_target<decltype(Do)>();
		std::cout << "for_normal" << std::endl;
		std::cout << "double:" << Check.check(5.5) << std::endl;
		std::cout << "vector:" << Check.check(hmLib::monad::vector<double>()) << std::endl;
		std::cout << "maybe:" << Check.check(hmLib::monad::just<double>(5.5)) << std::endl;
	}

	auto Val = hmLib::monadic(8);
	auto Val2 = Val >> [](int a) {return a*0.5; }>>hmLib::monad::do_evaluate();
	std::cout << typeid(Val2).name() << std::endl;

	std::cout << (hmLib::monadic(8) >> hmLib::monad::do_evaluate()).get() << std::endl;
	std::cout << (hmLib::monadic(8) >> [](int a) {return a*0.5; } >> hmLib::monad::do_evaluate()).get() << std::endl;
	auto Ans = hmLib::monadic(8) >> [](int a) {return a*0.5; } >> [](double f) {return hmLib::monad::make_optional(f); } >> hmLib::monad::do_evaluate();
	if(Ans.get())std::cout << Ans.get().get()<< std::endl;
	else std::cout << "null" << std::endl;

	system("pause");
	return 0;
}