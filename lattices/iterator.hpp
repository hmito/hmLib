#ifndef HMLIB_LATTICES_ITERATOR_INC
#define HMLIB_LATTICES_ITERATOR_INC 100
#
#include"utility.hpp"
#include"locator.hpp"
namespace hmLib{
	namespace lattices{
		template<typename base_iterator_, typename indexer_>
		struct basic_iterator {
		private:
			using this_type = basic_iterator<base_iterator_, indexer_>;
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
			using locator = basic_locator<base_iterator, indexer>;
		public:
			basic_iterator() = default;
			basic_iterator(const this_type&) = default;
			this_type& operator=(const this_type&) = default;
			basic_iterator(this_type&&) = default;
			this_type& operator=(this_type&&) = default;
			basic_iterator(base_iterator Beg_, indexer BaseIndexer_, index_type FIndex_)
				: Beg(Beg_)
				, Indexer(BaseIndexer_)
				, FIndex(FIndex_) {
			}
			template<
				typename other_iterator,
				typename std::enable_if<
				std::is_convertible<other_iterator, base_iterator>::value
				&& !std::is_same<other_iterator, base_iterator>::value, std::nullptr_t
				>::type = nullptr
			>
			basic_iterator(const basic_iterator<other_iterator, indexer>& Other)
				: Beg(Other.Beg)
				, Indexer(Other.Indexer)
				, FIndex(Other.FIndex) {
			}
		public:
			point_type base_point()const { return Indexer.calc_point(FIndex); }
			extent_type base_extent()const { return Indexer.extent(); }
			index_type base_index()const { return FIndex; }
			base_iterator base()const { return Beg+base_index(); }
			reference operator*()const { return base().operator*(); }
			pointer operator->()const { return base().operator->(); }
			reference operator[](index_type Dif_)const { return Beg[FIndex+Dif_]; }
		public:
			locator locate()const { return locator(Beg, Indexer, base_point()); }
			explicit operator locator()const { return locate(); }
		public:
			this_type& operator++() { ++FIndex; return *this; }
			this_type operator++(int) {
				this_type Other(*this);
				++(*this);
				return Other;
			}
			this_type& operator--() { --FIndex; return *this; }
			this_type operator--(int) {
				this_type Other(*this);
				--(*this);
				return Other;
			}
			this_type& operator+=(index_type Dif_) { FIndex += Dif_; return *this; }
			friend this_type operator+(const this_type& Loc, diff_type Dif) {
				auto Ans(Loc);
				Ans += Dif;
				return Ans;
			}
			friend this_type operator+(index_type Dif, const this_type& Loc) { return operator+(Loc, Dif); }
			this_type& operator-=(index_type Dif) { FIndex -= Dif; return *this; }
			friend this_type operator-(const this_type& Loc, diff_type Dif) {
				auto Ans(Loc);
				Ans -= Dif;
				return Ans;
			}
			friend diff_type operator-(const this_type& Loc1, const this_type& Loc2) {
				return Loc1.FIndex - Loc2.FIndex;
			}
		public:
			friend bool operator==(const this_type& val1, const this_type& val2) { return val1.FIndex == val2.FIndex; }
			friend bool operator!=(const this_type& val1, const this_type& val2) { return val1.FIndex != val2.FIndex; }
			friend bool operator<(const this_type& val1, const this_type& val2) { return val1.FIndex < val2.FIndex; }
			friend bool operator>(const this_type& val1, const this_type& val2) { return val1.FIndex > val2.FIndex; }
			friend bool operator<=(const this_type& val1, const this_type& val2) { return val1.FIndex <= val2.FIndex; }
			friend bool operator>=(const this_type& val1, const this_type& val2) { return val1.FIndex >= val2.FIndex; }
		private:
			base_iterator Beg;	//original begin
			indexer Indexer;	//original range
			index_type FIndex;	//original index
		};
		template<typename base_iterator_, typename indexer_>
		struct basic_sub_iterator{
		private:
			using this_type = basic_sub_iterator<base_iterator_, indexer_>;
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
			using locator = basic_locator<base_iterator, indexer>;
		public:
			basic_sub_iterator() = default;
			basic_sub_iterator(const this_type&) = default;
			this_type& operator=(const this_type&) = default;
			basic_sub_iterator(this_type&&) = default;
			this_type& operator=(this_type&&) = default;
			basic_sub_iterator(base_iterator Beg_, indexer BaseIndexer_, point_type Pos_, extent_type Extent_, index_type FIndex_)
				: Beg(Beg_)
				, Indexer(BaseIndexer_)
				, Pos(Pos_)
				, FIndexer(Extent_)
				, FIndex(FIndex_) {
			}
			template<
				typename other_iterator,
				typename std::enable_if<
					std::is_convertible<other_iterator, base_iterator>::value && !std::is_same<other_iterator, base_iterator>::value, std::nullptr_t
				>::type = nullptr
			>
			basic_sub_iterator(const basic_sub_iterator<other_iterator, indexer>& Other)
				: Beg(Other.Beg)
				, Indexer(Other.Indexer)
				, Pos(Other.Pos)
				, FIndexer(Other.FIndexer)
				, FIndex(Other.FIndex){
			}
		public:
			point_type base_point()const { return Pos+FIndexer.calc_point(FIndex); }
			extent_type base_extent()const { return Indexer.extent(); }
			index_type base_index()const { return Indexer(base_point()); }
			base_iterator base()const { return Beg+base_index(); }
			reference operator*()const{ return base().operator*(); }
			pointer operator->()const { return base().operator->();}
			reference operator[](index_type Dif_)const { return Beg[Indexer(Pos+FIndexer.calc_point(FIndex+Dif_))]; }
		public:
			locator locate()const { return locator(Beg,Indexer, base_point()); }
			explicit operator locator()const { return locate(); }
		public:
			this_type& operator++(){ ++FIndex; return *this; }
			this_type operator++(int){
				this_type Other(*this);
				++(*this);
				return Other;
			}
			this_type& operator--(){ --FIndex; return *this; }
			this_type operator--(int){
				this_type Other(*this);
				--(*this);
				return Other;
			}
			this_type& operator+=(index_type Dif_){ FIndex += Dif_; return *this; }
			friend this_type operator+(const this_type& Loc, diff_type Dif){
				auto Ans(Loc);
				Ans += Dif;
				return Ans;
			}
			friend this_type operator+(index_type Dif, const this_type& Loc){ return operator+(Loc, Dif); }
			this_type& operator-=(index_type Dif){ FIndex -= Dif; return *this;}
			friend this_type operator-(const this_type& Loc, diff_type Dif){
				auto Ans(Loc);
				Ans -= Dif;
				return Ans;
			}
			friend diff_type operator-(const this_type& Loc1, const this_type& Loc2){
				return Loc1.FIndex - Loc2.FIndex;
			}
		public:
			friend bool operator==(const this_type& val1, const this_type& val2){ return val1.FIndex == val2.FIndex; }
			friend bool operator!=(const this_type& val1, const this_type& val2){ return val1.FIndex != val2.FIndex; }
			friend bool operator<(const this_type& val1, const this_type& val2){ return val1.FIndex < val2.FIndex; }
			friend bool operator>(const this_type& val1, const this_type& val2){ return val1.FIndex > val2.FIndex; }
			friend bool operator<=(const this_type& val1, const this_type& val2){ return val1.FIndex <= val2.FIndex; }
			friend bool operator>=(const this_type& val1, const this_type& val2){ return val1.FIndex >= val2.FIndex; }
		private:
			base_iterator Beg;	//original begin
			indexer Indexer;	//original range
			indexer FIndexer;	//focal range
			point_type Pos;		//focal range pos
			index_type FIndex;	//focal index
			//index (FIndexer::point)-> focal point
			//focal point + Pos = original point
			//original point (Indexer.index)-> original index
		};
	}
}
#
#endif
