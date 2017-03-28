#ifndef HMLIB_LATTICES_LOCATOR_INC
#define HMLIB_LATTICES_LOCATOR_INC 100
#
#include"utility.hpp"
#include"iterator.hpp"
namespace hmLib{
	namespace lattices{
		template<typename lattice_>
		struct basic_locator{
		public:
			using this_type = basic_locator<lattice_>;
			using lattice = lattice_;
			using reference = typename lattice::reference;
			using const_reference = typename lattice::const_reference;
			using pointer = typename lattice::pointer;
			using const_pointer = typename lattice::const_pointer;
			using point_type = typename lattice::point_type;
		private:
			lattice* pLattice;
			point_type Pos;
		public:
			basic_locator() :pLattice(nullptr), Pos(){}
			basic_locator(lattice& Lattice_, point_type Pos_) :pLattice(&Lattice_), Pos(Pos_){}
			basic_locator(basic_iterator<lattice_>& Itr) :pLattice(&(Itr.get_lattice().base_lattice())), Pos(Itr.get_lattice().index_to_point(Itr.get_index())){}
		public:
			reference operator*(){ return pLattice->at(Pos); }
			const_reference operator*()const{ return pLattice->at(Pos); }
			pointer operator->(){ return &(pLattice->at(Pos)); }
			const_pointer operator->()const{ return &(pLattice->at(Pos)); }
			reference operator[](const point_type& Pos_){ return pLattice->at(Pos + lattices::make_point(Args...)); }
			const_reference operator[](const point_type& Pos_)const{ return pLattice->at(Pos + lattices::make_point(Args...)); }
			reference at(const point_type& Pos_){ return pLattice->at(Pos + lattices::make_point(Args...)); }
			const_reference at(const point_type& Pos_)const{ return pLattice->at(Pos + lattices::make_point(Args...)); }
			template<typename... args>
			reference at(args... Args){ return pLattice->at(Pos + lattices::make_point(Args...)); }
			template<typename... args>
			const_reference at(args... Args)const{ return pLattice->at(Pos + lattices::make_point(Args...)); }
		public:
			this_type& operator+=(const point_type& Dif){ Pos += Dif;  return *this; }
			friend this_type operator+(const this_type& Loc, const point_type& Dif){
				auto Ans(Loc);
				Ans += Dif;
				return Ans;
			}
			friend this_type operator+(const point_type& Dif, const this_type& Loc){ return operator+(Loc, Dif); }
			this_type& operator-=(const point_type& Dif){ Pos -= Dif;  return *this; }
			friend this_type operator-(const this_type& Loc, const point_type& Dif){
				auto Ans(Loc);
				Ans -= Dif;
				return Ans;
			}
			this_type& operator*=(int val){ Pos *= val;  return *this; }
			friend this_type operator*(const this_type& Loc, int val){
				auto Ans(Loc);
				Ans *= val;
				return Ans;
			}
			friend this_type operator*(int val, const this_type& Loc){ return operator*(Loc, val); }
			this_type& operator*=(double val){ Pos *= val;  return *this; }
			friend this_type operator*(const this_type& Loc, double val){
				auto Ans(Loc);
				Ans *= val;
				return Ans;
			}
			friend this_type operator*(double  val, const this_type& Loc){ return operator*(Loc, val); }
			this_type& operator/=(int val){ Pos /= val;  return *this; }
			friend this_type operator/(const this_type& Loc, int val){
				auto Ans(Loc);
				Ans /= val;
				return Ans;
			}
			this_type& operator/=(double val){ Pos /= val;  return *this; }
			friend this_type operator/(const this_type& Loc, double val){
				auto Ans(Loc);
				Ans /= val;
				return Ans;
			}
			friend point_type operator-(const this_type& Loc1, const this_type& Loc2){
				return Loc1.pos() - Loc2.pos();
			}
		public:
			friend bool operator==(const this_type& Loc1, const this_type& Loc2){ return Loc1.Pos == Loc2.Pos; }
			friend bool operator!=(const this_type& Loc1, const this_type& Loc2){ return Loc1.Pos != Loc2.Pos; }
		public:
			void set(const point_type& Pos_) { Pos = Pos_; }
			this_type& add(const point_type& Dif){ return operator+=(Dif); }
			this_type plus(const point_type& Dif){ return *this + (Dif); }
			template<typename... args>
			this_type& add(args... Args){ return operator+=(lattices::make_point(Args...)); }
			template<typename... args>
			this_type plus(args... Args){ return *this + lattices::make_point(Args...); }
		public:
			lattice_view& get_lattice(){ return *pLattice; }
			const lattice_view& get_lattice()const { return *pLattice; }
			point_type& get_point(){ return Pos; }
			const point_type& get_point()const { return Pos; }
		};
	}
}
#
#endif
