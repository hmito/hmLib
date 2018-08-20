#ifndef HMLIB_GEOMETRY_SEGMENT_INC
#define HMLIB_GEOMETRY_SEGMENT_INC
#
#include"point.hpp"
namespace hmLib {
	namespace plane_geometry {
		template<typename T>
		struct segment {
		private:
			using this_type = segment<T>;
		public:
			using point_type = point<T>;
		public:
			point_type p1;
			point_type p2;
		public:
			segment() = default;
			segment(const this_type&) = default;
			this_type& operator=(const this_type&) = default;
			segment(this_type&&) = default;
			this_type& operator=(this_type&&) = default;
		public:
			segment(point_type p1_, point_type p2_) : p1(p1_), p2(p2_) {}
			void set(point_type p1_, point_type p2_) {
				p1 = p1_;
				p2 = p2_;
			}
			template<typename U, typename std::enable_if<std::is_convertible<U, T>::value && !std::is_same<U,T>::value>::type*& = hmLib::utility::enabler>
			explicit segment(const segment<U>& Other) : p1(static_cast<point_type>(Other.p1)), p2(static_cast<point_type>(Other.p2)) {}
			template<typename U, typename std::enable_if<std::is_convertible<U, T>::value && !std::is_same<U,T>::value>::type*& = hmLib::utility::enabler>
			operator segment<U>()const{
				return segment<U>(p1, p2);
			}
		public:
			segment<T> operator+()const { return *this; }
			segment<T> operator-()const { return segment<T>(-p1, -p2); }
			template<typename U, typename std::enable_if<std::is_convertible<decltype(std::declval<point_type>() + std::declval<U>()), point_type>::value>::type*& = hmLib::utility::enabler>
			segment<T>& operator+=(const point<U>& p) {
				p1 += p;
				p2 += p;
				return *this;
			}
			template<typename U, typename std::enable_if<std::is_convertible<decltype(std::declval<point_type>() - std::declval<U>()), point_type>::value>::type*& = hmLib::utility::enabler>
			segment<T>& operator-=(const point<U>& p) {
				p1 -= p;
				p2 -= p;
				return *this;
			}
			template<typename U, typename std::enable_if<std::is_convertible<decltype(std::declval<point_type>() * std::declval<U>()), point_type>::value>::type*& = hmLib::utility::enabler>
			segment<T>& operator*=(const point<U>& p) {
				p1 *= p;
				p2 *= p;
				return *this;
			}
			template<typename U, typename std::enable_if<std::is_convertible<decltype(std::declval<point_type>() / std::declval<U>()), point_type>::value>::type*& = hmLib::utility::enabler>
			segment<T>& operator/=(const point<U>& p) {
				p1 /= p;
				p2 /= p;
				return *this;
			}
			template<typename U, typename std::enable_if<std::is_convertible<decltype(std::declval<point_type>() * std::declval<U>()), point_type>::value>::type*& = hmLib::utility::enabler>
			segment<T>& operator*=(U val) {
				p1 *= val;
				p2 *= val;
				return *this;
			}
			template<typename U, typename std::enable_if<std::is_convertible<decltype(std::declval<point_type>() / std::declval<U>()), point_type>::value>::type*& = hmLib::utility::enabler>
			segment<T>& operator/=(U val) {
				p1 /= val;
				p2 /= val;
				return *this;
			}
			void reverse() {std::swap(p1, p2);}
		};
		template<typename T, typename U>
		bool operator==(const segment<T>& s1, const segment<U>& s2) {
			return(s1.p1 == s2.p1 && s1.p2 == s2.p2);
		}
		template<typename T, typename U>
		bool operator!=(const segment<T>& s1, const segment<U>& s2) {
			return !(s1 == s2);
		}
		template<typename T, typename U>
		bool is_equal(const segment<T>& s1, const segment<U>& s2) {
			return (s1.p1 == s2.p1 && s1.p2 == s2.p2) || (s1.p1 == s2.p2 && s1.p2 == s2.p1);

		}
		template<typename T,typename U>
		bool is_cross(const segment<T>& s1, const segment<U>& s2) {
			if(s1.p1.x >= s1.p2.x) {
				if((s1.p1.x < s2.p1.x && s1.p1.x < s2.p2.x) || (s1.p2.x > s2.p1.x && s1.p2.x > s2.p2.x)) {
					return false;
				}
			} else {
				if((s1.p2.x < s2.p1.x && s1.p2.x < s2.p2.x) || (s1.p1.x > s2.p1.x && s1.p1.x > s2.p2.x)) {
					return false;
				}
			}

			if(s1.p1.y >= s1.p2.y) {
				if((s1.p1.y < s2.p1.y && s1.p1.y < s2.p2.y) || (s1.p2.y > s2.p1.y && s1.p2.y > s2.p2.y)) {
					return false;
				}
			} else {
				if((s1.p2.y < s2.p1.y && s1.p2.y < s2.p2.y) || (s1.p1.y > s2.p1.y && s1.p1.y > s2.p2.y)) {
					return false;
				}
			}

			if(
				((s1.p1.x - s1.p2.x) * (s2.p1.y - s1.p1.y) + (s1.p1.y - s1.p2.y) * (s1.p1.x - s2.p1.x)) *
				((s1.p1.x - s1.p2.x) * (s2.p2.y - s1.p1.y) + (s1.p1.y - s1.p2.y) * (s1.p1.x - s2.p2.x)) > 0
				) {
				return false;
			}
			if(
				((s2.p1.x - s2.p2.x) * (s1.p1.y - s2.p1.y) + (s2.p1.y - s2.p2.y) * (s2.p1.x - s1.p1.x)) *
				((s2.p1.x - s2.p2.x) * (s1.p2.y - s2.p1.y) + (s2.p1.y - s2.p2.y) * (s2.p1.x - s1.p2.x)) > 0
				) {
				return false;
			}
			return true;
		}
		template<typename T, typename U>
		auto length(const segment<T>& s) { return norm(s.p1 - s.p2); }
		template<typename T, typename U>
		auto length2(const segment<T>& s) { return norm2(s.p1 - s.p2); }
		namespace detail {
			template<typename T, typename U>
			auto line_function_value(const segment<T>& s, const point<U>& p) {
				return (s.p1.y-s.p2.y)*p.x - (s.p1.x-s.p2.x)*p.y + (s.p1.x*s.p2.y-s.p2.x*s.p1.y);
			}
		}
	}
}
#
#endif
