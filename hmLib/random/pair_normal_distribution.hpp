#ifndef HMLIB_RANDOM_PAIRNORMALDISTRIBUTION_INC
#define HMLIB_RANDOM_PAIRNORMALDISTRIBUTION_INC 100
#
#include<limits>
#include<random>
namespace hmLib{
	template<typename type_ = double>
	struct pair_normal_distribution{
	public:
		using type = type_;
		using result_type = std::pair<type, type>;
		struct param_type{
			type e1;
			type e2;
			type sigma1;
			type sigma2;
			type cov;
		public:
			param_type() = default;
			param_type(type mean1_, type mean2_, type sigma1_, type sigma2_, type cov_)
				: e1(mean1_)
				, e2(mean2_)
				, sigma1(sigma1_)
				, sigma2(sigma2_)
				, cov(cov){
			}
		};
	private:
		using this_type = pair_normal_distribution<type>;
	private:
		param_type Param;
		std::normal_distribution<type> Dist;
	public:
		pair_normal_distribution() = delete;
		pair_normal_distribution(type mean1_ = 0.0, type mean2_ = 0.0, type sigma1_ = 1.0, type sigma2_ = 1.0, type cov_ = 0.0)
			: Param(mean1_, mean2_, sigma1_, sigma2_, cov_)
			, Dist(0.0, 1.0){
		}
		pair_normal_distribution(const this_type&) = default;
		pair_normal_distribution(this_type&&) = default;
		this_type& operator=(const this_type&) = default;
		this_type& operator=(this_type&&) = default;
	public:
		template<typename URNG>
		result_type operator()(URNG& Urng){ return operator()(Urng, Param); }
		template<typename URNG>
		result_type operator()(URNG& Urng, param_type Param_){
			type X = Dist(Urng);
			type Y = Dist(Urng);
			type Z = Param_.cov * X + std::sqrt(1 - Param_.cov*Param_.cov)*Y;

			return result_type(Param_.e1 + Param_.sigma1*X, Param_.e2 + Param_.sigma2*Z);
		}
	public:
		void reset(){ Dist.reset(); }
		type mean1()const{ return Param.mean1; }
		type mean2()const{ return Param.mean2; }
		type stddev1()const{ return Param.sigma1; }
		type stddev2()const{ return Param.sigma2; }
		type cov()const{ return Param.cov; }
		param_type param()const{ return Param; }
		void param(const param_type& Param_){ Param = Param_; }
		result_type min()const{ return std::pair<type, type>(std::numeric_limits<type>::lowest(), std::numeric_limits<type>::lowest()); }
		result_type max()const{ return std::pair<type, type>(std::numeric_limits<type>::max(), std::numeric_limits<type>::max()); }
	};
}
#
#endif
