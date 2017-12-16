#ifndef HMLIB_LATTICES_ITERATOR_INC
#define HMLIB_LATTICES_ITERATOR_INC 100
#
#include"utility.hpp"
namespace hmLib{
	namespace lattices{
		template<typename lattice_>
		struct basic_const_iterator;
		template<typename lattice_>
		struct basic_iterator{
			friend struct basic_const_iterator<lattice_>;
		public:
			using this_type = basic_iterator<lattice_>;
			using lattice = lattice_;
			using value_type = typename lattice::value_type;
			using reference = typename lattice::reference;
			using const_reference = typename lattice::const_reference;
			using pointer = typename lattice::pointer;
			using const_pointer = typename lattice::const_pointer;
			using point_type = typename lattice::point_type;
			using locator = typename lattice::locator;
			using difference_type = diff_type;
			using iterator_category = std::random_access_iterator_tag;
		private:
			lattice* pLattice;
			diff_type SeqNo;
		public:
			basic_iterator() :pLattice(nullptr), SeqNo(0){}
			basic_iterator(const this_type&) = default;
			this_type& operator=(const this_type&) = default;
			basic_iterator(this_type&&) = default;
			this_type& operator=(this_type&&) = default;
			basic_iterator(lattice& Lattice_, diff_type SeqNo_) :pLattice(&Lattice_), SeqNo(SeqNo_){}
		public:
			operator bool()const{ return pLattice != nullptr; }
			reference operator*(){ return pLattice->at(pLattice->index_to_point(SeqNo)); }
			const_reference operator*()const{ return pLattice->at(pLattice->index_to_point(SeqNo)); }
			pointer operator->(){ return &(pLattice->at(pLattice->index_to_point(SeqNo))); }
			const_pointer operator->()const{ return &(pLattice->at(pLattice->index_to_point(SeqNo))); }
			reference operator[](diff_type Dif_){ return pLattice->at(pLattice->index_to_point(SeqNo + Dif_)); }
			const_reference operator[](diff_type Dif_)const{ return pLattice->at(pLattice->index_to_point(SeqNo+ Dif_)); }
		public:
			locator locate(){ return pLattice->locate(pLattice->index_to_point(SeqNo)); }
			operator locator(){ return locate(); }
		public:
			this_type& operator++(){ ++SeqNo; return *this; }
			this_type operator++(int){
				this_type Other(*this);
				++(*this);
				return Other;
			}
			this_type& operator--(){ --SeqNo; return *this; }
			this_type operator--(int){
				this_type Other(*this);
				--(*this);
				return Other;
			}
			this_type& operator+=(diff_type Dif_){ SeqNo += Dif_; return *this; }
			friend this_type operator+(const this_type& Loc, diff_type Dif){
				auto Ans(Loc);
				Ans += Dif;
				return Ans;
			}
			friend this_type operator+(diff_type Dif, const this_type& Loc){ return operator+(Loc, Dif); }
			this_type& operator-=(diff_type Dif){ SeqNo -= Dif; return *this;}
			friend this_type operator-(const this_type& Loc, diff_type Dif){
				auto Ans(Loc);
				Ans -= Dif;
				return Ans;
			}
			friend diff_type operator-(const this_type& Loc1, const this_type& Loc2){
				return Loc1.SeqNo - Loc2.SeqNo;
			}
		public:
			lattice& get_lattice(){ return *pLattice; }
			const lattice& get_lattice()const { return *pLattice; }
			diff_type get_index()const { return SeqNo; }
		public:
			friend bool operator==(const this_type& val1, const this_type& val2){
				return val1.SeqNo == val2.SeqNo;
			}
			friend bool operator!=(const this_type& val1, const this_type& val2){
				return val1.SeqNo != val2.SeqNo;
			}
			friend bool operator<(const this_type& val1, const this_type& val2){
				return val1.SeqNo < val2.SeqNo;
			}
			friend bool operator>(const this_type& val1, const this_type& val2){
				return val1.SeqNo > val2.SeqNo;
			}
			friend bool operator<=(const this_type& val1, const this_type& val2){
				return val1.SeqNo <= val2.SeqNo;
			}
			friend bool operator>=(const this_type& val1, const this_type& val2){
				return val1.SeqNo >= val2.SeqNo;
			}
		};
		template<typename lattice_>
		struct basic_const_iterator{
		public:
			using this_type = basic_const_iterator<lattice_>;
			using lattice = lattice_;
			using value_type = typename lattice::value_type;
			using reference = typename lattice::const_reference;
			using pointer = typename lattice::const_pointer;
			using point_type = typename lattice::point_type;
			using locator = typename lattice::const_locator;
			using difference_type = diff_type;
			using iterator_category = std::random_access_iterator_tag;
		private:
			const lattice* pLattice;
			diff_type SeqNo;
		public:
			basic_const_iterator() :pLattice(nullptr), SeqNo(0){}
			basic_const_iterator(const this_type&) = default;
			this_type& operator=(const this_type&) = default;
			basic_const_iterator(this_type&&) = default;
			this_type& operator=(this_type&&) = default;
			basic_const_iterator(const lattice& Lattice_, diff_type SeqNo_) :pLattice(&Lattice_), SeqNo(SeqNo_){}
			basic_const_iterator(basic_iterator<lattice_> Iterator): pLattice(Iterator.pLattice), SeqNo(Iterator.SeqNo){}
		public:
			operator bool()const{ return pLattice != nullptr; }
			reference operator*()const{ return pLattice->at(pLattice->index_to_point(SeqNo)); }
			pointer operator->()const{ return &(pLattice->at(pLattice->index_to_point(SeqNo))); }
			reference operator[](diff_type Dif_)const{ return pLattice->at(pLattice->index_to_point(SeqNo+Dif_)); }
		public:
			locator locate(){ return pLattice->locate(pLattice-?index_to_point(SeqNo)); }
			operator locator(){ return locate(); }
		public:
			this_type& operator++(){ ++SeqNo; return *this; }
			this_type operator++(int){
				this_type Other(*this);
				++(*this);
				return Other;
			}
			this_type& operator--(){ --SeqNo; return *this; }
			this_type operator--(int){
				this_type Other(*this);
				--(*this);
				return Other;
			}
			this_type& operator+=(diff_type Dif_){ SeqNo += Dif_; return *this; }
			friend this_type operator+(const this_type& Loc, diff_type Dif){
				auto Ans(Loc);
				Ans += Dif;
				return Ans;
			}
			friend this_type operator+(diff_type Dif, const this_type& Loc){ return operator+(Loc, Dif); }
			this_type& operator-=(diff_type Dif){ SeqNo -= Dif; return *this; }
			friend this_type operator-(const this_type& Loc, diff_type Dif){
				auto Ans(Loc);
				Ans -= Dif;
				return Ans;
			}
			friend diff_type operator-(const this_type& Loc1, const this_type& Loc2){
				return Loc1.SeqNo - Loc2.SeqNo;
			}
		public:
			friend bool operator==(const this_type& val1, const this_type& val2){
				return val1.SeqNo == val2.SeqNo;
			}
			friend bool operator!=(const this_type& val1, const this_type& val2){
				return val1.SeqNo != val2.SeqNo;
			}
			friend bool operator<(const this_type& val1, const this_type& val2){
				return val1.SeqNo < val2.SeqNo;
			}
			friend bool operator>(const this_type& val1, const this_type& val2){
				return val1.SeqNo > val2.SeqNo;
			}
			friend bool operator<=(const this_type& val1, const this_type& val2){
				return val1.SeqNo <= val2.SeqNo;
			}
			friend bool operator>=(const this_type& val1, const this_type& val2){
				return val1.SeqNo >= val2.SeqNo;
			}
		public:
			const lattice& get_lattice()const { return *pLattice; }
			diff_type get_index()const { return SeqNo; }
		};
	}
}
#
#endif
