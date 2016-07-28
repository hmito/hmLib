#ifndef HMLIB_RVALUEREFERENCE_INC
#define HMLIB_RVALUEREFERENCE_INC 100
#
/*
自前の右辺値参照&ムーブ用クラス
C++03環境向けであり、当然本来は非推奨

=== rvalue_reference ===
v1_00/140405 hmIto
	作成
*/
namespace hmLib {
	template<typename T>
	struct rvalue_reference {
		T& ref;
		explicit rvalue_reference(T& ref_) :ref(ref_) {}
	};
	template<typename T>
	inline rvalue_reference<T> move(T& Ptr_) { return rvalue_reference<T>(Ptr_); }
}
#
#endif
