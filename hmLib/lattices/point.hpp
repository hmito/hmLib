#ifndef HMLIB_LATTICES_POINT_INC
#define HMLIB_LATTICES_POINT_INC 100
#
namespace hmLib{
	namespace lattices{
		using index_type = int;
		using difference_type = int;

		template<unsigned int dim_>
		struct point{
		private:
			using this_type = point<dim_>;
			using container = std::array<int, dim_>;
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
			template<typename... others>
			point(int val, others... Others) :Array{val, Others...}{}
		public:
			this_type& operator+=(const this_type& Other){
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
		auto make_point(others... Others)->point<sizeof...(others)>{
			return point<sizeof...(others)>{Others...};
		}
	}
}
#
#endif
