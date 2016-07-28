#ifndef HMLIB_NUMERICALANALYSIS_INC
#define HMLIB_NUMERICALANALYSIS_INC 100
#
/*
数値解析の手法を提供するライブラリ
===numerical_analysis===
v1_00/131004 hmIto	
	small_step法, 二分法を追加
*/
#include<utility>
//#include<hmLib/parameter.hpp>
namespace hmLib{
	namespace numerical{
		template<typename dFn_d>
		double small_step_analysis(dFn_d Fn,double Begin_,double End_,double Step_){
			bool IsPositive=(Fn(Begin_)>0);

			while(IsPositive==Fn(Begin_+Step_)>0 && Begin_<End_){
				Begin_+=Step_;
			}

			return std::min(Begin_,End_);
		}
		template<typename dFn_d>
		std::pair<double,double> bisection_analysis(dFn_d Fn,double Begin_,double End_,double Error_){
			double Val=Fn(Begin_);

			if(Val==0.)return std::make_pair(Begin_,End_-Begin_);

			bool IsPositive=(Val>0);

			while(End_-Begin_>Error_){
				double Pos=(End_+Begin_)/2;

				Val=Fn(Pos);
				//ゼロ点を見つけた場合
				if(Val==0.){
					double CheckLow=Fn((Begin_+Pos)/2.);
					double CheckHgh=Fn((End_+Pos)/2.);

					//最中間点もゼロとなってしまわないか確認
					if(CheckHgh==0. || CheckLow==0.){
						if(CheckLow!=0.)Begin_=(Begin_+Pos)/2.;
						if(CheckHgh!=0.)End_=(Begin_+Pos)/2.;
						break;
					}
					Begin_=(Begin_+Pos)/2.;
					End_=(End_+Pos)/2.;
				}else if(Val>0==IsPositive){
					Begin_=Pos;
				}else{
					End_=Pos;
				}
			}

			return std::make_pair((End_+Begin_)/2.,End_-Begin_);
		}
	}
}
#
#endif
