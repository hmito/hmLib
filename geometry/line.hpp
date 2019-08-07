#ifndef HMLIB_GEOMETRY_LINE_INC
#define HMLIB_GEOMETRY_LINE_INC 100
#
#include<vector>
#include"point.hpp"
#include"segment.hpp"
#include"rectangle.hpp"
namespace hmLib {
	namespace plane_geometry {
		template<typename T>
		struct fast_cross_check_line {
		private:
			using this_type = fast_cross_check_line<T>;
		public:
			using base_line = std::vector<point<T>>;
			using rectset = std::vector<rectangle<T>>;
			using const_iterator = typename base_line::const_iterator;
			using const_reverse_iterator = typename base_line::const_reverse_iterator;
		private:
			base_line Line;
			rectset Rect;		//size of rectangle including Line[0:Step], Line[Step:2Step]
			unsigned int Step;
		public:
			fast_cross_check_line():Step(8) = default;
			fast_cross_check_line(const this_type&) = default;
			this_type& operator=(const this_type&) = default;
			fast_cross_check_line(this_type&&) = default;
			this_type& operator=(this_type&&) = default;
			explicit fast_cross_check_line(unsigned int Step_):Step(Step_) {}
		private:
			void push_back(point<T> p) {
				if (!Rect.empty()) {
					Rect.back().lower = min_element(Rect.back().lower, p);
					Rect.back().upper = max_element(Rect.back().upper, p);
				}
				if (Line.size() % Step == 0) {
					Rect.emplace_back(p, p);
				}
				Line.push_back(p);
			}
			template<typename input_iterator>
			void push_back(input_iterator Beg, input_iterator End) {
				unsigned int pos = Line.size();
				Line.insert(Line.end(), Beg, End);

				for(; Beg!=End; ++Beg, ++pos) {
					if (!Rect.empty()) {
						Rect.back().lower = min_element(Rect.back().lower, *Beg);
						Rect.back().upper = max_element(Rect.back().upper, *Beg);
					}
					if (Line.size() % Step == 0) {
						Rect.emplace_back(*Beg, *Beg);
					}
				}
			}
			void clear() {
				Rect.clear();
				Line.clear();
			}
			void reset_step(unsigned int Step_) {
				Step = Step_;

				Rect.clear();
				for(unsigned int i = 0; i<Line.size(); ++i) {
					if (!Rect.empty()) {
						Rect.back().lower = min_element(Rect.back().lower, Line[i]);
						Rect.back().upper = max_element(Rect.back().upper, Line[i]);
					}
					if (i % Step == 0) {
						Rect.emplace_back(Line[i], Line[i]);
					}
				}
			}
			template<typename input_iterator>
			void assign(input_iterator Beg, input_iterator End) {
				clear();
				push_back(Beg, End);
			}
			template<typename input_iterator>
			void assign(input_iterator Beg, input_iterator End, unsigned int Step_) {
				clear();
				reset_step(Step_)
				push_back(Beg, End);
			}
			void assign(base_line&& Line_) {assign(std::move(Line_), Step);}
			void assign(base_line&& Line_, unsigned int Step_) {
				Line = std::move(Line_);
				reset_step(Step_);
			}
			base_line release() {
				base_line Ans;
				std::swap(Ans, Line);
				Rect.clear();

				return Ans;
			}
		public:
			const point<T>& front()const { return Line.front(); }
			const point<T>& back()const { return Line.back(); }
			const point<T>& operator[](unsigned int n)const { return Line[n]; }
			const point<T>& at(unsigned int n)const { return Line.at(n); }
		public:
			const_iterator begin()const { return Line.begin(); }
			const_iterator end()const { return Line.end(); }
			const_iterator cbegin()const { return Line.begin(); }
			const_iterator cend()const { return Line.end(); }
			const_reverse_iterator rbegin()const { return Line.rbegin(); }
			const_reverse_iterator rend()const { return Line.rend(); }
			const_reverse_iterator crbegin()const { return Line.rbegin(); }
			const_reverse_iterator crend()const { return Line.rend(); }
		public:
			const_iterator find_cross(const segment<T>& s)const {
				for(unsigned int i = 0; i<Rect.size(); ++i) {
					if(is_cross(s, Rect[i])){
						unsigned int BegI = i * Step;
						unsigned int EndI = std::min((i+1)*Step, Line.size() - 1);
						for(; BegI<EndI; ++BegI) {
							if(is_cross(s, segment<T>(Line[BegI], Line[BegI+1]))) {
								return std::next(begin(), BegI);
							}
						}
					}
				}
				return end();
			}
			const_iterator find_cross(const rectangle<T>& r)const {
				for(unsigned int i = 0; i<Rect.size(); ++i) {
					if(is_cross(r, Rect[i])) {
						unsigned int BegI = i * Step;
						unsigned int EndI = std::min((i + 1) * Step, Line.size() - 1);
						for (; BegI < EndI; ++BegI) {
							if (is_cross(r, segment<T>(Line[BegI], Line[BegI + 1]))) {
								return std::next(begin(), BegI);
							}
						}
					}
				}
				return end();
			}
			bool is_cross(const segment<T>& s)const{
				return find_cross(s)!=end();
			}
			bool is_cross(const rectangle<T>& r)const {
				return find_cross(r)!=end();
			}
		};

		/*
		template<typename T, typename input_iterator, typename output_iterator>
		output_iterator connect_closest_points(input_iterator beg, input_iterator end, T connect_distance, output_iterator out){
			T lim = connect_distance*connect_distance;
			line OriginalLine(beg, end);
			auto OriginalEnd = OriginalLine.end();
			std::vector<std::pair<point<T>,point<T>>> Stack;
			
			while(!OriginalLine.empty()){
				line Line;
				if(!Stack.empty()){
					Line.push_back(Stack.back().first);
					Line.push_back(Stack.back().second);
					Stack.pop_back();
				}else{
					--OriginalEnd;
					Line.push_back(OriginalLine[std::distance(OriginalLine.begin(),OriginalEnd)]);
				}

				while(true){
					line Ans;
					OriginalEnd = std::remove_copy_if(OriginalLine.begin(),OriginalEnd, std::back_inserter(Ans), [p=Line.back()](const point_type<T>& fp){return norm2(p-fp)<=lim;});

					if(Ans.size()==0){
						*(out++) = Line;
						break;
					}
					if(Ans.size()>2){
						*(out++) = Line;
						for(auto p: Ans){
							Stack.emplace_back(Line.back(),p);
						}
						break;
					}

					Line.push_back(Ans.back());
				}
			}

			return out;
		}*/
	}
}
#
#endif
