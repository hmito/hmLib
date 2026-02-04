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
	template<typename trait_type>
	struct real_mutator {
	private:
		trait_type sigma;
	public:
		real_mutator(trait_type sigma_) :sigma(sigma_) {}
		template<typename RandEngine>
		trait_type operator()(trait_type x_, RandEngine Engine_)const {
			return std::normal_distribution<trait_type>(x_, sigma)(Engine_);
		}
	};
	template<typename trait_type>
	struct positive_clamp_mutator {
	private:
		trait_type sigma;
	public:
		positive_clamp_mutator(trait_type sigma_) :sigma(sigma_) {}
		template<typename RandEngine>
		trait_type operator()(trait_type x_, RandEngine Engine_)const {
			return std::max(0., std::normal_distribution<trait_type>(x_, sigma)(Engine_));
		}
	};
	template<typename trait_type>
	struct positive_recreate_mutator {
	private:
		trait_type sigma;
	public:
		positive_recreate_mutator(trait_type sigma_) :sigma(sigma_) {}
		template<typename RandEngine>
		trait_type operator()(trait_type x_, RandEngine Engine_)const {
			while (true) {
				trait_type ans = std::normal_distribution<trait_type>(x_, sigma)(Engine_);
				if (0 <= ans)return ans;
			}
		}
	};
	template<typename trait_type>
	struct range_clamp_mutator {
	private:
		trait_type lower;
		trait_type upper;
		trait_type sigma;
	public:
		range_clamp_mutator(trait_type lower_, trait_type upper_, trait_type sigma_) :lower(lower_), upper(upper_), sigma(sigma_) {}
		template<typename RandEngine>
		trait_type operator()(trait_type x_, RandEngine Engine_)const {
			return std::clamp(std::normal_distribution<trait_type>(x_, sigma)(Engine_), lower, upper);
		}
	};
	template<typename trait_type>
	struct range_recreate_mutator {
	private:
		trait_type lower;
		trait_type upper;
		trait_type sigma;
	public:
		range_recreate_mutator(trait_type lower_, trait_type upper_, trait_type sigma_) :lower(lower_), upper(upper_), sigma(sigma_){}
		template<typename RandEngine>
		trait_type operator()(trait_type x_, RandEngine Engine_)const {
			while (true) {
				trait_type ans = std::normal_distribution<trait_type>(x_, sigma)(Engine_);
				if (lower <= ans && ans <= upper)return ans;
			}
		}
	};
}}
#
#endif
