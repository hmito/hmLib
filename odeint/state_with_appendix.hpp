#ifndef HMLIB_ODEINT_STATEWITHAPPENDIX_INC
#define HMLIB_ODEINT_STATEWITHAPPENDIX_INC 100
#
#include<type_traits>
#include<boost/numeric/odeint/util/same_size.hpp>
#include<boost/numeric/odeint/util/resize.hpp>
#include<boost/numeric/odeint/util/is_resizeable.hpp>
#include<boost/numeric/odeint/algebra/norm_result_type.hpp>
namespace hmLib {
	namespace odeint {
		template<typename state_, typename appendix_>
		struct state_with_appendix {
			using state = state_;
			using appendix = appendix_;
			using iterator = decltype(std::declval<state>().begin());
			using const_iterator = decltype(std::declval<const state>().begin());
		public:
			state x;
			appendix ap;
		public:
			state_with_appendix() = default;
			state_with_appendix(state x_, appendix ap_) :x(x_), ap(ap_) {}
			iterator begin() { return x.begin(); }
			const_iterator begin() const { return x.begin(); }
			iterator end() { return x.end(); }
			const_iterator end() const { return x.end(); }
			void resize(const std::size_t n) { x.resize(n); }
		};
		template<typename algebra_>
		struct state_with_appendix_algebra {
			using algebra = algebra_;

			template< class S1, class Op >
			static void for_each1(S1& s1, Op op){
				algebra().for_each1(s1.x, op);
			}
			template< class S1, class S2, class Op >
			static void for_each2(S1& s1, S2& s2, Op op){
				algebra().for_each2(s1.x, s2.x, op);
			}

			template< class S1, class S2, class S3, class Op >
			static void for_each3(S1& s1, S2& s2, S3& s3, Op op){
				algebra().for_each3(s1.x, s2.x, s3.x, op);
			}

			template< class S1, class S2, class S3, class S4, class Op >
			static void for_each4(S1& s1, S2& s2, S3& s3, S4& s4, Op op){
				algebra().for_each4(s1.x, s2.x, s3.x, s4.x, op);
			}

			template< class S1, class S2, class S3, class S4, class S5, class Op >
			static void for_each5(S1& s1, S2& s2, S3& s3, S4& s4, S5& s5, Op op) {
				algebra().for_each5(s1.x, s2.x, s3.x, s4.x, s5.x, op);
			}

			template< class S1, class S2, class S3, class S4, class S5, class S6, class Op >
			static void for_each6(S1& s1, S2& s2, S3& s3, S4& s4, S5& s5, S6& s6, Op op) {
				algebra().for_each6(s1.x, s2.x, s3.x, s4.x, s5.x, s6.x, op);
			}

			template< class S1, class S2, class S3, class S4, class S5, class S6, class S7, class Op >
			static void for_each7(S1& s1, S2& s2, S3& s3, S4& s4, S5& s5, S6& s6, S7& s7, Op op) {
				algebra().for_each7(s1.x, s2.x, s3.x, s4.x, s5.x, s6.x, s7.x, op);
			}

			template< class S1, class S2, class S3, class S4, class S5, class S6, class S7, class S8, class Op >
			static void for_each8(S1& s1, S2& s2, S3& s3, S4& s4, S5& s5, S6& s6, S7& s7, S8& s8, Op op) {
				algebra().for_each8(s1.x, s2.x, s3.x, s4.x, s5.x, s6.x, s7.x, s8.x, op);
			}

			template< class S1, class S2, class S3, class S4, class S5, class S6, class S7, class S8, class S9, class Op >
			static void for_each9(S1& s1, S2& s2, S3& s3, S4& s4, S5& s5, S6& s6, S7& s7, S8& s8, S9& s9, Op op) {
				algebra().for_each9(s1.x, s2.x, s3.x, s4.x, s5.x, s6.x, s7.x, s8.x, s9.x, op);
			}

			template< class S1, class S2, class S3, class S4, class S5, class S6, class S7, class S8, class S9, class S10, class Op >
			static void for_each10(S1& s1, S2& s2, S3& s3, S4& s4, S5& s5, S6& s6, S7& s7, S8& s8, S9& s9, S10& s10, Op op) {
				algebra().for_each10(s1.x, s2.x, s3.x, s4.x, s5.x, s6.x, s7.x, s8.x, s9.x, s10.x, op);
			}

			template< class S1, class S2, class S3, class S4, class S5, class S6, class S7, class S8, class S9, class S10, class S11, class Op >
			static void for_each11(S1& s1, S2& s2, S3& s3, S4& s4, S5& s5, S6& s6, S7& s7, S8& s8, S9& s9, S10& s10, S11& s11, Op op) {
				algebra().for_each11(s1.x, s2.x, s3.x, s4.x, s5.x, s6.x, s7.x, s8.x, s9.x, s10.x, s11.x, op);
			}

			template< class S1, class S2, class S3, class S4, class S5, class S6, class S7, class S8, class S9, class S10, class S11, class S12, class Op >
			static void for_each12(S1& s1, S2& s2, S3& s3, S4& s4, S5& s5, S6& s6, S7& s7, S8& s8, S9& s9, S10& s10, S11& s11, S12& s12, Op op) {
				algebra().for_each12(s1.x, s2.x, s3.x, s4.x, s5.x, s6.x, s7.x, s8.x, s9.x, s10.x, s11.x, s12.x, op);
			}

			template< class S1, class S2, class S3, class S4, class S5, class S6, class S7, class S8, class S9, class S10, class S11, class S12, class S13, class Op >
			static void for_each13(S1& s1, S2& s2, S3& s3, S4& s4, S5& s5, S6& s6, S7& s7, S8& s8, S9& s9, S10& s10, S11& s11, S12& s12, S13& s13, Op op) {
				algebra().for_each13(s1.x, s2.x, s3.x, s4.x, s5.x, s6.x, s7.x, s8.x, s9.x, s10.x, s11.x, s12.x, s13.x, op);
			}

			template< class S1, class S2, class S3, class S4, class S5, class S6, class S7, class S8, class S9, class S10, class S11, class S12, class S13, class S14, class Op >
			static void for_each14(S1& s1, S2& s2, S3& s3, S4& s4, S5& s5, S6& s6, S7& s7, S8& s8, S9& s9, S10& s10, S11& s11, S12& s12, S13& s13, S14& s14, Op op) {
				algebra().for_each14(s1.x, s2.x, s3.x, s4.x, s5.x, s6.x, s7.x, s8.x, s9.x, s10.x, s11.x, s12.x, s13.x, s14.x, op);
			}

			template< class S1, class S2, class S3, class S4, class S5, class S6, class S7, class S8, class S9, class S10, class S11, class S12, class S13, class S14, class S15, class Op >
			static void for_each15(S1& s1, S2& s2, S3& s3, S4& s4, S5& s5, S6& s6, S7& s7, S8& s8, S9& s9, S10& s10, S11& s11, S12& s12, S13& s13, S14& s14, S15& s15, Op op) {
				algebra().for_each15(s1.x, s2.x, s3.x, s4.x, s5.x, s6.x, s7.x, s8.x, s9.x, s10.x, s11.x, s12.x, s13.x, s14.x, s15.x, op);
			}

			template< typename S >
			static auto norm_inf(const S& s){
				return algebra().norm_inf(s.x);
			}
		};
	}
}
namespace boost {
	namespace numeric {
		namespace odeint {
			template<typename state_, typename appendix_>
			struct norm_result_type < hmLib::odeint::state_with_appendix<state_, appendix_> > {
				typedef typename detail::extract_value_type< state_ >::type type;
			};

			template<typename state_, typename appendix_>
			struct same_size_impl< hmLib::odeint::state_with_appendix<state_, appendix_>, hmLib::odeint::state_with_appendix<state_, appendix_>> {
				static bool same_size(const  hmLib::odeint::state_with_appendix<state_, appendix_>& v1, const  hmLib::odeint::state_with_appendix<state_, appendix_>& v2) {
					return boost::numeric::odeint::same_size(v1.x, v2.x);
				}
			};

			template<typename state_, typename appendix_>
			struct resize_impl<  hmLib::odeint::state_with_appendix<state_, appendix_>, hmLib::odeint::state_with_appendix<state_, appendix_> > {
				static void resize(hmLib::odeint::state_with_appendix<state_, appendix_>& v1, const hmLib::odeint::state_with_appendix<state_, appendix_>& v2) {
					return boost::numeric::odeint::resize(v1.x, v2.x);
				}
			};

			template<typename state_, typename appendix_>
			struct is_resizeable<  hmLib::odeint::state_with_appendix<state_, appendix_> > {
				using type = typename is_resizeable<state_>::type;
				static const bool value = type::value;
			};
		}
	}
}
#
#endif
