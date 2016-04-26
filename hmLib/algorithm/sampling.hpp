#ifndef HMLIB_ALGORITHM_SAMPLING_INC
#define HMLIB_ALGORITHM_SAMPLING_INC 104
#
/*
===algorithm::sampling===
ある範囲の中から値を選択するアルゴリズムを提供する
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
#include<vector>
#ifndef HMLIB_RANDOM_INC
#	include<hmLib/random.hpp>
#endif
#ifndef HMLIB_EXCEPTIONS_INC
#	include<hmLib/exceptions.hpp>
#endif
namespace hmLib{
	namespace algorithm{
		//ランダム選択
		template<class InputIterator,class OutputIterator>
		OutputIterator random_sample(InputIterator Begin,InputIterator End,OutputIterator Out){
			unsigned int Size=0;
			InputIterator Itr;
			for(Itr=Begin;Itr!=End;++Itr)++Size;

			unsigned int Pos=hmLib::random::uniform_int(0,Size-1);
			Itr=Begin;
			for(unsigned int Cnt=0;Cnt<Pos;++Cnt)++Itr;
			*Out=*Itr;
			++Out;

			return Out;
		}
		//ランダム選択 OutputIteratorの範囲を埋めるまで
		template<class InputIterator,class OutputIterator>
		OutputIterator random_sample(InputIterator Begin,InputIterator End,OutputIterator OutBegin,OutputIterator OutEnd){
			unsigned int Size=0;
			InputIterator Itr;
			for(Itr=Begin;Itr!=End;++Itr)++Size;

			for(;OutBegin!=OutEnd;++OutBegin){
				unsigned int Pos=hmLib::random::uniform_int(0,Size-1);
				Itr=Begin;
				for(unsigned int Cnt=0;Cnt<Pos;++Cnt)++Itr;
				*OutBegin=*Itr;
			}

			return OutBegin;
		}
		//ランダム選択 n個分
		template<class InputIterator,class OutputIterator>
		OutputIterator random_sample(InputIterator Begin,InputIterator End,OutputIterator Out,unsigned int n){
			unsigned int Size=0;
			InputIterator Itr;
			for(Itr=Begin;Itr!=End;++Itr)++Size;

			for(unsigned int m=0;m!=n;++m){
				unsigned int Pos=hmLib::random::uniform_int(0,Size-1);
				Itr=Begin;
				for(unsigned int Cnt=0;Cnt<Pos;++Cnt)++Itr;
				*Out=*Itr;
				++Out;
			}

			return Out;
		}
		//ランダム選択クラス
		template<typename InputIterator>
		class random_sampler{
		private:
			InputIterator Begin;
			InputIterator End;
		public:
			random_sampler(){}
			random_sampler(InputIterator Begin_,InputIterator End_){sync(Begin_,End_);}
		public:
			InputIterator operator()(){
				return Begin+hmLib::random::uniform_int(0,End-Begin-1);
			}
			void sync(InputIterator Begin_,InputIterator End_){
				Begin=Begin_;
				End=End_;
			}
		};
		//ランダム選択クラスのビルダー
		template<typename InputIterator>
		random_sampler<InputIterator> make_random_sampler(InputIterator Begin_,InputIterator End_){
			return random_sampler<InputIterator>(Begin_,End_);
		}
		//ルーレット選択
		template<class InputIterator,class FnRealValue,class OutputIterator>
		OutputIterator roulette_sample(InputIterator Begin,InputIterator End,FnRealValue Value,OutputIterator Out){
			double TotalFitness=0.;
			InputIterator Itr;
			for(Itr=Begin;Itr!=End;++Itr){
				TotalFitness+=Value(*Itr);
			}

			double SelectFitnes=hmLib::random::uniform_real(0.,TotalFitness);
			for(Itr=Begin;Itr!=End;++Itr){
				SelectFitness-=Value(*Itr);
				if(SelectFitness<=0.)break;
			}
			*Out=*Itr;
			++Out;

			return Out;
		}
		//ルーレット選択 OutputIteratorの範囲を埋めるまで
		template<class InputIterator,class FnRealValue,class OutputIterator>
		OutputIterator roulette_sample(InputIterator Begin,InputIterator End,FnRealValue Value,OutputIterator OutBegin,OutputIterator OutEnd){
			double TotalFitness=0.;
			InputIterator Itr;
			for(Itr=Begin;Itr!=End;++Itr){
				TotalFitness+=Value(*Itr);
			}

			for(;OutBegin!=OutEnd;++OutBegin){
				double SelectFitnes=hmLib::random::uniform_real(0.,TotalFitness);
				for(Itr=Begin;Itr!=End;++Itr){
					SelectFitness-=Value(*Itr);
					if(SelectFitness<=0.)break;
				}
				*OutBegin=*Itr;
				++OutBegin;
			}

			return OutBegin;
		}
		//ルーレット選択 n個分
		template<class InputIterator,class FnRealValue,class OutputIterator>
		OutputIterator roulette_sample(InputIterator Begin,InputIterator End,FnRealValue Value,OutputIterator Out,unsigned int n){
			double TotalFitness=0.;
			InputIterator Itr;
			for(Itr=Begin;Itr!=End;++Itr){
				TotalFitness+=Value(*Itr);
			}

			for(unsigned int m=0;m!=n;++m){
				double SelectFitnes=hmLib::random::uniform_real(0.,TotalFitness);
				for(Itr=Begin;Itr!=End;++Itr){
					SelectFitness-=Value(*Itr);
					if(SelectFitness<=0.)break;
				}
				*Out=*Itr;
				++Out;
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
			roulette_sampler(InputIterator Begin_,InputIterator End_,fnAssess FnAssess_){sync(Begin_,End_,FnAssess_);}
		public:
			InputIterator operator()(){
				return std::lower_bound(
					AssessVec.begin()
					,AssessVec.end()
					,assess_itr(hmLib::random::uniform_real(0.,assess())))->Itr;
			}
			double assess()const{
				if(AssessVec.size()==0)return 0.0;
				return AssessVec.back().Val;
			}
			template<typename fnAssess>
			void sync(InputIterator Begin_,InputIterator End_,fnAssess FnAssess_){
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
		roulette_sampler<InputIterator> make_roulette_sampler(InputIterator Begin_,InputIterator End_,fnAssess FnAssess_){
			return roulette_sampler<InputIterator>(Begin_,End_,FnAssess_);
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
			void shuffle() { 
				std::shuffle(Vec.begin(), Vec.end(), hmLib::random::Engine);
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
			void add(InputIterator Itr_,bool SuppressShuffle_=false){
				Vec.push_back(Itr_);
				if(!SuppressShuffle_)shuffle();
			}
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
