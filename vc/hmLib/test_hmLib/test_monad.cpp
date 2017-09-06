#include "stdafx.h"
#include "CppUnitTest.h"
#include <vector>
#include <algorithm>
#include <list>
#include"../../../monad.hpp"
#include"../../../monad/just.hpp"
#include"../../../monad/lazy.hpp"
using namespace Microsoft::VisualStudio::CppUnitTestFramework;


struct heavy_object{
	using this_type = heavy_object;
	static int Cur;
	static int All;
	double val;
	heavy_object(double val_):val(val_) { ++Cur; ++All; }
	heavy_object(const this_type& My) :val(My.val) { ++Cur; ++All; }
	heavy_object(this_type&& My) :val(My.val) { ++Cur; ++All; }
	this_type& operator=(const this_type&) = default;
	this_type& operator=(this_type&&) = default;
	~heavy_object() { --Cur; }
	double func() const{ return -val; }
};
int heavy_object::Cur = 0;
int heavy_object::All = 0;

heavy_object make_heavy_object(double val) {
	return heavy_object(val);
}
namespace hmLib {
	TEST_CLASS(test_monad) {
	public:
	TEST_METHOD(lazy_monad_lambda) {
		auto Ans = monadic(4.5) >> monad::later<double>([](double v) {return heavy_object(v); }) >> monad::do_evaluate();

		Assert::AreEqual(0, heavy_object::Cur);

		Ans.get().get().func();

		Assert::AreEqual(1, heavy_object::Cur);

		Ans.get().get().func();

		Assert::AreEqual(1, heavy_object::Cur);
	}
	TEST_METHOD(lazy_monad_fnp) {
		auto Ans = monadic(4.5) >> monad::later<double>(make_heavy_object) >> monad::do_evaluate();

		Assert::AreEqual(0, heavy_object::Cur);

		Ans.get().get().func();

		Assert::AreEqual(1, heavy_object::Cur);

		Ans.get().get().func();

		Assert::AreEqual(1, heavy_object::Cur);
	}
	TEST_METHOD(lazy_monad_lambda_chain) {
		auto Ans = monadic(4.5) >> monad::later<double>([](double v) {return heavy_object(v); }) >> [](const heavy_object& h) {return static_cast<int>(h.func()); } >> monad::do_evaluate();

		Assert::AreEqual(1, heavy_object::Cur);

		Assert::AreEqual<int>(-4,Ans.get());
	}

	};
}
