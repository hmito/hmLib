#ifndef HMLIB_ALGORITHM_COMPARE_INC
#define HMLIB_ALGORITHM_COMPARE_INC 201
#
/*===algorithm::compare===
大小関係のアルゴリズムを提供
algorithm::compare v2_00/170207 hmIto
	sort_value is now renamed to sorting_swap
algorithm::compare v2_00/160509 hmIto
	min,maxを多変数版、min_value, max_valueに変更
	swap_sortをsort_valueに変更
algorithm::compare v1_00/130328 hmIto
	algorthmから分離
*/
#include<algorithm>

namespace hmLib{
	//非コンテナ引数をソートする
	template<typename T>
	inline void sorting_swap(T& val1,T& val2){
		if(val1>val2)std::swap(val1,val2);
	}
	//非コンテナ引数をソートする
	template<typename T>
	inline void sorting_swap(T& val1,T& val2,T& val3){
		if(val1>val2){
			if(val2>val3){
				//val3 < val2 < val1
				std::swap(val1,val3);
			}else if(val1>val3){
				//val2 < val3 < val1
				std::swap(val1,val2);
				std::swap(val2,val3);
			}else{
				//val2 < val1 < val3
				std::swap(val1,val2);
			}
		}else{
			if(val1>val3){
				//val3 < val1 < val2
				std::swap(val1,val3);
				std::swap(val2,val3);
			}else if(val2>val3){
				//val1 < val3 < val2
				std::swap(val2,val3);					
			}else return;
		}
	}
	//3要素のmedian
	template<class T>
	inline constexpr T median(T val1,T val2,T val3){
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

	//3要素のmedian
	template<class T>
	inline constexpr T clamp(T val, T lower, T upper) {
		return std::min(std::max(lower, val), upper);
	}

	template<typename T, typename... Others>
	inline constexpr T min_value(T val1,T val2,Others... vals){return std::min(val1,min_value(val2,vals...));}
	template<typename T>
	inline constexpr T min_value(T val){return val;}
	template<typename T, typename... Others>
	inline constexpr T max_value(T val1,T val2,Others... vals){return std::max(val1,max_value(val2,vals...));}
	template<typename T>
	inline constexpr T max_value(T val){return val;}
	
	//mod with positive value
	template<class T>
	[[deprecated("please use euclidean_mod() function")]]
	inline T positive_mod(T num,T divisor){
		num %= divisor;
		if(num < 0)return num + divisor;
		return num;
	}

	template<class T>
	inline T euclidean_mod(T n, T divisor){
		return (n % divisor + divisor)%divisor;
	}
	template<class T>
	inline T euclidean_div(T n, T divisor){
		return (n-euclidean_mod(n,divisor))/divisor;
	}
	template<class T>
	inline std::pair<T,T> eucledian_divmod(T n, T divisor){
		T m = euclidean_mod(n,divisor);
		return std::pair<T,T>((n-m)/diviso,m);
	}

}
#
#endif
