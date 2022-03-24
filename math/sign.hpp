#ifndef HMLIB_MATH_COMPARE_INC
#define HMLIB_MATH_COMPARE_INC 100
#
#include<cstddef>
#include<iostream>
namespace hmLib{
	namespace math{
		struct sign{
			static const sign positive;
			static const sign zero;
			static const sign negative;
		public:
			sign():v(0){}
			template<typename T>
			explicit sign(T val)noexcept: v( val > static_cast<T>(0)? 1: (val<static_cast<T>(0)? -1:0) ) {}
			sign(const sign&) = default;
			sign(sign&&) = default;
			sign& operator=(sign&&) = default;
			sign& operator=(const sign&) = default;
		public:
			template<typename T>
			explicit operator T()const{return T(v);}
			sign operator-()const noexcept{
				return sign(v*-1,nullptr);
			}
			sign& operator *=(const sign& x)noexcept{
				v *=x.v;
				return *this;
			}
			friend sign operator*(const sign& x, const sign& y)noexcept{ sign z=x; return z*=y;}
			template<typename T>
			friend T& operator*=(T val, const sign& c){
				return val*=c.v;
			}
			template<typename T>
			friend auto operator*(T val, const sign& c){
				return val*static_cast<signed char>(c.v);
			}
			template<typename T>
			friend auto operator*(const sign& c, T val){
				return static_cast<signed char>(c.v)*val;
			}
			friend bool operator==(const sign& x,const sign& y)noexcept{ return x.v==y.v;}
			friend bool operator!=(const sign& x,const sign& y)noexcept{ return x.v!=y.v;}
			friend bool operator<=(const sign& x,const sign& y)noexcept{ return x.v<=y.v;}
			friend bool operator>=(const sign& x,const sign& y)noexcept{ return x.v>=y.v;}
			friend bool operator<(const sign& x,const sign& y)noexcept{ return x.v<y.v;}
			friend bool operator>(const sign& x,const sign& y)noexcept{ return x.v>y.v;}
			friend std::ostream& operator<<(std::ostream& out, const sign& x){
				if(x.v>0)out<<'+';
				else if(x.v<0)out<<'-';
				else out<<'0';
				return out;
			}
			friend std::istream& operator>>(std::istream& in, sign& x){
				char c = in.get();
				if(c=='+')x = positive;
				else if(c=='-')x = negative;
				else x = zero;
				return in;
			}
		private:
			constexpr sign(int v_, std::nullptr_t)noexcept: v(v_){}
		private:
			int v;
		};
		inline constexpr sign sign::positive(+1,nullptr);
		inline constexpr sign sign::zero(0,nullptr);
		inline constexpr sign sign::negative(-1,nullptr);
		inline sign abs(sign x){
			return x>=sign::zero? x: -x;
		}
		inline sign pow(sign x, unsigned int num){
			return (num%2)? x: -x;
		}
	}
}
#
#endif
