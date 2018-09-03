#ifndef HMLIB_MATH_COMBINATORICS_INC
#define HMLIB_MATH_COMBINATORICS_INC 100
#
#include<vector>
#include<algorithm>
#include<numeric>
#include"../exceptions.hpp"
#include"../algorithm/compare.hpp"
namespace hmLib {
	template<typename integer_type, typename integer_ans_type = unsigned long long>
	constexpr integer_ans_type factorial(integer_type val) {
		return val>1 ? val*factorial(val - 1) : 1;
	}
	namespace math {
		namespace detail {
			template<typename integer_ans_type, typename integer_type, typename... other_types>
			constexpr unsigned long long factorial_ratio_impl(integer_ans_type value, integer_type max_lower, integer_type lower, other_types... other_lowers) {
				if(max_lower==lower) {
					return factorial_ratio_impl(value, 0, other_lowers...);
				} else {
					return factorial_ratio_impl(value/factorial(lower), max_lower, other_lowers...);
				}
			}
			template<typename integer_ans_type, typename integer_type>
			constexpr unsigned long long factorial_ratio_impl(integer_ans_type value, integer_type) {
				return value;
			}
		}
	}
	template<typename integer_type, typename... other_types>
	constexpr unsigned long long factorial_ratio(integer_type upper, integer_type lower, other_types... other_lowers) {
		auto max_lower = max_value(lower, other_lowers...);
		auto ans = nPr(upper, upper-max_lower);
		return math::detail::factorial_ratio_impl(ans, max_lower, lower, other_lowers...);
	}
	template<typename integer_type, typename integer_ans_type = unsigned long long>
	constexpr integer_ans_type nPr(integer_type n, integer_type r) {
		integer_ans_type Ans = 1;
		for(integer_type i = 1; i<=r; ++i) {
			Ans *= (n - i + 1);
		}
		return Ans;
	}
	template<typename integer_type, typename integer_ans_type = unsigned long long>
	constexpr integer_ans_type nCr(integer_type n, integer_type r) {
		r = std::min(r, n-r);
		integer_ans_type dividend = 1;
		integer_ans_type divisor = 1;
		for(integer_type i = 1; i <= r; ++i) {
			dividend *= (n - i + 1);
			divisor *= i;
		}
		return dividend / divisor;
	}
	template<typename integer_type, typename integer_ans_type = unsigned long long>
	constexpr integer_ans_type nHr(integer_type n, integer_type r) {
		return nCr<integer_type, integer_ans_type>(n + r - 1, r);
	}

	namespace math {
		namespace detail {
			template<typename forward_iterator1, typename forward_iterator2>
			void removed_order_index(forward_iterator1 Beg, forward_iterator1 End, forward_iterator2 RemoveBeg, forward_iterator2 RemoveEnd) {
				for(auto Itr = Beg; Itr!=End; ++Itr) {
					auto RItr = std::lower_bound(RemoveBeg, RemoveEnd, *Itr);
					if(RItr != RemoveEnd) {
						hmLib_assert(*RItr!=*Itr, hmLib::numeric_exceptions::incorrect_arithmetic_request, "Index in except index range [ExceptBeg,ExceptEnd) is included.");
					}

					*Itr -= std::distance(RemoveBeg, RItr);
				}
			}
			template<typename forward_iterator1, typename forward_iterator2>
			void inserted_order_index(forward_iterator1 Beg, forward_iterator1 End, forward_iterator2 InsertedBeg, forward_iterator2 InsertedEnd) {
				for(auto Itr = Beg; Itr!=End; ++Itr) {
					for(auto IItr = InsertedBeg; IItr!=InsertedEnd; ++IItr) {
						if(*Itr <*IItr)break;
						(*Itr)++;
					}
				}
			}
		}
	}

	template<typename integer_type, typename bidirectional_iterator>
	bool next_combination_index(integer_type Size, bidirectional_iterator Beg, bidirectional_iterator End) {
		auto Itr = End;
		integer_type Pos = 0;
		do {
			--Itr;
			++Pos;
			if(static_cast<integer_type>(*Itr)+Pos < Size) {
				auto no = *Itr + 1;

				for(; Itr!=End; ++Itr) {
					*Itr = no++;
				}
				return true;
			}
		} while(Beg!=Itr);

		return false;
	}

	template<typename integer_type, typename bidirectional_iterator, typename forward_iterator>
	bool next_combination_index(integer_type Size, bidirectional_iterator Beg, bidirectional_iterator End, forward_iterator ExceptBeg, forward_iterator ExceptEnd) {
		math::detail::removed_order_index(Beg, End, ExceptBeg, ExceptEnd);
		auto Ans = next_combination_index(Size-std::distance(ExceptBeg, ExceptEnd), Beg, End);
		math::detail::inserted_order_index(Beg, End, ExceptBeg, ExceptEnd);
		return Ans;
	}

	template<typename integer_type, typename bidirectional_iterator>
	bool next_multicombination_index(integer_type Size, bidirectional_iterator Beg, bidirectional_iterator End) {
		auto Itr = End;
		do {
			--Itr;
			if(static_cast<integer_type>(*Itr)+1 < Size) {
				auto no = *Itr + 1;

				for(; Itr!=End; ++Itr) {
					*Itr = no;
				}
				return true;
			}
		} while(Beg!=Itr);

		return false;
	}

	template<typename integer_type, typename bidirectional_iterator, typename forward_iterator>
	bool next_multicombination_index(integer_type Size, bidirectional_iterator Beg, bidirectional_iterator End, forward_iterator ExceptBeg, forward_iterator ExceptEnd) {
		math::detail::removed_order_index(Beg, End, ExceptBeg, ExceptEnd);
		auto Ans = next_multicombination_index(Size-std::distance(ExceptBeg, ExceptEnd), Beg, End);
		math::detail::inserted_order_index(Beg, End, ExceptBeg, ExceptEnd);
		return Ans;
	}

	template<typename integer_type, typename bidirectional_iterator>
	bool next_permutation_index(integer_type Size, bidirectional_iterator Beg, bidirectional_iterator End) {
		if(std::next_permutation(Beg, End))return true;
		std::sort(Beg, End);
		return next_combination_index(Size, Beg, End);
	}

	template<typename integer_type, typename bidirectional_iterator, typename forward_iterator>
	bool next_permutation_index(integer_type Size, bidirectional_iterator Beg, bidirectional_iterator End, forward_iterator ExceptBeg, forward_iterator ExceptEnd) {
		if(std::next_permutation(Beg, End))return true;
		std::sort(Beg, End);
		return next_combination_index(Size, Beg, End, ExceptBeg, ExceptEnd);
	}

	template<typename integer_type, typename bidirectional_iterator>
	bool next_multipermutation_index(integer_type Size, bidirectional_iterator Beg, bidirectional_iterator End) {
		auto Itr = End;
		do {
			--Itr;
			if(static_cast<integer_type>(*Itr)+1 < Size) {
				*Itr = *Itr + 1;

				for(++Itr; Itr!=End; ++Itr) {
					*Itr = 0;
				}
				return true;
			}
		} while(Beg!=Itr);

		return false;
	}

	template<typename integer_type, typename bidirectional_iterator, typename forward_iterator>
	bool next_multipermutation_index(integer_type Size, bidirectional_iterator Beg, bidirectional_iterator End, forward_iterator ExceptBeg, forward_iterator ExceptEnd) {
		math::detail::removed_order_index(Beg, End, ExceptBeg, ExceptEnd);
		auto Ans = next_multipermutation_index(Size-std::distance(ExceptBeg, ExceptEnd), Beg, End);
		math::detail::inserted_order_index(Beg, End, ExceptBeg, ExceptEnd);
		return Ans;
	}

	template<typename integer_type = unsigned int, typename integer_count_type = unsigned long long>
	struct combination_indexer {
	private:
		using container = std::vector<integer_type>;
		using base_iterator = typename container::const_iterator;
	public:
		struct iterator {
			using value_type = integer_type;
			using reference = integer_type;
			using pointer = void;
			using difference_type = typename std::iterator_traits<base_iterator>::difference_type;
			using iterator_category = std::random_access_iterator_tag;
		private:
			base_iterator Itr;
			base_iterator ExcBeg;
			base_iterator ExcEnd;
		public:
			iterator() = default;
			iterator(base_iterator Itr_, base_iterator ExcBeg_, base_iterator ExcEnd_)
				: Itr(Itr_), ExcBeg(ExcBeg_), ExcEnd(ExcEnd_) {
			}
			reference operator*() {
				auto i = *Itr;
				return i + std::distance(ExcBeg, std::upper_bound(ExcBeg, ExcEnd, i));
			}
			iterator& operator++() { ++Itr; return *this; }
			iterator operator++(int) { iterator ans = *this; operator++(); return ans; }
			iterator& operator--() { --Itr; return *this; }
			iterator operator--(int) { iterator ans = *this; operator--(); return ans; }
			iterator& operator+=(difference_type n) { Itr += n; return *this; }
			friend iterator operator+(const iterator& itr, difference_type n) { auto ans = itr; ans += n; return ans; }
			friend iterator operator+(difference_type n, const iterator& itr) { auto ans = itr; ans += n; return ans; }
			iterator& operator-=(difference_type n) { Itr -= n; return *this; }
			friend iterator operator-(const iterator& itr, difference_type n) { auto ans = itr; ans -= n; return ans; }
			friend difference_type operator-(const iterator& itr1, const iterator& itr2) { return itr1.Itr-itr2.Itr; }
			reference operator[](difference_type n) { auto itr = *this; itr += n; return *itr; }
			friend bool operator==(const iterator& itr1, const iterator& itr2) { return itr1.Itr==itr2.Itr; }
			friend bool operator!=(const iterator& itr1, const iterator& itr2) { return itr1.Itr!=itr2.Itr; }
			friend bool operator>=(const iterator& itr1, const iterator& itr2) { return itr1.Itr>=itr2.Itr; }
			friend bool operator<=(const iterator& itr1, const iterator& itr2) { return itr1.Itr<=itr2.Itr; }
			friend bool operator>(const iterator& itr1, const iterator& itr2) { return itr1.Itr>itr2.Itr; }
			friend bool operator<(const iterator& itr1, const iterator& itr2) { return itr1.Itr<itr2.Itr; }
		};
	private:
		bool Valid;
		integer_type N;
		container Index;
		container ExcIndex;
	public:
		combination_indexer() = default;
		combination_indexer(integer_type N_, integer_type R_):N(N_), Index(R_, 0) { reset(); }
		template<typename except_index_iterator>
		combination_indexer(integer_type N_, integer_type R_, except_index_iterator Beg_, except_index_iterator End_) {
			reset(N_, R_, Beg_, End_);
		}
		integer_count_type total_casenum()const { return nCr<integer_type, integer_count_type>(N, Index.size()); }
		bool next() {
			Valid = next_combination_index(N, Index.begin(), Index.end());
			return valid();
		}
		bool valid()const { return Valid; }
		void reset() {
			std::iota(Index.begin(), Index.end(), 0);
			Valid = true;
		}
		void reset(integer_type N_, integer_type R_) {
			N = N_;
			Index.assign(R_, 0);
			ExcIndex.clear();
			reset();
		}
		template<typename except_index_iterator>
		void reset(integer_type N_, integer_type R_, except_index_iterator Beg_, except_index_iterator End_) {
			N = N_;
			Index.assign(R_, 0);

			ExcIndex.assign(Beg_, End_);
			std::sort(ExcIndex.begin(), ExcIndex.end());
			auto Itr = std::unique(ExcIndex.begin(), ExcIndex.end());
			Itr = std::lower_bound(ExcIndex.begin(), Itr, N);
			ExcIndex.erase(Itr, ExcIndex.end());
			for(unsigned int i = 1; i<ExcIndex.size(); ++i) {
				ExcIndex[i] -= i;
			}
			N -= ExcIndex.size();

			reset();
		}
		bool empty()const { return Index.empty(); }
		std::size_t size()const { return Index.size(); }
		integer_type operator[](std::size_t n)const {
			auto i = Index[n];
			return i + std::distance(ExcIndex.begin(), std::upper_bound(ExcIndex.begin(), ExcIndex.end(), i));
		}
		integer_type at(std::size_t n)const {
			hmLib_assert(n<size(), hmLib::access_exceptions::out_of_range_access, "out of range access is requested.");
			return operator[](n);
		}
		iterator begin()const { return iterator(Index.begin(), ExcIndex.begin(), ExcIndex.end()); }
		iterator end()const { return iterator(Index.end(), ExcIndex.begin(), ExcIndex.end()); }
	};

	template<typename integer_type = unsigned int, typename integer_count_type = unsigned long long>
	struct multicombination_indexer {
	private:
		using container = std::vector<integer_type>;
		using base_iterator = typename container::const_iterator;
	public:
		struct iterator {
			using value_type = integer_type;
			using reference = integer_type;
			using pointer = void;
			using difference_type = typename std::iterator_traits<base_iterator>::difference_type;
			using iterator_category = std::random_access_iterator_tag;
		private:
			base_iterator Itr;
			base_iterator ExcBeg;
			base_iterator ExcEnd;
		public:
			iterator() = default;
			iterator(base_iterator Itr_, base_iterator ExcBeg_, base_iterator ExcEnd_)
				: Itr(Itr_), ExcBeg(ExcBeg_), ExcEnd(ExcEnd_) {
			}
			reference operator*() {
				auto i = *Itr;
				return i + std::distance(ExcBeg, std::upper_bound(ExcBeg, ExcEnd, i));
			}
			iterator& operator++() { ++Itr; return *this; }
			iterator operator++(int) { iterator ans = *this; operator++(); return ans; }
			iterator& operator--() { --Itr; return *this; }
			iterator operator--(int) { iterator ans = *this; operator--(); return ans; }
			iterator& operator+=(difference_type n) { Itr += n; return *this; }
			friend iterator operator+(const iterator& itr, difference_type n) { auto ans = itr; ans += n; return ans; }
			friend iterator operator+(difference_type n, const iterator& itr) { auto ans = itr; ans += n; return ans; }
			iterator& operator-=(difference_type n) { Itr -= n; return *this; }
			friend iterator operator-(const iterator& itr, difference_type n) { auto ans = itr; ans -= n; return ans; }
			friend difference_type operator-(const iterator& itr1, const iterator& itr2) { return itr1.Itr-itr2.Itr; }
			reference operator[](difference_type n) { auto itr = *this; itr += n; return *itr; }
			friend bool operator==(const iterator& itr1, const iterator& itr2) { return itr1.Itr==itr2.Itr; }
			friend bool operator!=(const iterator& itr1, const iterator& itr2) { return itr1.Itr!=itr2.Itr; }
			friend bool operator>=(const iterator& itr1, const iterator& itr2) { return itr1.Itr>=itr2.Itr; }
			friend bool operator<=(const iterator& itr1, const iterator& itr2) { return itr1.Itr<=itr2.Itr; }
			friend bool operator>(const iterator& itr1, const iterator& itr2) { return itr1.Itr>itr2.Itr; }
			friend bool operator<(const iterator& itr1, const iterator& itr2) { return itr1.Itr<itr2.Itr; }
		};
	private:
		bool Valid;
		integer_type N;
		container Index;
		container ExcIndex;
	public:
		multicombination_indexer() = default;
		multicombination_indexer(integer_type N_, integer_type R_):N(N_), Index(R_, 0) { reset(); }
		template<typename except_index_iterator>
		multicombination_indexer(integer_type N_, integer_type R_, except_index_iterator Beg_, except_index_iterator End_){
			reset(N_, R_, Beg_, End_);
		}
		integer_count_type total_casenum()const { return nHr<integer_type, integer_count_type>(N, Index.size()); }
		integer_count_type order_casenum()const {
			auto ans = factorial(Index.size());

			integer_type cnt = 1;
			for(unsigned int i = 1; i<Index.size(); ++i) {
				if(Index[i-1] == Index[i]) {
					cnt++;
				} else {
					if(cnt>1) {
						ans /= factorial(cnt);
					}
					cnt = 1;
				}
			}
			if(cnt>1) {
				ans /= factorial(cnt);
			}
			return ans;
		}
		bool next() {
			Valid = next_multicombination_index(N, Index.begin(), Index.end());
			return valid();
		}
		bool valid()const { return Valid; }
		void reset() {
			std::fill(Index.begin(), Index.end(), 0);
			Valid = true;
		}
		void reset(integer_type N_, integer_type R_) {
			N = N_;
			Index.assign(R_, 0);
			ExcIndex.clear();
			reset();
		}
		template<typename except_index_iterator>
		void reset(integer_type N_, integer_type R_, except_index_iterator Beg_, except_index_iterator End_) {
			N = N_;
			Index.assign(R_, 0);

			ExcIndex.assign(Beg_, End_);
			std::sort(ExcIndex.begin(), ExcIndex.end());
			auto Itr = std::unique(ExcIndex.begin(), ExcIndex.end());
			Itr = std::lower_bound(ExcIndex.begin(), Itr, N);
			ExcIndex.erase(Itr, ExcIndex.end());
			for(unsigned int i = 1; i<ExcIndex.size(); ++i) {
				ExcIndex[i] -= i;
			}

			N -= ExcIndex.size();

			reset();
		}
		bool empty()const { return Index.empty(); }
		std::size_t size()const { return Index.size(); }
		integer_type operator[](std::size_t n)const {
			auto i = Index[n];
			return i + std::distance(ExcIndex.begin(), std::upper_bound(ExcIndex.begin(), ExcIndex.end(), i));
		}
		integer_type at(std::size_t n)const {
			hmLib_assert(n<size(), hmLib::access_exceptions::out_of_range_access, "out of range access is requested.");
			return operator[](n);
		}
		iterator begin()const { return iterator(Index.begin(), ExcIndex.begin(), ExcIndex.end()); }
		iterator end()const { return iterator(Index.end(), ExcIndex.begin(), ExcIndex.end()); }
	};

	template<typename integer_type = unsigned int, typename integer_count_type = unsigned long long>
	struct permutation_indexer {
	private:
		using container = std::vector<integer_type>;
		using base_iterator = typename container::const_iterator;
	public:
		using iterator = base_iterator;
	private:
		bool Valid;
		integer_type N;
		container Index;
		container ExcIndex;
	public:
		permutation_indexer() = default;
		permutation_indexer(integer_type N_, integer_type R_):N(N_), Index(R_, 0) { reset(); }
		template<typename except_index_iterator>
		permutation_indexer(integer_type N_, integer_type R_, except_index_iterator Beg_, except_index_iterator End_) {
			reset(N_, R_, Beg_, End_);
		}
		integer_count_type total_casenum()const { return nPr<integer_type, integer_count_type>(N-ExcIndex.size(), Index.size()); }
		bool next() {
			Valid = next_permutation_index(N, Index.begin(), Index.end(), ExcIndex.begin(), ExcIndex.end());
			return valid();
		}
		bool valid()const { return Valid; }
		void reset() {
			integer_type Cnt = 0;
			for(auto Itr = Index.begin(); Itr!=Index.end(); ++Itr) {
				while(std::find(ExcIndex.begin(), ExcIndex.end(), Cnt) != ExcIndex.end()) {
					++Cnt;
				}
				*Itr = (Cnt++);
			}
			Valid = true;
		}
		void reset(integer_type N_, integer_type R_) {
			N = N_;
			Index.assign(R_, 0);
			ExcIndex.clear();
			reset();
		}
		template<typename except_index_iterator>
		void reset(integer_type N_, integer_type R_, except_index_iterator Beg_, except_index_iterator End_) {
			N = N_;
			Index.assign(R_, 0);

			ExcIndex.assign(Beg_, End_);
			std::sort(ExcIndex.begin(), ExcIndex.end());
			auto Itr = std::unique(ExcIndex.begin(), ExcIndex.end());
			Itr = std::lower_bound(ExcIndex.begin(), Itr, N);
			ExcIndex.erase(Itr, ExcIndex.end());

			reset();
		}
		bool empty()const { return Index.empty(); }
		std::size_t size()const { return Index.size(); }
		integer_type operator[](std::size_t n)const {
			return Index[n];
		}
		integer_type at(std::size_t n)const {
			return Index.at(n);
		}
		iterator begin()const { return Index.begin(); }
		iterator end()const { return Index.end(); }
	};

	template<typename integer_type = unsigned int, typename integer_count_type = unsigned long long>
	struct multipermutation_indexer {
	private:
		using container = std::vector<integer_type>;
		using base_iterator = typename container::const_iterator;
	public:
		struct iterator {
			using value_type = integer_type;
			using reference = integer_type;
			using pointer = void;
			using difference_type = typename std::iterator_traits<base_iterator>::difference_type;
			using iterator_category = std::random_access_iterator_tag;
		private:
			base_iterator Itr;
			base_iterator ExcBeg;
			base_iterator ExcEnd;
		public:
			iterator() = default;
			iterator(base_iterator Itr_, base_iterator ExcBeg_, base_iterator ExcEnd_)
				: Itr(Itr_), ExcBeg(ExcBeg_), ExcEnd(ExcEnd_) {
			}
			reference operator*() {
				auto i = *Itr;
				return i + std::distance(ExcBeg, std::upper_bound(ExcBeg, ExcEnd, i));
			}
			iterator& operator++() { ++Itr; return *this; }
			iterator operator++(int) { iterator ans = *this; operator++(); return ans; }
			iterator& operator--() { --Itr; return *this; }
			iterator operator--(int) { iterator ans = *this; operator--(); return ans; }
			iterator& operator+=(difference_type n) { Itr += n; return *this; }
			friend iterator operator+(const iterator& itr, difference_type n) { auto ans = itr; ans += n; return ans; }
			friend iterator operator+(difference_type n, const iterator& itr) { auto ans = itr; ans += n; return ans; }
			iterator& operator-=(difference_type n) { Itr -= n; return *this; }
			friend iterator operator-(const iterator& itr, difference_type n) { auto ans = itr; ans -= n; return ans; }
			friend difference_type operator-(const iterator& itr1, const iterator& itr2) { return itr1.Itr-itr2.Itr; }
			reference operator[](difference_type n) { auto itr = *this; itr += n; return *itr; }
			friend bool operator==(const iterator& itr1, const iterator& itr2) { return itr1.Itr==itr2.Itr; }
			friend bool operator!=(const iterator& itr1, const iterator& itr2) { return itr1.Itr!=itr2.Itr; }
			friend bool operator>=(const iterator& itr1, const iterator& itr2) { return itr1.Itr>=itr2.Itr; }
			friend bool operator<=(const iterator& itr1, const iterator& itr2) { return itr1.Itr<=itr2.Itr; }
			friend bool operator>(const iterator& itr1, const iterator& itr2) { return itr1.Itr>itr2.Itr; }
			friend bool operator<(const iterator& itr1, const iterator& itr2) { return itr1.Itr<itr2.Itr; }
		};
	private:
		bool Valid;
		integer_type N;
		container Index;
		container ExcIndex;
	public:
		multipermutation_indexer() = default;
		multipermutation_indexer(integer_type N_, integer_type R_):N(N_), Index(R_, 0) { reset(); }
		template<typename except_index_iterator>
		multipermutation_indexer(integer_type N_, integer_type R_, except_index_iterator Beg_, except_index_iterator End_){
			reset(N_,R_,Beg_,End_);
		}
		integer_count_type total_casenum()const { integer_count_type Ans = N; for(std::size_t i = 1; i<Index.size(); ++i) Ans *= N; return Ans; }
		bool next() {
			Valid = next_multipermutation_index(N, Index.begin(), Index.end());
			return valid();
		}
		bool valid()const { return Valid; }
		void reset() {
			std::fill(Index.begin(), Index.end(), 0);
			Valid = true;
		}
		void reset(integer_type N_, integer_type R_) {
			N = N_;
			Index.assign(R_, 0);
			ExcIndex.clear();
			reset();
		}
		template<typename except_index_iterator>
		void reset(integer_type N_, integer_type R_, except_index_iterator Beg_, except_index_iterator End_) {
			N = N_;
			Index.assign(R_, 0);

			ExcIndex.assign(Beg_, End_);
			std::sort(ExcIndex.begin(), ExcIndex.end());
			auto Itr = std::unique(ExcIndex.begin(), ExcIndex.end());
			Itr = std::lower_bound(ExcIndex.begin(), Itr, N);
			ExcIndex.erase(Itr, ExcIndex.end());

			N -= ExcIndex.size();
			for(unsigned int i = 1; i<ExcIndex.size(); ++i) {
				ExcIndex[i] -= i;
			}

			reset();
		}
		bool empty()const { return Index.empty(); }
		std::size_t size()const { return Index.size(); }
		integer_type operator[](std::size_t n)const {
			auto i = Index[n];
			return i + std::distance(ExcIndex.begin(), std::upper_bound(ExcIndex.begin(), ExcIndex.end(), i));
		}
		integer_type at(std::size_t n)const {
			hmLib_assert(n<size(), hmLib::access_exceptions::out_of_range_access, "out of range access is requested.");
			return operator[](n);
		}
		iterator begin()const { return iterator(Index.begin(), ExcIndex.begin(), ExcIndex.end()); }
		iterator end()const { return iterator(Index.end(), ExcIndex.begin(), ExcIndex.end()); }
	};
}
#
#endif
