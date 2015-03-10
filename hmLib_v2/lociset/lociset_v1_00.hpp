#ifndef INC_HMLIB_LOCISET
#define INC_HMLIB_LOCISET 100
#
#include <limits>
#include <sstream>
#include <hmLib_v2/random.hpp>
#include <boost/operators.hpp>
namespace hmLib{
	class lociset{
	private:
		typedef unsigned long long loci_type;
	public:
		class abt_mutation_mode{
		protected:
			typedef lociset::loci_type loci_type;
		public:
			virtual void mutate(loci_type& Loci_)=0;
		};
		class abt_expression_mode{
		protected:
			typedef lociset::loci_type loci_type;
		public:
			virtual double express(const loci_type& Loci_)=0;
		};
		class abt_meiosis_mode{
		protected:
			typedef lociset::loci_type loci_type;
		public:
			virtual loci_type meiosis(const loci_type& Loci1_,const loci_type& Loci2_)=0;
		};
	private:
		loci_type Loci;
	public:
		lociset():Loci(0){}
		lociset(const loci_type& Loci_):Loci(Loci_){}
		lociset(const lociset& My_):Loci(My_.Loci){}
		lociset(const lociset& My1_,const lociset& My2_,abt_meiosis_mode& Meiosis):Loci(Meiosis.meiosis(My1_.Loci,My2_.Loci)){}
		//突然変異
		void mutate(abt_mutation_mode& Mutation){Mutation.mutate(Loci);}
		//遺伝子発現
		double express(abt_expression_mode& Expression)const{return Expression.express(Loci);}
		//減数分裂
		void meiosis(const lociset& My1_,const lociset& My2_,abt_meiosis_mode& Meiosis){Loci=Meiosis.meiosis(My1_.Loci,My2_.Loci);}
		//文字列化
		std::string to_string()const{
			std::stringstream sout;
			loci_type tmp=Loci;
			for(unsigned int i=0;i<64;++i){
				sout<<(tmp&1);
				tmp>>=1;
			}
			return sout.str();
		}
	};
	namespace loci{
		//突然変異を各遺伝子座に引き起こす
		class mutation_mode_each:public lociset::abt_mutation_mode{
		private:
			double MutationRate_EachLocus;
		public:
			mutation_mode_each(const double& MutationRate_EachLocus_):MutationRate_EachLocus(MutationRate_EachLocus_){}
			void mutate(loci_type& Loci){
				for(loci_type i=1;i!=0;i<<=1){
					if(random::uniform_real(0.,1.)<MutationRate_EachLocus)Loci^=i;
				}
			}
		};
		//突然変異が全体で起こる確率から計算
		class mutation_mode_any:public lociset::abt_mutation_mode{
		private:
			double MutationRate_AnyLocus;
		public:
			mutation_mode_any(const double& MutationRate_AnyLocus_):MutationRate_AnyLocus(MutationRate_AnyLocus_){}
			void mutate(loci_type& Loci){
				while(random::uniform_real(0.,1.)<MutationRate_AnyLocus){
					Loci^=(static_cast<loci_type>(1)<<random::uniform_int(0,63));
				}
			}
		};
		//遺伝子発現:均等に評価
		class expression_mode_count:public lociset::abt_expression_mode{
		private:
			unsigned int LociSize;
		public:
			expression_mode_count(unsigned int LociSize_)
				:LociSize(LociSize_){
			}
			double express(const loci_type& Loci){
				loci_type tmp=Loci;
				unsigned int Num=0;
				for(unsigned int Cnt=0;Cnt<LociSize;++Cnt){
					if(tmp&1)++Num;
					tmp>>=1;
				}

				return Num;
			}
		};
		//遺伝子発現:均等に評価
		class expression_mode_equal:public lociset::abt_expression_mode{
		private:
			double Min;
			double Max;
			unsigned int LociSize;
		public:
			expression_mode_equal(double Min_,double Max_,unsigned int LociSize_)
				:Min(Min_)
				,Max(Max_)
				,LociSize(LociSize_){
			}
			double express(const loci_type& Loci){
				loci_type tmp=Loci;
				unsigned int Num=0;
				for(unsigned int Cnt=0;Cnt<LociSize;++Cnt){
					if(tmp&1)++Num;
					tmp>>=1;
				}

				return (Max-Min)*static_cast<double>(Num)/static_cast<double>(LociSize)+Min;
			}
		};
		//減数分裂:与えられたlinkageでシャッフル
		class meiosis_mode_linkage:public lociset::abt_meiosis_mode{
		private:
			loci_type LociLincage;
		public:
			meiosis_mode_linkage(const loci_type& LociLincage_):LociLincage(LociLincage_){}
			loci_type meiosis(const loci_type& Loci1_,const loci_type& Loci2_){
				loci_type ParentChoice(hmLib::random::Engine());
				if(LociLincage){
					loci_type tmpLincage=LociLincage;
					loci_type Pos=1;
					do{
						if(LociLincage&Pos){
							ParentChoice=ParentChoice&(~Pos) | (ParentChoice<<1)&Pos;
							tmpLincage&=(~Pos);
						}
						Pos<<=1;
					}while(tmpLincage);
				}

				return (ParentChoice&Loci1_)|((~ParentChoice)&Loci2_);
			}
		};
	}
}
#
#endif
