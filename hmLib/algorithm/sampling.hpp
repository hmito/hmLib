#ifndef HMLIB_ALGORITHM_SAMPLING_INC
#define HMLIB_ALGORITHM_SAMPLING_INC 105
#
/*
===algorithm::sampling===
ある範囲の中から値を選択するアルゴリズムを提供する
algorithm::sampling:v2_00/160426 hmIto
	build系関数をmakeに変更
	distance/next等のiteratorへの特殊されている関数を利用するよう変更
	hmLib::randomへの依存性を削除
algorithm::sampling:v1_04/140731 hmIto
	可能なsamplerに、add/clear関数を追加
		手動で一個ずつデータを追加 / すべてクリアする関数
algorithm::sampling:v1_03/140708 hmIto
	shuffle_samplerを追加
		イテレータでレンジを受け取り、シャッフルして順に返す
		枯渇すると、endを返す
algorithm::sampling:v1_02/130917 hmIto
	例外処理のヘッダが不足していた問題を修正
algorithm::sampling:v1_01/130711 hmIto
	random_sampler追加
		イテレータでレンジを受け取り、その中からoperator()でランダムに返す
algorithm::sampling:v1_00/130328 hmIto
	algorithmから分離
*/
#include<algorithm>
#include<numeric>
#include<vector>
#include<random>
namespace hmLib{
	namespace algorithm{
		//ランダム選択
		template<typename InputIterator, typename RandEngine>
		InputIterator random_sample(InputIterator Begin,InputIterator End, RandEngine&& Engine){
			if(Begin == End)return End;
			return std::next(Begin, std::uniform_int<int>(0,std::distance(Begin,End)-1)(Engine));
		}
		//ランダム選択 OutputIteratorの範囲を埋めるまで
		template<typename InputIterator, typename OutputIterator, typename RandEngine>
		void random_sample(InputIterator Begin,InputIterator End,OutputIterator OutBegin,OutputIterator OutEnd, RandEngine&& Engine){
			if(Begin == End)return End;

			std::uniform_int<int> Dist(0, std::distance(Begin, End) - 1);
			while(OutBegin!=OutEnd){
				*OutBegin++ = *std::next(Begin, Dist(Engine));
			}
		}
		//ランダム選択 n個分
		template<typename InputIterator, typename OutputIterator, typename RandEngine>
		OutputIterator random_sample(InputIterator Begin,InputIterator End, OutputIterator Out, unsigned int n, RandEngine&& Engine){
			if(Begin == End)return End;

			std::uniform_int<int> Dist(0, std::distance(Begin, End) - 1);
			for(unsigned int i = 0; i < n; ++i){
				*Out++ = *std::next(Begin, Dist(Engine));
			}
			return Out;
		}
		//ランダム選択クラス
		template<typename InputIterator>
		class random_sampler{
			using dist_type = std::uniform_int_distribution<int>;
		private:
			InputIterator Begin;
			dist_type Dist;
		public:
			random_sampler(){}
			random_sampler(InputIterator Begin_,InputIterator End_){sync(Begin_,End_);}
		public:
			template<typename RandEngine>
			InputIterator operator()(RandEngine Engine){
				return std::next(Begin, Dist(Engine));
			}
			void sync(InputIterator Begin_,InputIterator End_){
				Begin=Begin_;

				dist_type::param_type prm(0, std::distance(Begin_, End_));
				Dist.param(prm);
			}
		};
		//ランダム選択クラスのビルダー
		template<typename InputIterator>
		random_sampler<InputIterator> make_random_sampler(InputIterator Begin_,InputIterator End_){
			return random_sampler<InputIterator>(Begin_,End_);
		}
		//ルーレット選択
		template<typename InputIterator, typename fnAssess, typename RandEngine>
		InputIterator roulette_sample(InputIterator Begin,InputIterator End, fnAssess&& FnAssess, RandEngine&& Engine){
			if(Begin == End)return End;

			double TotalFitness = 0;
			std::for_each(Begin, End, [&](decltype(*Begin)& Val){TotalFitness += FnAssess(Val); });

			double SelectFitness =std::uniform_real_distribution<double>(0.,TotalFitness)(Engine);
			for(;Begin!=End; ++Begin){
				SelectFitness-= FnAssess(*Begin);
				if(SelectFitness <= 0.)break;
			}
			return Begin;
		}
		//ルーレット選択 OutputIteratorの範囲を埋めるまで
		template<typename InputIterator, typename OutputIterator, typename fnAssess, typename RandEngine>
		void roulette_sample(InputIterator Begin,InputIterator End, fnAssess&& FnAssess,OutputIterator OutBegin,OutputIterator OutEnd, RandEngine&& Engine){
			if(Begin == End)return End;

			double TotalFitness = 0;
			std::for_each(Begin, End, [&](decltype(*Begin)& Val){TotalFitness += FnAssess(Val); });

			std::uniform_real_distribution<double> Dist(0., TotalFitness);

			while(OutBegin != OutEnd){
				double SelectFitness = Dist(Engine);
				for(auto Itr = Begin; Itr != End; ++Itr){
					SelectFitness -= FnAssess(*Itr);
					if(SelectFitness <= 0.)break;
				}
				*OutBegin++ = *Itr;
			}
		}
		//ルーレット選択 n個分
		template<typename InputIterator, typename OutputIterator, typename fnAssess, typename RandEngine>
		OutputIterator roulette_sample(InputIterator Begin,InputIterator End, fnAssess&& FnAssess, OutputIterator Out, unsigned int n, RandEngine&& Engine){
			if(Begin == End)return End;

			double TotalFitness = 0;
			std::for_each(Begin, End, [&](decltype(*Begin)& Val){TotalFitness += FnAssess(Val); });

			std::uniform_real_distribution<double> Dist(0., TotalFitness);

			for(unsigned int i = 0; i < n; ++i){
				double SelectFitness = Dist(Engine);
				for(auto Itr = Begin; Itr != End; ++Itr){
					SelectFitness -= FnAssess(*Itr);
					if(SelectFitness <= 0.)break;
				}
				*Out++ = *Itr;
			}

			return Out;
		}
		//ルーレット選択クラス
		template<typename InputIterator>
		class roulette_sampler{
			struct assess_itr{
				InputIterator Itr;
				double Val;
			public:
				assess_itr(double Val_):Val(Val_){}
				assess_itr(InputIterator Itr_,double Val_):Itr(Itr_),Val(Val_){}
				bool operator<(const assess_itr& My_)const{return Val<My_.Val;}
			};
		private:
			std::vector<assess_itr> AssessVec;
		public:
			roulette_sampler(){}
			template<typename fnAssess>
			roulette_sampler(InputIterator Begin_,InputIterator End_,fnAssess&& FnAssess_){sync(Begin_,End_, std::forward(FnAssess_));}
		public:
			template<typename RandEngine>
			InputIterator operator()(RandEngine Engine){
				return std::lower_bound(
					AssessVec.begin()
					,AssessVec.end()
					,assess_itr(InputIterator(), std::uniform_real_distribution<double>(0.,assess())(Engine))
				)->Itr;
			}
			double assess()const{
				if(AssessVec.size()==0)return 0.0;
				return AssessVec.back().Val;
			}
			template<typename fnAssess>
			void sync(InputIterator Begin_,InputIterator End_,fnAssess&& FnAssess_){
				double Val=0.;
				AssessVec.clear();
				while(Begin_!=End_){
					Val+=FnAssess_(*Begin_);
					AssessVec.push_back(assess_itr(Begin_++,Val));
				}
			}
			unsigned int size()const{return AssessVec.size();}
			void clear(){AssessVec.clear();}
			void add(InputIterator Itr_,double Assess_){
				AssessVec.push_back(assess_itr(Itr_,Assess_+assess()));
			}
		};
		//ルーレット選択クラスのビルダー
		template<typename InputIterator,typename fnAssess>
		roulette_sampler<InputIterator> make_roulette_sampler(InputIterator Begin_,InputIterator End_,fnAssess&& FnAssess_){
			return roulette_sampler<InputIterator>(Begin_,End_,std::forward(FnAssess_));
		}
		//シャッフル選択クラス
		template<typename InputIterator>
		class shuffle_sampler {
			typedef typename std::vector<InputIterator>::iterator iterator;
		private:
			std::vector<InputIterator> Vec;
			iterator Now;
			InputIterator End;
		public:
			shuffle_sampler() {}
			shuffle_sampler(InputIterator Begin_, InputIterator End_) { sync(Begin_, End_); }
		public:
			InputIterator operator()() {
				if(Now==Vec.end())return End;
				return *(Now++);
			}
			InputIterator sample(bool IsAutoShuffle_=true) {
				if(Now==Vec.end()){
					if(!IsAutoShuffle_ || Vec.empty())return End;
					shuffle();
				}
				return *(Now++);
			}
			operator bool()const { return Vec.size()>0 && Now!=Vec.end(); }
			template<typename RandEngine>
			void shuffle(RandEngine&& Engine) {
				std::shuffle(Vec.begin(), Vec.end(), Engine);
				Now=Vec.begin();
			}
			void sync(InputIterator Begin_, InputIterator End_) {
				End=End_;
				Vec.clear();
				for(; Begin_!=End_; ++Begin_) {
					Vec.push_back(Begin_);
				}
				shuffle();
			}
			unsigned int size()const{return Vec.size();}
			void clear(){Vec.clear();}
			void add(InputIterator Itr_){Vec.push_back(Itr_);}
		};
		//シャッフル選択クラスのビルダー
		template<typename InputIterator>
		shuffle_sampler<InputIterator> make_shuffle_sampler(InputIterator Begin_, InputIterator End_) {
			return shuffle_sampler<InputIterator>(Begin_, End_);
		}
	}
}
#
#endif
