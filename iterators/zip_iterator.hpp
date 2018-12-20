#ifndef HMLIB_ITERATORS_ZIPITERATOR_INC
#define HMLIB_ITERATORS_ZIPITERATOR_INC 100
#
#include<iterator>
#include<tuple>
#include<limits>
#include"../tuple.hpp"
#include"../clone_ptrproxy.hpp"
namespace hmLib {

	namespace iterators {
		template<typename... iterators>
		struct zip_iterator_traits {
		public:
			using value_type = std::tuple<typename std::iterator_traits<iterators>::value_type...>;
			using difference_type = typename std::common_type<typename std::iterator_traits<iterators>::difference_type...>::type;
			using reference = std::tuple<typename std::iterator_traits<iterators>::reference...>;
			using pointer = std::tuple<typename std::iterator_traits<iterators>::pointer...>;
			using iterator_category = typename std::common_type<typename std::iterator_traits<iterators>::iterator_category...>::type;
		};
		template<typename iterator_category_, unsigned int order_iterator_pos_, typename... iterators_>
		struct zip_iterator_impl;
		template<unsigned int order_iterator_pos_, typename... iterators_>
		struct zip_iterator_impl<std::input_iterator_tag, order_iterator_pos_, iterators_...> {
		private:
			using this_type = zip_iterator_impl<std::input_iterator_tag, order_iterator_pos_, iterators_...>;
			using iterator_traits = zip_iterator_traits<iterators_...>;
		public:
			using iterator_pack = std::tuple<iterators_...>;
			using value_type = typename iterator_traits::value_type;
			using difference_type = typename iterator_traits::difference_type;
			using reference = typename iterator_traits::reference;
			using pointer = clone_ptrproxy<reference>;
			using iterator_category = std::input_iterator_tag;
			constexpr static std::size_t static_size() { return sizeof...(iterators_); }
		public:
			zip_iterator_impl() = default;
			explicit zip_iterator_impl(iterator_pack Itrs_):Itrs(Itrs_) {}
			template<typename... other_iterators_>
			zip_iterator_impl(other_iterators_... Itrs_):Itrs(Itrs_...){}
			reference operator*() { return hmLib::tuple_for_each([](auto& itr) ->auto&{return *itr; }, Itrs); }
			pointer operator->() { return pointer(operator*()); }
			this_type& operator++() { hmLib::tuple_for_each([](auto& itr) {return ++itr; }, Itrs); return *this; }
			this_type operator++(int) {
				this_type Prev = *this;
				operator++();
				return Prev;
			}
			const iterator_pack& pack()const { return Itrs; }
			friend bool operator==(const this_type& itr1, const this_type& itr2) {
				return std::get<order_iterator_pos_>(itr1.Itrs)==std::get<order_iterator_pos_>(itr2.Itrs);
			}
			friend bool operator!=(const this_type& itr1, const this_type& itr2) {
				return std::get<order_iterator_pos_>(itr1.Itrs)!=std::get<order_iterator_pos_>(itr2.Itrs);
			}
		private:
			iterator_pack Itrs;
		};
		template<unsigned int order_iterator_pos_, typename... iterators_>
		struct zip_iterator_impl<std::forward_iterator_tag, order_iterator_pos_, iterators_...> {
		private:
			using this_type = zip_iterator_impl<std::forward_iterator_tag, order_iterator_pos_, iterators_...>;
			using iterator_traits = zip_iterator_traits<iterators_...>;
		public:
			using iterator_pack = std::tuple<iterators_...>;
			using value_type = typename iterator_traits::value_type;
			using difference_type = typename iterator_traits::difference_type;
			using reference = typename iterator_traits::reference;
			using pointer = clone_ptrproxy<reference>;
			using iterator_category = std::forward_iterator_tag;
			constexpr static std::size_t static_size() { return sizeof...(iterators_); }
		public:
			zip_iterator_impl() = default;
			explicit zip_iterator_impl(iterator_pack Itrs_):Itrs(Itrs_) {}
			template<typename... other_iterators_>
			zip_iterator_impl(other_iterators_... Itrs_) : Itrs(Itrs_...) {}
			reference operator*() { return hmLib::tuple_for_each([](auto& itr)->auto& {return *itr; }, Itrs); }
			pointer operator->() { return pointer(operator*()); }
			this_type& operator++() { hmLib::tuple_for_each([](auto& itr) {return ++itr; }, Itrs); return *this; }
			this_type operator++(int) {
				this_type Prev = *this;
				operator++();
				return Prev;
			}
			const iterator_pack& pack()const { return Itrs; }
			friend bool operator==(const this_type& itr1, const this_type& itr2) {
				return std::get<order_iterator_pos_>(itr1.Itrs)==std::get<order_iterator_pos_>(itr2.Itrs);
			}
			friend bool operator!=(const this_type& itr1, const this_type& itr2) {
				return std::get<order_iterator_pos_>(itr1.Itrs)!=std::get<order_iterator_pos_>(itr2.Itrs);
			}
		private:
			iterator_pack Itrs;
		};
		template<unsigned int order_iterator_pos_, typename... iterators_>
		struct zip_iterator_impl<std::bidirectional_iterator_tag, order_iterator_pos_, iterators_...> {
		private:
			using this_type = zip_iterator_impl<std::bidirectional_iterator_tag, order_iterator_pos_, iterators_...>;
			using iterator_traits = zip_iterator_traits<iterators_...>;
		public:
			using iterator_pack = std::tuple<iterators_...>;
			using value_type = typename iterator_traits::value_type;
			using difference_type = typename iterator_traits::difference_type;
			using reference = typename iterator_traits::reference;
			using pointer = clone_ptrproxy<reference>;
			using iterator_category = std::bidirectional_iterator_tag;
			constexpr static std::size_t static_size() { return sizeof...(iterators_); }
		public:
			zip_iterator_impl() = default;
			explicit zip_iterator_impl(iterator_pack Itrs_):Itrs(Itrs_) {}
			template<typename... other_iterators_>
			zip_iterator_impl(other_iterators_... Itrs_) : Itrs(Itrs_...) {}
			reference operator*() { return hmLib::tuple_for_each([](auto& itr)->auto& {return *itr; }, Itrs); }
			pointer operator->() { return pointer(operator*()); }
			this_type& operator++() { hmLib::tuple_for_each([](auto& itr) {return ++itr; }, Itrs); return *this; }
			this_type operator++(int) {
				this_type Prev = *this;
				operator++();
				return Prev;
			}
			this_type& operator--() { hmLib::tuple_for_each([](auto& itr) {return --itr; }, Itrs); return *this; }
			this_type operator--(int) {
				this_type Prev = *this;
				operator--();
				return Prev;
			}
			const iterator_pack& pack()const { return Itrs; }
			friend bool operator==(const this_type& itr1, const this_type& itr2) {
				return std::get<order_iterator_pos_>(itr1.Itrs)==std::get<order_iterator_pos_>(itr2.Itrs);
			}
			friend bool operator!=(const this_type& itr1, const this_type& itr2) {
				return std::get<order_iterator_pos_>(itr1.Itrs)!=std::get<order_iterator_pos_>(itr2.Itrs);
			}
		private:
			iterator_pack Itrs;
		};
		template<unsigned int order_iterator_pos_, typename... iterators_>
		struct zip_iterator_impl<std::random_access_iterator_tag, order_iterator_pos_, iterators_...> {
		private:
			using this_type = zip_iterator_impl<std::random_access_iterator_tag, order_iterator_pos_, iterators_...>;
			using iterator_traits = zip_iterator_traits<iterators_...>;
		public:
			using iterator_pack = std::tuple<iterators_...>;
			using value_type = typename iterator_traits::value_type;
			using difference_type = typename iterator_traits::difference_type;
			using reference = typename iterator_traits::reference;
			using pointer = clone_ptrproxy<reference>;
			using iterator_category = std::random_access_iterator_tag;
			constexpr static std::size_t static_size() { return sizeof...(iterators_); }
		public:
			zip_iterator_impl() = default;
			explicit zip_iterator_impl(iterator_pack Itrs_):Itrs(Itrs_) {}
			template<typename... other_iterators_>
			zip_iterator_impl(other_iterators_... Itrs_) : Itrs(Itrs_...) {}
			reference operator*() { return hmLib::tuple_for_each([](auto& itr)->decltype((*itr)) {return *itr; }, Itrs); }
			reference operator[](difference_type n) { return hmLib::tuple_for_each([n](auto& itr)->decltype((*itr)) {return itr[n]; }, Itrs); }
			pointer operator->() { return pointer(operator*()); }
			this_type& operator++() { hmLib::tuple_for_each([](auto& itr) {return ++itr; }, Itrs); return *this; }
			this_type operator++(int) {
				this_type Prev = *this;
				operator++();
				return Prev;
			}
			this_type& operator--() { hmLib::tuple_for_each([](auto& itr) {return --itr; }, Itrs); return *this; }
			this_type operator--(int) {
				this_type Prev = *this;
				operator--();
				return Prev;
			}
			this_type& operator+=(difference_type n) { hmLib::tuple_for_each([n](auto& itr) {return itr += n; }, Itrs); return *this; }
			this_type& operator-=(difference_type n) { hmLib::tuple_for_each([n](auto& itr) {return itr -= n; }, Itrs); return *this; }
			const iterator_pack& pack()const { return Itrs; }
			friend this_type operator+(const this_type& itr, difference_type n) {
				auto ans = itr;
				ans += n;
				return ans;
			}
			friend this_type operator+(difference_type n, const this_type& itr) {
				auto ans = itr;
				ans += n;
				return ans;
			}
			friend this_type operator-(const this_type& itr, difference_type n) {
				auto ans = itr;
				ans -= n;
				return ans;
			}
			friend difference_type operator-(const this_type& itr1, const this_type& itr2) {
				return std::get<order_iterator_pos_>(itr1.Itrs)-std::get<order_iterator_pos_>(itr2.Itrs);
			}
			friend bool operator==(const this_type& itr1, const this_type& itr2) {
				return std::get<order_iterator_pos_>(itr1.Itrs)==std::get<order_iterator_pos_>(itr2.Itrs);
			}
			friend bool operator!=(const this_type& itr1, const this_type& itr2) {
				return std::get<order_iterator_pos_>(itr1.Itrs)!=std::get<order_iterator_pos_>(itr2.Itrs);
			}
			friend bool operator>(const this_type& itr1, const this_type& itr2) {
				return std::get<order_iterator_pos_>(itr1.Itrs)>std::get<order_iterator_pos_>(itr2.Itrs);
			}
			friend bool operator<(const this_type& itr1, const this_type& itr2) {
				return std::get<order_iterator_pos_>(itr1.Itrs)<std::get<order_iterator_pos_>(itr2.Itrs);
			}
			friend bool operator>=(const this_type& itr1, const this_type& itr2) {
				return std::get<order_iterator_pos_>(itr1.Itrs)>=std::get<order_iterator_pos_>(itr2.Itrs);
			}
			friend bool operator<=(const this_type& itr1, const this_type& itr2) {
				return std::get<order_iterator_pos_>(itr1.Itrs)<=std::get<order_iterator_pos_>(itr2.Itrs);
			}
		private:
			iterator_pack Itrs;
		};
		namespace detail {
			template<unsigned int n>
			struct zip_iterator_shorten_distance{
				template<typename iterator>
				int operator()(const iterator& itr1, const iterator& itr2) {
					return std::min<int>(
						std::distance(std::get<n-1>(itr1.pack()), std::get<n-1>(itr2.pack())),
						zip_iterator_shorten_distance<n-1>()(itr1, itr2)
					);
				}
			};
			template<>
			struct zip_iterator_shorten_distance<0> {
				template<typename iterator>
				int operator()(const iterator& itr1, const iterator& itr2) {
					return std::numeric_limits<int>::max();
				}
			};
		}
	}
	template<typename... iterators_>
	using zip_iterator = iterators::zip_iterator_impl<
		typename iterators::zip_iterator_traits<iterators_...>::iterator_category, 0, iterators_...
	>;
	template<typename... iterators_>
	auto make_zip_iterator(iterators_&&... Itrs) {
		return zip_iterator<typename std::decay<iterators_>::type...>(std::forward<iterators_>(Itrs)...);
	}
	template<typename... iterators_>
	auto make_zip_iterator(std::tuple<iterators_...> Itrs) {
		return zip_iterator<typename std::decay<iterators_>::type...>(std::move(Itrs));
	}

	template<typename... iterators_>
	auto zip_iterator_shorten(
		const zip_iterator<iterators_...>& Beg,
		const zip_iterator<iterators_...>& End) {
		int Dif = iterators::detail::zip_iterator_shorten_distance<sizeof...(iterators_)>()(Beg, End);

		hmLib_assert_any(Dif>=0, "distance of given iterator pair is negative.");

		return std::next(Beg, Dif);
	}

	namespace iterators {
		namespace detail {
			template<bool is_random_access, typename... iterators_>
			struct zip_range_impl {
			public:
				using iterator = zip_iterator<iterators_...>;
				using range_category = typename std::iterator_traits<iterator>::iterator_category;
				using value_type = typename std::iterator_traits<iterator>::value_type;
				using reference = typename std::iterator_traits<iterator>::reference;
				using pointer = typename std::iterator_traits<iterator>::pointer;
				using difference_type = typename std::iterator_traits<iterator>::difference_type;
			private:
				iterator Beg;
				iterator End;
			public:
				zip_range_impl() = default;
				zip_range_impl(iterator Beg_, iterator End_):Beg(Beg_), End(End_) {
					End = zip_iterator_shorten(Beg, End);
				}
				template<typename... ranges>
				zip_range_impl(ranges&&... Ranges) {
					Beg = iterator(std::begin(Ranges)...);
					End = iterator(std::end(Ranges)...);
					End = zip_iterator_shorten(Beg, End);
				}
				iterator begin()const { return Beg; }
				iterator end()const { return End; }
			};
			template<typename... iterators_>
			struct zip_range_impl<true, iterators_...>{
			public:
				using iterator = zip_iterator<iterators_...>;
				using range_category = typename std::iterator_traits<iterator>::iterator_category;
				using value_type = typename std::iterator_traits<iterator>::value_type;
				using reference = typename std::iterator_traits<iterator>::reference;
				using pointer = typename std::iterator_traits<iterator>::pointer;
				using difference_type = typename std::iterator_traits<iterator>::difference_type;
				using size_type = std::size_t;
			private:
				iterator Beg;
				iterator End;
			public:
				zip_range_impl() = default;
				zip_range_impl(iterator Beg_, iterator End_):Beg(Beg_), End(End_) {
					End = zip_iterator_shorten(Beg, End);
				}
				template<typename... ranges>
				zip_range_impl(ranges&&... Ranges) {
					Beg = iterator(std::begin(Ranges)...);
					End = iterator(std::end(Ranges)...);
					End = zip_iterator_shorten(Beg, End);
				}
				iterator begin()const { return Beg; }
				iterator end()const { return End; }
				reference operator[](difference_type Index) { return Beg[Index]; }
				reference at(difference_type Index) {
					hmLib_assert(0 <= Index && static_cast<size_type>(Index)<size(), hmLib::access_exceptions::out_of_range_access, "out of range access for zip_container.");
					return Beg[Index];
				}
				size_type size() { return static_cast<size_type>(End-Beg); }
			};
		}
	}
	template<typename... iterators_>
	using zip_range = iterators::detail::zip_range_impl<
		std::is_same<std::random_access_iterator_tag, typename std::iterator_traits<zip_iterator<iterators_...>>::iterator_category>::value,
		iterators_...
	>;

	template<typename... ranges>
	auto make_zip_range(ranges&&... Ranges) {
		return zip_range<decltype(std::begin(Ranges))...>(std::forward<ranges>(Ranges)...);
	}
	template<typename... iterators_>
	auto make_zip_range(zip_iterator<iterators_...> Beg, zip_iterator<iterators_...> End) {
		return zip_range<iterators_...>(std::move(Beg), std::move(End));
	}
}
#
#endif
