#ifndef HMLIB_BIO_MUTATOR_INC
#define HMLIB_BIO_MUTATOR_INC 100
#
#include <algorithm>
#include <random>
namespace hmLib{ namespace bio {
	template<typename mutator_type>
	struct stocastic_mutator {
	private:
		double rate;
		mutator_type Mutator;
	public:
		stocastic_mutator(double rate_, mutator_type Mutator_) :rate(rate_), Mutator(Mutator_) {}
		template<typename player_type, typename RandEngine>
		player_type operator()(player_type x_, RandEngine Engine_)const {
			std::uniform_real_distribution<double> Dist(0.0, 1.0);
			if (Dist(Engine_) < rate) {
				return Mutator(x_, Engine_);
			} else {
				return x_;
			}
		}
	};
	struct real_mutator {
	private:
		double sigma;
	public:
		real_mutator(double sigma_) :sigma(sigma_) {}
		template<typename RandEngine>
		double operator()(double x_, RandEngine Engine_)const {
			return std::normal_distribution<double>(x_, sigma)(Engine_);
		}
	};
	struct positive_clamp_mutator {
	private:
		double sigma;
	public:
		positive_clamp_mutator(double sigma_) :sigma(sigma_) {}
		template<typename RandEngine>
		double operator()(double x_, RandEngine Engine_)const {
			return std::max(0., std::normal_distribution<double>(x_, sigma)(Engine_));
		}
	};
	struct positive_recreate_mutator {
	private:
		double sigma;
	public:
		positive_recreate_mutator(double sigma_) :sigma(sigma_) {}
		template<typename RandEngine>
		double operator()(double x_, RandEngine Engine_)const {
			while (true) {
				double ans = std::normal_distribution<double>(x_, sigma)(Engine_);
				if (0 <= ans)return ans;
			}
		}
	};
	struct range_clamp_mutator {
	private:
		double lower;
		double upper;
		double sigma;
	public:
		range_clamp_mutator(double lower_, double upper_, double sigma_) :lower(lower_), upper(upper_), sigma(sigma_) {}
		template<typename RandEngine>
		double operator()(double x_, RandEngine Engine_)const {
			return std::clamp(std::normal_distribution<double>(x_, sigma)(Engine_), lower, upper);
		}
	};
	struct range_recreate_mutator {
	private:
		double lower;
		double upper;
		double sigma;
	public:
		range_recreate_mutator(double lower_, double upper_, double sigma_) :lower(lower_), upper(upper_), sigma(sigma_){}
		template<typename RandEngine>
		double operator()(double x_, RandEngine Engine_)const {
			while (true) {
				double ans = std::normal_distribution<double>(x_, sigma)(Engine_);
				if (lower <= ans && ans <= upper)return ans;
			}
		}
	};
}}
#
#endif
