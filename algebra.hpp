#ifndef HMLIB_ALGEBRA_INC
#define HMLIB_ALGEBRA_INC 100
#
namespace hmLib{
	namespace algebra{
		namespace detail{
			template<typename op_t>
			class nullary_expr{
			public:
				template<typename evaluator>
				auto operator()(evaluator& eval){
					return eval(op_t);
				}
			};
			template<typename op_t, typename rhs_t>
			class unary_expr{
				const rhs_t& rhs;
			public:
				explicit unary_expr(const rhs_t& rhs_):rhs(rhs_){}
				template<typename evaluator>
				auto operator()(evaluator& eval){
					return eval(op_t(),rhs);
				}
			};
			template<typename op_t, typename lhs_t, typename rhs_t>
			class binary_expr{
				const lhs_t& lhs;
				const rhs_t& rhs;
			public:
				binary_expr(const lhs_t& lhs_, const rhs_t& rhs_):lhs(lhs_),rhs(rhs_){}
				template<typename evaluator>
				auto operator()(evaluator& eval){
					return eval(op_t(),lhs,rhs);
				}
			};
		}
		struct operators{
			struct add{
				template<typename lhs_t, typename rhs_t>
				auto operator()(const lhs_t& lhs, const rhs_t& rhs){
					return lhs+rhs;
				}
			};
			struct sub{
				template<typename lhs_t, typename rhs_t>
				auto operator()(const lhs_t& lhs, const rhs_t& rhs){
					return lhs-rhs;
				}
			};
			struct prd{
				template<typename lhs_t, typename rhs_t>
				auto operator()(const lhs_t& lhs, const rhs_t& rhs){
					return lhs*rhs;
				}
			};
			struct div{
				template<typename lhs_t, typename rhs_t>
				auto operator()(const lhs_t& lhs, const rhs_t& rhs){
					return lhs/rhs;
				}
			};
		};
		namespace vector_space_algebra{
			template<typename vector_base>
			struct vector{
				using this_type = vector<vector_base>;
				template<typename other_vector_base>
				friend auto operator+(const this_type& lhs, const vector<other_base>& rhs){
					return detail::binary_expression<operators::add, this_type, vector<other_base>>(lhs,rhs);
				}
				template<typename other_vector_base>
				friend auto operator-(const this_type& lhs, const vector<other_base>& rhs){
					return detail::binary_expression<operators::sub, this_type, vector<other_base>>(lhs,rhs);
				}
				template<typename scalar>
				friend auto operator*(const scalar& s, const this_type& v){
					return detail::binary_expression<operators::prd, scalar, this_type>(lhs,rhs);
				}
				template<typename scalar>
				friend auto operator*(const this_type& v, scalar s){return s*v;}
			};
			auto identity();
			template<typename vector_base>
			auto v_(const vector_base& v){return vector<vector_base>(v);}
		};
		namespace normed_vector_space_algebra{
			using namespace vector_space_algebra;
			auto norm(vector v);
		};
		struct evaluated{
			template<typename assigned_t>
			friend assigned_t& operator=(assigned_t& ass, do_evaluate&){
			}
		};
		struct range_evaluator{
			template<typename op_t>
			auto operator()(op_t op);
			template<typename op_t, typename rhs_t>
			auto operator()(op_t op, const rhs_t& rhs);
			template<typename return_t, typename lhs_t, typename rhs_t>
			auto operator()(return_t& ans, operators::add op, const lhs_t& lhs, const rhs_t& rhs){
				for_each(ans, op, lhs, rhs);
			}
			template<typename return_t, typename lhs_t, typename rhs_t>
			auto operator()(return_t& ans, operators::sub op, const lhs_t& lhs, const rhs_t& rhs){
				for_each(ans, op, lhs, rhs);
			}
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
