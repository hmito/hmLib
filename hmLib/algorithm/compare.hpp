#ifndef HMLIB_ALGORITHM_COMPARE_INC
#define HMLIB_ALGORITHM_COMPARE_INC 100
#
/*===algorithm::compare===
大小関係のアルゴリズムを提供

algorithm::compare v1_00/130328 hmIto
	algorthmから分離
*/
#include<algorithm>

namespace hmLib{
	namespace algorithm{
		//2要素のsort
		template<class T>
		inline void sort_swap(T& val1,T& val2){if(val1>val2)swap(val1,val2);}
		//3要素のsort
		template<class T>
		inline void sort_swap(T& val1,T& val2,T& val3){
			if(val1>val2)swap(val1,val2);
			if(val2>val3){
				swap(val2,val3);
				if(val1>val2)swap(val1,val2);
			}
		}
		//3要素のmedian
		template<class T>
		inline T med(T val1,T val2,T val3){
			if(val1>val2){
				if(val2>val3)return val2;
				else if(val1>val3)return val3;
				else return val1;
			}else{
				if(val1>val3)return val1;
				else if(val2>val3)return val3;
				else return val2;
			}
		}
		//3要素のmin
		template<class T>
		inline T min(T val1,T val2,T val3){return std::min(val1,std::min(val2,val3));}
		//3要素のmax
		template<class T>
		inline T max(T val1,T val2,T val3){return std::max(val1,std::max(val2,val3));}
		//負にならない余り
		template<class T>
		inline T positive_mod(T num,T divisor){
			if(num>=0)return num%divisor;
			else return divisor-1-((-1-num)%divisor);
		}
	}
}
#
#endif
