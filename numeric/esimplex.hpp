#ifndef HMLIB_NUMERIC_ESIMPLEX_INC
#define HMLIB_NUMERIC_ESIMPLEX_INC 100
#
#include<cmath>
#include<iterator>
#include<vector>
#include<ranges>
#include"../exceptions.hpp"
#include"evalue.hpp"
namespace hmLib{
	namespace numeric{
		template<typename elem_type_, typename eval_type_>
		struct esimplex{
			using elem_type = elem_type_;
			using value_type = std::vector<elem_type_>;
			using eval_type = eval_type_;
			using evalue_type = evalue<value_type,eval_type_>;
		private:
			using container = std::vector<evalue_type>;
		public:
			using size_type = container::size_type; 
			using iterator = decltype(std::declval<container>().begin());
			using const_iterator = decltype(std::declval<container>().cbegin());
			using reference = evalue_type&;
			using const_reference = const evalue_type&;
		private:
			container Simplex;
		public:
			esimplex() = default;
			template<std::ranges::input_range evalue_range>
			esimplex(evalue_range&& Range):Simplex(std::forward<evalue_range>(Range)){
				for(auto& evalue : Simplex){
					hmLib_assert_any(evalue.value().size()==Simplex.size()-1, "size of evalue_type in esimplex is incorrect.");
				}
			}
			template<typename fn, std::ranges::input_range value_range>
			esimplex(fn Fn, value_range&& inival, elem_type rel_value, elem_type abs_value){
				std::size_t size = inival.size();
				//initialize evalue_type
				Simplex.assign(size + 1, evalue_type(inival));
				for (std::size_t i = 0; i < size; ++i) {
					Simplex[i].v[i] += rel_value*Simplex[i].v[i] + abs_value;
					Simplex[i].e = Fn(Simplex[i].v);
				}
				Simplex[size].e = Fn(Simplex[size].v);
			}
			template<typename fn, std::ranges::input_range value_range, typename urbg>
			esimplex(fn Fn, value_range&& inival, elem_type rel_value, elem_type abs_value, urbg URBG,bool AlowNegative=false){
				auto dist = std::uniform_int_distribution(AlowNegative?-1:0,1);

				std::size_t size = inival.size();
				//initialize evalue_type
				Simplex.assign(size + 1, evalue_type(inival));
				for (std::size_t i = 0; i < Simplex.size(); ++i) {
					for(std::size_t j = 0; j < size; ++j){
						Simplex[i].v[j] += (rel_value*Simplex[i].v[j] + abs_value)*dist(URBG);
					}
					Simplex[i].e = Fn(Simplex[i].v);
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

	}
}
#
#endif
