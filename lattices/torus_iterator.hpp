#ifndef HMLIB_LATTICES_TORUSITERATOR_INC
#define HMLIB_LATTICES_TORUSITERATOR_INC 100
#
#include"utility.hpp"
#include"torus_locator.hpp"
namespace hmLib {
	namespace lattices {
		template<typename base_iterator_, typename indexer_>
		struct basic_torus_iterator {
		private:
			using this_type = basic_torus_iterator<base_iterator_, indexer_>;
			using base_iterator = base_iterator_;
			using indexer = indexer_;
		public:
			using value_type = typename std::iterator_traits<base_iterator>::value_type;
			using difference_type = typename std::iterator_traits<base_iterator>::difference_type;
			using reference = typename std::iterator_traits<base_iterator>::reference;
			using pointer = typename std::iterator_traits<base_iterator>::pointer;
			using iterator_category = std::random_access_iterator_tag;
		public:
			using point_type = typename indexer::point_type;
			using extent_type = typename indexer::extent_type;
			using locator = basic_torus_locator<base_iterator,indexer>;
		public:
			basic_torus_iterator() = default;
			basic_torus_iterator(const this_type&) = default;
			this_type& operator=(const this_type&) = default;
			basic_torus_iterator(this_type&&) = default;
			this_type& operator=(this_type&&) = default;
			basic_torus_iterator(base_iterator Beg_, indexer BaseIndexer_, index_type FIndex_)
				: Begin(Beg_)
				, Indexer(BaseIndexer_)
				, Index(FIndex_) {
			}
			template<
				typename other_iterator, 
				typename std::enable_if<
				std::is_convertible<other_iterator, base_iterator>::value
				&& !std::is_same<other_iterator, base_iterator>::value, std::nullptr_t
				>::type = nullptr
			>
			basic_torus_iterator(const basic_torus_iterator<other_iterator, indexer>& Other)
				: Begin(Other.Begin)
				, Indexer(Other.Indexer)
				, Index(Other.Index) {
			}
		public:
			point_type base_point()const { return Indexer.calc_point(Index); }
			extent_type base_extent()const { return Indexer.extent(); }
			index_type base_index()const { return Indexer.torus_index(base_point()); }
			base_iterator base_begin()const { return Begin; }
			base_iterator base()const { return Begin+base_index(); }
			reference operator*()const { return base().operator*(); }
			pointer operator->()const { return base().operator->(); }
			reference operator[](index_type Dif_)const { return Begin[Indexer.torus_index(Indexer.calc_point(Index+Dif_)]); }
		public:
			locator locate()const { return locator(Begin, Indexer, base_point()); }
			explicit operator locator()const { return locate(); }
		public:
			this_type& operator++() { ++Index; return *this; }
			this_type operator++(int) {
				this_type Other(*this);
				++(*this);
				return Other;
			}
			this_type& operator--() { --Index; return *this; }
			this_type operator--(int) {
				this_type Other(*this);
				--(*this);
				return Other;
			}
			this_type& operator+=(index_type Dif_) { Index += Dif_; return *this; }
			friend this_type operator+(const this_type& Loc, diff_type Dif) {
				auto Ans(Loc);
				Ans += Dif;
				return Ans;
			}
			friend this_type operator+(index_type Dif, const this_type& Loc) { return operator+(Loc, Dif); }
			this_type& operator-=(index_type Dif) { Index -= Dif; return *this; }
			friend this_type operator-(const this_type& Loc, diff_type Dif) {
				auto Ans(Loc);
				Ans -= Dif;
				return Ans;
			}
			friend diff_type operator-(const this_type& Loc1, const this_type& Loc2) {
				return Loc1.Index - Loc2.Index;
			}
		public:
			friend bool operator==(const this_type& val1, const this_type& val2) { return val1.Index == val2.Index; }
			friend bool operator!=(const this_type& val1, const this_type& val2) { return val1.Index != val2.Index; }
			friend bool operator<(const this_type& val1, const this_type& val2) { return val1.Index < val2.Index; }
			friend bool operator>(const this_type& val1, const this_type& val2) { return val1.Index > val2.Index; }
			friend bool operator<=(const this_type& val1, const this_type& val2) { return val1.Index <= val2.Index; }
			friend bool operator>=(const this_type& val1, const this_type& val2) { return val1.Index >= val2.Index; }
		private:
			base_iterator Begin;	//original begin
			indexer Indexer;	//original range
			index_type Index;	//original index
		};
		template<typename base_iterator_, typename indexer_>
		struct basic_torus_sub_iterator {
		private:
			using this_type = basic_torus_sub_iterator<base_iterator_, indexer_>;
			using base_iterator = base_iterator_;
			using indexer = indexer_;
		public:
			using value_type = typename std::iterator_traits<base_iterator>::value_type;
			using difference_type = typename std::iterator_traits<base_iterator>::difference_type;
			using reference = typename std::iterator_traits<base_iterator>::reference;
			using pointer = typename std::iterator_traits<base_iterator>::pointer;
			using iterator_category = std::random_access_iterator_tag;
		public:
			using point_type = typename indexer::point_type;
			using extent_type = typename indexer::extent_type;
			using locator = basic_torus_locator<base_iterator, indexer>;
		public:
			basic_torus_sub_iterator() = default;
			basic_torus_sub_iterator(const this_type&) = default;
			this_type& operator=(const this_type&) = default;
			basic_torus_sub_iterator(this_type&&) = default;
			this_type& operator=(this_type&&) = default;
			basic_torus_sub_iterator(base_iterator Beg_, indexer OriginalIndexer_, point_type Pos_, extent_type Extent_, index_type SubIndex_)
				: Begin(Beg_)
				, OriginalIndexer(OriginalIndexer_)
				, Pos(Pos_)
				, SubIndexer(Extent_)
				, SubIndex(SubIndex_) {
			}
			template<
				typename other_iterator,
				typename std::enable_if<
				std::is_convertible<other_iterator, base_iterator>::value && !std::is_same<other_iterator, base_iterator>::value, std::nullptr_t
				>::type = nullptr
			>
				basic_torus_sub_iterator(const basic_torus_sub_iterator<other_iterator, indexer>& Other)
				: Begin(Other.Begin)
				, OriginalIndexer(Other.OriginalIndexer)
				, Pos(Other.Pos)
				, SubIndexer(Other.SubIndexer)
				, SubIndex(Other.SubIndex) {
			}
		public:
			point_type base_point()const { return Pos+SubIndexer.calc_point(SubIndex); }
			extent_type base_extent()const { return OriginalIndexer.extent(); }
			index_type base_index()const { return OriginalIndexer.torus_index(base_point()); }
			base_iterator base_begin()const { return Begin; }
			base_iterator base()const { return Begin+base_index(); }
			reference operator*()const { return base().operator*(); }
			pointer operator->()const { return base().operator->(); }
			reference operator[](index_type Dif_)const { return Begin[OriginalIndexer.torus_index(Pos+SubIndexer.calc_point(SubIndex+Dif_))]; }
		public:
			locator locate()const { return locator(Begin, OriginalIndexer, base_point()); }
			explicit operator locator()const { return locate(); }
		public:
			this_type& operator++() { ++SubIndex; return *this; }
			this_type operator++(int) {
				this_type Other(*this);
				++(*this);
				return Other;
			}
			this_type& operator--() { --SubIndex; return *this; }
			this_type operator--(int) {
				this_type Other(*this);
				--(*this);
				return Other;
			}
			this_type& operator+=(index_type Dif_) { SubIndex += Dif_; return *this; }
			friend this_type operator+(const this_type& Loc, diff_type Dif) {
				auto Ans(Loc);
				Ans += Dif;
				return Ans;
			}
			friend this_type operator+(index_type Dif, const this_type& Loc) { return operator+(Loc, Dif); }
			this_type& operator-=(index_type Dif) { SubIndex -= Dif; return *this; }
			friend this_type operator-(const this_type& Loc, diff_type Dif) {
				auto Ans(Loc);
				Ans -= Dif;
				return Ans;
			}
			friend diff_type operator-(const this_type& Loc1, const this_type& Loc2) {
				return Loc1.SubIndex - Loc2.SubIndex;
			}
		public:
			friend bool operator==(const this_type& val1, const this_type& val2) { return val1.SubIndex == val2.SubIndex; }
			friend bool operator!=(const this_type& val1, const this_type& val2) { return val1.SubIndex != val2.SubIndex; }
			friend bool operator<(const this_type& val1, const this_type& val2) { return val1.SubIndex < val2.SubIndex; }
			friend bool operator>(const this_type& val1, const this_type& val2) { return val1.SubIndex > val2.SubIndex; }
			friend bool operator<=(const this_type& val1, const this_type& val2) { return val1.SubIndex <= val2.SubIndex; }
			friend bool operator>=(const this_type& val1, const this_type& val2) { return val1.SubIndex >= val2.SubIndex; }
		private:
			base_iterator Begin;	//original begin
			indexer OriginalIndexer;	//original range
			indexer SubIndexer;	//focal range
			point_type Pos;		//focal range pos
			index_type SubIndex;	//focal index
									//index (SubIndexer::point)-> focal point
									//focal point + Pos = original point
									//original point (OriginalIndexer.index)-> original index
		};
	}
}
#
#endif
