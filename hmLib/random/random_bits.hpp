#ifndef HMLIB_RANDOM_RANDOMBITS_INC
#define HMLIB_RANDOM_RANDOMBITS_INC 100
#
/*===random_bit===
bit単位で乱数を提供する

random_bit:v1_00/130321 hmIto
	bit単位での乱数保持機能、random_bitを追加
	random::uniform_bool関数を追加
*/
#include<type_traits>
#include<cinttypes>
#include<bitset>
#include"random_engine.hpp"
namespace hmLib{
	template<typename random_engine>
	class random_bits_engine{
		using my_type = random_bits_engine<random_engine>;
		using bits_result_type = typename random_engine::result_type;
	public:
		using bits_type = typename std::make_unsigned<bits_result_type>::type;
	public:
		static std::size_t interval(){
			std::size_t s = 0;
			bits_type bit_s = 1;

			for(; s < sizeof(bits_type); ++s, bit_s = ((bit_s << 1) | 1)){
				if(bit_s > random_engine::max())break;
			}
			return s;
		}
		static bits_type max_interval_bits(){
//			static_assert(sizeof(bits_type) >= interval(),"size of bits_type is smaller than interval.");
			return (~bits_type(0))>>(sizeof(bits_type)- interval());
		}	
	private:
		bits_type Table;
		std::size_t Cnt;
		random_engine Engine;
	public:
		random_bits_engine(random_engine&& Engine_):Table(0),Cnt(0),Engine(Engine_){}
		random_bits_engine(const my_type&) = delete;
		random_bits_engine operator=(const my_type&) = delete;
	public:
		bool operator()(void){
			if(Cnt < 1){
				Table = std::uniform_int_distribution<bits_type>(0,max_interval_bits())(Engine);
				Cnt = interval();
			}
			
			bool Bool = ((Table&1)!=0);
			Table>>=1;
			--Cnt;

			return Bool;
		}
		template<unsigned int bit_size>
		std::bitset<bit_size> bits(){
			std::size_t Request = bit_size;			
			std::bitset<bit_size> RandomBits;
			
			if(Cnt==0){
				Table = std::uniform_int_distribution<bits_type>(0,max_interval_bits())(Engine);
				Cnt = interval();				
			}
			
			if(Request > Cnt){
				RandomBits=std::bitset<bit_size>(Table);

				Request-=Cnt;
				Table = std::uniform_int_distribution<bits_type>(0, max_interval_bits())(Engine);
				Cnt = interval();

				while(Request > Cnt){
					RandomBits <<= Cnt;
					RandomBits |= std::bitset<bit_size>(Table);

					Request -= Cnt;
					Table = std::uniform_int_distribution<bits_type>(0, max_interval_bits())(Engine);
					Cnt = interval();
				}
				RandomBits <<= Request;
				RandomBits |= std::bitset<bit_size>(Table)&((~std::bitset<bit_size>(0)) >> (bit_size - Request));
				Table >>= Request;
				Cnt -= Request;
			}else{
				RandomBits=std::bitset<bit_size>(Table);
				Table>>=Request;
				Cnt-=Request;
				Request=0;
			}
			
			return RandomBits;
		}
	};
	template<typename tag, typename random_engine_type = std::mt19937_64>
	class singleton_random_bits_engine{
	private:
		using my_type = singleton_random_bits_engine<tag, random_engine_type>;
		using my_engine = singleton_random_engine<tag, random_engine_type>;
		using my_bits_engine = random_bits_engine<my_engine>;
	public:
		using bits_type = typename my_bits_engine::bits_type;
	private:
		static my_bits_engine Engine;
	public:
		static std::size_t interval(){ return my_bits_engine::interval(); }
		static bits_type max_interval_bits(){ return my_bits_engine::max_interval_bits(); }
	public:
		singleton_random_bits_engine() = default;
	public:
		bool operator()(void){ return Engine(); }
		template<unsigned int bit_size>
		std::bitset<bit_size> bits(){ return Engine.bits<bit_size>(); }
	};
	template<typename tag, typename random_engine_type>
	random_bits_engine<singleton_random_engine<tag, random_engine_type>> singleton_random_bits_engine<tag, random_engine_type>::Engine(std::move(singleton_random_engine<tag, random_engine_type>()));

	namespace random{
		using default_bits_engine = singleton_random_bits_engine<default_engine_tag>;
		inline bool uniform_bool(){return default_bits_engine()();}
		template<unsigned int bit_size>
		inline std::bitset<bit_size> uniform_bits(){ return default_bits_engine().bits<bit_size>(); }
	}
}
#
#endif
