#ifndef HMLIB_MATH_PLANEROOT_INC
#define HMLIB_MATH_PLANEROOT_INC 100
#
#include<bitset>
#include<array>
#include<vector>
#include<unordered_set>
#include<utility>
#include<iterator>
#include<boost/math/tools/roots.hpp>
#include<boost/function_output_iterator.hpp>
#include"axis.hpp"
#include"../geometry.hpp"
namespace hmLib {
	namespace plane_root {
		template<typename T>
		using point = plane_geometry::point<T>;
		using grid_point = point<std::size_t>;
		namespace grid {
			template<typename func, typename T, typename grid_point_output_iterator>
			grid_point_output_iterator search_border_grid(func Fn, const hmLib::axis<T>& xaxis, const hmLib::axis<T>& yaxis, grid_point_output_iterator out) {
				//low-low, up-low, low-up, up-up
				using corner_info = std::bitset<4>;

				corner_info CInfo;
				for(std::size_t x = 0; x < xaxis.size() - 1; ++x) {
					CInfo[2] = Fn(xaxis[x], yaxis[0]) > 0;		//LL
					CInfo[3] = Fn(xaxis[x + 1], yaxis[0]) > 0;	//UL
					for(std::size_t y = 0; y < yaxis.size() - 1; ++y) {
						CInfo >>= 2;
						T yval = yaxis[y + 1];
						CInfo[2] = Fn(xaxis[x], yval) > 0;		//LU
						CInfo[3] = Fn(xaxis[x + 1], yval) > 0;	//UU

						if(CInfo != corner_info(0) && CInfo != corner_info(0xff)) {
							*(out++) = grid_point( x, y );
						}
					}
				}
				return out;
			}
			template<typename func, typename T, typename grid_point_output_iterator>
			grid_point_output_iterator search_border_grid(func Fn, hmLib::axis<T>& xaxis, hmLib::axis<T>& yaxis, point<T> error, grid_point_output_iterator out) {
				std::size_t xaxis_maxsize = std::max<std::size_t>(static_cast<std::size_t>(std::round(std::pow(2, std::floor(std::log2(xaxis.size()))))), 2);
				std::size_t yaxis_maxsize = std::max<std::size_t>(static_cast<std::size_t>(std::round(std::pow(2, std::floor(std::log2(yaxis.size()))))), 2);

				std::size_t xaxis_div = std::max<std::size_t>(static_cast<std::size_t>(std::round(std::pow(2, std::ceil(std::log2(xaxis.width() / error.x / xaxis.size()))))), 1);
				std::size_t yaxis_div = std::max<std::size_t>(static_cast<std::size_t>(std::round(std::pow(2, std::ceil(std::log2(yaxis.width() / error.y / yaxis.size()))))), 1);

				std::vector<grid_point> Cross;
				search_border_grid(Fn, xaxis, yaxis, std::back_inserter(Cross));

				while(xaxis_div > 1 && yaxis_div > 1) {
					std::size_t xaxis_curdiv = xaxis_div > xaxis_maxsize ? xaxis_maxsize : xaxis_div;
					std::size_t yaxis_curdiv = yaxis_div > yaxis_maxsize ? yaxis_maxsize : yaxis_div;
					xaxis_div /= xaxis_curdiv;
					yaxis_div /= yaxis_curdiv;
					xaxis.assign(xaxis.min(), xaxis.max(), (xaxis.size() - 1)*xaxis_curdiv + 1);
					yaxis.assign(yaxis.min(), yaxis.max(), (yaxis.size() - 1)*yaxis_curdiv + 1);

					std::vector<grid_point> NewCross;
					for(const auto& c : Cross) {
						search_border_grid(
							Fn,
							xaxis.subaxis(c.x * xaxis_curdiv, (c.x + 1) * xaxis_curdiv),
							yaxis.subaxis(c.y * yaxis_curdiv, (c.y + 1) * yaxis_curdiv),
							boost::make_function_output_iterator([&](const grid_point& v) {NewCross.push_back(grid_point(c.x * xaxis_curdiv + v.x, c.y * yaxis_curdiv + v.y)); })
						);
					}

					std::swap(Cross, NewCross);
				}

				for(const auto& v : Cross) {
					*(out++) = v;
				}
				return out;
			}
			template<typename func1, typename func2, typename T, typename grid_point_output_iterator>
			grid_point_output_iterator search_cross_grid(func1 Fn1, func2 Fn2, const hmLib::axis<T>& xaxis, const hmLib::axis<T>& yaxis, grid_point_output_iterator out) {
				//low-low, up-low, low-up, up-up
				using corner_info = std::bitset<4>;

				corner_info CInfo1;
				corner_info CInfo2;

				T y0val = yaxis[0];
				for(std::size_t x = 0; x < xaxis.size() - 1; ++x) {
					T lxval = xaxis[x];
					T uxval = xaxis[x + 1];
					CInfo1[2] = Fn1(lxval, y0val) > 0;		//LL
					CInfo1[3] = Fn1(uxval, y0val) > 0;	//UL
					CInfo2[2] = Fn2(lxval, y0val) > 0;		//LL
					CInfo2[3] = Fn2(uxval, y0val) > 0;	//UL
					for(std::size_t y = 0; y < yaxis.size() - 1; ++y) {
						CInfo1 >>= 2;
						CInfo2 >>= 2;

						T yval = yaxis[y + 1];
						CInfo1[2] = Fn1(lxval, yval) > 0;		//LU
						CInfo1[3] = Fn1(uxval, yval) > 0;	//UU
						CInfo2[2] = Fn2(lxval, yval) > 0;		//LU
						CInfo2[3] = Fn2(uxval, yval) > 0;	//UU

						if(CInfo1 != corner_info(0) && CInfo1 != corner_info(0xff) && CInfo2 != corner_info(0) && CInfo2 != corner_info(0xff)) {
							*(out++) = grid_point( x, y);
						}
					}
				}
				return out;
			}
			template<typename func1, typename func2, typename T, typename grid_point_output_iterator>
			grid_point_output_iterator search_cross_grid(func1 Fn1, func2 Fn2, hmLib::axis<T>& xaxis, hmLib::axis<T>& yaxis, point<T> error, grid_point_output_iterator out) {
				std::size_t xaxis_maxsize = std::max<std::size_t>(static_cast<std::size_t>(std::round(std::pow(2, std::floor(std::log2(xaxis.size()))))), 2);
				std::size_t yaxis_maxsize = std::max<std::size_t>(static_cast<std::size_t>(std::round(std::pow(2, std::floor(std::log2(yaxis.size()))))), 2);

				std::size_t xaxis_div = std::max<std::size_t>(static_cast<std::size_t>(std::round(std::pow(2, std::ceil(std::log2(xaxis.width() / error.x / xaxis.size()))))), 1);
				std::size_t yaxis_div = std::max<std::size_t>(static_cast<std::size_t>(std::round(std::pow(2, std::ceil(std::log2(yaxis.width() / error.y / yaxis.size()))))), 1);

				std::vector<grid_point> Cross;
				search_cross_grid(Fn1, Fn2, xaxis, yaxis, std::back_inserter(Cross));

				while(xaxis_div > 1 && yaxis_div > 1) {
					std::size_t xaxis_curdiv = xaxis_div > xaxis_maxsize ? xaxis_maxsize : xaxis_div;
					std::size_t yaxis_curdiv = yaxis_div > yaxis_maxsize ? yaxis_maxsize : yaxis_div;
					xaxis_div /= xaxis_curdiv;
					yaxis_div /= yaxis_curdiv;
					xaxis.assign(xaxis.min(), xaxis.max(), (xaxis.size() - 1)*xaxis_curdiv + 1);
					yaxis.assign(yaxis.min(), yaxis.max(), (yaxis.size() - 1)*yaxis_curdiv + 1);

					std::vector<grid_point> NewCross;
					for(const auto& c : Cross) {
						search_cross_grid(
							Fn1, Fn2,
							xaxis.subaxis(c.x * xaxis_curdiv, (c.x + 1) * xaxis_curdiv),
							yaxis.subaxis(c.y * yaxis_curdiv, (c.y + 1) * yaxis_curdiv),
							boost::make_function_output_iterator([&](const grid_point& v) {NewCross.push_back(grid_point(c.x * xaxis_curdiv + v.x, c.y * yaxis_curdiv + v.y)); })
						);
					}

					std::swap(Cross, NewCross);
				}

				for(const auto& v : Cross) {
					*(out++) = v;
				}
				return out;
			}

			namespace corners {
				enum direction_value :unsigned char { top_left = 1, top_right= 2, bottom_left= 4, bottom_right = 8 };
			}
			template<typename func, typename T>
			unsigned char get_grid_corners(func Fn, T xlower, T xupper, T ylower, T yupper) {
				return (Fn(xlower, ylower) > 0 ? corners::top_left : 0)
					+ (Fn(xupper, ylower) > 0 ? corners::top_right : 0)
					+ (Fn(xlower, yupper) > 0 ? corners::bottom_left : 0)
					+ (Fn(xupper, yupper) > 0 ? corners::bottom_right: 0);
			}

			namespace direction {
				enum direction_value :unsigned char { top = 1, bottom = 2, left = 4, right = 8 };
			}
			unsigned char corners_to_direction(unsigned char Corners) {
				if(Corners > 7)Corners = 15 - Corners;

				switch(Corners) {
				case 1:
					return direction::top | direction::left;
				case 2:
					return direction::top | direction::right;
				case 3:
					return direction::left | direction::right;
				case 4:
					return direction::left | direction::bottom;
				case 5:
					return direction::top | direction::bottom;
				case 6:
					return direction::top | direction::bottom | direction::left | direction::right;
				case 7:
					return direction::bottom | direction::right;
				default:
					return 0;
				}
			}

			template<typename func, typename T, typename point_output_iterator>
			point_output_iterator bisect_grid_border_point(func Fn, unsigned char CornerStatus, T xlower, T xupper, T ylower, T yupper, T xerror, T yerror, point_output_iterator out) {
				using boost::math::tools::bisect;

				auto s = get_grid_corner_status(Fn, xlower, xupper, ylower, yupper);
				if(s > 7)s = 15 - s;

				switch(s) {
				case 1:
					//Left-Top corner: LL is irregular
				{
					T x = xlower;
					auto ypair = bisect([x, &Fn](T y) {return Fn(x, y); }, ylower, yupper, [=](T v1, T v2) {return v2 - v1 < yerror; });
					T y = (ypair.first + ypair.second) / 2;
					*(out++) = point<T>( x,y );
				}

				{
					T y = ylower;
					auto xpair = bisect([y, &Fn](T x) {return Fn(x, y); }, xlower, xupper, [=](T v1, T v2) {return v2 - v1 < xerror; });
					T x = (xpair.first + xpair.second) / 2;
					*(out++) = point<T>(x, y);
				}
				return out;
				case 2:
					//Right-Top corner: UL is irregular
				{
					T x = xupper;
					auto ypair = bisect([x, &Fn](T y) {return Fn(x, y); }, ylower, yupper, [=](T v1, T v2) {return v2 - v1 < yerror; });
					T y = (ypair.first + ypair.second) / 2;
					*(out++) = point<T>(x, y);
				}

				{
					T y = ylower;
					auto xpair = bisect([y, &Fn](T x) {return Fn(x, y); }, xlower, xupper, [=](T v1, T v2) {return v2 - v1 < xerror; });
					T x = (xpair.first + xpair.second) / 2;
					*(out++) = point<T>(x, y);
				}
				return out;
				case 3:
					//Left-Right: LL-LU is irregular
				{
					T x = xlower;
					auto ypair = bisect([x, &Fn](T y) {return Fn(x, y); }, ylower, yupper, [=](T v1, T v2) {return v2 - v1 < yerror; });
					T y = (ypair.first + ypair.second) / 2;
					*(out++) = point<T>(x, y);
				}
				{
					T x = xupper;
					auto ypair = bisect([x, &Fn](T y) {return Fn(x, y); }, ylower, yupper, [=](T v1, T v2) {return v2 - v1 < yerror; });
					T y = (ypair.first + ypair.second) / 2;
					*(out++) = point<T>(x, y);
				}
				return out;
				case 4:
					//Left-Bottom corner: LU is irregular
				{
					T x = xlower;
					auto ypair = bisect([x, &Fn](T y) {return Fn(x, y); }, ylower, yupper, [=](T v1, T v2) {return v2 - v1 < yerror; });
					T y = (ypair.first + ypair.second) / 2;
					*(out++) = point<T>(x, y);
				}

				{
					T y = yupper;
					auto xpair = bisect([y, &Fn](T x) {return Fn(x, y); }, xlower, xupper, [=](T v1, T v2) {return v2 - v1 < xerror; });
					T x = (xpair.first + xpair.second) / 2;
					*(out++) = point<T>(x, y);
				}
				return out;
				case 5:
					//Top-Bottom: LL-UL is irregular
				{
					T y = ylower;
					auto xpair = bisect([y, &Fn](T x) {return Fn(x, y); }, xlower, xupper, [=](T v1, T v2) {return v2 - v1 < xerror; });
					T x = (xpair.first + xpair.second) / 2;
					*(out++) = point<T>(x, y);
				}
				{
					T y = yupper;
					auto xpair = bisect([y, &Fn](T x) {return Fn(x, y); }, xlower, xupper, [=](T v1, T v2) {return v2 - v1 < xerror; });
					T x = (xpair.first + xpair.second) / 2;
					*(out++) = point<T>(x, y);
				}
				return out;
				case 6:
					//Cross: UL-LU is irregular
				{
					T x = xlower;
					auto ypair = bisect([x, &Fn](T y) {return Fn(x, y); }, ylower, yupper, [=](T v1, T v2) {return v2 - v1 < yerror; });
					T y = (ypair.first + ypair.second) / 2;
					*(out++) = point<T>(x, y);
				}
				{
					T x = xupper;
					auto ypair = bisect([x, &Fn](T y) {return Fn(x, y); }, ylower, yupper, [=](T v1, T v2) {return v2 - v1 < yerror; });
					T y = (ypair.first + ypair.second) / 2;
					*(out++) = point<T>(x, y);
				}
				{
					T y = ylower;
					auto xpair = bisect([y, &Fn](T x) {return Fn(x, y); }, xlower, xupper, [=](T v1, T v2) {return v2 - v1 < xerror; });
					T x = (xpair.first + xpair.second) / 2;
					*(out++) = point<T>(x, y);
				}
				{
					T y = yupper;
					auto xpair = bisect([y, &Fn](T x) {return Fn(x, y); }, xlower, xupper, [=](T v1, T v2) {return v2 - v1 < xerror; });
					T x = (xpair.first + xpair.second) / 2;
					*(out++) = point<T>(x, y);
				}

				return out;
				case 7:
				{
					T x = xupper;
					auto ypair = bisect([x, &Fn](T y) {return Fn(x, y); }, ylower, yupper, [=](T v1, T v2) {return v2 - v1 < yerror; });
					T y = (ypair.first + ypair.second) / 2;
					*(out++) = point<T>(x, y);
				}
				{
					T y = yupper;
					auto xpair = bisect([y, &Fn](T x) {return Fn(x, y); }, xlower, xupper, [=](T v1, T v2) {return v2 - v1 < xerror; });
					T x = (xpair.first + xpair.second) / 2;
					*(out++) = point<T>(x, y);
				}

				return out;
				default:
					return out;
				}
			}
		}

		template<typename T>
		struct border_info {
			struct potential_cross {
				point<T> Lower;
				point<T> Upper;
				std::vector<std::size_t> BordersNo;
			};
			std::vector<std::vector<point<T>>> Borders;
			std::vector<potential_cross> PotentialCross;
		public:
			void new_border(point<T> p) { Borders.push_back(std::vector<point<T>>(1, p)); }
			void new_border(point<T> p, unsigned int cross_no_) { Borders.push_back(std::vector<point<T>>(1, p)); }
			void extend_border(point<T> p) { Borders.back().push_back(p); }
			unsigned int add_potential_cross()
		};
		template<typename func, typename T>
		border_info<T> grid_border_search(func Fn, point<T> p1, point<T> p2, point<T> grid_density, point<T> error, std::size_t n=100) {
			//create axis
			hmLib::axis<T> xaxis(std::min(p1.x, p2.x), std::max(p1.x, p2.x), n);
			hmLib::axis<T> yaxis(std::min(p1.y, p2.y), std::max(p1.y, p2.y), n);

			std::unordered_set<grid_point> Grid;
			grid::search_border_grid(Fn, xaxis, yaxis, grid_density, std::back_inserter(Grid));
			std::sort(Grid.begin(), Grid.end());

			auto End = Grid.end();
			unsigned char PrevDirection = 0;

			border_info<T> BorderInfo;
			while(Grid.begin() != End) {
				auto g = End[-1];
				auto Directions = grid::corners_to_direction(grid::get_grid_corners(Fn, xaxis[g.x], xaxis[g.x + 1], yaxis[g.y], yaxis[g.y + 1]));

				//Remove previous direction
				Directions &= ~PrevDirection;

				//Skip when there is no direction
				if(Directions == 0) {
					--End;
					continue;
				}

				using boost::math::tools::bisect;
				switch(Directions) {
				case grid::direction::top:
					T y = yaxis[g.y];
					auto xpair = bisect([=, &Fn](double x){return Fn(x, y); }, xaxis[g.x], xaxis[g.x + 1], [=](double v1, double v2) {return v2 - v1 < error.x; });
					T x = (xpair.first + xpair.second) / 2;
					BorderInfo.extend_border(point<T>(x, y));
					break;
				case grid::direction::bottom:
					T y = yaxis[g.y + 1];
					auto xpair = bisect([=, &Fn](double x) {return Fn(x, y); }, xaxis[g.x], xaxis[g.x + 1], [=](double v1, double v2) {return v2 - v1 < error.x; });
					T x = (xpair.first + xpair.second) / 2;
					BorderInfo.extend_border(point<T>(x, y));
					break;
				case grid::direction::left:
					T x = xaxis[g.x];
					auto ypair = bisect([=, &Fn](double y) {return Fn(x, y); }, yaxis[g.y], yaxis[g.y + 1], [=](double v1, double v2) {return v2 - v1 < error.y; });
					T y = (ypair.first + ypair.second) / 2;
					BorderInfo.extend_border(point<T>(x, y));
					break;
				case grid::direction::right:
					T x = xaxis[g.x + 1];
					auto ypair = bisect([=, &Fn](double y) {return Fn(x, y); }, yaxis[g.y], yaxis[g.y + 1], [=](double v1, double v2) {return v2 - v1 < error.y; });
					T y = (ypair.first + ypair.second) / 2;
					BorderInfo.extend_border(point<T>(x, y));
					break;
				default:

				}

				std::array<point<T>, 4> dp1;


				auto s1 = std::distance(dp1.begin(), grid::bisect_grid_border_point(Fn1, , error.x, error.y, dp1.begin()));

			}

			for(const auto& g : Grid) {

				auto CornerStatus = grid::bisect_grid_border_point(Fn1, xaxis[g.x], xaxis[g.x + 1], yaxis[g.y], yaxis[g.y + 1], error.x, error.y, dp1.begin()));

				std::array<point<T>, 4> dp1;

				auto s1 = std::distance(dp1.begin(), grid::bisect_grid_border_point(Fn1, xaxis[g.x], xaxis[g.x + 1], yaxis[g.y], yaxis[g.y + 1], error.x, error.y, dp1.begin()));

				if(s1 == 0 || s2 == 0) {
					continue;
				} else if(s1 == 4 || s2 == 4) {
					*(out++) = point<T>((xaxis[g.x] + xaxis[g.x + 1]) / 2, (yaxis[g.y] + yaxis[g.y + 1]) / 2);

				} else if(plane_geometry::is_cross(plane_geometry::segment<T>(dp1[0], dp1[1]), plane_geometry::segment<T>(dp2[0], dp2[1]))) {
					*(out++) = point<T>((xaxis[g.x] + xaxis[g.x + 1]) / 2, (yaxis[g.y] + yaxis[g.y + 1]) / 2);
				}

			}
		}
		template<typename func1, typename func2, typename T, typename point_output_iterator>
		point_output_iterator grid_cross_search(func1 Fn1, func2 Fn2, point<T> p1, point<T> p2, point<T> grid_density, point<T> error, point_output_iterator out, std::size_t n=100) {
			std::vector<grid_point> Grid;

			hmLib::axis<T> xaxis(std::min(p1.x, p2.x), std::max(p1.x, p2.x), n);
			hmLib::axis<T> yaxis(std::min(p1.y, p2.y), std::max(p1.y, p2.y), n);

			grid::search_cross_grid(Fn1, Fn2, xaxis, yaxis, grid_density, std::back_inserter(Grid));

			for(const auto& g : Grid) {

			}
			for(const auto& g : Grid) {
				std::array<point<T>, 4> dp1;
				std::array<point<T>, 4> dp2;

				auto s1 = std::distance(dp1.begin(), grid::bisect_grid_border_point(Fn1, xaxis[g.x], xaxis[g.x + 1], yaxis[g.y], yaxis[g.y + 1], error.x, error.y, dp1.begin()));
				auto s2 = std::distance(dp2.begin(), grid::bisect_grid_border_point(Fn2, xaxis[g.x], xaxis[g.x + 1], yaxis[g.y], yaxis[g.y + 1], error.x, error.y, dp2.begin()));

				if(s1 == 0 || s2 == 0) {
					continue;
				} else if(s1 == 4 || s2 == 4) {
					*(out++) = point<T>((xaxis[g.x] + xaxis[g.x + 1]) / 2, (yaxis[g.y] + yaxis[g.y + 1]) / 2);

				}else if(plane_geometry::is_cross(plane_geometry::segment<T>(dp1[0], dp1[1]), plane_geometry::segment<T>(dp2[0], dp2[1]))) {
					*(out++) = point<T>((xaxis[g.x] + xaxis[g.x + 1]) / 2, (yaxis[g.y] + yaxis[g.y + 1]) / 2);
				}
			}
			return out;
		}
	}
}
#
#endif