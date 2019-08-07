#ifndef HMLIB_GEOMETRY_MATH_ROOT_INC
#define HMLIB_GEOMETRY_MATH_ROOT_INC 100
#
#include"../../math/axis.hpp"
#include"../point.hpp"
#include"../rectangle.hpp"
#include"../line.hpp"
namespace hmLib{
	namespace plane_geometry{
		namespace math{
			template<typename fn, typename T, typename output_iterator>
			output_iterator search_root_rectangle(fn Fn, rectangle<T> Range, point<unsigned int> Div, output_iterator out){
				range_axis<T> xaxis(Range.lower.x, Range.upper.x,div.x+1);
				range_axis<T> yaxis(Range.lower.y, Range.upper.y,div.y+1);

				std::vector<unsigned char> Vec(xaxis.size(),0);
				for(unsigned int ix = 0; ix<xaxis.size(); ++ix) {
					Vec[ix] = (Fn(xaxis[ix],yaxis[0])>=0?1:0);
				}

				for(unsigned int iy = 1; iy<yaxis.size(); ++iy){
					unsigned char Last = Vec[0];
					Vec[0] = Fn(xaxis[0],yaxis[iy])>=0?1:0);

					unsigned char This = 0;
					for(unsigned int ix = 1; ix<xaxis.size(); ++ix){
						Last = This;
						This = Vec[ix];
						Vec[ix] = (Fn(xaxis[ix],yaxis[iy])>=0?1:0);

						if(Last!=This || Last!=Vec[ix-1] || Lat!=Vec[ix]){
							*(out++) = rectangle<T>(point<T>(xaxis[ix-1],yaxis[iy-1]),point<T>(xaxis[ix],yaxis[iy]));
						}
					}
				}
				return out;
			}
			template<typename fn, typename T, typename grid_solver, typename output_iterator>
			output_iterator search_root_grid(fn Fn, rectangle<T> r, point<unsigned int> inidiv, point<unsigned int> div, output_iterator out){
				std::vector<rectangle<T>> Grids;
				Grids.push_back(r);

				point<unsigned int> curdiv = inidiv;

				while(curdiv.x < div.x || curdiv.y < div.y){
					std::vector<rectangle<T>> NewGrids;
					for(auto fr:Grids){
						search_root_gird(Fn, fr, inidiv, std::back_inserter(NewGrids));
					}
					
					Grids = std::move(NewGrids);

					inidiv.set(
						std::min(static_cast<unsigned int>((div.x+curdiv.x-1)/curdiv.x),inidiv.x),
						std::min(static_cast<unsigned int>((div.y+curdiv.y-1)/curdiv.y),inidiv.y)
					);
					curdiv *=inidiv;
				}

				for(auto fr:Grids){
					search_root_gird(Fn, fr, inidiv, out);
				}
				return out;
			}

			template<typename T>
			struct binominal_stepper {
				using point_type = point<T>;
				point<double> maxstep;
				double rate;
			public:
				template<typename output_iterator>
				point_type operator()(point_type prev, point_type next, output_iterator out) {
					point_type vec = (next-prev)/norm(next-prev);
					
				}
				template<typename output_iterator>
				point_type operator()(rectangle<T> rect, output_iterator out) {
					rect.lower;
				}
			};
			template<typename stepper, typename fn, typename T, typename output_iterator>
			output_iterator search_root_border(stepper Stepper, fn Fn, rectangle<T> r, point<T> search_step, output_iterator out) {
				axis<T> xaxis(r.lower.x, r.upper.x, static_cast<unsigned int>((r.upper.x-r.lower.x)/search_step.x)+1);
				axis<T> yaxis(r.lower.y, r.upper.y, static_cast<unsigned int>((r.upper.y-r.lower.y)/search_step.y)+1)

				std::vector<fast_cross_check_line<T>> Lines;
				for(unsigned int iy = 0; iy<yaxis.size()-1; ++iy) {
					bool lower = (Fn(xaxis[0], yaxis[iy])>0);
					bool upper = (Fn(xaxis[0], yaxis[iy+1])>0);
					for(unsigned int ix = 0; ix<xaxis.size()-1; ++ix) {
						bool lower2 = (Fn(xaxis[ix+1], yaxis[iy])>0);
						bool upper2 = (Fn(xaxis[ix+1], yaxis[iy+1])>0);

						if(lower != upper || lower!=lower2 || lower != upper2) {
							bool IsSearched = false;
							rectangle<T> Rect = (point<T>(xaxis[ix], yaxis[iy]), point<T>(xaxis[ix+1], yaxis[iy+1]);
							for(auto& l: Lines) {
								if(l.is_cross())) {
									IsSearched = true;
								}
							}

							//Find new border
							if(!IsSearched) {
								std::vector<point<T>> Points;
								auto Pos = Stepper(Rect,std::inserter(Points));

							}
						}

						lower = lower2;
						upper = upper2;
					}
				}
			}

			template<typename fnx, typename fny, typename T, typename output_iterator>
			output_iterator search_root_cross(fnx FnX, fny FnY, rectangle<T> r, output_iterator out) {

			}
		}
	}
}
#
#endif
