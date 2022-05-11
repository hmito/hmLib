#ifndef HMLIB_NUMERIC_NELDERMEADMINIMA_INC
#define HMLIB_NUMERIC_NELDERMEADMINIMA_INC 100
#
#include<utility>
#include<vector>
#include<algorithm>
#include<cmath>
#include<iostream>
#include"numeric_result.hpp"
#include"simplex_precision_breaker.hpp"
namespace hmLib{
	namespace numeric{
		template<typename T>
		struct nelder_mead_minima_stepper{
		private:
			using point = std::vector<T>;
			struct vertex {
			private:
				point val;
				T fval;
			public:
				vertex() = default;
				vertex(point val_, T fval_) :val(std::move(val_)), fval(fval_) {}
				point& value(){ return val; }
				T& fvalue(){ return fval; }
				const point& value()const{ return val; }
				const T& fvalue()const { return fval; }
				friend bool operator==(const vertex& v1, const vertex& v2) { return v1.fvalue() == v2.fvalue(); }
				friend bool operator!=(const vertex& v1, const vertex& v2) { return v1.fvalue() != v2.fvalue(); }
				friend bool operator<(const vertex& v1, const vertex& v2) { return v1.fvalue() < v2.fvalue(); }
				friend bool operator<=(const vertex& v1, const vertex& v2) { return v1.fvalue() <= v2.fvalue(); }
				friend bool operator>(const vertex& v1, const vertex& v2) { return v1.fvalue() > v2.fvalue(); }
				friend bool operator>=(const vertex& v1, const vertex& v2) { return v1.fvalue() >= v2.fvalue(); }
			};
		public:
			struct state{
				friend struct nelder_mead_minima_stepper<T>;
			private:
				using container = std::vector<vertex>;
			public:
				using iterator = decltype(std::declval<container>().begin());
				using const_iterator = decltype(std::declval<container>().cbegin());
				using reference = vertex&;
				using const_reference = const vertex&;
			private:
				std::vector<vertex> Amoeba;
			public:
				state() = default;
				template<typename fn, typename error_type>
				state(fn Fn, const point& ini, error_type rel_value, error_type abs_value) {
					std::size_t size = std::distance(ini.begin(), ini.end());
					//initialize vertex
					Amoeba.assign(size + 1, vertex(ini, Fn(ini)));
					for (std::size_t i = 1; i < Amoeba.size(); ++i) {
						auto itr = std::next(Amoeba[i].value().begin(), i - 1);
						(*itr) += abs_value + rel_value * (*itr);
						Amoeba[i].fvalue() = Fn(Amoeba[i].value());
					}
				}
				point& value() {
					return std::min_element(Amoeba.begin(), Amoeba.end())->value();
				}
				T& fvalue() {
					return std::min_element(Amoeba.begin(), Amoeba.end())->fvalue();
				}
				const point& value()const{
					return std::min_element(Amoeba.begin(),Amoeba.end())->value();
				}
				const T& fvalue()const {
					return std::min_element(Amoeba.begin(),Amoeba.end())->fvalue();
				}
				iterator begin() { return Amoeba.begin(); }
				iterator end() { return Amoeba.end(); }
				const_iterator begin()const { return Amoeba.begin(); }
				const_iterator end()const { return Amoeba.end(); }
				const_iterator cbegin()const { return Amoeba.cbegin(); }
				const_iterator cend()const { return Amoeba.cend(); }
				reference at(unsigned int n) { return Amoeba.at(n); }
				const_reference at(unsigned int n)const { return Amoeba.at(n); }
				reference operator[](unsigned int n) { return Amoeba[n]; }
				const_reference operator[](unsigned int n)const { return Amoeba[n]; }
			};
		public:
			nelder_mead_minima_stepper()
				: alpha(1)
				, gamma(2)
				, rho(0.5)
				, sigma(0.5){
			}
			nelder_mead_minima_stepper(T alpha_, T gamma_, T rho_, T sigma_)
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
