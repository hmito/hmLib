#ifndef HMLIB_ITERATORS_RANGE_INC
#define HMLIB_ITERATORS_RANGE_INC 100
#
#include<iterator>
namespace hmLib {
	template<typename iterator_, typename iterator_category_ = typename std::iterator_traits<iterator_>::iterator_category>
	struct range{};
	template<typename iterator_>
	struct range<iterator_, std::input_iterator_tag>{
		using iterator = iterator_;
	public:
		range() = default;
		range(const iterator& Beg_, const iterator& End_):Beg(Beg_), End(End_) {}
		range(iterator&& Beg_, iterator&& End_):Beg(std::move(Beg_)),End(std::move(End_)){}
		iterator begin() { return Beg; }
		iterator end() { return End; }
	private:
		iterator Beg;
		iterator End;
	};
	template<typename iterator_>
	struct range<iterator_, std::forward_iterator_tag> {
		using iterator = iterator_;
	public:
		range() = default;
		range(const iterator& Beg_, const iterator& End_):Beg(Beg_), End(End_) {}
		range(iterator&& Beg_, iterator&& End_):Beg(std::move(Beg_)), End(std::move(End_)) {}
		iterator begin() { return Beg; }
		iterator end() { return End; }
	private:
		iterator Beg;
		iterator End;
	};
	template<typename iterator_>
	struct range<iterator_, std::bidirectional_iterator_tag> {
		using iterator = iterator_;
	public:
		range() = default;
		range(const iterator& Beg_, const iterator& End_):Beg(Beg_), End(End_) {}
		range(iterator&& Beg_, iterator&& End_):Beg(std::move(Beg_)), End(std::move(End_)) {}
		iterator begin() { return Beg; }
		iterator end() { return End; }
	private:
		iterator Beg;
		iterator End;
	};
	template<typename iterator_>
	struct range<iterator_, std::random_access_iterator_tag> {
		using iterator = iterator_;
		using reference = typename std::iterator_traits<iterator>::reference;
		using difference_type = typename std::iterator_traits<iterator>::difference_type;
	public:
		range() = default;
		range(const iterator& Beg_, const iterator& End_):Beg(Beg_), End(End_) {}
		range(iterator&& Beg_, iterator&& End_):Beg(std::move(Beg_)), End(std::move(End_)) {}
		iterator begin() { return Beg; }
		iterator end() { return End; }
		reference operator[](difference_type n) { return Beg[n]; }
		reference at(difference_type n) {
			hmLib_assert(n<size()&& 0<=n, hmLib::access_exceptions::out_of_range_access, "out of range.");
			return Beg[n];
		}
		difference_type size() { return std::distance(Beg,End); }
	private:
		iterator Beg;
		iterator End;
	};

	template<typename iterator_>
	auto make_range(iterator_&& Beg, iterator_&& End) {
		return range<typename std::decay<iterator_>::type>(std::forward<iterator_>(Beg), std::forward<iterator_>(End));
	}
}
#
#endif
