#ifndef HMLIB_LATTICES_TORUSLOCATOR_INC
#define HMLIB_LATTICES_TORUSLOCATOR_INC 100
#
#include<type_traits>
#include<iterator>
#include"../algorithm/compare.hpp"
#include"utility.hpp"
namespace hmLib{
	namespace lattices{
		template<typename base_iterator_, typename indexer_>
		struct basic_torus_locator{
		private:
			using this_type = basic_torus_locator<base_iterator_, indexer_>;
			using base_iterator = base_iterator_;
			using indexer = indexer_;
		public:
			using value_type = typename std::iterator_traits<base_iterator>::value_type;
			using difference_type = typename std::iterator_traits<base_iterator>::difference_type;
			using reference = typename std::iterator_traits<base_iterator>::reference;
			using pointer = typename std::iterator_traits<base_iterator>::pointer;
			using point_type = typename indexer::point_type;
			using extent_type = typename indexer::extent_type;
		public:
			basic_torus_locator() = default;
			basic_torus_locator(base_iterator Begin_, indexer Indexer_, point_type Pos_) :Begin(Begin_), Indexer(Indexer_), Pos(Pos_) {}
			template<typename other_iterator,
				typename std::enable_if<
				std::is_convertible<other_iterator, base_iterator>::value
				&& !std::is_same<other_iterator, base_iterator>::value, std::nullptr_t
				>::type = nullptr
			>
			basic_torus_locator(const basic_torus_locator<other_iterator, indexer>& Other):Begin(Other.Beg), Indexer(Other.Indexer), Pos(Other.Pos) {}
			basic_torus_locator(const this_type&) = default;
			basic_torus_locator(this_type&&) = default;
			this_type& operator=(const this_type&) = default;
			this_type& operator=(this_type&&) = default;
		public:
			point_type base_point()const { return Pos; }
			point_type base_torus_point()const { return Indexer.translate_for_torus(Pos); }
			index_type base_index()const { return Indexer.torus_index(Pos); }
			extent_type base_extent()const { return Indexer.extent(); }
			base_iterator base()const { return Beg+base_index(); }
			reference operator*()const { return base.operator*(); }
			pointer operator->()const { return base.operator->(); }
			reference operator[](const point_type& Pos_)const { return Beg[Indexer.torus_index(Pos + Pos_)]; }
			reference at(const point_type& Pos_)const { return Beg[Indexer.torus_index(Pos + Pos_)]; }
			template<typename... args>
			reference at(args... Args)const { return at(lattices::point(Args...)); }
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
			friend point_type operator-(const this_type& Loc1, const this_type& Loc2){
				return Loc1.base_point() - Loc2.base_point();
			}
			friend point_type distance(const this_type& Loc1, const this_type& Loc2) {
				point_type v1 = Loc1.base_torus_point();
				point_type v2 = Loc2.base_torus_point();

				point_type Ans;
				for(unsigned int i = 0; i < v1.size(); ++i) {
					diff_type Dis = v2[i] - v1[i];
					if(Dis > Loc1.Indexer.size()[i] / 2) {
						Dis -= Loc1.Indexer.size()[i];
					} else if(Dis < -Loc1.Indexer.size()[i] / 2) {
						Dis += Loc1.Indexer.size()[i];
					}
					Ans[i] = Dis;
				}
				return Ans;
			}
		public:
			friend bool operator==(const this_type& Loc1, const this_type& Loc2){ return Loc1.base_torus_point() == Loc2.base_torus_point(); }
			friend bool operator!=(const this_type& Loc1, const this_type& Loc2){ return !(Loc1 == Loc2); }
		public:
			void set(const point_type& Pos_) { Pos = Pos_; }
			this_type& advance(const point_type& Dif) { return operator+=(Dif); }
			template<typename... args>
			this_type& advance(args... Args) { return operator+=(lattices::point(Args...)); }
			this_type next(const point_type& Dif)const { return *this + (Dif); }
			template<typename... args>
			this_type next(args... Args)const { return *this + lattices::point(Args...); }
		private:
			base_iterator Beg;
			indexer Indexer;
			point_type Pos;
		};
	}
}
#
#endif
