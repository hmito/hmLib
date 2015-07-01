#ifndef HMLIB_RANDOM_RANDOMBITSET_INC
#define HMLIB_RANDOM_RANDOMBITSET_INC 100
#
/*===random_bitset===
乱数で初期化されたbitsetを提供する

random_bitset:v1_00/130709 hmIto
	乱数初期化が行われたbitset生成クラス、random_bitsetを追加
	random::uniform_bool関数を追加
	*/
#include<bitset>
#ifndef HMLIB_RANDOM_RANDOMENGINE_INC
#	include"random_engine.hpp"
#endif
namespace hmLib{
	template<unsigned int bit_num>
	class random_bitset{
	public:
		template<typename random_engine_>
		std::bitset<bit_num> operator()(random_engine_& Engine_){
			unsigned int random_bits=sizeof(random_engine_::result_type)*8;

			std::bitset<bit_num> BitSet;
			for(unsigned int Cnt=0;Cnt<bit_num;Cnt+=random_bits){
				BitSet<<=random_bits;
				BitSet|=std::bitset<bit_num>(Engine_());
			}
			return BitSet;
		}
		std::bitset<bit_num> operator()(void){return (*this)(hmLib::random::Engine);}
	};
	namespace random{
		template<unsigned int bit_num>
		struct random_bitset_holder{
			static hmLib::random_bitset<bit_num> RandomBitset;
		};
		template<unsigned int bit_num>
		hmLib::random_bitset<bit_num> random_bitset_holder<bit_num>::RandomBitset;
		template<unsigned int bit_num>
		std::bitset<bit_num> uniform_bitset(){
			return random_bitset_holder<bit_num>::RandomBitset();
		}
	}
}
#
#endif
