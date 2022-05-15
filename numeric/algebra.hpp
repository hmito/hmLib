#ifndef HMLIB_ALGEBRA_INC
#define HMLIB_ALGEBRA_INC 100
#
namespace hmLib{
	namespace algebra{
		template<typename algebra_type>
		struct vector_space{
			vector identity();
			auto operator-(vector v){return -1*v;}
			auto operator+(vector v1, vector v2);
			auto operator-(vector v1, vector v2){return v1 + (-v2);}
			auto operator*(scalar s, vector v);
			auto operator*(vector v, scalar s){return s*v;}
		};
		template<typename algebra_type>
		struct normed_vector_space: vector_space<algebra_type>{
			auto norm(vector v);
		};
		template<typename algebra_type>
		struct metric_vector_space: normed_vector_space<algebra_type>{
			auto inner_prod(vector v1, vector v2);
		};
		template<typename algebra_type>
		struct ndim_vector_space: metric_vector_space<algebra_type>{
			template<typename fn>
			vector map(fn Fn, vector v);
			template<typename scalar, typename fn>
			scalar reduce(scalar ini, fn Fn, vector v);
		};
		template<typename container>
		struct vector{
		private:
			container& vec;
		public:
			vector(container& vec_):vec(vec_){}
		};
		
	}
}
#
#endif
