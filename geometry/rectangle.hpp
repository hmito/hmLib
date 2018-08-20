#ifndef HMLIB_GEOMETRY_RECTANGLE_INC
#define HMLIB_GEOMETRY_RECTANGLE_INC 100
#
#include"point.hpp"
namespace hmLib{
	namespace plane_geometry {
		template<typename T>
		struct rectangle {
		private:
			using rectangle = segment<T>;
		public:
			using point_type = point<T>;
		public:
			point_type lower;
			point_type upper;
		public:
			rectangle() = default;
			rectangle(const this_type&) = default;
			this_type& operator=(const this_type&) = default;
			rectangle(this_type&&) = default;
			this_type& operator=(this_type&&) = default;
		public:
			rectangle(point_type p1_, point_type p2_) : lower(min_element(p1_,p2_)), upper(max_element(p1_,p2_)) {}
			void set(point_type x_, point_type y_) {
				lower = min_element(p1_,p2_));
				upper = max_element(p1_,p2_));
			}
			template<typename U, typename std::enable_if<std::is_convertible<U, T>::value && !std::is_same<U,T>::value>::type*& = hmLib::utility::enabler>
			explicit rectangle(const rectangle<U>& Other) : lower(static_cast<T>(Other.lower)), upper(static_cast<T>(Other.upper)) {}
			template<typename U>
			operator rectangle<U>()const{
				return rectangle<U>(lower,upper);
			}
		public:
			rectangle<T> operator+()const { return *this; }
			rectangle<T> operator-()const { return rectangle<T>(-lower, -upper); }
			template<typename U, typename std::enable_if<std::is_convertible<decltype(std::declval<point_type>() + std::declval<U>()), point_type>::value>::type*& = hmLib::utility::enabler>
			rectangle<T>& operator+=(const point<U>& p) {
				lower += p;
				upper += p;
				return *this;
			}
			template<typename U, typename std::enable_if<std::is_convertible<decltype(std::declval<point_type>() - std::declval<U>()), point_type>::value>::type*& = hmLib::utility::enabler>
			rectangle<T>& operator-=(const point<U>& p) {
				lower -= p;
				upper -= p;
				return *this;
			}
			template<typename U, typename std::enable_if<std::is_convertible<decltype(std::declval<point_type>() * std::declval<U>()), point_type>::value>::type*& = hmLib::utility::enabler>
			rectangle<T>& operator*=(const point<U>& p) {
				lower *= p;
				upper *= p;
				return *this;
			}
			template<typename U, typename std::enable_if<std::is_convertible<decltype(std::declval<point_type>() / std::declval<U>()), point_type>::value>::type*& = hmLib::utility::enabler>
			rectangle<T>& operator/=(const point<U>& p) {
				lower /= p;
				upper /= p;
				return *this;
			}
			template<typename U, typename std::enable_if<std::is_convertible<decltype(std::declval<point_type>() * std::declval<U>()), point_type>::value>::type*& = hmLib::utility::enabler>
			rectangle<T>& operator*=(U val) {
				lower *= val;
				upper *= val;
				return *this;
			}
			template<typename U, typename std::enable_if<std::is_convertible<decltype(std::declval<point_type>() / std::declval<U>()), point_type>::value>::type*& = hmLib::utility::enabler>
			rectangle<T>& operator/=(U val) {
				lower /= val;
				upper /= val;
				return *this;
			}
		};
		template<typename T, typename U>
		bool operator==(const rectangle<T>& r1, const rectangle<U>& r2) {
			return(r1.lower == r2.lower && r1.upper == r2.upper);
		}
		template<typename T, typename U>
		bool operator!=(const rectangle<T>& r1, const rectangle<U>& r2) {
			return !(r1 == r2);
		}
		template<typename T, typename U>
		auto area(const rectangle<T>& r) {
			auto p = (r.upper-r.lower);
			return p.x*p.y;
		}
		template<typename T, typename U>
		bool is_inside(const point<U>& p, const rectangle<T>& of_r){
			return of_r.lower.x <= p.x && of_r.lower.y <= p.y && p.x < of_r.upper.x && p.y < of_r.upper.y;
		}
		template<typename T, typename U>
		bool is_inside(const segment<U>& s, const rectangle<T>& of_r) {
			return inside(of_r, s.p1) && inside(of_r, s.p2);
		}
		template<typename T, typename U>
		bool is_inside(const rectangle<T>& r, const rectangle<U>& of_r) {
			return of_r.lower.x <= r.lower.x && of_r.lower.y <= r.lower.y && r.upper.x <= of_r.upper.x && r.upper.y <= of_r.upper.y;
		}
		template<typename T, typename U>
		bool is_cross(const rectangle<T>& r, const rectangle<U>& with_r) {
			//four corners is out of with_r & with_r is not inside of r
			return is_inside(r.lower, with_r)
				|| is_inside(r.upper, with_r)
				|| is_inside(point<T>(r.lower.x, r.upper.y), with_r)
				|| is_inside(point<T>(r.upper.x, r.lower.y), with_r)
				|| is_insider(with_r, r);
		}
		template<typename T, typename U>
		bool is_cross(const segment<T>& s, const rectangle<U>& with_r) {
			if(!is_cross(rectangle<T>(s.p1, s.p2), with_r))return false;
			if(is_inside(s.p1, with_r) || is_inside(s.p2, with_r))return true;

			return is_cross(s, segment<U>(with_r.lower, point<U>(with_r.lower.x, with_r.upper.y)))
				|| is_cross(s, segment<U>(with_r.upper, point<U>(with_r.lower.x, with_r.upper.y)))
				|| is_cross(s, segment<U>(with_r.lower, point<U>(with_r.upper.x, with_r.lower.y)))
				|| is_cross(s, segment<U>(with_r.upper, point<U>(with_r.upper.x, with_r.lower.y)));
		}
		template<typename T, typename U>
		bool is_cross(const rectangle<U>& r, const segment<T>& with_s) {
			return is_cross(with_s, r);
		}
	}
}
#
#endif
