#ifndef HMLIB_NUMERIC_ESIMPLEX_INC
#define HMLIB_NUMERIC_ESIMPLEX_INC 100
#
#include<cmath>
#include<iterator>
#include<vector>
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
			using iterator = decltype(std::declval<container>().begin());
			using const_iterator = decltype(std::declval<container>().cbegin());
			using reference = evalue_type&;
			using const_reference = const evalue_type&;
		private:
			container Simplex;
		public:
			esimplex() = default;
			template<typename input_evalue_type_iterator>
			esimplex(input_evalue_type_iterator Beg, input_evalue_type_iterator End):Simplex(Beg,End){
				for(auto& evalue_type : Simplex){
					hmLib_assert_any(evalue_type.value().size()==Simplex.size()-1, "size of evalue_type in esimplex is incorrect.");
				}
			}
			template<typename fn>
			esimplex(fn Fn, const std::vector<elem_type>& inival, elem_type rel_value, elem_type abs_value){
				std::size_t size = inival.size();
				//initialize evalue_type
				Simplex.assign(size + 1, evalue_type(inival));
				for (std::size_t i = 0; i < Simplex.size(); ++i) {
					if(i < size ){
						Simplex[i].v[i] += rel_value*Simplex[i].v[i] + abs_value;
					}
					Simplex[i].e = Fn(Simplex[i].v);
				}
			}
			template<typename fn, typename input_value_iterator>
			esimplex(fn Fn, input_value_iterator vBeg, input_value_iterator vEnd, elem_type rel_value, elem_type abs_value){
				std::size_t size = std::distance(vBeg, vEnd);
				//initialize evalue_type
				Simplex.assign(size + 1, evalue_type(std::vector<elem_type>(vBeg,vEnd)));
				for (std::size_t i = 0; i < Simplex.size(); ++i) {
					if(i < size ){
						Simplex[i].value()[i] += rel_value*Simplex[i].v[i] + abs_value;
					}
					Simplex[i].fvalue() = Fn(Simplex[i].value());
				}
			}
			template<typename fn,typename urbg>
			esimplex(fn Fn, const std::vector<elem_type>& inival, elem_type rel_value, elem_type abs_value, urbg URBG){
				std::size_t size = inival.size();
				//initialize evalue_type
				Simplex.assign(size + 1, evalue_type(inival));
				for (std::size_t i = 0; i < Simplex.size(); ++i) {
					for(std::size_t j = 0; j < size; ++j){
						Simplex[i].value()[j] += (rel_value*Simplex[i].v[j] + abs_value)*std::uniform_int_distribution(0,1)(URBG);
					}
					Simplex[i].fvalue() = Fn(Simplex[i].value());
				}
			}
			template<typename fn, typename input_value_iterator,typename urbg>
			esimplex(fn Fn, input_value_iterator vBeg, input_value_iterator vEnd, elem_type rel_value, elem_type abs_value, urbg URBG){
				std::size_t size = std::distance(vBeg, vEnd);
				//initialize evalue_type
				Simplex.assign(size + 1, evalue_type(std::vector<elem_type>(vBeg,vEnd)));
				for (std::size_t i = 0; i < Simplex.size(); ++i) {
					for(std::size_t j = 0; j < size; ++j){
						Simplex[i].value()[j] += (rel_value*Simplex[i].v[j] + abs_value)*std::uniform_int_distribution(0,1)(URBG);
					}
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
	}
}
#
#endif
