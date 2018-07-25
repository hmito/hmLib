#ifndef HMLIB_ITERATORS_TRANSFORMITERATOR_INC
#define HMLIB_ITERATORS_TRANSFORMITERATOR_INC 100
#
#include<iterator>
#include<type_traits>
namespace hmLib {
	template<typename base_iterator_, typename transform_, typename iterator_category_>
	struct transform_iterator {};
	template<typename base_iterator_, typename transform_>
	struct transform_iterator<base_iterator_, transform_, std::input_iterator_tag>{
		using this_type = transform_iterator<base_iterator_, transform_, std::input_iterator_tag>;
		using base_iterator = base_iterator_;
		using transform = transform_;
		using value_type = decltype(std::declval<transform_>()(std::declval<typename std::iterator_traits<base_iterator>::reference>()));
		using reference = decltype((std::declval<transform_>()(std::declval<typename std::iterator_traits<base_iterator>::reference>())));
		using pointer = void;
		using difference_type = typename std::iterator_traits<base_iterator>::difference_type;
		using iterator_category = std::input_iterator_tag;
	private:
		base_iterator Itr;
		transform_ Transform;
	public:
		transform_iterator() = default;
		transform_iterator(base_iterator Itr_, transform Transform_):Itr(Itr_), Transform(Transform_) {}
		this_type& operator++() { ++Itr; return *this; }
		this_type operator++(int) { this_type Prev = *this; operator++(); return Prev; }
		reference operator*() { return Transform(*Itr); }
		friend bool operator==(const this_type& itr1, const this_type& itr2) { return itr1.Itr==itr2.Itr; }
		friend bool operator!=(const this_type& itr1, const this_type& itr2) { return itr1.Itr!=itr2.Itr; }
	};
	template<typename base_iterator_, typename transform_>
	struct transform_iterator<base_iterator_, transform_, std::output_iterator_tag> {
		using this_type = transform_iterator<base_iterator_, transform_, std::output_iterator_tag>;
		using base_iterator = base_iterator_;
		using transform = transform_;
		using value_type = decltype(std::declval<transform_>()(std::declval<typename std::iterator_traits<base_iterator>::reference>()));
		using reference = decltype((std::declval<transform_>()(std::declval<typename std::iterator_traits<base_iterator>::reference>())));
		using pointer = void;
		using difference_type = typename std::iterator_traits<base_iterator>::difference_type;
		using iterator_category = std::output_iterator_tag;
	private:
	  base_iterator Itr;
	  transform_ Transform;
	public:
		transform_iterator() = default;
		transform_iterator(base_iterator Itr_, transform Transform_):Itr(Itr_), Transform(Transform_) {}
		this_type& operator++() { ++Itr; return *this; }
		this_type operator++(int) { this_type Prev = *this; operator++(); return Prev; }
		reference operator*() { return Transform(*Itr); }
	};
	template<typename base_iterator_, typename transform_>
	struct transform_iterator<base_iterator_, transform_, std::forward_iterator_tag>{
		using this_type = transform_iterator<base_iterator_, transform_, std::forward_iterator_tag>;
		using base_iterator = base_iterator_;
		using transform = transform_;
		using value_type = decltype(std::declval<transform_>()(std::declval<typename std::iterator_traits<base_iterator>::reference>()));
		using reference = decltype((std::declval<transform_>()(std::declval<typename std::iterator_traits<base_iterator>::reference>())));
		using pointer = void;
		using difference_type = typename std::iterator_traits<base_iterator>::difference_type;
		using iterator_category = std::forward_iterator_tag;
	private:
	  base_iterator Itr;
	  transform_ Transform;
	public:
		transform_iterator() = default;
		transform_iterator(base_iterator Itr_, transform Transform_):Itr(Itr_), Transform(Transform_) {}
		this_type& operator++() { ++Itr; return *this; }
		this_type operator++(int) { this_type Prev = *this; operator++(); return Prev; }
		reference operator*() { return Transform(*Itr); }
		friend bool operator==(const this_type& itr1, const this_type& itr2) { return itr1.Itr==itr2.Itr; }
		friend bool operator!=(const this_type& itr1, const this_type& itr2) { return itr1.Itr!=itr2.Itr; }
	};
	template<typename base_iterator_, typename transform_>
	struct transform_iterator<base_iterator_, transform_, std::bidirectional_iterator_tag> {
		using this_type = transform_iterator<base_iterator_, transform_, std::bidirectional_iterator_tag>;
		using base_iterator = base_iterator_;
		using transform = transform_;
		using value_type = decltype(std::declval<transform_>()(std::declval<typename std::iterator_traits<base_iterator>::reference>()));
		using reference = decltype((std::declval<transform_>()(std::declval<typename std::iterator_traits<base_iterator>::reference>())));
		using pointer = void;
		using difference_type = typename std::iterator_traits<base_iterator>::difference_type;
		using iterator_category = std::bidirectional_iterator_tag;
	private:
	  base_iterator Itr;
	  transform_ Transform;
	public:
		transform_iterator() = default;
		transform_iterator(base_iterator Itr_, transform Transform_):Itr(Itr_), Transform(Transform_) {}
		this_type& operator++() { ++Itr; return *this; }
		this_type operator++(int) { this_type Prev = *this; operator++(); return Prev; }
		this_type& operator--() { --Itr; return *this; }
		this_type operator--(int) { this_type Prev = *this; operator--(); return Prev; }
		reference operator*() { return Transform(*Itr); }
		friend bool operator==(const this_type& itr1, const this_type& itr2) { return itr1.Itr==itr2.Itr; }
		friend bool operator!=(const this_type& itr1, const this_type& itr2) { return itr1.Itr!=itr2.Itr; }
	};
	template<typename base_iterator_, typename transform_>
	struct transform_iterator<base_iterator_, transform_, std::random_access_iterator_tag> {
		using this_type = transform_iterator<base_iterator_, transform_, std::random_access_iterator_tag>;
		using base_iterator = base_iterator_;
		using transform = transform_;
		using value_type = decltype(std::declval<transform_>()(std::declval<typename std::iterator_traits<base_iterator>::reference>()));
		using reference = decltype((std::declval<transform_>()(std::declval<typename std::iterator_traits<base_iterator>::reference>())));
		using pointer = void;
		using difference_type = typename std::iterator_traits<base_iterator>::difference_type;
		using iterator_category = std::random_access_iterator_tag;
	private:
	  base_iterator Itr;
	  transform_ Transform;
	public:
		transform_iterator() = default;
		transform_iterator(base_iterator Itr_, transform Transform_):Itr(Itr_), Transform(Transform_) {}
		this_type& operator++() { ++Itr; return *this; }
		this_type operator++(int) { this_type Prev = *this; operator++(); return Prev; }
		this_type& operator--() { --Itr; return *this; }
		this_type operator--(int) { this_type Prev = *this; operator--(); return Prev; }
		this_type& operator+=(difference_type n) { Itr += n; return *this; }
		friend this_type operator+(const this_type& itr, difference_type n) { this_type nitr = itr; nitr += n; return nitr; }
		friend this_type operator+(difference_type n, const this_type& itr) { this_type nitr = itr; nitr += n; return nitr; }
		this_type& operator-=(difference_type n) { Itr -= n; return *this; }
		friend this_type operator-(const this_type& itr, difference_type n) { this_type nitr = itr; nitr -= n; return nitr; }
		friend difference_type operator-(this_type itr1, const this_type& itr2) {return itr1.Itr - itr2.Itr; }
		reference operator*() { return Transform(*Itr); }
		friend bool operator==(const this_type& itr1, const this_type& itr2) { return itr1.Itr==itr2.Itr; }
		friend bool operator!=(const this_type& itr1, const this_type& itr2) { return itr1.Itr!=itr2.Itr; }
		friend bool operator>(const this_type& itr1, const this_type& itr2) { return itr1.Itr>itr2.Itr; }
		friend bool operator>=(const this_type& itr1, const this_type& itr2) { return itr1.Itr>=itr2.Itr; }
		friend bool operator<(const this_type& itr1, const this_type& itr2) { return itr1.Itr<itr2.Itr; }
		friend bool operator<=(const this_type& itr1, const this_type& itr2) { return itr1.Itr<=itr2.Itr; }
	};

	template<typename base_iterator_, typename transform_, typename iterator_category_>
	struct transform_range {
		using base_iterator = base_iterator_;
		using transform = transform_;
		using iterator = transform_iterator<base_iterator_, transform_, iterator_category_>;
	private:
		base_iterator Beg;
		base_iterator End;
		transform Transform;
	public:
		transform_range() = default;
		transform_range(base_iterator Beg_, base_iterator End_, transform Transform_):Beg(std::move(Beg_)), End(std::move(End_)), Transform(std::move(Transform_)) {}
		iterator begin() { return iterator(Beg, Transform); }
		iterator end() { return iterator(End, Transform); }
	};
	template<typename base_iterator_, typename transform_>
	transform_range<typename std::decay<base_iterator_>::type, typename std::decay<transform_>::type, std::input_iterator_tag> make_input_transform_range(base_iterator_ Beg, base_iterator_ End, transform_&& Transform) {
		return transform_range<typename std::decay<base_iterator_>::type, typename std::decay<transform_>::type, std::input_iterator_tag>(std::move(Beg), std::move(End), std::move(Transform));
	}
	template<typename base_iterator_, typename transform_>
	transform_range<typename std::decay<base_iterator_>::type, typename std::decay<transform_>::type, std::output_iterator_tag> make_output_transform_range(base_iterator_ Beg, base_iterator_ End, transform_&& Transform) {
		return transform_range<typename std::decay<base_iterator_>::type, typename std::decay<transform_>::type, std::output_iterator_tag>(std::move(Beg), std::move(End), std::move(Transform));
	}
	template<typename base_iterator_, typename transform_>
	transform_range<typename std::decay<base_iterator_>::type, typename std::decay<transform_>::type, std::forward_iterator_tag> make_forward_transform_range(base_iterator_ Beg, base_iterator_ End, transform_&& Transform) {
		return transform_range<typename std::decay<base_iterator_>::type, typename std::decay<transform_>::type, std::forward_iterator_tag>(std::move(Beg), std::move(End), std::move(Transform));
	}
	template<typename base_iterator_, typename transform_>
	transform_range<typename std::decay<base_iterator_>::type, typename std::decay<transform_>::type, std::bidirectional_iterator_tag> make_bidirectional_transform_range(base_iterator_ Beg, base_iterator_ End, transform_&& Transform) {
		return transform_range<typename std::decay<base_iterator_>::type, typename std::decay<transform_>::type, std::bidirectional_iterator_tag>(std::move(Beg), std::move(End), std::move(Transform));
	}
	template<typename base_iterator_, typename transform_>
	transform_range<typename std::decay<base_iterator_>::type, typename std::decay<transform_>::type, std::random_access_iterator_tag> make_random_access_transform_range(base_iterator_ Beg, base_iterator_ End, transform_&& Transform) {
		return transform_range<typename std::decay<base_iterator_>::type, typename std::decay<transform_>::type, std::random_access_iterator_tag>(std::move(Beg), std::move(End), std::move(Transform));
	}
}
#
#endif
