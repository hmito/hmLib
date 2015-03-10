#ifndef HMLIB_RANDOM_INC
#define HMLIB_RANDOM_INC 206
#
/*
===random===
C++11の乱数ライブラリをラッピングして提供

random_v2_06/121128 hmIto
	拡張子をhからhppに変更。
*/
#include<algorithm>
#include<functional>
#include<vector>
#include<random>
#include<numeric>
#
#ifndef HMLIB_CONFIG_INC
#	include "hmLib_config.h"
#endif
#ifndef HMLIB_NOLIB
#	ifdef _DEBUG
#		pragma comment(lib,"hmLib_v3_03/lib/Debug/random.lib")
#	else
#		pragma comment(lib,"hmLib_v3_03/lib/Release/random.lib")
#	endif
#endif

namespace hmLib{
	template<class random_engine_type = std::mt19937_64>
	class random_engine:public random_engine_type{
	public:
		random_engine(){
			// ランダムデバイス
			std::random_device rnd ;

			// 初期化用ベクタ
			std::vector< unsigned int> v(10) ;

			// ベクタの初期化
			std::generate( v.begin(), v.end(), std::ref(rnd) ) ;

			//シードで初期化
			seed(std::seed_seq( v.begin(), v.end() ));
		}
		template<class InputIterator>
		random_engine(InputIterator Begin,InputIterator End){
			seed(std::seed_seq(Begin,End));
		}
		int uniform_int(int Min,int Max){return std::uniform_int_distribution<int>(Min,Max)(*this);}
		double uniform_real(double Min=0.0,double Max=1.0){return std::uniform_real_distribution<double>(Min,Max)(*this);}
		double normal(double meen,double sigma){return std::normal_distribution<double>(meen,sigma)(*this);}
	};
	namespace random{
		extern random_engine<> Engine;
		inline int uniform_int(int Min,int Max){return Engine.uniform_int(Min,Max);}
		inline double uniform_real(double Min=0.0,double Max=1.0){return Engine.uniform_real(Min,Max);}
		inline double normal(double meen,double sigma){return Engine.normal(meen,sigma);}
	}
}
#
#endif
