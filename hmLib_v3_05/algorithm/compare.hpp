#ifndef HMLIB_ALGORITHM_COMPARE_INC
#define HMLIB_ALGORITHM_COMPARE_INC 100
/*===algorithm::compare===
‘å¬ŠÖŒW‚ÌƒAƒ‹ƒSƒŠƒYƒ€‚ğ’ñ‹Ÿ

algorithm::compare v1_00/130328 hmIto
	algorthm‚©‚ç•ª—£
*/
#include<algorithm>

namespace hmLib{
	namespace algorithm{
		//2—v‘f‚Ìsort
		template<class T>
		inline void sort_swap(T& val1,T& val2){if(val1>val2)swap(val1,val2);}
		//3—v‘f‚Ìsort
		template<class T>
		inline void sort_swap(T& val1,T& val2,T& val3){
			if(val1>val2)swap(val1,val2);
			if(val2>val3){
				swap(val2,val3);
				if(val1>val2)swap(val1,val2);
			}
		}
		//3—v‘f‚Ìmedian
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
		//3—v‘f‚Ìmin
		template<class T>
		inline T min(T val1,T val2,T val3){return std::min(val1,std::min(val2,val3));}
		//3—v‘f‚Ìmax
		template<class T>
		inline T max(T val1,T val2,T val3){return std::max(val1,std::max(val2,val3));}
		//•‰‚É‚È‚ç‚È‚¢—]‚è
		template<class T>
		inline T positive_mod(T num,T divisor){
			if(num>=0)return num%divisor;
			else return divisor-1-((-1-num)%divisor);
		}
	}
}
#
#endif
