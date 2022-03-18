#ifndef HMLIB_ODEINT_OBSERVER_HISTOGRAMSTREAMOBSERVER_INC
#define HMLIB_ODEINT_OBSERVER_HISTOGRAMSTREAMOBSERVER_INC 100
#
#include<ostream>
#include<string>
#include<algorithm>
#include"../../math/histgram.hpp"
namespace hmLib {
	namespace odeint {
		struct histgram_stream_observer {
		private:
			std::ostream& out;
			double lower;
			double upper;
			unsigned int hsize;
			std::string delm;
			std::string end;
		public:
			histgram_stream_observer(std::ostream& out_, double lower_, double upper_, unsigned int hsize_, std::string delm_ = ", ", std::string end_ = "\n")
				: out(out_)
				, lower(lower_)
				, upper(upper_)
				, hsize(hsize_)
				, delm(delm_)
				, end(end_) {
			}
			template<typename state_type,typename time_type>
			void operator()(const state_type& s, time_type t) {
				if (n % 10 == 0) {
					out << t;

					auto Hist = hmLib::make_histgram(lower, upper, hsize);
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
			template<typename state_type,typename time_type>
			void operator()(const state_type& s, time_type t) {
				if (n % 10 == 0) {
					out << t;

					auto MinMax = std::minmax_element(s.begin(), s.end());
					auto Mergin = (*(MinMax.second) - *(MinMax.first)) / hsize * 1e-6;
					auto Hist = hmLib::make_histgram(*(MinMax.first), *(MinMax.second) + Mergin, hsize);
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
