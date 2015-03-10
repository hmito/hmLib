#ifndef HMLIB_ALGORITHM_INC
#define HMLIB_ALGORITHM_INC 110
/*===algorithm===
C++ algorthmを補助するアルゴリズムを提供する

algorithm v1_10/130328 hmito
	algorithm名前空間に基づいてファイルを解体
algorithm:v1_09/130128 hmIto
	roulette_samplerにsync関数を追加
		roulette_sampler自体を再利用可能に変更
algorithm:v1_08/130109
	roulette_samplerの高速化
		内部で配列の形で保持しておくことで、繰り返し時の動作を高速化
		std::lower_boundを利用する事で高速化
algorithm.hpp v1_07
		class roulette_samplerの初期化でBeginを不正に書き換えていたのを修正
algorithm.hpp v1_06
		class random_samplerの調整
algorithm.hpp v1_05
	ランダム選択を行う以下のクラスと関数を追加。
	//ルーレット選択クラス
	template<typename InputIterator,typename fnAssess>
	class roulette_sampler{
	public:
		roulette_sampler(InputIterator Begin_,InputIterator End_,fnAssess FnAssess_);
		OutputIterator operator()();
	};
	//ルーレット選択クラスのビルダー
	template<typename InputIterator,typename fnAssess>
	roulette_sampler<InputIterator,fnAssess> build_roulette_sampler(InputIterator Begin_,InputIterator End_,fnAssess FnAssess_);
algorithm.hpp v1_04
	ランダム選択、ルーレット選択で指定範囲から抜き出す、以下の6関数を追加。
	//ランダム選択
	template<class InputIterator,class OutputIterator>
	OutputIterator random_sample(InputIterator Begin,InputIterator End,OutputIterator Out);
	//ランダム選択 OutputIteratorの範囲を埋めるまで
	template<class InputIterator,class OutputIterator>
	OutputIterator random_sample(InputIterator Begin,InputIterator End,OutputIterator OutBegin,OutputIterator OutEnd);
	//ランダム選択 n個分
	template<class InputIterator,class OutputIterator>
	OutputIterator random_sample(InputIterator Begin,InputIterator End,OutputIterator Out,unsigned int n);
	//ルーレット選択
	template<class InputIterator,class FnRealValue,class OutputIterator>
	OutputIterator roulette_sample(InputIterator Begin,InputIterator End,FnRealValue Value,OutputIterator Out);
	//ルーレット選択 OutputIteratorの範囲を埋めるまで
	template<class InputIterator,class FnRealValue,class OutputIterator>
	OutputIterator roulette_sample(InputIterator Begin,InputIterator End,FnRealValue Value,OutputIterator OutBegin,OutputIterator OutEnd);
	//ルーレット選択 n個分
	template<class InputIterator,class FnRealValue,class OutputIterator>
	OutputIterator roulette_sample(InputIterator Begin,InputIterator End,FnRealValue Value,OutputIterator Out,unsigned int n);
*/
#ifndef HMLIB_ALGORITHM_COMPARE_INC
#	include"algorthm/compare.hpp"
#endif
#ifndef HMLIB_ALGORITHM_SAMPLING_INC
#	include"algorthm/sampling.hpp"
#endif
#
#endif
