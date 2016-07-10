#ifndef HMLIB_ODEINT_EQSTATEBREAKOBSERVER_INC
#define HMLIB_ODEINT_EQSTATEBREAKOBSERVER_INC 100
#
#include <array>
#include "container_observer.hpp"
#include "utility.hpp"
namespace hmLib{
	namespace odeint{
		template<typename state_>
		struct eqstate_from_past{
			template<typename iterator>
			bool operator()(const state_& x, iterator Beg, iterator End, double Error){
				for(auto itr = Beg; itr != End; ++itr){
					if(detail::abs_distance(x, itr->second) < Error){
						return true;
					}
				}
				return false;
			}
		};
		template<>
		struct eqstate_from_past < std::array<double, 2> > {
			using state = std::array<double, 2>;
		private:
			bool cross_segment(std::array<double, 2> a1, std::array<double, 2> a2, std::array<double, 2> b1, std::array<double, 2> b2){
				double ta = (b1[0] - b2[0]) * (a1[1] - b1[1]) + (b1[1] - b2[1]) * (b1[0] - a1[0]);
				double tb = (b1[0] - b2[0]) * (a2[1] - b1[1]) + (b1[1] - b2[1]) * (b1[0] - a2[0]);
				double tc = (a1[0] - a2[0]) * (b1[1] - a1[1]) + (a1[1] - a2[1]) * (a1[0] - b1[0]);
				double td = (a1[0] - a2[0]) * (b2[1] - a1[1]) + (a1[1] - a2[1]) * (a1[0] - b2[0]);

				return tc * td < 0 && ta * tb < 0;
			};
		public:
			template<typename iterator>
			bool operator()(const state& x, iterator Beg, iterator End, double Error){
				if(Beg == End)return false;

				state y = (End-1)->second;

				state p = Beg->second;
				++Beg;

				for(; Beg != End; ++Beg){
					if(cross_segment(x, y, p, Beg->second))return true;
					p = Beg->second;
				}
				return false;
			}
		};
		template<typename state_, typename time_=double>
		struct eqstate_break_observer: private container_observer<state_, time_>{
			using state = state_;
			using time = time_;
			using pair = std::pair<time, state>;
			using base_type = container_observer<state, time>;
			using this_type = eqstate_break_observer<state, time>;
			using iterator = typename base_type::iterator;
			using const_iterator = typename base_type::const_iterator;
			using reverse_iterator = typename base_type::reverse_iterator;
			using const_reverse_iterator = typename base_type::const_reverse_iterator;
		private:
			double Error;
			unsigned int Interval;
			unsigned int Cnt;
		public:
			eqstate_break_observer(double Error_, unsigned int Interval_):Error(Error_), Interval(Interval_), Cnt(0){}
			bool operator()(const state& x, time t){ 
				bool Ans = false;
				if(++Cnt >= Interval){
					Cnt = 0;
					Ans = eqstate_from_past<state>()(x, begin(), end(), Error);
				}

				base_type::operator()(x, t);
				return Ans;
			}
		public:
			pair& at(size_t Pos){ return base_type::at(Pos); }
			const pair& at(size_t Pos)const{ return base_type::at(Pos); }
			pair& operator[](size_t Pos){ return base_type::operator[](Pos); }
			const pair& operator[](size_t Pos)const{ return base_type::operator[](Pos); }
			bool empty()const{ return base_type::empty(); }
			void size()const{ return base_type::size(); }
			void reserve(size_t Size){ base_type::reserve(Size); }
		public:
			const container& get()const{ return base_type::get(); }
			container&& release(){ return base_type::release(); }
			void swap(container& Container){ base_type::swap(Container); }
			void clear(){
				Cnt = 0;
				base_type::clear();
			}
		public:
			iterator begin(){ return base_type::begin(); }
			iterator end(){ return base_type::end(); }
			const_iterator begin()const{ return base_type::begin(); }
			const_iterator end()const{ return base_type::end(); }
			const_iterator cbegin()const{ return base_type::begin(); }
			const_iterator cend()const{ return base_type::end(); }
			reverse_iterator rbegin(){ return base_type::rbegin(); }
			reverse_iterator rend(){ return base_type::rend(); }
			const_reverse_iterator rbegin()const{ return base_type::rbegin(); }
			const_reverse_iterator rend()const{ return base_type::rend(); }
			const_reverse_iterator crbegin()const{ return base_type::rbegin(); }
			const_reverse_iterator crend()const{ return base_type::rend(); }
		};
	}
}
#
#endif
