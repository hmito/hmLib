#ifndef HMLIB_NUMERIC_NELDERMEADMINIMA_INC
#define HMLIB_NUMERIC_NELDERMEADMINIMA_INC 100
#
#include<utility>
#include<vector>
#include<algorithm>
#include<cmath>
#include<iostream>
#include"evalpair.hpp"
#include"evalsimplex.hpp"
#include"numeric_result.hpp"
namespace hmLib{
	namespace numeric{
		template<typename value_type,typename evalue_type>
		struct nelder_mead_minima_stepper{
		private:
			using pair = evalpair<value_type,evalue_type>;
			using state = evalsimplex<value_type,evalue_type>;
			using this_type = nelder_mead_minima_stepper<value_type,evalue_type>;
		public:
			nelder_mead_minima_stepper()
				: alpha(1)
				, gamma(2)
				, rho(0.5)
				, sigma(0.5){
			}
			nelder_mead_minima_stepper(value_type alpha_, value_type gamma_, value_type rho_, value_type sigma_)
				: alpha(alpha_)
				, gamma(gamma_)
				, rho(rho_)
				, sigma(sigma_){
			}
			template<typename fn>
			void operator()(fn Fn, state& State)const {
				std::size_t n = std::distance(State.begin(),State.end())-1;

				//sort by the order of the function
				std::sort(State.begin(),State.end());

				//calculate centroid of all points except State[n]
				vertex Center = State[0];
				for(std::size_t i=0; i < n; ++i){
					for(std::size_t j = 1; j < n; ++j){
						Center.value()[i] += State[j].value()[i];
					}
					Center.value()[i] /= n;
				}
				Center.fvalue() = Fn(Center.value());

				//calculate rerected point, which is located on the opposite side of the Center from the worst point
				auto Refrect = Center;
				for(std::size_t i=0; i < n; ++i){
					Refrect.value()[i] += alpha*(Center.value()[i] - State[n].value()[i]);
				}
				Refrect.fvalue() = Fn(Refrect.value());
				
				if(Refrect.fvalue() < State[0].fvalue()){
					//if the rerected point is best, calculate expanded point,
					//	try another type of point: expanded point, which is farer than the refrected point from the center
					//	(gamma > 1 for this purpose)
					auto Expand = Center;
					for(std::size_t i=0; i < n; ++i){
						Expand.value()[i] += gamma*(Refrect.value()[i] - Center.value()[i]);
					}
					Expand.fvalue() = Fn(Expand.value());

					if(Expand.fvalue() < Refrect.fvalue()){
						//if the expanded point is best,
						//	replace the worst point by the expanded point
						State[n] = Expand;
					}else{
						//otherwise, replace the worst point by the expanded point
						State[n] = Refrect;
					}
				}else if(State[0].fvalue() <= Refrect.fvalue() && Refrect.fvalue() < State[n-1].fvalue()){
					//if the refrected point is not best but better than worst point
					State[n] = Refrect;
				}else{
					//if the refrected point is the worst
					//	try another type of point: contracted point, which is located between the worst point and the center
					auto Contracted = Center;
					for(std::size_t i=0; i < n; ++i){
						Contracted.value()[i] += rho*(State[n].value()[i] - Center.value()[i]);
					}				
					Contracted.fvalue() = Fn(Contracted.value());
					if(Contracted.fvalue() < State[n].fvalue()){
						//if the contracted point is better than the worst, replace
						State[n] = Contracted;
					}else{
						//otherwise, all points shrink to the best point.
						for(std::size_t j = 1; j < n+1; ++j){
							for(std::size_t i=0; i < n; ++i){
								State[j].value()[i] = State[0].value()[i] + sigma*(State[j].value()[i] - State[0].value()[i]);
							}
							State[j].fvalue() = Fn(State[j].value());
						}
					}
				}
			}
		private:
			double alpha;
			double gamma;
			double rho;
			double sigma;
		};
		template<typename fn, typename vect, typename breaker>
		auto breakable_nelder_mead_minima(fn Fn, vect ini, unsigned int maxitr, breaker Brk){
			using stepper = nelder_mead_minima_stepper<decltype(std::declval<fn>()(std::declval<vect>()))>;
			using state =typename stepper::state;
			stepper Stepper;
			state State(Fn, ini, 0.2, 0.001);

			for(unsigned int i = 0; i<maxitr; ++i){
				if(Brk(State, i))return std::make_pair(State, count_result(true, i));
				Stepper(Fn,State);
			}

			return std::make_pair(State, count_result(Brk(State,maxitr), maxitr));
		}
		template<typename fn, typename vect, typename breaker,typename observer>
		auto breakable_nelder_mead_minima(fn Fn, vect ini, unsigned int maxitr, breaker Brk, observer Obs){
			using stepper = nelder_mead_minima_stepper<decltype(std::declval<fn>()(std::declval<vect>()))>;
			using state =typename stepper::state;
			stepper Stepper;
			state State(Fn, ini, 0.2, 0.001);
			
			for(unsigned int i = 0; i<maxitr; ++i){
				if(Brk(State, i))return std::make_pair(State, count_result(true, i));
				Stepper(Fn,State);
				Obs(State,i);
			}

			return std::make_pair(State, count_result(Brk(State,maxitr), maxitr));
		}
		template<typename fn, typename vect, typename error_type>
		auto nelder_mead_minima(fn Fn, vect ini, unsigned int maxitr, error_type relerr, error_type abserr){
			return breakable_nelder_mead_minima(Fn, ini, maxitr,simplex_precision_breaker<error_type>(relerr,abserr));
		}
		template<typename fn, typename vect, typename error_type,typename observer>
		auto nelder_mead_minima(fn Fn, vect ini, unsigned int maxitr, error_type relerr, error_type abserr, observer obs){
			return breakable_nelder_mead_minima(Fn, ini, maxitr,simplex_precision_breaker<error_type>(relerr,abserr), obs);
		}
	}
}
#
#endif
