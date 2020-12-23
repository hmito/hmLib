#ifndef HMLIB_RECURSIVE_OBSERVER_HISTOGRAMOBSERVER_INC
#define HMLIB_RECURSIVE_OBSERVER_HISTOGRAMOBSERVER_INC 100
#
#include<ostream>
#include<string>
namespace hmLib {
	namespace recursive {
		struct const_histgram_stream_observer {
		private:
			std::ostream& out;
			double lower;
			double upper;
			unsigned int hsize;
			std::string delm;
			std::string end;
		public:
			const_histgram_stream_observer(std::ostream& out_, double lower_, double upper_, unsigned int hsize_, std::string delm_ = ", ", std::string end_ = "\n")
				: out(out_)
				, lower(lower_)
				, upper(upper_)
				, hsize(hsize_)
				, delm(delm_)
				, end(end_) {
			}
			template<typename state_type>
			void operator()(const state_type& s, unsigned int n) {
				if (n % 10 == 0) {
					out << n;

					auto MinMax = std::minmax_element(s.begin(), s.end());
					auto Mergin = (*(MinMax.second) - *(MinMax.first)) / hsize * 1e-6;
					auto Hist = hmLib::make_histgram(lower, upper, hsize);
					//auto Hist = hmLib::make_histgram(0.0, 4.0, 100);
					Hist(s.begin(), s.end());
					out << "," << Hist.underflow();
					for (auto v : Hist) {
						out << "," << v;
					}
					out << "," << Hist.overflow();
					out << std::endl;
				}
			}
		};
		struct adaptive_histgram_stream_observer {
		private:
			std::ostream& out;
			unsigned int hsize;
			std::string delm;
			std::string end;
		public:
			adaptive_histgram_stream_observer(std::ostream& out_, unsigned int hsize_, std::string delm_ = ", ", std::string end_ = "\n")
				: out(out_)
				, hsize(hsize_)
				, delm(delm_)
				, end(end_) {
			}
			template<typename state_type>
			void operator()(const state_type& s, unsigned int n) {
				if (n % 10 == 0) {
					out << n;

					auto MinMax = std::minmax_element(s.begin(), s.end());
					auto Mergin = (*(MinMax.second) - *(MinMax.first)) / hsize * 1e-6;
					auto Hist = hmLib::make_histgram(*(MinMax.first), *(MinMax.second) + Mergin, hsize);
					//auto Hist = hmLib::make_histgram(0.0, 4.0, 100);
					Hist(s.begin(), s.end());
					out << "," << Hist.lower() << "," << Hist.upper();
					for (auto v : Hist) {
						out << "," << v;
					}
					out << std::endl;
				}
			}
		};
	}
}
#
#endif
