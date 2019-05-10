#ifndef HMLIB_ODEINT_BORDER_ELEMENTBORDER_INC
#define HMLIB_ODEINT_BORDER_ELEMENTBORDER_INC 100
#
#include <functional>
#include <boost/range.hpp>
namespace hmLib {
	namespace odeint {
		template<typename value_type>
		struct lower_border {
		public:
			lower_border() = default;
			lower_border(size_t pos_, value_type value_) :pos(pos_), value(value_) {}
			void set(size_t pos_, value_type value_) {
				pos = pos_;
				value = value_;
			}
		public:
			template<typename state_type>
			void force_zero_deriv(state_type& dx, bool ap)const {
				if (ap) {
					*std::next(boost::begin(dx), pos) = 0;
				}
			}
			template<typename state_type, typename time_type>
			bool operator()(const state_type& x, state_type& dx, time_type t, bool ap)const {
				if (ap && *std::next(boost::begin(dx), pos) < 0) {
					*std::next(boost::begin(dx), pos) = 0;
					return true;
				}
				return false;
			}
			template<typename state_type, typename time_type>
			bool is_invalid_step(const state_type& x, const state_type& dx, time_type t, bool ap)const {
				if (ap) {
					return *std::next(boost::begin(dx), pos) > 0;
				}
				else {
					return *std::next(boost::begin(x), pos) < value;
				}
			}
			template<typename state_type, typename time_type>
			bool will_validate(const state_type& x, time_type t, bool ap)const {
				return (*std::next(boost::begin(x), pos) < value);
			}
			template<typename state_type, typename time_type>
			bool validate(state_type& x, const state_type& dx, time_type t, bool& ap)const {
				bool ans = false;
				if (ap) {
					if (*std::next(boost::begin(x), pos) < value) {
						ans = true;
						*std::next(boost::begin(x), pos) = value;
					}

					ap = (*std::next(boost::begin(dx), pos) <= 0);
				}
				else {
					if (*std::next(boost::begin(x), pos) < value) {
						ans = true;
						*std::next(boost::begin(x), pos) = value;

						ap = (*std::next(boost::begin(dx), pos) <= 0);
					}
					else {
						ap = false;

					}
				}
				return ans;
			}
		private:
			size_t pos;
			value_type value;
		};

		template<typename value_type>
		struct upper_border {
		public:
			upper_border() = default;
			upper_border(size_t pos_, value_type value_) :pos(pos_), value(value_) {}
			void set(size_t pos_, value_type value_) {
				pos = pos_;
				value = value_;
			}
		public:
			template<typename state_type>
			void force_zero_deriv(state_type& dx, bool ap)const {
				if (ap) {
					*std::next(boost::begin(dx), pos) = 0;
				}
			}
			template<typename state_type, typename time_type>
			bool operator()(const state_type& x, state_type& dx, time_type t, bool ap)const {
				if (ap && *std::next(boost::begin(dx), pos) > 0) {
					*std::next(boost::begin(dx), pos) = 0;
					return true;
				}
				return false;
			}
			template<typename state_type, typename time_type>
			bool is_invalid_step(const state_type& x, const state_type& dx, time_type t, bool ap)const {
				if (ap) {
					return *std::next(boost::begin(dx), pos) < 0;
				}
				else {
					return *std::next(boost::begin(x), pos) > value;
				}
			}
			template<typename state_type, typename time_type>
			bool will_validate(const state_type& x, time_type t, bool ap)const {
				return (*std::next(boost::begin(x), pos) > value);
			}
			template<typename state_type, typename time_type>
			bool validate(state_type& x, const state_type& dx, time_type t, bool& ap)const {
				bool ans = false;
				if (ap) {
					if (*std::next(boost::begin(x), pos) > value) {
						ans = true;
						*std::next(boost::begin(x), pos) = value;
					}

					ap = (*std::next(boost::begin(dx), pos) >= 0);
				}
				else {
					if (*std::next(boost::begin(x), pos) > value) {
						ans = true;
						*std::next(boost::begin(x), pos) = value;

						ap = (*std::next(boost::begin(dx), pos) >= 0);
					}
					else {
						ap = false;

					}
				}
				return ans;
			}
		private:
			size_t pos;
			value_type value;
		};

	}
}
#
#endif
