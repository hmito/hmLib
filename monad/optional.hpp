#ifndef HMLIB_MONAD_OPTIONAL_INC
#define HMLIB_MONAD_OPTIONAL_INC 100
#
#include<utility>
#include<boost/optional.hpp>
#include"monad_traits.hpp"
#include"monad_mixin.hpp"
namespace hmLib {
	namespace monad {
		template<typename T>
		struct optional :public monad_mixin<optional<T>, T, monad_categories::omittable_monad_tag> {
		private:
			using this_type = optional<T>;
		private:
			boost::optional<T> val;
		public:
			optional() = default;
			optional(const this_type&) = default;
			this_type& operator=(const this_type&) = default;
			optional(this_type&&) = default;
			this_type& operator=(this_type&&) = default;
			explicit optional(boost::optional<T> val_) :val(std::move(val_)) {}
		public:
			//for monad
			template<typename U>
			struct rebind { using type = optional<U>; };
			explicit optional(T val_) :val(std::move(val_)) {}
			template<typename fn>
			auto apply(fn&& Func)const {
				using ans_type = decltype(omitted_wrap<this_type>(hmLib::functional::fmap(std::declval<T>(), Func)));
				if (!val)return ans_type();
				return omitted_wrap<this_type>(hmLib::functional::fmap(*val, Func));
			}
		public:
			//for omitable_monad/flattenable_monad
			static optional<T> flatten(const optional<optional<T>>& m_) {
				if (!m_)return this_type();
				if (!(m_.get()))return this_type();
				return this_type(m_.get().get());
			}
		public:
			operator bool()const { return static_cast<bool>(val); }
			T& get() { return val.get(); }
			const T& get()const { return val.get(); }
			void set(T val_) { val.get() = std::move(val_); }
		};
		template<typename T>
		auto make_optional(T&& val) { return optional<typename std::decay<T>::type>(std::forward<T>(val)); }

		template<typename T>
		struct monadic_define<boost::optional<T>> {
			auto operator()(boost::optional<T> v)const noexcept { return optional<T>(std::move(v)); }
		};
	}
}
#
#endif
