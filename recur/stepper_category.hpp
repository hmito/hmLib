#ifndef HMLIB_RECUR_STEPPERCATEGORY_INC
#define HMLIB_RECUR_STEPPERCATEGORY_INC 100
#
#include<concepts>
namespace hmLib{
	namespace recur{
		struct naive_stepper_tag{};
		struct stepper_tag {};

		template <typename stepper_type>
		concept naive_stepper = requires (stepper_type st) {
			typename stepper_type::stepper_category;
			requires std::convertible_to<typename stepper_type::stepper_category, naive_stepper_tag>;
		};
		template <typename stepper_type>
		concept stepper = requires (stepper_type st) {
			typename stepper_type::stepper_category;
			requires std::convertible_to<typename stepper_type::stepper_category, stepper_tag>;
		};
	}
}
#
#endif
