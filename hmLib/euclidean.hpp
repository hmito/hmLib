#ifndef HMLIB_EUCLIDEAN_INC 
#define HMLIB_EUCLIDEAN_INC 100
#
#include<array>
#include<type_traits>
namespace hmLib{
	namespace euclidean{
		template<typename T, unsigned int dim_>
		struct point{
		private:
			using this_type = point<T, dim_>;
			template<typename U>
			using other_type = point<U, dim_>;
			using container = std::array<T, dim_>;
		public:
			using iterator = typename container::iterator;
			using const_iterator = typename container::const_iterator;
			static constexpr unsigned int dim(){ return dim_; }
		private:
			container Array;
		public:
			point() = default;
			point(const this_type&) = default;
			this_type& operator=(const this_type&) = default;
			point(this_type&&) = default;
			this_type& operator=(this_type&&) = default;
			template<typename U>
			point(const other_type<U>& Other)
				: Array(Otehr.begin(), Other.end()){}
			template<typename U>
			this_type& operator=(const other_type<U>& Other){
				for(unsigned int i = 0; i < Array.size(); ++i){
					Array[i] = Other[i];
				}
				return *this;
			}
			point(std::initializer_list<T> Values) :Array(){
				std::copy(Values.begin(), Values.end(), Array.begin());
			}
			template<typename input_iterator>
			point(input_iterator Begin_, input_iterator End_) : Array(){
				std::copy(Begin_, End_, Array.begin());
			}
			explicit point(T val) :Array(){ Array.fill(val); }
		public:
			this_type& operator+=(const this_type& Other){
				for(unsigned int i = 0; i < dim_; ++i){
					Array[i] += Other.Array[i];
				}
				return *this;
			}
			template<typename U>
			this_type& operator+=(const this_type& Other){
				static_assert(std::is_same<T, decltype(T() + U())>::value, "It is impossible to implicitly convert T from U.");
				for(unsigned int i = 0; i < dim_; ++i){
					Array[i] += Other.Array[i];
				}
				return *this;
			}
			this_type& operator-=(const this_type& Other){
				for(unsigned int i = 0; i < dim_; ++i){
					Array[i] -= Other.Array[i];
				}
				return *this;
			}
			friend this_type operator+(const this_type& This1, const this_type& This2){
				this_type Ans = This1;
				return Ans += This2;
			}
			template<typename U>
			friend auto operator+(const this_type& This1, const other_type<U>& This2)->other_type<decltype(T()+U())>{
				other_type<decltype(T() + U())> Ans = This1;
				return Ans += This2;
			}
			friend this_type operator-(const this_type& This1, const this_type& This2){
				this_type Ans = This1;
				return Ans -= This2;
			}
			this_type& operator*=(int Val){
				for(unsigned int i = 0; i < dim_; ++i){
					Array[i] *= Val;
				}
				return *this;
			}
			this_type& operator*=(double Val){
				for(unsigned int i = 0; i < dim_; ++i){
					Array[i] *= Val;
				}
				return *this;
			}
			friend this_type operator*(const this_type& This, int Val){
				this_type Ans = This;
				return Ans *= Val;
			}
			friend this_type operator*(const this_type& This, double Val){
				this_type Ans = This;
				return Ans *= Val;
			}
			friend this_type operator*(int Val, const this_type& This){
				this_type Ans = This;
				return Ans *= Val;
			}
			friend this_type operator*(double Val, const this_type& This){
				this_type Ans = This;
				return Ans *= Val;
			}
			this_type& operator/=(int Val){
				for(unsigned int i = 0; i < dim_; ++i){
					Array[i] /= Val;
				}
				return *this;
			}
			this_type& operator/=(double Val){
				for(unsigned int i = 0; i < dim_; ++i){
					Array[i] /= Val;
				}
				return *this;
			}
			friend this_type operator/(const this_type& This, int Val){
				this_type Ans = This;
				return Ans /= Val;
			}
			friend this_type operator/(const this_type& This, double Val){
				this_type Ans = This;
				return Ans /= Val;
			}
		public:
			T& operator[](int pos){ return Array[pos]; }
			const T& operator[](int pos)const{ return Array[pos]; }
			T& at(int pos){ return Array.at(pos); }
			const T& at(int pos)const{ return Array.at(pos); }
			iterator begin(){ return Array.begin(); }
			iterator end(){ return Array.end(); }
			const_iterator begin()const{ return Array.begin(); }
			const_iterator end()const{ return Array.end(); }
			const_iterator cbegin()const{ return Array.begin(); }
			const_iterator cend()const{ return Array.end(); }
			void fill(T val){ Array.fill(val); }
			void swap(this_type& Other){ Array.swap(Other.Array); }
		public:
			friend bool operator==(const this_type& v1, const this_type& v2){
				for(unsigned int i = 0; i < dim_; ++i){
					if(v1[i] != v2[i])return false;
				}
				return true;
			}
			friend bool operator!=(const this_type& v1, const this_type& v2){
				return !(v1 == v2);
			}
			friend bool operator<(const this_type& v1, const this_type& v2){
				for(unsigned int i = 0; i < dim_; ++i){
					if(v1[i] != v2[i])return v1[i] < v2[i];
				}
				return false;
			}
			friend bool operator<=(const this_type& v1, const this_type& v2){
				for(unsigned int i = 0; i < dim_; ++i){
					if(v1[i] != v2[i])return v1[i] < v2[i];
				}
				return true;
			}
			friend bool operator > (const this_type& v1, const this_type& v2){
				for(unsigned int i = 0; i < dim_; ++i){
					if(v1[i] != v2[i])return v1[i] > v2[i];
				}
				return false;
			}
			friend bool operator>=(const this_type& v1, const this_type& v2){
				for(unsigned int i = 0; i < dim_; ++i){
					if(v1[i] != v2[i])return v1[i] > v2[i];
				}
				return true;
			}
			friend bool operator<<(const this_type& v1, const this_type& v2){
				for(unsigned int i = 0; i < dim_; ++i){
					if(!(v1[i] < v2[i]))return false;
				}
				return true;
			}
			friend bool operator<<=(const this_type& v1, const this_type& v2){
				for(unsigned int i = 0; i < dim_; ++i){
					if(!(v1[i] <= v2[i]))return false;
				}
				return true;
			}
			friend bool operator>>(const this_type& v1, const this_type& v2){
				for(unsigned int i = 0; i < dim_; ++i){
					if(!(v1[i] > v2[i]))return false;
				}
				return true;
			}
			friend bool operator>>=(const this_type& v1, const this_type& v2){
				for(unsigned int i = 0; i < dim_; ++i){
					if(!(v1[i] >= v2[i]))return false;
				}
				return true;
			}
		};

		template<typename T, typename ...others>
		auto make_point(T Value, others... Others)->point<T,1+sizeof...(others)>{
			return point<T,1+sizeof...(others)>{Value, Others...};
		}

		template<typename T, unsigned int dim>
		T sum_of_square(const point<T, dim>& Elem){
			T value = Elem[0] * Elem[0];
			for(unsigned int pos = 1; pos < dim; ++pos){
				value += Elem[pos] * Elem[pos];
			}
			return value;
		}
		template<typename T, unsigned int dim>
		T abs(const point<T, dim>& Elem){
			return std::sqrt(sum_of_square(Elem));
		}

		template<typename T>
		using point1d = point<T, 1>;
		template<typename T>
		using point2d = point<T, 2>;
		template<typename T>
		using point3d = point<T, 3>;

		using p1int = point<int, 1>;
		using p2int = point<int, 2>;
		using p3int = point<int, 3>;
		using p1double = point<double, 1>;
		using p2double = point<double, 2>;
		using p3double = point<double, 3>;
	}
}
#
#endif
