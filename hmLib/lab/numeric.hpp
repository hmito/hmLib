#ifndef HMLIB_NUMERIC_INC
#define HMLIB_NUMERIC_INC 102
#
/*===hmLib::numeric===
数値演算に関する、標準ライブラリの補助関数群
v1_02/140709
	初頭型量を算出するstatisticsを追加
v1_01/140618 hmIto
	未調整のcovarianceを廃止
	variance関数は、Averageを加算する形に変更
v1_00/130805 hmIto
	平均、分散、共分散を追加
*/
#include<iterator>
#include<numeric>
#include<hmLib/exceptions.hpp>

namespace hmLib{
	namespace numeric{
		template<typename input_iterator,typename T>
		T average(input_iterator Begin,input_iterator End,T Init){
			return std::accumulate(Begin,End,Init)/std::distance(Begin,End);
		}
		template<typename input_iterator,typename T,typename accumulate_functor>
		T average(input_iterator Begin,input_iterator End,T Init,accumulate_functor Func){
			return std::accumulate(Begin,End,Init,Func)/std::distance(Begin,End);
		}
		template<typename input_iterator,typename T>
		T variance(input_iterator Begin,input_iterator End,T Init,T Ave){
			return average(Begin,End,Init,[](const T& Val)->T{return Val*Val;})-Ave*Ave;
		}
		template<typename input_iterator,typename T,typename square_accumulate_functor>
		T variance(input_iterator Begin, input_iterator End, T Init, T Ave, square_accumulate_functor Func) {
			return average(Begin, End, Init, Func)-Ave*Ave;
		}
		//四分位数を算出する
		class  statistics {			
			double Low5;
			double Q1;
			double Med;
			double Q3;
			double Up5;
			double Min;
			double Max;
			double Ave;
			double Var;
		public:
			template<typename input_iterator>
			void operator()(input_iterator Begin_, input_iterator End_) {
				static std::vector<input_iterator> Vec;
				Vec.clear();

				for(input_iterator Itr=Begin_; Itr!=End_; ++Itr) {
					Vec.push_back(Itr);
				}

				std::sort(Vec.begin(), Vec.end(), [&](input_iterator val1, input_iterator val2)->bool {return *val1<*val2; });
			 

				Min=*Vec.front();
				Max=*Vec.back();
				Q1=*Vec[(Vec.size()/4)];
				Med=*Vec[(Vec.size()/2)];
				Q3=*Vec[(3*Vec.size()/4)];
				Low5=*Vec[(Vec.size()/40)];
				Up5=*Vec[(Vec.size()*39/40)];

				Ave=hmLib::numeric::average(Begin_, End_, 0.0, [&](double val_, double add_)->double {return val_+add_; });
				Var=hmLib::numeric::variance(Begin_, End_, 0.0, Ave,[&](double val_, double add_)->double {return val_+add_*add_; });
			}
			template<typename input_iterator, typename expression_>
			void operator()(input_iterator Begin_, input_iterator End_, expression_ Exp_) {
				static std::vector<input_iterator> Vec;
				Vec.clear();

				for(input_iterator Itr=Begin_; Itr!=End_; ++Itr) {
					Vec.push_back(Itr);
				}

				std::sort(Vec.begin(), Vec.end(), [&](input_iterator val1, input_iterator val2)->bool {return Exp_(*val1)<Exp_(*val2); });

				Min=Exp_(*Vec.front());
				Max=Exp_(*Vec.back());
				Q1=Exp_(*Vec[(Vec.size()/4)]);
				Med=Exp_(*Vec[(Vec.size()/2)]);
				Q3=Exp_(*Vec[(3*Vec.size()/4)]);
				Low5=Exp_(*Vec[(Vec.size()/40)]);
				Up5=Exp_(*Vec[(Vec.size()*39/40)]);

				Ave=hmLib::numeric::average(Begin_, End_, 0.0, [&](double val_, decltype(*Begin_) player_)->double {return val_+Exp_(player_); });
				Var=hmLib::numeric::variance(Begin_, End_, 0.0, Ave,[&](double val_, decltype(*Begin_) player_)->double {return val_+Exp_(player_)*Exp_(player_); });
			}
			double q1()const { return Q1; }
			double median()const { return Med; }
			double q3()const { return Q3; }
			double min_f95()const { return Low5; }
			double max_f95()const { return Up5; }
			double min()const { return Min; }
			double max()const { return Max; }
			double iqr()const { return Q3-Q1; }
			double lower_iqr3()const { return Q1-3*iqr(); }
			double upper_iqr3()const { return Q3+3*iqr(); }
			double min_iqr3()const { return std::max(lower_iqr3(), min()); }
			double max_iqr3()const { return std::min(upper_iqr3(), max());}
			double min_iqr3_with_f95()const { return std::max(std::min(lower_iqr3(),min_f95()), min()); }
			double max_iqr3_with_f95()const { return std::min(std::max(upper_iqr3(),max_f95()), max());}
			double average()const { return Ave; }
			double variance()const { return Var; }

		};
	}
}
#
#endif
