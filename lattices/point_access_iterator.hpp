#ifndef HMLIB_LATTICES_POINTACCESSITERATOR_INC
#define HMLIB_LATTICES_POINTACCESSITERATOR_INC 100
#
#include<iterator>
namespace hmLib {
	template<typename lattice_type_, typename point_iterator_, typename point_iterator_category_ = typename std::iterator_traits<point_iterator_>::iterator_category>
	struct point_access_iterator {};
	template<typename lattice_type_, typename point_iterator_>
	struct point_access_iterator<lattice_type_, point_iterator_, std::input_iterator_tag> {
	private:
		using this_type = point_access_iterator<lattice_type_, point_iterator_, std::input_iterator_tag>;
		using lattice_type = lattice_type_;
		using lattice_reference = lattice_type&;
		using point_iterator = point_iterator_;
	public:
		using value_type = typename std::iterator_traits<base_iterator>::value_type;
		using difference_type = typename std::iterator_traits<base_iterator>::difference_type;
		using reference = typename std::iterator_traits<base_iterator>::reference;
		using pointer = typename std::iterator_traits<base_iterator>::pointer;
		using iterator_category = std::input_iterator_tag;
	private:
		lattice_type Lattice;
		point_iterator IItr;
	public://constructer
		point_access_iterator() = default;
		point_access_iterator(base_iterator Beg_, index_iterator_ IItr_):Beg(Beg_), IItr(IItr_) {}
		template<typename other_iterator_, typename std::enable_if<std::is_convertible<base_iterator, other_iterator_>::value&&!std::is_same<base_iterator, other_iterator_>::value>::type *& = hmLib::utility::enabler>
		point_access_iterator(const point_access_iterator<other_iterator_, index_iterator_>& Other) : Beg(Other.Beg), IItr(Other.IItr) {}
		template<typename other_iterator_, typename std::enable_if<std::is_convertible<base_iterator, other_iterator_>::value&&!std::is_same<base_iterator, other_iterator_>::value>::type *& = hmLib::utility::enabler>
		this_type& operator=(const point_access_iterator<other_iterator_, index_iterator_>& Other) {
			Beg = Other.Beg;
			IItr = Other.IItr;
			return *this;
		}
	public:
		reference operator*()const { return Beg[*IItr]; }
		pointer operator->()const { return &Beg[*IItr]; }
		this_type& operator++() {
			++IItr;
			return *this;
		}
		this_type operator++(int) {
			this_type Ans = *this;
			operator++();
			return Ans;
		}
		base_iterator base()const { return Beg+(*IItr); }
		auto index()const { return *IItr; }
		friend bool operator==(const this_type& v1, const this_type& v2) {
			return v1.IItr == v2.IItr;
		}
		friend bool operator!=(const this_type& v1, const this_type& v2) {
			return v1.IItr != v2.IItr;
		}
	};
	template<typename random_access_iterator_, typename index_iterator_>
	struct index_access_iterator<random_access_iterator_, index_iterator_, std::forward_iterator_tag> {
	private:
		static_assert(std::is_same<typename std::iterator_traits<random_access_iterator_>::iterator_category, std::random_access_iterator_tag>::value, "index_iterator can use only for random_access_iterator");
		using this_type = index_access_iterator<random_access_iterator_, index_iterator_, std::forward_iterator_tag>;
		using base_iterator = random_access_iterator_;
		using index_iterator = index_iterator_;
	public:
		using value_type = typename std::iterator_traits<base_iterator>::value_type;
		using difference_type = typename std::iterator_traits<base_iterator>::difference_type;
		using reference = typename std::iterator_traits<base_iterator>::reference;
		using pointer = typename std::iterator_traits<base_iterator>::pointer;
		using iterator_category = std::forward_iterator_tag;
	private:
		base_iterator Beg;
		index_iterator IItr;
	public://constructer
		index_access_iterator() = default;
		index_access_iterator(base_iterator Beg_, index_iterator_ IItr_):Beg(Beg_), IItr(IItr_) {}
		template<typename other_iterator_, typename std::enable_if<std::is_convertible<base_iterator, other_iterator_>::value&&!std::is_same<base_iterator, other_iterator_>::value>::type *& = hmLib::utility::enabler>
		index_access_iterator(const index_access_iterator<other_iterator_, index_iterator_>& Other) : Beg(Other.Beg), IItr(Other.IItr) {}
		template<typename other_iterator_, typename std::enable_if<std::is_convertible<base_iterator, other_iterator_>::value&&!std::is_same<base_iterator, other_iterator_>::value>::type *& = hmLib::utility::enabler>
		this_type& operator=(const index_access_iterator<other_iterator_, index_iterator_>& Other) {
			Beg = Other.Beg;
			IItr = Other.IItr;
			return *this;
		}
	public:
		reference operator*()const { return Beg[*IItr]; }
		pointer operator->()const { return &Beg[*IItr]; }
		this_type& operator++() {
			++IItr;
			return *this;
		}
		this_type operator++(int) {
			this_type Ans = *this;
			operator++();
			return Ans;
		}
		base_iterator base() { return Beg+(*IItr); }
		auto index()const { return *IItr; }
		friend bool operator==(const this_type& v1, const this_type& v2) {
			return v1.IItr == v2.IItr;
		}
		friend bool operator!=(const this_type& v1, const this_type& v2) {
			return v1.IItr != v2.IItr;
		}
	};
	template<typename random_access_iterator_, typename index_iterator_>
	struct index_access_iterator<random_access_iterator_, index_iterator_, std::bidirectional_iterator_tag> {
	private:
		static_assert(std::is_same<typename std::iterator_traits<random_access_iterator_>::iterator_category, std::random_access_iterator_tag>::value, "index_iterator can use only for random_access_iterator");
		using this_type = index_access_iterator<random_access_iterator_, index_iterator_, std::bidirectional_iterator_tag>;
		using base_iterator = random_access_iterator_;
		using index_iterator = index_iterator_;
	public:
		using value_type = typename std::iterator_traits<base_iterator>::value_type;
		using difference_type = typename std::iterator_traits<base_iterator>::difference_type;
		using reference = typename std::iterator_traits<base_iterator>::reference;
		using pointer = typename std::iterator_traits<base_iterator>::pointer;
		using iterator_category = std::bidirectional_iterator_tag;
	private:
		base_iterator Beg;
		index_iterator IItr;
	public://constructer
		index_access_iterator() = default;
		index_access_iterator(base_iterator Beg_, index_iterator_ IItr_):Beg(Beg_), IItr(IItr_) {}
		template<typename other_iterator_, typename std::enable_if<std::is_convertible<base_iterator, other_iterator_>::value&&!std::is_same<base_iterator, other_iterator_>::value>::type *& = hmLib::utility::enabler>
		index_access_iterator(const index_access_iterator<other_iterator_, index_iterator_>& Other) : Beg(Other.Beg), IItr(Other.IItr) {}
		template<typename other_iterator_, typename std::enable_if<std::is_convertible<base_iterator, other_iterator_>::value&&!std::is_same<base_iterator, other_iterator_>::value>::type *& = hmLib::utility::enabler>
		this_type& operator=(const index_access_iterator<other_iterator_, index_iterator_>& Other) {
			Beg = Other.Beg;
			IItr = Other.IItr;
			return *this;
		}
	public:
		reference operator*()const { return Beg[*IItr]; }
		pointer operator->()const { return &Beg[*IItr]; }
		this_type& operator++() {
			++IItr;
			return *this;
		}
		this_type operator++(int) {
			this_type Ans = *this;
			operator++();
			return Ans;
		}
		this_type& operator--() {
			--IItr;
			return *this;
		}
		this_type operator--(int) {
			this_type Ans = *this;
			operator--();
			return Ans;
		}
		base_iterator base()const { return Beg+(*IItr); }
		auto index()const { return *IItr; }
		friend bool operator==(const this_type& v1, const this_type& v2) {
			return v1.IItr == v2.IItr;
		}
		friend bool operator!=(const this_type& v1, const this_type& v2) {
			return v1.IItr != v2.IItr;
		}
	};
	template<typename random_access_iterator_, typename index_iterator_>
	struct index_access_iterator<random_access_iterator_, index_iterator_, std::random_access_iterator_tag> {
	private:
		static_assert(std::is_same<typename std::iterator_traits<random_access_iterator_>::iterator_category, std::random_access_iterator_tag>::value, "index_iterator can use only for random_access_iterator");
		using this_type = index_access_iterator<random_access_iterator_, index_iterator_, std::random_access_iterator_tag>;
		using base_iterator = random_access_iterator_;
		using index_iterator = index_iterator_;
	public:
		using value_type = typename std::iterator_traits<base_iterator>::value_type;
		using difference_type = typename std::iterator_traits<base_iterator>::difference_type;
		using reference = typename std::iterator_traits<base_iterator>::reference;
		using pointer = typename std::iterator_traits<base_iterator>::pointer;
		using iterator_category = std::random_access_iterator_tag;
	private:
		base_iterator Beg;
		index_iterator IItr;
	public://constructer
		index_access_iterator() = default;
		index_access_iterator(base_iterator Beg_, index_iterator_ IItr_):Beg(Beg_), IItr(IItr_) {}
		template<typename other_iterator_, typename std::enable_if<std::is_convertible<base_iterator, other_iterator_>::value&&!std::is_same<base_iterator, other_iterator_>::value>::type *& = hmLib::utility::enabler>
		index_access_iterator(const index_access_iterator<other_iterator_, index_iterator_>& Other) : Beg(Other.Beg), IItr(Other.IItr) {}
		template<typename other_iterator_, typename std::enable_if<std::is_convertible<base_iterator, other_iterator_>::value&&!std::is_same<base_iterator, other_iterator_>::value>::type *& = hmLib::utility::enabler>
		this_type& operator=(const index_access_iterator<other_iterator_, index_iterator_>& Other) {
			Beg = Other.Beg;
			IItr = Other.IItr;
			return *this;
		}
	public:
		reference operator*()const { return Beg[*IItr]; }
		pointer operator->()const { return &Beg[*IItr]; }
		reference operator[](difference_type d) { return Beg[IItr[d]]; }
		this_type& operator++() {
			++IItr;
			return *this;
		}
		this_type operator++(int) {
			this_type Ans = *this;
			operator++();
			return Ans;
		}
		this_type& operator--() {
			--IItr;
			return *this;
		}
		this_type operator--(int) {
			this_type Ans = *this;
			operator--();
			return Ans;
		}
		this_type& operator+=(difference_type d) {
			IItr = +d;
			return *this;
		}
		this_type& operator-=(difference_type d) {
			IItr = -d;
			return *this;
		}
		friend this_type operator+(const this_type& itr, difference_type d) {
			this_type ans = itr;
			ans += d;
			return ans;
		}
		friend this_type operator+(difference_type d, const this_type& itr) {
			return operator+(itr, d);
		}
		friend this_type operator-(const this_type& itr, difference_type d) {
			this_type ans = itr;
			ans -= d;
			return ans;
		}
		friend difference_type operator-(const this_type& itr1, const this_type& itr2) {
			return itr1.IItr - itr2.IItr;
		}
		base_iterator base()const { return Beg+(*IItr); }
		auto index()const { return *IItr; }
		friend bool operator==(const this_type& v1, const this_type& v2) {
			return v1.IItr == v2.IItr;
		}
		friend bool operator!=(const this_type& v1, const this_type& v2) {
			return v1.IItr != v2.IItr;
		}
		friend bool operator>(const this_type& v1, const this_type& v2) {
			return v1.IItr > v2.IItr;
		}
		friend bool operator<(const this_type& v1, const this_type& v2) {
			return v1.IItr < v2.IItr;
		}
		friend bool operator>=(const this_type& v1, const this_type& v2) {
			return v1.IItr >= v2.IItr;
		}
		friend bool operator<=(const this_type& v1, const this_type& v2) {
			return v1.IItr <= v2.IItr;
		}
	};

	template<typename lattice_type_, typename point_iterator_>
	struct point_access_iterator {
	};
}
#
#endif
