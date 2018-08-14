#ifndef HMLIB_GEOMETRY_POINT_INC
#define HMLIB_GEOMETRY_POINT_INC 100
#include<cmath>
#include<type_traits>
#include"../utility.hpp"
#include"../algorithm/compare.hpp"
namespace hmLib{
	namespace plane_geometry {
		template<class T>
		struct point {
		private:
			using this_type = point<T>;
		public:
			T x;
			T y;
		public:
			point() = default;
			point(const this_type&) = default;
			this_type& operator=(const this_type&) = default;
			point(this_type&&) = default;
			this_type& operator=(this_type&&) = default;
		public:
			point(T x_, T y_) : x(x_), y(y_) {}
			void set(T x_, T y_) {
				x = x_;
				y = y_;
			}
			template<typename U, typename std::enable_if<std::is_convertible<U, T>::value && !std::is_same<U,T>::value>::type*& = hmLib::utility::enabler>
			explicit point(const point<U>& Other) : x(static_cast<T>(Other.x)), y(static_cast<T>(Other.y)) {}
			template<typename U, typename std::enable_if<std::is_convertible<U, T>::value && !std::is_same<U,T>::value>::type*& = hmLib::utility::enabler>
			operator point<U>()const {
				return point<U>(x,y);
			}
		public:
			point<T> operator+()const { return *this; }
			point<T> operator-()const { return point<T>(-x, -y); }
			template<typename U, typename std::enable_if<std::is_convertible<decltype(std::declval<T>() + std::declval<U>()), T>::value>::type*& = hmLib::utility::enabler>
			point<T>& operator+=(const point<U>& Other) {
				x += Other.x;
				y += Other.y;
				return *this;
			}
			template<typename U, typename std::enable_if<std::is_convertible<decltype(std::declval<T>() - std::declval<U>()), T>::value>::type*& = hmLib::utility::enabler>
			point<T>& operator-=(const point<U>& Other) {
				x -= Other.x;
				y -= Other.y;
				return *this;
			}
			template<typename U, typename std::enable_if<std::is_convertible<decltype(std::declval<T>() * std::declval<U>()), T>::value>::type*& = hmLib::utility::enabler>
			point<T>& operator*=(const point<U>& Other) {
				x *= Other.x;
				y *= Other.y;
				return *this;
			}
			template<typename U, typename std::enable_if<std::is_convertible<decltype(std::declval<T>() / std::declval<U>()), T>::value>::type*& = hmLib::utility::enabler>
			point<T>& operator/=(const point<U>& Other) {
				x /= Other.x;
				y /= Other.y;
				return *this;
			}
			template<typename U, typename std::enable_if<std::is_convertible<decltype(std::declval<T>() * std::declval<U>()), T>::value>::type*& = hmLib::utility::enabler>
			point<T>& operator*=(U val) {
				x *= val;
				y *= val;
				return *this;
			}
			template<typename U, typename std::enable_if<std::is_convertible<decltype(std::declval<T>() / std::declval<U>()), T>::value>::type*& = hmLib::utility::enabler>
			point<T>& operator/=(U val) {
				x /= val;
				y /= val;
				return *this;
			}
		};
		template<typename T, typename U, typename ans_type = decltype(std::declval<T>() + std::declval<U>())>
		point<ans_type> operator+(const point<T>& v1, const point<U>& v2) {
			return point<ans_type>(v1.x + v2.x, v1.y + v2.y);
		}
		template<typename T, typename U, typename ans_type = decltype(std::declval<T>() - std::declval<U>())>
		point<ans_type> operator-(const point<T>& v1, const point<U>& v2) {
			return point<ans_type>(v1.x - v2.x, v1.y - v2.y);
		}
		template<typename T, typename U, typename ans_type = decltype(std::declval<T>() * std::declval<U>())>
		point<ans_type> operator*(const point<T>& v1, const point<U>& v2) {
			return point<ans_type>(v1.x * v2.x, v1.y * v2.y);
		}
		template<typename T, typename U, typename ans_type = decltype(std::declval<T>() / std::declval<U>())>
		point<ans_type> operator/(const point<T>& v1, const point<U>& v2) {
			return point<ans_type>(v1.x / v2.x, v1.y / v2.y);
		}
		template<typename T, typename U, typename ans_type = decltype(std::declval<T>() * std::declval<U>())>
		point<ans_type> operator*(const point<T>& v1, U val) {
			return point<ans_type>(v1.x * val, v1.y * val);
		}
		template<typename T, typename U, typename ans_type = decltype(std::declval<T>() * std::declval<U>())>
		point<ans_type> operator*(T val, const point<U>& v2) {
			return point<ans_type>(val * v2.x, val * v2.y);
		}
		template<typename T, typename U, typename ans_type = decltype(std::declval<T>() / std::declval<U>())>
		point<ans_type> operator/(const point<T>& v1, U val) {
			return point<ans_type>(v1.x / val, v1.y / val);
		}
		template<typename T, typename U, typename std::enable_if<std::is_convertible<decltype(std::declval<T>() == std::declval<U>()), bool>::value>::type*& = hmLib::utility::enabler>
		bool operator==(const point<T>& v1, const point<U>& v2) {
			return static_cast<bool>(v1.x == v2.x) && static_cast<bool>(v1.y == v2.y);
		}
		template<typename T, typename U, typename std::enable_if<std::is_convertible<decltype(std::declval<T>() == std::declval<U>()), bool>::value>::type*& = hmLib::utility::enabler>
		bool operator!=(const point<T>& v1, const point<U>& v2) {
			return !(v1 == v2);
		}
		template<typename T, typename U, typename std::enable_if<std::is_convertible<decltype(std::declval<T>() < std::declval<U>()), bool>::value>::type*& = hmLib::utility::enabler>
		bool operator<(const point<T>& v1, const point<U>& v2) {
			if(v1.x == v2.x)return v1.y < v2.y;
			return v1.x < v2.x;
		}
		template<typename T, typename U, typename std::enable_if<std::is_convertible<decltype(std::declval<T>() < std::declval<U>()), bool>::value>::type*& = hmLib::utility::enabler>
		bool operator<=(const point<T>& v1, const point<U>& v2) {
			if(v1.x == v2.x) {
				return v1.y <= v2.y;
			}
			return v1.x < v2.x;
		}
		template<typename T, typename U, typename std::enable_if<std::is_convertible<decltype(std::declval<T>() > std::declval<U>()), bool>::value>::type*& = hmLib::utility::enabler>
		bool operator>(const point<T>& v1, const point<U>& v2) {
			if(v1.x == v2.x)return v1.y > v2.y;
			return v1.x > v2.x;
		}
		template<typename T, typename U, typename std::enable_if<std::is_convertible<decltype(std::declval<T>() > std::declval<U>()), bool>::value>::type*& = hmLib::utility::enabler>
		bool operator>=(const point<T>& v1, const point<U>& v2) {
			if(v1.x == v2.x) {
				return v1.y >= v2.y;
			}
			return v1.x > v2.x;
		}
		template<typename T>
		decltype(std::sqrt(std::declval<T>()*std::declval<T>())) norm(const point<T>& v) {
			return std::sqrt(v.x*v.x+v.y*v.y);
		}
		template<typename T>
		decltype(std::declval<T>()*std::declval<T>()) norm2(const point<T>& v) {
			return (v.x*v.x + v.y*v.y);
		}
		template<typename T>
		void sorting_element_swap(point<T>& v1, point<T>& v2){
			sorting_swap(v1.x, v2.x);
			sorting_swap(v1.y, v2.y);
		}
		template<typename T>
		point<T> min_element(const point<T>& v1, const point<T>& v2) {
			return point<T>(
				min_value(v1.x, v2.x),
				min_value(v1.y, v2.y)
			);
		}
		template<typename T>
		point<T> max_element(const point<T>& v1, const point<T>& v2) {
			return point<T>(
				max_value(v1.x, v2.x),
				max_value(v1.y, v2.y)
			);
		}
		using pint = point<int>;
		using pdouble = point<double>;
	}
}
#endif
