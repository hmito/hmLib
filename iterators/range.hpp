#ifndef HMLIB_ITERATORS_RANGE_INC
#define HMLIB_ITERATORS_RANGE_INC 100
#
#include<iterator>
#include<type_traits>
namespace hmLib {
	namespace iterators {
		template<typename iterator_, bool is_random_access_iterator_ = std::is_same<typename std::iterator_traits<iterator_>::iterator_category, std::random_access_iterator_tag>::value>
		struct range_impl {
			using iterator = iterator_;
		public:
			range_impl() = default;
			range_impl(const iterator& Beg_, const iterator& End_):Beg(Beg_), End(End_) {}
			range_impl(iterator&& Beg_, iterator&& End_):Beg(std::move(Beg_)), End(std::move(End_)) {}
			iterator begin() { return Beg; }
			iterator end() { return End; }
		private:
			iterator Beg;
			iterator End;
		};
		template<typename iterator_>
		struct range_impl<iterator_, true> {
			using iterator = iterator_;
			using reference = typename std::iterator_traits<iterator>::reference;
			using difference_type = typename std::iterator_traits<iterator>::difference_type;
		public:
			range_impl() = default;
			range_impl(const iterator& Beg_, const iterator& End_):Beg(Beg_), End(End_) {}
			range_impl(iterator&& Beg_, iterator&& End_):Beg(std::move(Beg_)), End(std::move(End_)) {}
			iterator begin() { return Beg; }
			iterator end() { return End; }
			reference operator[](difference_type n) { return Beg[n]; }
			reference at(difference_type n) {
				hmLib_assert(n<size()&& 0<=n, hmLib::access_exceptions::out_of_range_access, "out of range.");
				return Beg[n];
			}
			difference_type size() { return std::distance(Beg, End); }
		private:
			iterator Beg;
			iterator End;
		};

	}
	template<typename iterator_, typename iterator_category_ = typename std::iterator_traits<iterator_>::iterator_category>
	using range = iterators::range_impl<iterator_>;

	template<typename iterator_>
	auto make_range(iterator_&& Beg, iterator_&& End) {
		return range<typename std::decay<iterator_>::type>(std::forward<iterator_>(Beg), std::forward<iterator_>(End));
	}
}
#
#endif
