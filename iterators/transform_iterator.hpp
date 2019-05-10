#ifndef HMLIB_ITERATORS_TRANSFORMITERATOR_INC
#define HMLIB_ITERATORS_TRANSFORMITERATOR_INC 100
#
#include<iterator>
#include<type_traits>
#include"../exceptions.hpp"
#include"../clone_ptrproxy.hpp"
#include"iterator.hpp"
namespace hmLib {
	namespace iterators {
		namespace detail {
			template<typename base_iterator_, typename transform_>
			struct transform_iterator_category {
			private:
				using base_reference = typename std::iterator_traits<base_iterator_>::reference;
			private:
				using raw_value_type = typename std::remove_reference<decltype(std::declval<transform_>()(std::declval<base_reference>()))>::type;
				using raw_reference = decltype((std::declval<transform_>()(std::declval<base_reference>())));
			public:
				static constexpr bool is_reference = std::is_same<raw_value_type&, raw_reference>::value;
				using value_type = raw_value_type;
				using reference = typename std::conditional<is_reference, raw_reference, const raw_value_type>::type;
				using pointer = typename std::conditional<is_reference, raw_value_type*, clone_ptrproxy<raw_value_type> >::type;
			};
		}
	}

	template<typename base_iterator_, typename transform_, typename iterator_category_ = typename std::iterator_traits<base_iterator_>::iterator_category>
	struct transform_iterator{};
	template<typename base_iterator_, typename transform_>
	struct transform_iterator<base_iterator_, transform_, std::input_iterator_tag>{
		using this_type = transform_iterator<base_iterator_, transform_, std::input_iterator_tag>;
	private:
		using base_iterator = base_iterator_;
		using transform = transform_;
		using transform_iterator_category = iterators::detail::transform_iterator_category<base_iterator, transform>;
	public:
		using value_type = typename transform_iterator_category::value_type;
		using reference = typename transform_iterator_category::reference;
		using pointer = typename transform_iterator_category::pointer;
		using difference_type = typename std::iterator_traits<base_iterator>::difference_type;
		using iterator_category = std::input_iterator_tag;
	private:
		base_iterator Itr;
		transform_ Transform;
	public:
		transform_iterator() = default;
		transform_iterator(base_iterator Itr_, transform Transform_):Itr(Itr_), Transform(Transform_) {}
		//template<typename... transform_args_>
		//transform_iterator(base_iterator Itr_, transform_args_... TransformArgs_):Itr(Itr_), Transform(TransformArgs_...) {}
		this_type& operator++() { ++Itr; return *this; }
		this_type operator++(int) { this_type Prev = *this; operator++(); return Prev; }
		reference operator*() { return Transform(*Itr); }
		pointer operator->() { return pointer(&Transform(*Itr)); }
		base_iterator base() { return Itr; }
		friend bool operator==(const this_type& itr1, const this_type& itr2) { return itr1.Itr==itr2.Itr; }
		friend bool operator!=(const this_type& itr1, const this_type& itr2) { return itr1.Itr!=itr2.Itr; }
	};
	template<typename base_iterator_, typename transform_>
	struct transform_iterator<base_iterator_, transform_, std::output_iterator_tag> {
		using this_type = transform_iterator<base_iterator_, transform_, std::output_iterator_tag>;
	private:
		using base_iterator = base_iterator_;
		using transform = transform_;
		using transform_iterator_category = iterators::detail::transform_iterator_category<base_iterator, transform>;
	public:
		using value_type = typename transform_iterator_category::value_type;
		using reference = typename transform_iterator_category::reference;
		using pointer = typename transform_iterator_category::pointer;
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
		base_iterator base() { return Itr; }
	};
	template<typename base_iterator_, typename transform_>
	struct transform_iterator<base_iterator_, transform_, std::forward_iterator_tag>{
		using this_type = transform_iterator<base_iterator_, transform_, std::forward_iterator_tag>;
	private:
		using base_iterator = base_iterator_;
		using transform = transform_;
		using transform_iterator_category = iterators::detail::transform_iterator_category<base_iterator, transform>;
	public:
		using value_type = typename transform_iterator_category::value_type;
		using reference = typename transform_iterator_category::reference;
		using pointer = typename transform_iterator_category::pointer;
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
		pointer operator->() { return pointer(&Transform(*Itr)); }
		friend bool operator==(const this_type& itr1, const this_type& itr2) { return itr1.Itr==itr2.Itr; }
		friend bool operator!=(const this_type& itr1, const this_type& itr2) { return itr1.Itr!=itr2.Itr; }
		base_iterator base() { return Itr; }
	};
	template<typename base_iterator_, typename transform_>
	struct transform_iterator<base_iterator_, transform_, std::bidirectional_iterator_tag> {
		using this_type = transform_iterator<base_iterator_, transform_, std::bidirectional_iterator_tag>;
	private:
		using base_iterator = base_iterator_;
		using transform = transform_;
		using transform_iterator_category = iterators::detail::transform_iterator_category<base_iterator, transform>;
	public:
		using value_type = typename transform_iterator_category::value_type;
		using reference = typename transform_iterator_category::reference;
		using pointer = typename transform_iterator_category::pointer;
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
		pointer operator->() { return pointer(&Transform(*Itr)); }
		base_iterator base() { return Itr; }
		friend bool operator==(const this_type& itr1, const this_type& itr2) { return itr1.Itr==itr2.Itr; }
		friend bool operator!=(const this_type& itr1, const this_type& itr2) { return itr1.Itr!=itr2.Itr; }
	};
	template<typename base_iterator_, typename transform_>
	struct transform_iterator<base_iterator_, transform_, std::random_access_iterator_tag> {
		using this_type = transform_iterator<base_iterator_, transform_, std::random_access_iterator_tag>;
	private:
		using base_iterator = base_iterator_;
		using transform = transform_;
		using transform_iterator_category = iterators::detail::transform_iterator_category<base_iterator, transform>;
	public:
		using value_type = typename transform_iterator_category::value_type;
		using reference = typename transform_iterator_category::reference;
		using pointer = typename transform_iterator_category::pointer;
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
		pointer operator->() { return pointer(&Transform(*Itr)); }
		base_iterator base() { return Itr; }
		friend bool operator==(const this_type& itr1, const this_type& itr2) { return itr1.Itr==itr2.Itr; }
		friend bool operator!=(const this_type& itr1, const this_type& itr2) { return itr1.Itr!=itr2.Itr; }
		friend bool operator>(const this_type& itr1, const this_type& itr2) { return itr1.Itr>itr2.Itr; }
		friend bool operator>=(const this_type& itr1, const this_type& itr2) { return itr1.Itr>=itr2.Itr; }
		friend bool operator<(const this_type& itr1, const this_type& itr2) { return itr1.Itr<itr2.Itr; }
		friend bool operator<=(const this_type& itr1, const this_type& itr2) { return itr1.Itr<=itr2.Itr; }
	};

	template<typename base_iterator_, typename transform_, typename iterator_category_ = typename std::iterator_traits<base_iterator_>::iterator_category>
	struct transform_range {
		using base_iterator = base_iterator_;
		using transform = transform_;
		using iterator = transform_iterator<base_iterator_, transform_, iterator_category_>;
		using range_category = typename std::iterator_traits<iterator>::iterator_category;
		using value_type = typename std::iterator_traits<iterator>::value_type;
		using reference = typename std::iterator_traits<iterator>::reference;
		using pointer = typename std::iterator_traits<iterator>::pointer;
		using difference_type = typename std::iterator_traits<iterator>::difference_type;
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
	struct transform_range<base_iterator_,transform_, std::random_access_iterator_tag> {
		using base_iterator = base_iterator_;
		using transform = transform_;
		using iterator = transform_iterator<base_iterator_, transform_, std::random_access_iterator_tag>;
		using range_category = typename std::iterator_traits<iterator>::iterator_category;
		using value_type = typename std::iterator_traits<iterator>::value_type;
		using reference = typename std::iterator_traits<iterator>::reference;
		using pointer = typename std::iterator_traits<iterator>::pointer;
		using difference_type = typename std::iterator_traits<iterator>::difference_type;
		using size_type = std::size_t;
	private:
		base_iterator Beg;
		base_iterator End;
		transform Transform;
	public:
		transform_range() = default;
		transform_range(base_iterator Beg_, base_iterator End_, transform Transform_):Beg(std::move(Beg_)), End(std::move(End_)), Transform(std::move(Transform_)) {}
		iterator begin() { return iterator(Beg, Transform); }
		iterator end() { return iterator(End, Transform); }
		size_type size()const { return static_cast<size_type>(End-Beg); }
		reference operator[](difference_type n)const { return Beg[n]; }
		reference at(difference_type n)const {
			hmLib_assert(0<=n && static_cast<size_type>(n)<size(), hmLib::access_exceptions::out_of_range_access, "out of range access to trasnfrom_range.");
			return Beg[n];
		}
	};
	template<typename base_iterator_, typename transform_>
	transform_range<typename std::decay<base_iterator_>::type, typename std::decay<transform_>::type> make_transform_range(base_iterator_ Beg, base_iterator_ End, transform_&& Transform) {
		return transform_range<typename std::decay<base_iterator_>::type, typename std::decay<transform_>::type>(std::move(Beg), std::move(End), std::move(Transform));
	}
}
#
#endif
