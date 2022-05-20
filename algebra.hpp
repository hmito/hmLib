#ifndef HMLIB_ALGEBRA_INC
#define HMLIB_ALGEBRA_INC 100
#
namespace hmLib{
	namespace algebra{
		namespace detail{
			template<typename op, typename rhs>
			struct unary_expr{};
			template<typename lhs, typename op, typename rhs>
			struct binary_expr{};
		}
		struct operators{
			struct add{
				template<typename lhs, typename rhs>
				auto operator()(const lhs& l, const rhs& r){
					return l + r;
				}
			};
		};
		template<typename eval_type>
		struct vector_space_algebra{
			vector identity();
			auto operator-(vector v){return -1*v;}
			auto operator+(vector v1, vector v2);
			auto operator-(vector v1, vector v2){return v1 + (-v2);}
			auto operator*(scalar s, vector v);
			auto operator*(vector v, scalar s){return s*v;}
		};
		template<typename eval_type>
		struct normed_vector_space_algebra: vector_space_algebra<eval_type>{
			auto norm(vector v);
		};
		template<typename eval_type>
		struct metric_vector_space_algebra: normed_vector_space_algebra<eval_type>{
			auto inner_prod(vector v1, vector v2);
		};
		template<typename algebeval_typera_type>
		struct ndim_vector_space_algebra: metric_vector_space_algebra<eval_type>{
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
