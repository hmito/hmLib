#ifndef HMLIB_BINARY_INC
#define HMLIB_BINARY_INC 100
#
#ifndef HMLIB_BYTEBASETYPE_INC
#	include <hmLib_v3_04/bytebase_type.hpp>
#endif
//bitŠÖ˜A
namespace hmLib{
	namespace binary{
		template<class T>
		inline bool bittest(T c,type::uint32 num){
			if(num>=sizeof(T)*8 || num<0)return false;
			return (c&(1<<num))!=0;
		}
		template<class T>
		inline void bitput(T c,type::uint32 num,bool value){
			if(num>=sizeof(T)*8 || num<0)return;
			if(value)c|=(1<<num);
			else c&=(!(1<<num));
		}
		template<class T>
		inline void bitset(T c,type::uint32 num){
			if(num>=sizeof(T)*8 || num<0)return;
			c|=(1<<num);
		}
		template<class T>
		inline void bitclear(T c,type::uint32 num){
			if(num>=sizeof(T)*8 || num<0)return;
			c&=(!(1<<num));
		}
		template<class T>
		inline void bitxor(T c,type::uint32 num){
			if(num>=sizeof(T)*8 || num<0)return;
			c^=(!(1<<num));
		}
	}
}
#
#endif
