#ifndef HMLIB_ALGEBRA_INC
#define HMLIB_ALGEBRA_INC 100
#
namespace hmLib{
	namespace algebra{
		namespace detail{
			template<typename op, typename rhs>
			class unary_expr{
				const rhs& rhsv;
			public:
				explicit unary_expr(const rhs& rhsv_):rhsv(rhsv_){}
				template<typename evaluator>
				auto operator()(evaluator& eval){
					return op()(eval,rhsv);
				}
			};
			template<typename lhs, typename op, typename rhs>
			class binary_expr{
				const lhs& lhsv;
				const rhs& rhsv;
			public:
				binary_expr(const lhs& lhsv_, const rhs& rhsv_):lhsv(lhsv_),rhsv(rhsv_){}
				template<typename evaluator>
				auto operator()(evaluator& eval){
					return op()(eval,lhsv,rhsv);
				}
			};
		}
		struct operators{
			struct add{
				template<typename evaluator, typename lhs, typename rhs>
				auto operator()(const evaluator& Eval, const lhs& l, const rhs& r){
					return Eval.add(l,r);
				}
			};
		};
		template<typename eval_type>
		struct vector_space_algebra{
			static auto identity();
			static auto minus(vector v){return -1*v;}
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
