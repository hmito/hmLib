#ifndef HMLIB_MONAD_MONADMIXIN_INC
#define HMLIB_MONAD_MONADMIXIN_INC 100
#
#include <utility>
#include "monad_traits.hpp"
#include "fmap.hpp"
#include "fmap_later.hpp"
namespace hmLib {
	namespace monad {
		template<typename Type, typename value_type_, typename monad_category_>
		struct monad_mixin {
			using monad_category = monad_category_;
			using value_type = value_type_;
		public:
			//for monad
			//template<typename U>
			//struct rebind { using type = Type<U>; };
			//explicit just(T val_) :val(std::move(val_)) {}
			//template<typename fn>
			//auto apply(fn&& Func)const {
			//	return omitted_wrap<this_type>(hmLib::monad::fmap(val, Func));
			//}
		public:
			//for omitable_monad/flattenable_monad
			//static just<T> flatten(const just<just<T>>& m);
		public:
			template<typename fn>
			auto fmap(fn&& Func)const & {
				return monad::fmap(static_cast<const Type&>(*this), std::forward<fn>(Func));
			}
			template<typename fn>
			auto fmap(fn&& Func) && {
				return monad::fmap(static_cast<Type&&>(*this), std::forward<fn>(Func));
			}
			template<typename fn>
			auto fmap_later(fn&& Func)const & {
				return monad::fmap_later(static_cast<const Type&>(*this), std::forward<fn>(Func));
			}
			template<typename fn>
			auto fmap_later(fn&& Func) && {
				return monad::fmap_later(static_cast<Type&&>(*this), std::forward<fn>(Func));
			}
		public:
			//for monad
			template<typename fn>
			friend auto operator >> (Type&& This, fn&& Func) {
				return std::move(This).fmap_later(std::forward<fn>(Func));
			}
			//for monad
			template<typename fn>
			friend auto operator >> (const Type& This, fn&& Func) {
				return This.fmap_later(std::forward<fn>(Func));
			}
		};

		
	}

	namespace monad {
		template<typename T>
		struct monadic_define {
			//monadic context of this type is not defined.
			//auto operator()(T v)const noexcept;
		};
	}
	template<typename T>
	auto monadic(T&& val) { return monad::monadic_define<typename std::decay<T>::type>()(std::forward<T>(val)); }
}
#
#endif
