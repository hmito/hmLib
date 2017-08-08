#ifndef HMLIB_LATTICES_TORUSLOCATOR_INC
#define HMLIB_LATTICES_TORUSLOCATOR_INC 100
#
#include<type_traits>
#include"../algorithm/compare.hpp"
#include"utility.hpp"
namespace hmLib{
	namespace lattices{
		template<typename iterator_, typename indexer_>
		struct basic_const_torus_locator;
		template<typename iterator_, typename indexer_>
		struct basic_torus_locator{
			friend struct basic_const_torus_locator<iterator_, indexer_>;
		public:
			using this_type = basic_torus_locator<iterator_, indexer_>;
			using iterator = iterator_;
			using indexer = indexer_;
			using reference = typename iterator::reference;
			using const_reference = typename std::add_const<reference>::type;
			using pointer = typename iterator::pointer;
			using const_pointer = typename std::add_const<pointer>::type;
			using point_type = typename indexer::point_type;
		public:
			basic_torus_locator() = default;
			basic_torus_locator(iterator Begin_, point_type Pos_, indexer Indexer_) :Begin(Begin_), Pos(Pos_), Indexer(Indexer_){}
			basic_torus_locator(const this_type&) = default;
			basic_torus_locator(this_type&&) = default;
			this_type& operator=(const this_type&) = default;
			this_type& operator=(this_type&&) = default;
		public:
			reference operator*(){ return Begin[Indexer(torus_point())]; }
			const_reference operator*()const{ return Begin[Indexer(torus_point())]; }
			pointer operator->(){ return &(operator*()); }
			const_pointer operator->()const{ return &(operator*()); }
			reference operator[](const point_type& Pos_){ return Begin[Indexer(make_torus_point(Pos + Pos_))]; }
			const_reference operator[](const point_type& Pos_)const{ return Begin[Indexer(make_torus_point(Pos + Pos_))]; }
			reference at(const point_type& Pos_){ return Begin[Indexer.index(make_torus_point(Pos + Pos_))]; }
			const_reference at(const point_type& Pos_)const{ return Begin[Indexer.index(make_torus_point(Pos + Pos_))]; }
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
				return Loc1.point() - Loc2.point();
			}
		public:
			friend bool operator==(const this_type& Loc1, const this_type& Loc2){ return Loc1.Begin == Loc2.Begin && Loc1.torus_point() == Loc2.torus_point(); }
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
			point_type torus_point()const{
				return lattices::make_torus_point(Pos, Indexer.size());
			}
			point_type& point(){ return Pos; }
			const point_type& point()const{ return Pos; }
			point_type size()const{ return Indexer.size(); }
			iterator get_base_iterator()const{ return Begin; }
		private:
			point_type make_torus_point(const point_type& Point){
				return lattices::make_torus_point(Point, Indexer.size());
			}
		private:
			iterator Begin;
			point_type Pos;
			indexer Indexer;
		};
		template<typename iterator_, typename indexer_>
		struct basic_const_torus_locator{
		public:
			using this_type = basic_const_torus_locator<iterator_, indexer_>;
			using iterator = iterator_;
			using indexer = indexer_;
			using pointer = typename iterator::pointer;
			using reference = typename iterator::reference;
			using point_type = typename indexer::point_type;
		public:
			basic_const_torus_locator() = default;
			basic_const_torus_locator(iterator Begin_, point_type Pos_, indexer Indexer_) :Begin(Begin_), Pos(Pos_), Indexer(Indexer_){}
			basic_const_torus_locator(const this_type&) = default;
			basic_const_torus_locator(this_type&&) = default;
			this_type& operator=(const this_type&) = default;
			this_type& operator=(this_type&&) = default;
			basic_const_torus_locator(basic_torus_locator<iterator_, indexer_> Locator) : Begin(Locator.Begin), Pos(Locator.Pos), Indexer(Locator.Indexer){}
		public:
			reference operator*()const{ return Begin[Indexer(torus_point())]; }
			pointer operator->()const{ return &(operator*()); }
			reference operator[](const point_type& Pos_)const{ return Begin[Indexer(make_torus_point(Pos + Pos_))]; }
			reference at(const point_type& Pos_)const{ return Begin[Indexer.index(make_torus_point(Pos + Pos_))]; }
			template<typename... args>
			reference at(args... Args)const{ return at(lattices::make_point(Args...)); }
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
				return Loc1.point() - Loc2.point();
			}
			friend point_type distance(const this_type& Loc1, const this_type& Loc2){
				point_type v1 = Loc1.torus_point();
				point_type v2 = Loc2.torus_point();

				point_type Ans;
				for(unsigned int i = 0; i < v1.size(); ++i){
					diff_type Dis = v2[i] - v1[i];
					if(Dis > Loc1.Indexer.size()[i] / 2){
						Dis -= Loc1.Indexer.size()[i];
					} else if(Dis < -Loc1.Indexer.size()[i] / 2){
						Dis += Loc1.Indexer.size()[i];
					}
					Ans[i] = Dis;
				}
				return Ans;
			}
		public:
			friend bool operator==(const this_type& Loc1, const this_type& Loc2){ return Loc1.Begin == Loc2.Begin && Loc1.torus_point() == Loc2.torus_point(); }
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
			point_type torus_point()const{
				return lattices::make_torus_point(Pos, Indexer.size());
			}
			point_type& point(){ return Pos; }
			const point_type& point()const{ return Pos; }
			point_type size()const{ return Indexer.size(); }
			iterator get_base_iterator()const{ return Begin; }
		private:
			point_type make_torus_point(const point_type& Point){
				return lattices::make_torus_point(Point, Indexer.size());
			}
		private:
			iterator Begin;
			point_type Pos;
			indexer Indexer;
		};
		template<typename iterator_, typename indexer_>
		typename basic_torus_locator<iterator_, indexer_>::point_type distance(const basic_torus_locator<iterator_, indexer_>& Loc1, const basic_torus_locator<iterator_, indexer_>& Loc2){
			auto v1 = Loc1.torus_point();
			auto v2 = Loc2.torus_point();

			typename basic_torus_locator<iterator_, indexer_>::point_type Ans;
			for(unsigned int i = 0; i < v1.size(); ++i){
				diff_type Dis = v2[i] - v1[i];
				if(Dis > Loc1.size()[i] / 2){
					Dis -= Loc1.size()[i];
				} else if(Dis < -Loc1.size()[i] / 2){
					Dis += Loc1.size()[i];
				}
				Ans[i] = Dis;
			}
			return Ans;
		}
		template<typename iterator_, typename indexer_>
		typename basic_const_torus_locator<iterator_, indexer_>::point_type distance(const basic_const_torus_locator<iterator_, indexer_>& Loc1, const basic_const_torus_locator<iterator_, indexer_>& Loc2){
			auto v1 = Loc1.torus_point();
			auto v2 = Loc2.torus_point();

			typename basic_const_torus_locator<iterator_, indexer_>::point_type Ans;
			for(unsigned int i = 0; i < v1.size(); ++i){
				diff_type Dis = v2[i] - v1[i];
				if(Dis > Loc1.size()[i] / 2){
					Dis -= Loc1.size()[i];
				} else if(Dis < -Loc1.size()[i] / 2){
					Dis += Loc1.size()[i];
				}
				Ans[i] = Dis;
			}
			return Ans;
		}
	}
}
#
#endif
