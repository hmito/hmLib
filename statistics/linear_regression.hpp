#ifndef HMLIB_STATISTICS_LINEARREGRESSION_INC
#define HMLIB_STATISTICS_LINEARREGRESSION_INC 100
#
#include<utility>
#include<iterator>
namespace hmLib{
	namespace statistics{
		template<typename xiterator,typename yiterator>
		std::pair<double,double> linaer_regression(xiterator xbeg, xiterator xend,yiterator ybeg){
			auto n = std::distance(xbeg,xend);

			double sum_x = 0;
			double sum_y = 0;
			double sum_xx = 0;
			double sum_xy = 0;

			for(; xbeg!=xend; ++xbeg, ++ybeg){
				sum_x += *xbeg;
				sum_y += *ybeg;
				sum_xx += *xbeg * *xbeg;
				sum_xy += *xbeg * *ybeg;
			}

			return std::make_pair(
				(sum_xy * n - sum_x * sum_y) / (sum_xx * n - sum_x * sum_x),
				(sum_xx * sum_y - sum_xy * sum_x) / (sum_xx * n - sum_x * sum_x)
			);
		}
	}
}
#
#endif
