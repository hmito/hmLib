#ifndef HMLIB_GEOMETRY_MATH_ROOT_INC
#define HMLIB_GEOMETRY_MATH_ROOT_INC 100
#
#include"../../math/axis.hpp"
#include"../point.hpp"
#include"../rectangle.hpp"
namespace hmLib{
	namespace plane_geometry{
		namespace math{
			template<typename fn, typename T, typename output_iterator>
			output_iterator search_root_grid(fn Fn, rectangle<T> r, point<unsigned int> div, output_iterator out){
				axis<T> xaxis(r.pmin.x,r.pmax.x,div.x+1);
				axis<T> yaxis(r.pmin.y,r.pmax.y,div.y+1);

				std::vector<unsigned char> Vec(xaxis.size(),0);
				for(unsigned int ix = 0l ix<xaxis.size()-1; ++ix){
					Vec[ix] = (Fn(xaxis[ix],yaxis[iy])>=0?1:0);
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

			namespace detail{
				struct bisect_stepper{

				};
				struct grid_holder{
					
				};
			}
		}
	}
}
#
#endif
