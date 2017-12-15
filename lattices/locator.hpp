#ifndef HMLIB_LATTICES_LOCATOR_INC
#define HMLIB_LATTICES_LOCATOR_INC 100
#
#include<type_traits>
#include"utility.hpp"
namespace hmLib{
	namespace lattices{
		template<typename iterator_, typename indexer_>
		struct basic_const_locator;
		template<typename iterator_, typename indexer_>
		struct basic_locator{
			friend struct basic_const_locator<iterator_, indexer_>;
		public:
			using this_type = basic_locator<iterator_, indexer_>;
			using iterator = iterator_;
			using indexer = indexer_;
			using reference = typename iterator::reference;
			using const_reference = typename std::add_const<reference>::type;
			using pointer = typename iterator::pointer;
			using const_pointer = typename std::add_const<pointer>::type;
			using point_type = typename indexer::point_type;
		public:
			basic_locator() = default;
			basic_locator(iterator Begin_, point_type Pos_, indexer Indexer_) :Begin(Begin_), Pos(Pos_), Indexer(Indexer_){}
			basic_locator(const this_type&) = default;
			basic_locator(this_type&&) = default;
			this_type& operator=(const this_type&) = default;
			this_type& operator=(this_type&&) = default;
		public:
			reference operator*(){ return Begin[Indexer(Pos)]; }
			const_reference operator*()const{ return Begin[Indexer(Pos)]; }
			pointer operator->(){ return &(operator*()); }
			const_pointer operator->()const{ return &(operator*()); }
			reference operator[](const point_type& Pos_){ return Begin[Indexer(Pos + Pos_)]; }
			const_reference operator[](const point_type& Pos_)const{ return Begin[Indexer(Pos + Pos_)]; }
			reference at(const point_type& Pos_){ return Begin[Indexer.index(Pos + Pos_)]; }
			const_reference at(const point_type& Pos_)const{ return Begin[Indexer.index(Pos + Pos_)]; }
			template<typename... args>
			reference at(args... Args){ return at(lattices::make_point(Args...)); }
			template<typename... args>
			const_reference at(args... Args)const{ return at(lattices::make_point(Args...)); }
		public:
			this_type& operator+=(const point_type& Dif){ Pos += Dif;  return *this; }
			friend this_type operator+(const this_type& Loc, const point_type& Dif){
				auto Ans(Loc);
				Ans += Dif;
				return Ans;
			}
			friend this_type operator+(const point_type& Dif, const this_type& Loc){ return Loc+Dif; }
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
			friend this_type operator*(int val, const this_type& Loc){ return Loc*val; }
			this_type& operator*=(double val){ Pos *= val;  return *this; }
			friend this_type operator*(const this_type& Loc, double val){
				auto Ans(Loc);
				Ans *= val;
				return Ans;
			}
			friend this_type operator*(double  val, const this_type& Loc){ return Loc*val; }
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
				return Loc1.point() - Loc2.point();
			}
		public:
			friend bool operator==(const this_type& Loc1, const this_type& Loc2){ return Loc1.Begin == Loc2.Begin && Loc1.Pos == Loc2.Pos; }
			friend bool operator!=(const this_type& Loc1, const this_type& Loc2){ return !(Loc1 == Loc2); }
		public:
			void set(const point_type& Pos_){ Pos = Pos_; }
			this_type& add(const point_type& Dif){ return operator+=(Dif); }
			this_type plus(const point_type& Dif){ return *this + (Dif); }
			template<typename... args>
			this_type& add(args... Args){ return operator+=(lattices::make_point(Args...)); }
			template<typename... args>
			this_type plus(args... Args){ return *this + lattices::make_point(Args...); }
		public:
			point_type& point(){ return Pos; }
			const point_type& point()const{ return Pos; }
			point_type size()const{ return Indexer.size(); }
			iterator get_base_iterator()const{ return Begin; }
		private:
			iterator Begin;
			point_type Pos;
			indexer Indexer;
		};
		template<typename iterator_, typename indexer_>
		struct basic_const_locator{
		public:
			using this_type = basic_const_locator<iterator_, indexer_>;
			using iterator = iterator_;
			using indexer = indexer_;
			using pointer = typename iterator::pointer;
			using reference = typename iterator::reference;
			using point_type = typename indexer::point_type;
		public:
			basic_const_locator() = default;
			basic_const_locator(iterator Begin_, point_type Pos_, indexer Indexer_) :Begin(Begin_), Pos(Pos_), Indexer(Indexer_){}
			basic_const_locator(const this_type&) = default;
			basic_const_locator(this_type&&) = default;
			this_type& operator=(const this_type&) = default;
			this_type& operator=(this_type&&) = default;
			basic_const_locator(basic_locator<iterator_, indexer_> Locator) : Begin(Locator.Begin), Pos(Locator.Pos), Indexer(Locator.Indexer){}
		public:
			reference operator*()const{ return Begin[Indexer(Pos)]; }
			pointer operator->()const{ return &(operator*()); }
			reference operator[](const point_type& Pos_)const{ return Begin[Indexer(Pos + Pos_)]; }
			reference at(const point_type& Pos_)const{ return Begin[Indexer.index(Pos + Pos_)]; }
			template<typename... args>
			reference at(args... Args)const{ return at(lattices::make_point(Args...)); }
		public:
			this_type& operator+=(const point_type& Dif){ Pos += Dif;  return *this; }
			friend this_type operator+(const this_type& Loc, const point_type& Dif){
				auto Ans(Loc);
				Ans += Dif;
				return Ans;
			}
			friend this_type operator+(const point_type& Dif, const this_type& Loc){ return Loc+Dif; }
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
			friend this_type operator*(int val, const this_type& Loc){ return Loc*val; }
			this_type& operator*=(double val){ Pos *= val;  return *this; }
			friend this_type operator*(const this_type& Loc, double val){
				auto Ans(Loc);
				Ans *= val;
				return Ans;
			}
			friend this_type operator*(double  val, const this_type& Loc){ return Loc*val; }
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
				return Loc1.point() - Loc2.point();
			}
		public:
			friend bool operator==(const this_type& Loc1, const this_type& Loc2){ return Loc1.Begin == Loc2.Begin && Loc1.Pos == Loc2.Pos; }
			friend bool operator!=(const this_type& Loc1, const this_type& Loc2){ return !(Loc1 == Loc2); }
		public:
			void set(const point_type& Pos_){ Pos = Pos_; }
			this_type& add(const point_type& Dif){ return operator+=(Dif); }
			this_type plus(const point_type& Dif){ return *this + (Dif); }
			template<typename... args>
			this_type& add(args... Args){ return operator+=(lattices::make_point(Args...)); }
			template<typename... args>
			this_type plus(args... Args){ return *this + lattices::make_point(Args...); }
		public:
			point_type& point(){ return Pos; }
			const point_type& point()const{ return Pos; }
			point_type size()const{ return Indexer.size(); }
			iterator get_base_iterator()const{ return Begin; }
		private:
			iterator Begin;
			point_type Pos;
			indexer Indexer;
		};
	}
}
#
#endif
