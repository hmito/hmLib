#ifndef HMLIB_UTILITY_INC
#define HMLIB_UTILITY_INC 100
#
#include<utility>
/*===utility===
主にオブジェクトの操作等に関わる汎用クラス/マクロ類を宣言する

v1_00/130717
	hmLib_static_restrictを追加
		template引数に宣言することで、template引数の実行条件に制約をかけるマクロ
	clone関数を追加
		コピーコンストラクタ/コピーファンクタを呼び出してクローンを生成する
	exchange関数を追加
		値を入れ替えつつ、前回の値を返す
*/
namespace hmLib{
	namespace utility{
		extern void* enabler;
	}
}
#define hmLib_static_restrict(condition) typename std::enable_if<condition>::type*& = hmLib::utility::enabler
namespace hmLib{
	//クローン関数
	template<typename T>
	T clone(const T& t_){return T(t_);}
	template<typename T, typename Fn>
	T clone(const T& t_,Fn Func){return Func(t_);}
	//値入れ替え関数
	template<typename T, typename U>
	T exchange(T& obj, U&& new_val) {
		T old_val = std::move(obj);
		obj = std::forward<U>(new_val);
		return old_val;
	}
}
#
#endif
