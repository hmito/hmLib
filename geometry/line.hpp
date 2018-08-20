#ifndef HMLIB_GEOMETRY_LINE_INC
#define HMLIB_GEOMETRY_LINE_INC
#
#include<vector>
#include"point.hpp"
#include"segment.hpp"
#include"rectangle.hpp"
namespace hmLib {
	namespace plane_geometry {
		template<typename T>
		using line = std::vector<point<T>>;

		template<typename T>
		struct fast_cross_check_line {
		private:
			using this_type = fast_cross_check_line<T>;
		public:
			using const_iterator = line::const_iterator;
			using const_reverse_iterator = line::const_reverse_iterator;
		private:
			line<T> Line;
			std::vector<rectangle<T>> Rect;
			unsigned int Step;
		public:
			fast_cross_check_line() = default;
			fast_cross_check_line(unsigned int Step_):Step(Step_) {}
			void push_back(point<T> p) {
				Line.push_back(p);

				if(Rect.empty()) {
					Rect.back().lower = min_element(Rect.back().lower, p);
					Rect.back().upper = max_element(Rect.back().upper, p);
				} else {
					Rect.back().lower = min_element(Rect.back().lower, p);
					Rect.back().upper = max_element(Rect.back().upper, p);

					if((Line.size()-1)%Step == 0) {
						Rect.emplace_back(p, p);
					}
				}
			}
			template<typename input_iterator>
			void push_back(input_iterator beg, input_iterator end) {
				unsigned int pos = Line.size();
				Line.insert(Line.end(), beg,end);

				for(; beg!=end; ++beg, ++pos) {
					if(Rect.empty()) {
						Rect.back().lower = min_element(Rect.back().lower, *beg);
						Rect.back().upper = max_element(Rect.back().upper, *beg);
					} else {
						Rect.back().lower = min_element(Rect.back().lower, *beg);
						Rect.back().upper = max_element(Rect.back().upper, *beg);

						if(pos%Step == 0) {
							Rect.emplace_back(*beg, *beg);
						}
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
					if(Rect.empty()) {
						Rect.back().lower = min_element(Rect.back().lower, Line[i]);
						Rect.back().upper = max_element(Rect.back().upper, Line[i]);
					} else {
						Rect.back().lower = min_element(Rect.back().lower, Line[i]);
						Rect.back().upper = max_element(Rect.back().upper, Line[i]);

						if(i%Step == 0) {
							Rect.emplace_back(Line[i], Line[i]);
						}
					}
				}
			}
			void assign(line<T> Line_) {
				Line = std::move(Line_);
				reset_step(Step);
			}
			void assign(line<T> Line_, unsigned int Step_) {
				Line = std::move(Line_);
				reset_step(Step_);
			}
			line<T> release() {
				line<T> Ans;
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
						unsigned int End = std::min(Line.size()-1, i*Step+Step);
						for(unsigned int j = i*Step; j<End; ++j) {
							if(is_cross(s, segment<T>(Line[i], Line[i+1]))) {
								return std::next(begin(), j);
							}
						}
					}
				}
				return end();
			}
			const_iterator find_cross(const rectangle<T>& r)const {
				for(unsigned int i = 0; i<Rect.size(); ++i) {
					if(is_cross(r, Rect[i])) {
						unsigned int End = std::min(Line.size()-1, i*Step+Step);
						for(unsigned int j = i*Step; j<End; ++j) {
							if(is_cross(r, segment<T>(Line[i], Line[i+1]))) {
								return std::next(begin(), j);
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
		}
	}
}
#
#endif
