#ifndef HMLIB_GEOMETRY_LINE_INC
#define HMLIB_GEOMETRY_LINE_INC
#
#include<vector>
#include"point.hpp"
namespace hmLib {
	namespace plane_geometry {
		template<typename T>
		using line = std::vector<point<T>>;
		
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
