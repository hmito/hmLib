#ifndef HMLIB_NUMERIC_SIMPLEX_INC
#define HMLIB_NUMERIC_SIMPLEX_INC 100
#
#include<cmath>
#include<iterator>
#include<vector>
#include"evalue.hpp"
namespace hmLib{
	namespace numeric{
		template<typename value_type, typename eval_type>
		struct esimplex{
			using pair = evalue<value_type,eval_type>
		private:
			using container = std::vector<vertex>;
		public:
			using iterator = decltype(std::declval<container>().begin());
			using const_iterator = decltype(std::declval<container>().cbegin());
			using reference = vertex&;
			using const_reference = const vertex&;
		private:
			std::vector<pair> Simplex;
		public:
			esimplex() = default;
			template<typename fn, typename input_iterator>
			esimplex(fn Fn, input_iterator Beg, input_iterator End){}
			template<typename fn, typename random_value_generator>
			esimplex(fn Fn, value_type inival, random_value_generator gen) {
				std::size_t size = std::distance(beg,end);
				//initialize vertex
				Simplex.reserve(size+1);
				Simplex.emplace_back(Fn, inival);
				//Simplex.assign(size + 1, vertex(ini, Fn(ini)));
				for (std::size_t i = 1; i < Simplex.size(); ++i) {
					auto val = inival;
					auto itr = std::next(Simplex[i].value().begin(), i - 1);
					(*itr) += abs_value + rel_value * (*itr);
					Simplex[i].fvalue() = Fn(Simplex[i].value());
				}
			}
			iterator begin() { return Simplex.begin(); }
			iterator end() { return Simplex.end(); }
			const_iterator begin()const { return Simplex.begin(); }
			const_iterator end()const { return Simplex.end(); }
			const_iterator cbegin()const { return Simplex.cbegin(); }
			const_iterator cend()const { return Simplex.cend(); }
			reference at(unsigned int n) { return Simplex.at(n); }
			const_reference at(unsigned int n)const { return Simplex.at(n); }
			reference operator[](unsigned int n) { return Simplex[n]; }
			const_reference operator[](unsigned int n)const { return Simplex[n]; }
			const_iterator minima()const { return std::min_element(Simplex.begin(),Simplex.end());}
		};
		template<typename error_type_>
		struct esimplex_precision_breaker{
			//following functions should be callable;
			// state_type::begin() : return begin iterator of the node array of the simplex
			// state_type::end() : return end iterator of the node array of simplex
			//  each node also should have begin/end for access value on each axis. 
			using error_type = error_type_;
		public:
			esimplex_precision_breaker() = delete;
			explicit esimplex_precision_breaker(error_type relative_error_)
				: relerr(relative_error_)
				, abserr(relative_error_/4){
			}
			esimplex_precision_breaker(error_type relative_error_, error_type absolute_error_)
				: relerr(relative_error_)
				, abserr(absolute_error_){
			}
			template<typename value_type, typename eval_type>
			bool operator()(const esimplex<value_type,eval_type>& x,step_type)const{
				unsigned int size = std::distance(x.begin(), x.end()) - 1;
				auto minima = x.minima();
				for (unsigned int i = 0; i < size; ++i) {
					auto nitr = x.begin(); 
					auto lower = (nitr->v)[i];
					auto upper = (nitr->v)[i];
					++nitr;
					for(;nitr != x.end(); ++nitr){
						lower = std::min(lower,(nitr->v)[i]);
						upper = std::max(upper,(nitr->v)[i]);
					}

					if(std::abs(minima.v[i] - (upper + lower) / 2) + (upper - lower) / 2 > (relerr * std::abs(minima.v[i]) + abserr / 4) * 2){
						return false;
					}
				}
				return true;
			}
		private:
			error_type relerr;
			error_type abserr;
		};
	}
}