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
			point_type pmin;
			point_type pmax;
		public:
			rectangle() = default;
			rectangle(const this_type&) = default;
			this_type& operator=(const this_type&) = default;
			rectangle(this_type&&) = default;
			this_type& operator=(this_type&&) = default;
		public:
			rectangle(point_type p1_, point_type p2_) : pmin(min_element(p1_,p2_)), pmax(max_element(p1_,p2_)) {}
			void set(point_type x_, point_type y_) {
				pmin = min_element(p1_,p2_));
				pmax = max_element(p1_,p2_));
			}
			template<typename U, typename std::enable_if<std::is_convertible<U, T>::value && !std::is_same<U,T>::value>::type*& = hmLib::utility::enabler>
			explicit rectangle(const rectangle<U>& Other) : pmin(static_cast<T>(Other.pmin)), pmax(static_cast<T>(Other.pmax)) {}
			template<typename U>
			operator rectangle<U>()const{
				return rectangle<U>(pmin,pmax);
			}
		public:
			rectangle<T> operator+()const { return *this; }
			rectangle<T> operator-()const { return rectangle<T>(-pmin, -pmax); }
			template<typename U, typename std::enable_if<std::is_convertible<decltype(std::declval<point_type>() + std::declval<U>()), point_type>::value>::type*& = hmLib::utility::enabler>
			rectangle<T>& operator+=(const point<U>& p) {
				pmin += p;
				pmax += p;
				return *this;
			}
			template<typename U, typename std::enable_if<std::is_convertible<decltype(std::declval<point_type>() - std::declval<U>()), point_type>::value>::type*& = hmLib::utility::enabler>
			rectangle<T>& operator-=(const point<U>& p) {
				pmin -= p;
				pmax -= p;
				return *this;
			}
			template<typename U, typename std::enable_if<std::is_convertible<decltype(std::declval<point_type>() * std::declval<U>()), point_type>::value>::type*& = hmLib::utility::enabler>
			rectangle<T>& operator*=(const point<U>& p) {
				pmin *= p;
				pmax *= p;
				return *this;
			}
			template<typename U, typename std::enable_if<std::is_convertible<decltype(std::declval<point_type>() / std::declval<U>()), point_type>::value>::type*& = hmLib::utility::enabler>
			rectangle<T>& operator/=(const point<U>& p) {
				pmin /= p;
				pmax /= p;
				return *this;
			}
			template<typename U, typename std::enable_if<std::is_convertible<decltype(std::declval<point_type>() * std::declval<U>()), point_type>::value>::type*& = hmLib::utility::enabler>
			rectangle<T>& operator*=(U val) {
				pmin *= val;
				pmax *= val;
				return *this;
			}
			template<typename U, typename std::enable_if<std::is_convertible<decltype(std::declval<point_type>() / std::declval<U>()), point_type>::value>::type*& = hmLib::utility::enabler>
			rectangle<T>& operator/=(U val) {
				pmin /= val;
				pmax /= val;
				return *this;
			}
		};
		template<typename T, typename U>
		bool operator==(const rectangle<T>& r1, const rectangle<U>& r2) {
			return(r1.pmin == r2.pmin && r1.pmax == r2.pmax);
		}
		template<typename T, typename U>
		bool operator!=(const rectangle<T>& r1, const rectangle<U>& r2) {
			return !(r1 == r2);
		}
		template<typename T, typename U>
		auto area(const rectangle<T>& r) {
			auto p = (r.pmax-r.pmin);
			return p.x*p.y;
		}
		template<typename T, typename U>
		bool inside(const rectangle<T>& r, const point<U>& p){
			return r.pmin.x <= p.x && r.pmin.y <= p.y && p.x < r.pmax.x && p.y < r.pmax.y;
		}
	}
}
#
#endif
