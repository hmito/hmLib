#include "stdafx.h"
#include "CppUnitTest.h"
#include<vector>
#include<hmLib/resource.hpp>
using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace hmLib{		
	TEST_CLASS(test_resource){
	private:
		struct releaser{
			static std::vector<int> Released;
			void operator()(int value){ Released.push_back(value); }
		};
	public:
		TEST_METHOD_INITIALIZE(start_up){
			releaser::Released.clear();
		}
		TEST_METHOD_CLEANUP(clean_up){
			releaser::Released.clear();
		}
		TEST_METHOD(construct){
			unique_resource<int, releaser> Resource;
			Assert::IsFalse(static_cast<bool>(Resource));
		}
		TEST_METHOD(construct_and_destruct){
			{
				unique_resource<int, releaser> Resource(10);
				Assert::IsTrue(static_cast<bool>(Resource));
			}
			Assert::AreEqual(1u,releaser::Released.size());
			Assert::AreEqual(10, releaser::Released.back());
		}
		TEST_METHOD(reset){
			unique_resource<int, releaser> Resource(10);
			Resource.reset();

			Assert::IsFalse(static_cast<bool>(Resource));
			Assert::AreEqual(1u, releaser::Released.size());
			Assert::AreEqual(10, releaser::Released.back());
		}
		TEST_METHOD(get_and_release){
			{
				unique_resource<int, releaser> Resource(10);

				Assert::AreEqual(10, Resource.get());
				Assert::IsTrue(static_cast<bool>(Resource));
				Assert::AreEqual(0u, releaser::Released.size());

				Assert::AreEqual(10, Resource.release());
				Assert::IsFalse(static_cast<bool>(Resource));
				Assert::AreEqual(0u, releaser::Released.size());
			}
			Assert::AreEqual(0u, releaser::Released.size());
		}
		TEST_METHOD(move){
			unique_resource<int, releaser> Resource;

			{
				unique_resource<int, releaser> MResource(10);
				Resource = std::move(MResource);
				Assert::IsTrue(static_cast<bool>(Resource));
				Assert::IsFalse(static_cast<bool>(MResource));
			}
			Assert::AreEqual(10, Resource.get());
			Assert::IsTrue(static_cast<bool>(Resource));
			Assert::AreEqual(0u, releaser::Released.size());
		}
		TEST_METHOD(use_make_resource){
			auto Resource = make_resource(40, releaser());

			{
				unique_resource<int, releaser> MResource(10);
				Resource = std::move(MResource);
				Assert::IsTrue(static_cast<bool>(Resource));
			}
			Assert::AreEqual(10, Resource.get());
			Assert::IsTrue(static_cast<bool>(Resource));
			Assert::AreEqual(1u, releaser::Released.size());
		}
	};

	std::vector<int> test_resource::releaser::Released;
}