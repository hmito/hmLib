#ifndef HMLIB_MEMORY_DEFAULTDELETER_INC
#define HMLIB_MEMORY_DEFAULTDELETER_INC 100
#
/*
配列かどうか判断して、deleteする方法を自分で変えるクラス
=== memory::default_deleter ===
v1_00/140405 hmIto
	作成
*/
namespace hmLib {
	template<typename T>
	struct default_deleter {
		void operator()(T* ptr) {
			if(ptr)delete ptr;
		}
	};
	template<typename T>
	struct default_deleter<T[]> {
		void operator()(T* ptr) {
			if(ptr)delete[] ptr;
		}
	};
}
#
#endif
