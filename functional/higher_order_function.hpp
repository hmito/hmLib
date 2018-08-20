#ifndef HMLIB_FUNCTIONAL_HIGHERORDERFUNCTION_INC
#define HMLIB_FUNCTIONAL_HIGHERORDERFUNCTION_INC 100
#
namespace hmLib {
	template<typename target, typename... function>
	auto fmap(target Target, function... Functions) {
		fmap_later(std::forward<terget>(Target, std::forward<function>(Functions)...)).get();
	}

	template<typename target, typename... function>
	auto fmap_later(target Target, function... Functions);
}
#
#endif
