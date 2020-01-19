#include "stdafx.h"
#include "CppUnitTest.h"
#include <vector>
#include "../../../csv/csvbase.hpp"
using namespace Microsoft::VisualStudio::CppUnitTestFramework;
namespace Microsoft {
	namespace VisualStudio {
		namespace CppUnitTestFramework {
			template<> inline std::wstring ToString<hmLib::csv::encode_mode>(const hmLib::csv::encode_mode& t) {
				switch (t) {
				case hmLib::csv::encode_mode::none:
					return L"none";
				case hmLib::csv::encode_mode::simple:
					return L"simple";
				case hmLib::csv::encode_mode::full:
					return L"full";
				default:
					return L"???";
				}
			}
		}
	}
}
namespace hmLib {

	TEST_CLASS(test_csv_cell) {
	public:
		TEST_METHOD(csv_cell_status_raw) {
			{
				auto Traits = csv::csv_traits();
				std::string Str = "test";
				auto State =csv::cell_state(Str.begin(),Str.end(),Traits);
				Assert::AreEqual(csv::encode_mode::none, State.Mode);
				Assert::IsFalse(State.NeedEncode);
			} {
				auto Traits = csv::csv_traits();
				std::string Str = ",test";
				auto State = csv::cell_state(Str.begin(), Str.end(), Traits);
				Assert::AreEqual(csv::encode_mode::none, State.Mode);
				Assert::IsTrue(State.NeedEncode);
			} {
				auto Traits = csv::csv_traits();
				std::string Str = "te,st";
				auto State = csv::cell_state(Str.begin(), Str.end(), Traits);
				Assert::AreEqual(csv::encode_mode::none, State.Mode);
				Assert::IsTrue(State.NeedEncode);
			} {
				auto Traits = csv::csv_traits();
				std::string Str = "test,";
				auto State = csv::cell_state(Str.begin(), Str.end(), Traits);
				Assert::AreEqual(csv::encode_mode::none, State.Mode);
				Assert::IsTrue(State.NeedEncode);
			} {
				auto Traits = csv::csv_traits();
				std::string Str = "\ntest";
				auto State = csv::cell_state(Str.begin(), Str.end(), Traits);
				Assert::AreEqual(csv::encode_mode::none, State.Mode);
				Assert::IsTrue(State.NeedEncode);
			} {
				auto Traits = csv::csv_traits();
				std::string Str = "te\nst";
				auto State = csv::cell_state(Str.begin(), Str.end(), Traits);
				Assert::AreEqual(csv::encode_mode::none, State.Mode);
				Assert::IsTrue(State.NeedEncode);
			} {
				auto Traits = csv::csv_traits();
				std::string Str = "test\n";
				auto State = csv::cell_state(Str.begin(), Str.end(), Traits);
				Assert::AreEqual(csv::encode_mode::none, State.Mode);
				Assert::IsTrue(State.NeedEncode);
			} {
				auto Traits = csv::csv_traits();
				std::string Str = "\"test";
				auto State = csv::cell_state(Str.begin(), Str.end(), Traits);
				Assert::AreEqual(csv::encode_mode::none, State.Mode);
				Assert::IsTrue(State.NeedEncode);
			} {
				auto Traits = csv::csv_traits();
				std::string Str = "te\"st";
				auto State = csv::cell_state(Str.begin(), Str.end(), Traits);
				Assert::AreEqual(csv::encode_mode::none, State.Mode);
				Assert::IsTrue(State.NeedEncode);
			} {
				auto Traits = csv::csv_traits();
				std::string Str = "test\"";
				auto State = csv::cell_state(Str.begin(), Str.end(), Traits);
				Assert::AreEqual(csv::encode_mode::none, State.Mode);
				Assert::IsTrue(State.NeedEncode);
			}
		}
		TEST_METHOD(csv_cell_status_nonraw) {
			{
				auto Traits = csv::csv_traits();
				std::string Str = "\"test\"";
				auto State = csv::cell_state(Str.begin(), Str.end(), Traits);
				Assert::AreEqual(csv::encode_mode::simple, State.Mode);
				Assert::IsFalse(State.NeedEncode);
			} {
				auto Traits = csv::csv_traits();
				std::string Str = "\",test\"";
				auto State = csv::cell_state(Str.begin(), Str.end(), Traits);
				Assert::AreEqual(csv::encode_mode::simple, State.Mode);
				Assert::IsTrue(State.NeedEncode);
			} {
				auto Traits = csv::csv_traits();
				std::string Str = "\"te,st\"";
				auto State = csv::cell_state(Str.begin(), Str.end(), Traits);
				Assert::AreEqual(csv::encode_mode::simple, State.Mode);
				Assert::IsTrue(State.NeedEncode);
			} {
				auto Traits = csv::csv_traits();
				std::string Str = "\"test,\"";
				auto State = csv::cell_state(Str.begin(), Str.end(), Traits);
				Assert::AreEqual(csv::encode_mode::simple, State.Mode);
				Assert::IsTrue(State.NeedEncode);
			} {
				auto Traits = csv::csv_traits();
				std::string Str = "\"\ntest\"";
				auto State = csv::cell_state(Str.begin(), Str.end(), Traits);
				Assert::AreEqual(csv::encode_mode::simple, State.Mode);
				Assert::IsTrue(State.NeedEncode);
			} {
				auto Traits = csv::csv_traits();
				std::string Str = "\"te\nst\"";
				auto State = csv::cell_state(Str.begin(), Str.end(), Traits);
				Assert::AreEqual(csv::encode_mode::simple, State.Mode);
				Assert::IsTrue(State.NeedEncode);
			} {
				auto Traits = csv::csv_traits();
				std::string Str = "\"test\n\"";
				auto State = csv::cell_state(Str.begin(), Str.end(), Traits);
				Assert::AreEqual(csv::encode_mode::simple, State.Mode);
				Assert::IsTrue(State.NeedEncode);
			} {
				auto Traits = csv::csv_traits();
				std::string Str = "\"\"\"test\"";
				auto State = csv::cell_state(Str.begin(), Str.end(), Traits);
				Assert::AreEqual(csv::encode_mode::full, State.Mode);
				Assert::IsTrue(State.NeedEncode);
			} {
				auto Traits = csv::csv_traits();
				std::string Str = "\"te\"\"st\"";
				auto State = csv::cell_state(Str.begin(), Str.end(), Traits);
				Assert::AreEqual(csv::encode_mode::full, State.Mode);
				Assert::IsTrue(State.NeedEncode);
			} {
				auto Traits = csv::csv_traits();
				std::string Str = "\"test\"\"\"";
				auto State = csv::cell_state(Str.begin(), Str.end(), Traits);
				Assert::AreEqual(csv::encode_mode::full, State.Mode);
				Assert::IsTrue(State.NeedEncode);
			} {
				auto Traits = csv::csv_traits();
				std::string Str = "\"\"test\"";
				auto State = csv::cell_state(Str.begin(), Str.end(), Traits);
				Assert::AreEqual(csv::encode_mode::none, State.Mode);
				Assert::IsTrue(State.NeedEncode);
			} {
				auto Traits = csv::csv_traits();
				std::string Str = "\"te\"st\"";
				auto State = csv::cell_state(Str.begin(), Str.end(), Traits);
				Assert::AreEqual(csv::encode_mode::none, State.Mode);
				Assert::IsTrue(State.NeedEncode);
			} {
				auto Traits = csv::csv_traits();
				std::string Str = "\"test\"\"";
				auto State = csv::cell_state(Str.begin(), Str.end(), Traits);
				Assert::AreEqual(csv::encode_mode::none, State.Mode);
				Assert::IsTrue(State.NeedEncode);
			} {
				auto Traits = csv::csv_traits();
				std::string Str = "\"\"\"\"test\"";
				auto State = csv::cell_state(Str.begin(), Str.end(), Traits);
				Assert::AreEqual(csv::encode_mode::none, State.Mode);
				Assert::IsTrue(State.NeedEncode);
			} {
				auto Traits = csv::csv_traits();
				std::string Str = "\"te\"\"\"st\"";
				auto State = csv::cell_state(Str.begin(), Str.end(), Traits);
				Assert::AreEqual(csv::encode_mode::none, State.Mode);
				Assert::IsTrue(State.NeedEncode);
			} {
				auto Traits = csv::csv_traits();
				std::string Str = "\"test\"\"\"\"";
				auto State = csv::cell_state(Str.begin(), Str.end(), Traits);
				Assert::AreEqual(csv::encode_mode::none, State.Mode);
				Assert::IsTrue(State.NeedEncode);
			}
		}
		TEST_METHOD(csv_cell_status_raw_nonencode) {
			{
				auto Traits = csv::csv_traits();
				std::string Str = "test";
				auto State = csv::cell_state(Str.begin(), Str.end(), Traits, true);
				Assert::AreEqual(csv::encode_mode::none, State.Mode);
				Assert::IsFalse(State.NeedEncode);
			} {
				auto Traits = csv::csv_traits();
				std::string Str = ",test";
				auto State = csv::cell_state(Str.begin(), Str.end(), Traits, true);
				Assert::AreEqual(csv::encode_mode::none, State.Mode);
				Assert::IsTrue(State.NeedEncode);
			} {
				auto Traits = csv::csv_traits();
				std::string Str = "te,st";
				auto State = csv::cell_state(Str.begin(), Str.end(), Traits, true);
				Assert::AreEqual(csv::encode_mode::none, State.Mode);
				Assert::IsTrue(State.NeedEncode);
			} {
				auto Traits = csv::csv_traits();
				std::string Str = "test,";
				auto State = csv::cell_state(Str.begin(), Str.end(), Traits, true);
				Assert::AreEqual(csv::encode_mode::none, State.Mode);
				Assert::IsTrue(State.NeedEncode);
			} {
				auto Traits = csv::csv_traits();
				std::string Str = "\ntest";
				auto State = csv::cell_state(Str.begin(), Str.end(), Traits, true);
				Assert::AreEqual(csv::encode_mode::none, State.Mode);
				Assert::IsTrue(State.NeedEncode);
			} {
				auto Traits = csv::csv_traits();
				std::string Str = "te\nst";
				auto State = csv::cell_state(Str.begin(), Str.end(), Traits, true);
				Assert::AreEqual(csv::encode_mode::none, State.Mode);
				Assert::IsTrue(State.NeedEncode);
			} {
				auto Traits = csv::csv_traits();
				std::string Str = "test\n";
				auto State = csv::cell_state(Str.begin(), Str.end(), Traits, true);
				Assert::AreEqual(csv::encode_mode::none, State.Mode);
				Assert::IsTrue(State.NeedEncode);
			} {
				auto Traits = csv::csv_traits();
				std::string Str = "\"test";
				auto State = csv::cell_state(Str.begin(), Str.end(), Traits, true);
				Assert::AreEqual(csv::encode_mode::none, State.Mode);
				Assert::IsTrue(State.NeedEncode);
			} {
				auto Traits = csv::csv_traits();
				std::string Str = "te\"st";
				auto State = csv::cell_state(Str.begin(), Str.end(), Traits, true);
				Assert::AreEqual(csv::encode_mode::none, State.Mode);
				Assert::IsTrue(State.NeedEncode);
			} {
				auto Traits = csv::csv_traits();
				std::string Str = "test\"";
				auto State = csv::cell_state(Str.begin(), Str.end(), Traits, true);
				Assert::AreEqual(csv::encode_mode::none, State.Mode);
				Assert::IsTrue(State.NeedEncode);
			}
		}
		TEST_METHOD(csv_cell_status_nonraw_nonencode) {
			{
				auto Traits = csv::csv_traits();
				std::string Str = "\"test\"";
				auto State = csv::cell_state(Str.begin(), Str.end(), Traits, true);
				Assert::AreEqual(csv::encode_mode::none, State.Mode);
				Assert::IsTrue(State.NeedEncode);
			} {
				auto Traits = csv::csv_traits();
				std::string Str = "\",test\"";
				auto State = csv::cell_state(Str.begin(), Str.end(), Traits, true);
				Assert::AreEqual(csv::encode_mode::none, State.Mode);
				Assert::IsTrue(State.NeedEncode);
			} {
				auto Traits = csv::csv_traits();
				std::string Str = "\"te,st\"";
				auto State = csv::cell_state(Str.begin(), Str.end(), Traits, true);
				Assert::AreEqual(csv::encode_mode::none, State.Mode);
				Assert::IsTrue(State.NeedEncode);
			} {
				auto Traits = csv::csv_traits();
				std::string Str = "\"test,\"";
				auto State = csv::cell_state(Str.begin(), Str.end(), Traits, true);
				Assert::AreEqual(csv::encode_mode::none, State.Mode);
				Assert::IsTrue(State.NeedEncode);
			} {
				auto Traits = csv::csv_traits();
				std::string Str = "\"\ntest\"";
				auto State = csv::cell_state(Str.begin(), Str.end(), Traits, true);
				Assert::AreEqual(csv::encode_mode::none, State.Mode);
				Assert::IsTrue(State.NeedEncode);
			} {
				auto Traits = csv::csv_traits();
				std::string Str = "\"te\nst\"";
				auto State = csv::cell_state(Str.begin(), Str.end(), Traits, true);
				Assert::AreEqual(csv::encode_mode::none, State.Mode);
				Assert::IsTrue(State.NeedEncode);
			} {
				auto Traits = csv::csv_traits();
				std::string Str = "\"test\n\"";
				auto State = csv::cell_state(Str.begin(), Str.end(), Traits, true);
				Assert::AreEqual(csv::encode_mode::none, State.Mode);
				Assert::IsTrue(State.NeedEncode);
			} {
				auto Traits = csv::csv_traits();
				std::string Str = "\"\"\"test\"";
				auto State = csv::cell_state(Str.begin(), Str.end(), Traits, true);
				Assert::AreEqual(csv::encode_mode::none, State.Mode);
				Assert::IsTrue(State.NeedEncode);
			} {
				auto Traits = csv::csv_traits();
				std::string Str = "\"te\"\"st\"";
				auto State = csv::cell_state(Str.begin(), Str.end(), Traits, true);
				Assert::AreEqual(csv::encode_mode::none, State.Mode);
				Assert::IsTrue(State.NeedEncode);
			} {
				auto Traits = csv::csv_traits();
				std::string Str = "\"test\"\"\"";
				auto State = csv::cell_state(Str.begin(), Str.end(), Traits, true);
				Assert::AreEqual(csv::encode_mode::none, State.Mode);
				Assert::IsTrue(State.NeedEncode);
			} {
				auto Traits = csv::csv_traits();
				std::string Str = "\"\"test\"";
				auto State = csv::cell_state(Str.begin(), Str.end(), Traits, true);
				Assert::AreEqual(csv::encode_mode::none, State.Mode);
				Assert::IsTrue(State.NeedEncode);
			} {
				auto Traits = csv::csv_traits();
				std::string Str = "\"te\"st\"";
				auto State = csv::cell_state(Str.begin(), Str.end(), Traits, true);
				Assert::AreEqual(csv::encode_mode::none, State.Mode);
				Assert::IsTrue(State.NeedEncode);
			} {
				auto Traits = csv::csv_traits();
				std::string Str = "\"test\"\"";
				auto State = csv::cell_state(Str.begin(), Str.end(), Traits, true);
				Assert::AreEqual(csv::encode_mode::none, State.Mode);
				Assert::IsTrue(State.NeedEncode);
			} {
				auto Traits = csv::csv_traits();
				std::string Str = "\"\"\"\"test\"";
				auto State = csv::cell_state(Str.begin(), Str.end(), Traits, true);
				Assert::AreEqual(csv::encode_mode::none, State.Mode);
				Assert::IsTrue(State.NeedEncode);
			} {
				auto Traits = csv::csv_traits();
				std::string Str = "\"te\"\"\"st\"";
				auto State = csv::cell_state(Str.begin(), Str.end(), Traits, true);
				Assert::AreEqual(csv::encode_mode::none, State.Mode);
				Assert::IsTrue(State.NeedEncode);
			} {
				auto Traits = csv::csv_traits();
				std::string Str = "\"test\"\"\"\"";
				auto State = csv::cell_state(Str.begin(), Str.end(), Traits, true);
				Assert::AreEqual(csv::encode_mode::none, State.Mode);
				Assert::IsTrue(State.NeedEncode);
			}
		}
		TEST_METHOD(csv_cell_decode_out) {
			auto Traits = csv::csv_traits();
			{
				std::string Str = "test";
				std::string Sout;
				csv::cell_decode(Str.begin(), Str.end(), std::back_inserter(Sout), Traits);
				Assert::AreEqual("test", Sout.c_str());
			}{
				std::string Str = "\"test\"";
				std::string Sout;
				csv::cell_decode(Str.begin(), Str.end(), std::back_inserter(Sout), Traits);
				Assert::AreEqual("test", Sout.c_str());
			}{
				std::string Str = "\",test\"";
				std::string Sout;
				csv::cell_decode(Str.begin(), Str.end(), std::back_inserter(Sout), Traits);
				Assert::AreEqual(",test", Sout.c_str());
			} {
				std::string Str = "\"te,st\"";
				std::string Sout;
				csv::cell_decode(Str.begin(), Str.end(), std::back_inserter(Sout), Traits);
				Assert::AreEqual("te,st", Sout.c_str());
			} {
				std::string Str = "\"test,\"";
				std::string Sout;
				csv::cell_decode(Str.begin(), Str.end(), std::back_inserter(Sout), Traits);
				Assert::AreEqual("test,", Sout.c_str());
			} {
				std::string Str = "\"\ntest\"";
				std::string Sout;
				csv::cell_decode(Str.begin(), Str.end(), std::back_inserter(Sout), Traits);
				Assert::AreEqual("\ntest", Sout.c_str());
			} {
				std::string Str = "\"te\nst\"";
				std::string Sout;
				csv::cell_decode(Str.begin(), Str.end(), std::back_inserter(Sout), Traits);
				Assert::AreEqual("te\nst", Sout.c_str());
			} {
				std::string Str = "\"test\n\"";
				std::string Sout;
				csv::cell_decode(Str.begin(), Str.end(), std::back_inserter(Sout), Traits);
				Assert::AreEqual("test\n", Sout.c_str());
			} {
				std::string Str = "\"\"\"test\"";
				std::string Sout;
				csv::cell_decode(Str.begin(), Str.end(), std::back_inserter(Sout), Traits);
				Assert::AreEqual("\"test", Sout.c_str());
			} {
				std::string Str = "\"te\"\"st\"";
				std::string Sout;
				csv::cell_decode(Str.begin(), Str.end(), std::back_inserter(Sout), Traits);
				Assert::AreEqual("te\"st", Sout.c_str());
			} {
				std::string Str = "\"test\"\"\"";
				std::string Sout;
				csv::cell_decode(Str.begin(), Str.end(), std::back_inserter(Sout), Traits);
				Assert::AreEqual("test\"", Sout.c_str());
			} {
				std::string Str = "\"\"\"\"\"\"\"\"";
				std::string Sout;
				csv::cell_decode(Str.begin(), Str.end(), std::back_inserter(Sout), Traits);
				Assert::AreEqual("\"\"\"", Sout.c_str());
			}
		}
		TEST_METHOD(csv_cell_decode_wrt) {
			auto Traits = csv::csv_traits();
			{
				std::string Str = "test";
				auto Range = csv::cell_decode(Str.begin(), Str.end(),Traits);
				std::string Sout(Range.begin(), Range.end());
				Assert::AreEqual("test", Sout.c_str());
			} {
				std::string Str = "\"test\"";
				auto Range = csv::cell_decode(Str.begin(), Str.end(), Traits);
				std::string Sout(Range.begin(), Range.end());
				Assert::AreEqual("test", Sout.c_str());
			} {
				std::string Str = "\",test\"";
				auto Range = csv::cell_decode(Str.begin(), Str.end(), Traits);
				std::string Sout(Range.begin(), Range.end());
				Assert::AreEqual(",test", Sout.c_str());
			} {
				std::string Str = "\"te,st\"";
				auto Range = csv::cell_decode(Str.begin(), Str.end(), Traits);
				std::string Sout(Range.begin(), Range.end());
				Assert::AreEqual("te,st", Sout.c_str());
			} {
				std::string Str = "\"test,\"";
				auto Range = csv::cell_decode(Str.begin(), Str.end(), Traits);
				std::string Sout(Range.begin(), Range.end());
				Assert::AreEqual("test,", Sout.c_str());
			} {
				std::string Str = "\"\ntest\"";
				auto Range = csv::cell_decode(Str.begin(), Str.end(), Traits);
				std::string Sout(Range.begin(), Range.end());
				Assert::AreEqual("\ntest", Sout.c_str());
			} {
				std::string Str = "\"te\nst\"";
				auto Range = csv::cell_decode(Str.begin(), Str.end(), Traits);
				std::string Sout(Range.begin(), Range.end());
				Assert::AreEqual("te\nst", Sout.c_str());
			} {
				std::string Str = "\"test\n\"";
				auto Range = csv::cell_decode(Str.begin(), Str.end(), Traits);
				std::string Sout(Range.begin(), Range.end());
				Assert::AreEqual("test\n", Sout.c_str());
			} {
				std::string Str = "\"\"\"test\"";
				auto Range = csv::cell_decode(Str.begin(), Str.end(), Traits);
				std::string Sout(Range.begin(), Range.end());
				Assert::AreEqual("\"test", Sout.c_str());
			} {
				std::string Str = "\"te\"\"st\"";
				auto Range = csv::cell_decode(Str.begin(), Str.end(), Traits);
				std::string Sout(Range.begin(), Range.end());
				Assert::AreEqual("te\"st", Sout.c_str());
			} {
				std::string Str = "\"test\"\"\"";
				auto Range = csv::cell_decode(Str.begin(), Str.end(), Traits);
				std::string Sout(Range.begin(), Range.end());
				Assert::AreEqual("test\"", Sout.c_str());
			} {
				std::string Str = "\"\"\"\"\"\"\"\"";
				auto Range = csv::cell_decode(Str.begin(), Str.end(), Traits);
				std::string Sout(Range.begin(), Range.end());
				Assert::AreEqual("\"\"\"", Sout.c_str());
			}

		}
		TEST_METHOD(csv_cell_decode_out_hint) {
			auto Traits = csv::csv_traits();
			{
				std::string Str = "test";
				std::string Sout;
				csv::cell_decode(Str.begin(), Str.end(), std::back_inserter(Sout), Traits, csv::cell_state(Str.begin(),Str.end(),Traits));
				Assert::AreEqual("test", Sout.c_str());
			} {
				std::string Str = "\"test\"";
				std::string Sout;
				csv::cell_decode(Str.begin(), Str.end(), std::back_inserter(Sout), Traits, csv::cell_state(Str.begin(), Str.end(), Traits));
				Assert::AreEqual("test", Sout.c_str());
			} {
				std::string Str = "\",test\"";
				std::string Sout;
				csv::cell_decode(Str.begin(), Str.end(), std::back_inserter(Sout), Traits, csv::cell_state(Str.begin(), Str.end(), Traits));
				Assert::AreEqual(",test", Sout.c_str());
			} {
				std::string Str = "\"te,st\"";
				std::string Sout;
				csv::cell_decode(Str.begin(), Str.end(), std::back_inserter(Sout), Traits, csv::cell_state(Str.begin(), Str.end(), Traits));
				Assert::AreEqual("te,st", Sout.c_str());
			} {
				std::string Str = "\"test,\"";
				std::string Sout;
				csv::cell_decode(Str.begin(), Str.end(), std::back_inserter(Sout), Traits, csv::cell_state(Str.begin(), Str.end(), Traits));
				Assert::AreEqual("test,", Sout.c_str());
			} {
				std::string Str = "\"\ntest\"";
				std::string Sout;
				csv::cell_decode(Str.begin(), Str.end(), std::back_inserter(Sout), Traits, csv::cell_state(Str.begin(), Str.end(), Traits));
				Assert::AreEqual("\ntest", Sout.c_str());
			} {
				std::string Str = "\"te\nst\"";
				std::string Sout;
				csv::cell_decode(Str.begin(), Str.end(), std::back_inserter(Sout), Traits, csv::cell_state(Str.begin(), Str.end(), Traits));
				Assert::AreEqual("te\nst", Sout.c_str());
			} {
				std::string Str = "\"test\n\"";
				std::string Sout;
				csv::cell_decode(Str.begin(), Str.end(), std::back_inserter(Sout), Traits, csv::cell_state(Str.begin(), Str.end(), Traits));
				Assert::AreEqual("test\n", Sout.c_str());
			} {
				std::string Str = "\"\"\"test\"";
				std::string Sout;
				csv::cell_decode(Str.begin(), Str.end(), std::back_inserter(Sout), Traits, csv::cell_state(Str.begin(), Str.end(), Traits));
				Assert::AreEqual("\"test", Sout.c_str());
			} {
				std::string Str = "\"te\"\"st\"";
				std::string Sout;
				csv::cell_decode(Str.begin(), Str.end(), std::back_inserter(Sout), Traits, csv::cell_state(Str.begin(), Str.end(), Traits));
				Assert::AreEqual("te\"st", Sout.c_str());
			} {
				std::string Str = "\"test\"\"\"";
				std::string Sout;
				csv::cell_decode(Str.begin(), Str.end(), std::back_inserter(Sout), Traits, csv::cell_state(Str.begin(), Str.end(), Traits));
				Assert::AreEqual("test\"", Sout.c_str());
			} {
				std::string Str = "\"\"\"\"\"\"\"\"";
				std::string Sout;
				csv::cell_decode(Str.begin(), Str.end(), std::back_inserter(Sout), Traits, csv::cell_state(Str.begin(), Str.end(), Traits));
				Assert::AreEqual("\"\"\"", Sout.c_str());
			}
		}
		TEST_METHOD(csv_cell_decode_wrt_hint) {
			auto Traits = csv::csv_traits();
			{
				std::string Str = "test";
				auto Range = csv::cell_decode(Str.begin(), Str.end(), Traits, csv::cell_state(Str.begin(), Str.end(), Traits));
				std::string Sout(Range.begin(), Range.end());
				Assert::AreEqual("test", Sout.c_str());
			} {
				std::string Str = "\"test\"";
				auto Range = csv::cell_decode(Str.begin(), Str.end(), Traits, csv::cell_state(Str.begin(), Str.end(), Traits));
				std::string Sout(Range.begin(), Range.end());
				Assert::AreEqual("test", Sout.c_str());
			} {
				std::string Str = "\",test\"";
				auto Range = csv::cell_decode(Str.begin(), Str.end(), Traits, csv::cell_state(Str.begin(), Str.end(), Traits));
				std::string Sout(Range.begin(), Range.end());
				Assert::AreEqual(",test", Sout.c_str());
			} {
				std::string Str = "\"te,st\"";
				auto Range = csv::cell_decode(Str.begin(), Str.end(), Traits, csv::cell_state(Str.begin(), Str.end(), Traits));
				std::string Sout(Range.begin(), Range.end());
				Assert::AreEqual("te,st", Sout.c_str());
			} {
				std::string Str = "\"test,\"";
				auto Range = csv::cell_decode(Str.begin(), Str.end(), Traits, csv::cell_state(Str.begin(), Str.end(), Traits));
				std::string Sout(Range.begin(), Range.end());
				Assert::AreEqual("test,", Sout.c_str());
			} {
				std::string Str = "\"\ntest\"";
				auto Range = csv::cell_decode(Str.begin(), Str.end(), Traits, csv::cell_state(Str.begin(), Str.end(), Traits));
				std::string Sout(Range.begin(), Range.end());
				Assert::AreEqual("\ntest", Sout.c_str());
			} {
				std::string Str = "\"te\nst\"";
				auto Range = csv::cell_decode(Str.begin(), Str.end(), Traits, csv::cell_state(Str.begin(), Str.end(), Traits));
				std::string Sout(Range.begin(), Range.end());
				Assert::AreEqual("te\nst", Sout.c_str());
			} {
				std::string Str = "\"test\n\"";
				auto Range = csv::cell_decode(Str.begin(), Str.end(), Traits, csv::cell_state(Str.begin(), Str.end(), Traits));
				std::string Sout(Range.begin(), Range.end());
				Assert::AreEqual("test\n", Sout.c_str());
			} {
				std::string Str = "\"\"\"test\"";
				auto Range = csv::cell_decode(Str.begin(), Str.end(), Traits, csv::cell_state(Str.begin(), Str.end(), Traits));
				std::string Sout(Range.begin(), Range.end());
				Assert::AreEqual("\"test", Sout.c_str());
			} {
				std::string Str = "\"te\"\"st\"";
				auto Range = csv::cell_decode(Str.begin(), Str.end(), Traits, csv::cell_state(Str.begin(), Str.end(), Traits));
				std::string Sout(Range.begin(), Range.end());
				Assert::AreEqual("te\"st", Sout.c_str());
			} {
				std::string Str = "\"test\"\"\"";
				auto Range = csv::cell_decode(Str.begin(), Str.end(), Traits, csv::cell_state(Str.begin(), Str.end(), Traits));
				std::string Sout(Range.begin(), Range.end());
				Assert::AreEqual("test\"", Sout.c_str());
			} {
				std::string Str = "\"\"\"\"\"\"\"\"";
				auto Range = csv::cell_decode(Str.begin(), Str.end(), Traits, csv::cell_state(Str.begin(), Str.end(), Traits));
				std::string Sout(Range.begin(), Range.end());
				Assert::AreEqual("\"\"\"", Sout.c_str());
			}

		}
		TEST_METHOD(csv_cell_encode_decode) {
			auto Traits = csv::csv_traits();
			{
				std::string Str = "test";
				std::string Sen;
				csv::cell_encode(Str.begin(), Str.end(), std::back_inserter(Sen), Traits);
				auto Range = csv::cell_decode(Sen.begin(), Sen.end(), Traits);
				std::string Sout(Range.begin(), Range.end());
				Assert::AreEqual(Str, Sout);
			} {
				std::string Str = "te\"st";
				std::string Sen;
				csv::cell_encode(Str.begin(), Str.end(), std::back_inserter(Sen), Traits);
				auto Range = csv::cell_decode(Sen.begin(), Sen.end(), Traits);
				std::string Sout(Range.begin(), Range.end());
				Assert::AreEqual(Str, Sout);
			} {
				std::string Str = "te\"\"st";
				std::string Sen;
				csv::cell_encode(Str.begin(), Str.end(), std::back_inserter(Sen), Traits);
				auto Range = csv::cell_decode(Sen.begin(), Sen.end(), Traits);
				std::string Sout(Range.begin(), Range.end());
				Assert::AreEqual(Str, Sout);
			} {
				std::string Str = "\"test";
				std::string Sen;
				csv::cell_encode(Str.begin(), Str.end(), std::back_inserter(Sen), Traits);
				auto Range = csv::cell_decode(Sen.begin(), Sen.end(), Traits);
				std::string Sout(Range.begin(), Range.end());
				Assert::AreEqual(Str, Sout);
			} {
				std::string Str = "\"\"test";
				std::string Sen;
				csv::cell_encode(Str.begin(), Str.end(), std::back_inserter(Sen), Traits);
				auto Range = csv::cell_decode(Sen.begin(), Sen.end(), Traits);
				std::string Sout(Range.begin(), Range.end());
				Assert::AreEqual(Str, Sout);
			} {
				std::string Str = "test\"";
				std::string Sen;
				csv::cell_encode(Str.begin(), Str.end(), std::back_inserter(Sen), Traits);
				auto Range = csv::cell_decode(Sen.begin(), Sen.end(), Traits);
				std::string Sout(Range.begin(), Range.end());
				Assert::AreEqual(Str, Sout);
			} {
				std::string Str = "test\"\"";
				std::string Sen;
				csv::cell_encode(Str.begin(), Str.end(), std::back_inserter(Sen), Traits);
				auto Range = csv::cell_decode(Sen.begin(), Sen.end(), Traits);
				std::string Sout(Range.begin(), Range.end());
				Assert::AreEqual(Str, Sout);
			} {
				std::string Str = "\"\"test\"\"";
				std::string Sen;
				csv::cell_encode(Str.begin(), Str.end(), std::back_inserter(Sen), Traits);
				auto Range = csv::cell_decode(Sen.begin(), Sen.end(), Traits);
				std::string Sout(Range.begin(), Range.end());
				Assert::AreEqual(Str, Sout);
			} {
				std::string Str = "test,,,";
				std::string Sen;
				csv::cell_encode(Str.begin(), Str.end(), std::back_inserter(Sen), Traits);
				auto Range = csv::cell_decode(Sen.begin(), Sen.end(), Traits);
				std::string Sout(Range.begin(), Range.end());
				Assert::AreEqual(Str, Sout);
			} {
				std::string Str = "\n\"test\n\"";
				std::string Sen;
				csv::cell_encode(Str.begin(), Str.end(), std::back_inserter(Sen), Traits);
				auto Range = csv::cell_decode(Sen.begin(), Sen.end(), Traits);
				std::string Sout(Range.begin(), Range.end());
				Assert::AreEqual(Str, Sout);
			}
		}
		TEST_METHOD(csv_cell_next) {
			auto Traits = csv::csv_traits();
			{
				std::string Str = "test,test,test";
				std::string Sen;
				auto [Itr, State] = csv::cell_next(Str.begin(), Str.end(), Traits);
				Assert::IsTrue(Str.begin() + 4 == Itr);
				Assert::AreEqual(csv::encode_mode::none, State.Mode);
				Assert::AreEqual(false, State.NeedEncode);
			} {
				std::string Str = "test\ntest,test";
				std::string Sen;
				auto [Itr, State] = csv::cell_next(Str.begin(), Str.end(), Traits);
				Assert::IsTrue(Str.begin() + 4 == Itr);
				Assert::AreEqual(csv::encode_mode::none, State.Mode);
				Assert::AreEqual(false, State.NeedEncode);
			} {
				std::string Str = "testtesttest";
				std::string Sen;
				auto [Itr, State] = csv::cell_next(Str.begin(), Str.end(), Traits);
				Assert::IsTrue(Str.begin() + 12 == Itr);
				Assert::AreEqual(csv::encode_mode::none, State.Mode);
				Assert::AreEqual(false, State.NeedEncode);
			} {
				std::string Str = "\"test\",\"test\"";
				std::string Sen;
				auto [Itr, State] = csv::cell_next(Str.begin(), Str.end(), Traits);
				Assert::IsTrue(Str.begin() + 6 == Itr);
				Assert::AreEqual(csv::encode_mode::simple, State.Mode);
				Assert::AreEqual(false, State.NeedEncode);
			} {
				std::string Str = "\"te,t\",\"test\"";
				std::string Sen;
				auto [Itr, State] = csv::cell_next(Str.begin(), Str.end(), Traits);
				Assert::IsTrue(Str.begin() + 6 == Itr);
				Assert::AreEqual(csv::encode_mode::simple, State.Mode);
				Assert::AreEqual(true, State.NeedEncode);
			} {
				std::string Str = "\"\ne,t\",\"test\"";
				std::string Sen;
				auto [Itr, State] = csv::cell_next(Str.begin(), Str.end(), Traits);
				Assert::IsTrue(Str.begin() + 6 == Itr);
				Assert::AreEqual(csv::encode_mode::simple, State.Mode);
				Assert::AreEqual(true, State.NeedEncode);
			} {
				std::string Str = "\"\"\"test\",\"test\"";
				std::string Sen;
				auto [Itr, State] = csv::cell_next(Str.begin(), Str.end(), Traits);
				Assert::IsTrue(Str.begin() + 8 == Itr);
				Assert::AreEqual(csv::encode_mode::full, State.Mode);
				Assert::AreEqual(true, State.NeedEncode);
			} {
				std::string Str = "\"te\"\"st\",\"test\"";
				std::string Sen;
				auto [Itr, State] = csv::cell_next(Str.begin(), Str.end(), Traits);
				Assert::IsTrue(Str.begin() + 8 == Itr);
				Assert::AreEqual(csv::encode_mode::full, State.Mode);
				Assert::AreEqual(true, State.NeedEncode);
			} {
				std::string Str = "\"test\"\"\",\"test\"";
				std::string Sen;
				auto [Itr, State] = csv::cell_next(Str.begin(), Str.end(), Traits);
				Assert::IsTrue(Str.begin() + 8 == Itr);
				Assert::AreEqual(csv::encode_mode::full, State.Mode);
				Assert::AreEqual(true, State.NeedEncode);
			} {
				std::string Str = "\"test\"\",\nte\nst\"";
				std::string Sen;
				auto [Itr, State] = csv::cell_next(Str.begin(), Str.end(), Traits);
				Assert::IsTrue(Str.end() == Itr);
				Assert::AreEqual(csv::encode_mode::full, State.Mode);
				Assert::AreEqual(true, State.NeedEncode);
			} {
				std::string Str = "\"test\"\",\nte\nst";
				std::string Sen;
				auto [Itr, State] = csv::cell_next(Str.begin(), Str.end(), Traits);
				Assert::IsTrue(Str.end() == Itr);
				Assert::AreEqual(csv::encode_mode::none, State.Mode);
				Assert::AreEqual(true, State.NeedEncode);
			}
		}
	};
}
