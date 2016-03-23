#ifndef HMLIB_RANDOM_RANDOMENGINE_INC
#define HMLIB_RANDOM_RANDOMENGINE_INC 100
#
/*
===random_engine===
C++11の乱数ライブラリをラッピングして提供

random_engine v1_00/130328 hmIto
	randomから分離
*/
#include<algorithm>
#include<functional>
#include<vector>
#include<random>
#include<numeric>
#

namespace hmLib{
	namespace random{
		template<typename engine_type>
		engine_type randomized_engine(){
			engine_type Engine;
			
			// ランダムデバイス
			std::random_device rnd ;

			// 初期化用ベクタ
			std::vector< unsigned int> v(10) ;

			// ベクタの初期化
			std::generate( v.begin(), v.end(), std::ref(rnd) ) ;

			//シード配列作成
			std::seed_seq Seq( v.begin(), v.end() );
			
			//シードで初期化
			Engine.seed(Seq);
			
			return Engine;
		}
	}
	template<typename tag, typename random_engine_type = std::mt19937_64>
	struct singleton_random_engine{
	private:
		using my_type = singleton_random_engine<tag, random_engine_type>;
	public:
		using result_type = typename random_engine_type::result_type;
	private:
		static random_engine_type Engine;
	public:
		constexpr static result_type min() { return random_engine_type::min(); }
		constexpr static result_type max() { return random_engine_type::max(); }
		static void singleton_seed(){Engine.seed();}
		static void singleton_seed(result_type Seed){Engine.seed(Seed);}
	public:
		singleton_random_engine()=default;
		result_type operator()(void){return Engine();}
		void discard(unsigned long long z){Engine.discard(z);}
	};
	template<typename tag, typename random_engine_type>
	random_engine_type singleton_random_engine<tag,random_engine_type>::Engine = random::randomized_engine<random_engine_type>();
	
	namespace random{
		struct default_engine_tag{};
		using defaul_engine = singleton_random_engine<default_engine_tag>;
		template<typename value_type>
		value_type uniform_int(value_type Min,value_type Max){return std::uniform_int_distribution<value_type>(Min,Max)(defaul_engine());}
		template<typename value_type>
		value_type uniform_real(value_type Min, value_type Max){return std::uniform_real_distribution<value_type>(Min,Max)(defaul_engine());}
		template<typename value_type>
		value_type normal(value_type meen, value_type sigma){return std::normal_distribution<value_type>(meen,sigma)(defaul_engine());}
	}
}
#
#endif
