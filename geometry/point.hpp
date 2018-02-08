#ifndef HMLIB_POSITION_INC
#define HMLIB_POSITION_INC 100
#include<cmath>
#include<iostream>
#include<type_traits>
#include"../utility.hpp"
namespace hmLib{
	namespace pgeometry {
		template<class T>
		struct point {
		public:
			T x;
			T y;
		public:
			point():x(0),y(0) {}
			template<typename S, hmLib_static_restrict(std::is_convertible<S, T>::value)>
			point(S x_, S y_) : x(x_), y(y_) {}
			template<typename S, hmLib_static_restrict(std::is_convertible<S, T>::value)>
			point(const point<S>& Other) : x(Other.x), y(Other.y) {}
//			template<typename S, typename std::enable_if<std::is_convertible<S, T>::value>::type*& = hmLib::utility::detail::enabler>
//			point(S _x, S _y): x(x_), y(y_) {}
		public:
			template<typename S, hmLib_static_restrict(std::is_convertible<S, T>::value)>
			void reset(S x_, S y_) {
				x = x_;
				y = y_;
			}
		public:
			bool operator!=(const point<T>& _T)const {
				if(x != _T.x || y != _T.y)return true;
				else return false;
			}
			bool operator==(const point<T>& _T)const {
				if(x != _T.x || y != _T.y)return false;
				else return true;
			}
			bool operator<(const point<T>& _T)const {
				if(x >= _T.x || y >= _T.y)return false;
				else return true;
			}
			bool operator>(const point<T>& _T)const {
				if(x <= _T.x || y <= _T.y)return false;
				else return true;
			}
			bool operator<=(const point<T>& _T)const {
				if(x>_T.x || y>_T.y)return false;
				else return true;
			}
			bool operator>=(const point<T>& _T)const {
				if(x<_T.x || y<_T.y)return false;
				else return true;
			}
			const point<T> operator=(const point<T>& _T) {
				if(this != &_T) {
					x = _T.x;
					y = _T.y;
				}
				return *this;
			}
			template<class S>
			const point<T> operator=(const point<S>& _S) {
				x = (T)_S.x;
				y = (T)_S.y;
				return *this;
			}
			const point<T> operator+=(const point<T>& _T) {
				x += _T.x;
				y += _T.y;
				return *this;
			}
			template<class S>
			const di_position<T> operator+=(const di_position<S>& _S) {
				x += (T)_S.x;
				y += (T)_S.y;
				return *this;
			}
			const di_position<T> operator-=(const di_position<T>& _T) {
				x -= _T.x;
				y -= _T.y;
				return *this;
			}
			template<class S>
			const di_position<T> operator-=(const di_position<S>& _S) {
				x -= (T)_S.x;
				y -= (T)_S.y;
				return *this;
			}
			di_position<T> operator+()const { return *this; }
			di_position<T> operator-()const {
				di_position<T> ans(*this);
				ans.x *= -1;
				ans.y *= -1;
				return ans;
			}
			di_position<T> operator+(const di_position<T>& _T)const {
				di_position<T> ans(*this);
				ans += _T;
				return ans;
			}
			di_position<T> operator-(const di_position<T>& _T)const {
				di_position<T> ans(*this);
				ans -= _T;
				return ans;
			}
			di_position<T> operator*(const di_position<T>& _T)const {
				di_position<T> ans(*this);
				ans.x *= _T.x;
				ans.y *= _T.y;
				return ans;
			}
			di_position<T> operator/(const di_position<T>& _T)const {
				di_position<T> ans(*this);
				if(_T.x == 0 || _T.y == 0)throw "di_position<T>::operator/(const di_position<T>& _T)const  div with 0";
				ans.x /= _T.x;
				ans.y /= _T.y;
				return ans;
			}
			template<class S>
			operator S()const {
				di_position<S> ans(*this);
				return ans;
			}
			//friend関数群
			//	friend bool operator==(const di_position<T>& _T1,const di_position<T>& _T2);
			friend di_position<T> operator*(const di_position<T>& _T, const T& p) {
				di_position<T> ans(_T);
				ans.x *= p;
				ans.y *= p;
				return ans;
			}
			friend di_position<T> operator*(const T& p, const di_position<T>& _T) {
				di_position<T> ans(_T);
				ans.x *= p;
				ans.y *= p;
				return ans;
			}
			friend di_position<T> operator/(const di_position<T>& _T, const T& p) {
				di_position<T> ans(_T);
				ans.x /= p;
				ans.y /= p;
				return ans;
			}
			template<class S, class U>
			friend di_position<U> operator+(const di_position<T>& _T, const di_position<S>& _S);
			template<class S, class U>
			friend di_position<U> operator-(const di_position<T>& _T, const di_position<S>& _S);
			template<class S, class U>
			friend di_position<U> operator*(const di_position<T>& _T, const di_position<S>& _S);
			template<class S, class U>
			friend di_position<U> operator/(const di_position<T>& _T, const di_position<S>& _S);
			template<class S, class U>
			friend di_position<U> operator*(const di_position<T>& _T, const S& _S);
			template<class S, class U>
			friend di_position<U> operator/(const di_position<T>& _T, const S& _S);
			template<class S, class U>
			friend di_position<U> operator*(const S& _S, const di_position<T>& _T);
			friend std::ostream& operator<<(std::ostream& out, const di_position<T>& pos) {
				out << '(' << pos.x << ',' << pos.y << ')';
				return out;
			}
		};
		using pint = point<int>;
		using pdouble = point<double>;
		inline pdouble operator+(const pint& i, const pdouble& d) { return pdouble(i) + d; }
		inline pdouble operator-(const pint& i, const pdouble& d) { return pdouble(i) - d; }
		inline pdouble operator*(const pint& i, const pdouble& d) { return pdouble(i)*d; }
		inline pdouble operator/(const pint& i, const pdouble& d) { return pdouble(i) / d; }
		inline pdouble operator*(const pint& i, const double& d) { return pdouble(i)*d; }
		inline pdouble operator/(const pint& i, const double& d) { return pdouble(i) / d; }
		inline pdouble operator*(const double& d, const pint& i) { return pdouble(i)*d; }
		inline pdouble operator+(const pdouble& d, const pint& i) { return d + pdouble(i); }
		inline pdouble operator-(const pdouble& d, const pint& i) { return d - pdouble(i); }
		inline pdouble operator*(const pdouble& d, const pint& i) { return d*pdouble(i); }
		inline pdouble operator/(const pdouble& d, const pint& i) { return d / pdouble(i); }
		inline pdouble operator*(const pdouble& d, const int& i) { return d*double(i); }
		inline pdouble operator/(const pdouble& d, const int& i) { return d / double(i); }
		inline pdouble operator*(const int& i, const pdouble& d) { return d*double(i); }
	}
}
#endif
