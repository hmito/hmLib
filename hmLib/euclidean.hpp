#ifndef HMLIB_EUCLIDEAN_INC 
#define HMLIB_EUCLIDEAN_INC 100
#
#include<array>
#include<type_traits>
namespace hmLib{
	namespace euclidean{
		template<typename T, unsigned int dim_>
		struct element{
		private:
			using this_type = element<T, dim_>;
			template<typename U>
			using other_type = element<U, dim_>;
			using container = std::array<T, dim_>;
		public:
			using iterator = typename container::iterator;
			using const_iterator = typename container::const_iterator;
			static constexpr unsigned int dim(){ return dim_; }
		private:
			container Array;
		public:
			element() = default;
			element(const this_type&) = default;
			this_type& operator=(const this_type&) = default;
			element(this_type&&) = default;
			this_type& operator=(this_type&&) = default;
			template<typename... others>
			element(int val, others... Others) :Array{val, Others...}{}
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
			int& operator[](int pos){ return Array[pos]; }
			const int& operator[](int pos)const{ return Array[pos]; }
			int& at(int pos){ return Array.at(pos); }
			const int& at(int pos)const{ return Array.at(pos); }
			iterator begin(){ return Array.begin(); }
			iterator end(){ return Array.end(); }
			const_iterator begin()const{ return Array.begin(); }
			const_iterator end()const{ return Array.end(); }
			const_iterator cbegin()const{ return Array.begin(); }
			const_iterator cend()const{ return Array.end(); }
			void fill(int val){ Array.fill(val); }
			void swap(this_type& Other){ Array.swap(Other.Array); }
		};

		template<typename ...others>
		auto make_element(others... Others)->element<sizeof...(others)>{
			return element<sizeof...(others)>{Others...};
		}
	}
}
#
#endif
