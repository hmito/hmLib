#ifndef HMLIB_LATTICEVIEW_INC
#define HMLIB_LATTICEVIEW_INC 100
#
#include<array>
#include<utility>
#include<iterator>
#include<hmLib/exceptions.hpp>
namespace hmLib{
	namespace lattices{
		struct exception_tag{};
		using exception = hmLib::exceptions::exception_pattern<exception_tag>;
		template<typename T,typename U,unsigned int dim_>
		T dim_value(){return }

		template<unsigned int dim_>
		using point = std::array<int, dim_>;
		template<typename ...others>
		auto make_point(others... Others)->point<sizeof...(others)>{
			return point<sizeof...(others)>{Others...};
		}
	}
	template<typename iterator_, unsigned int dim_>
	struct lattice_view{
		friend struct lattice_view<iterator_, dim_ + 1>;
		using this_type = lattice_view<iterator_, dim_>;
		using lower_type = lattice_view<iterator_, dim_ - 1>;
		using difference_type = int;
		using value_type = typename iterator_::value_type;
		using reference = typename iterator_::reference;
		using point_type = lattices::point<dim_>;
	public:
		static constexpr unsigned int dim(){ return dim_; }
	private:
		difference_type Size;
		difference_type Gap;
		lower_type Lower;
	public:
		lattice_view() = default;
		template<typename... others>
		lattice_view(iterator_ Begin_, iterator_ End_) = delete;//===ERROR=== Too few arguments for construct.
		template<typename... others>
		lattice_view(iterator_ Begin_, iterator_ End_, difference_type Size_, others... Others_)
			:Size(Size_)
			,Gap(0)
			,Lower(Begin_, End_, Others_...){
			static_assert(dim_ == 1 + sizeof...(others), "arguments is too few or many for this dim.");
			hmLib_assert(std::distance(Begin_, End_) >= lattice_step(), lattices::exception, "Too small range for lattice_view.");
		}
		template<typename... others>
		lattice_view(iterator_ Begin_, iterator_ End_, std::pair<difference_type, difference_type> SizeGap, others... Others_)
			:Size(0)
			,Gap(0)
			,Lower(Begin_, End_, Others_...){
			static_assert(dim_ == 1 + sizeof...(others), "arguments is too few or many for this dim.");
			Size = SizeGap.first;
			Gap = SizeGap.second;
			hmLib_assert(std::distance(Begin_, End_) >= lattice_step(), lattices::exception, "Too small range for lattice_view.");
		}
	public:
		template<typename... others>
		difference_type raw_position(difference_type Ini_, difference_type Pos_, others... Others_){
			static_assert(dim_ == 1 + sizeof...(others), "arguments is too few or many for this dim.");
			return Lower.raw_position(Ini_ + Pos_*lattice_step(), Others_...);
		}
		template<typename... others>
		reference at(difference_type Pos_, others... Others_){
			static_assert(dim_ == 1 + sizeof...(others), "arguments is too few or many for this dim.");
			return Lower.direct_at(Pos_*lattice_step(), Others_...);
		}
	public:
		template<unsigned int req_dim_>
		difference_type size()const{ 
			static_assert(req_dim_ < dim_, "requested dim is larger than lattice's dim.");
			return size_getter<req_dim_>()(*this);
		}
		template<unsigned int req_dim_>
		difference_type gap()const{ 
			static_assert(req_dim_ < dim_, "requested dim is larger than lattice's dim.");
			return gap_getter<req_dim_>()(*this);
		}
		difference_type lattice_size()const{ return Size*Lower.lattice_size(); }
		difference_type lattice_step()const{ return Gap+Lower.lattice_size()*Lower.lattice_step(); }
	private:
		reference direct_at(difference_type DirectPos_) = delete;	//===ERROR===: Too few arguments for access at.
		template<typename... others>
		reference direct_at(difference_type DirectPos_, difference_type Pos_, others... Others_){
			static_assert(dim_ == 1 + sizeof...(others), "arguments is too few or many for this dim.");
			return Lower.direct_at(DirectPos_+Pos_*lattice_step(), Others_...);
		}
	private:
		template<unsigned int req_dim_, typename T = void>
		struct size_getter{
			difference_type operator()(const this_type& This){ return This.Lower.size<req_dim_ - 1>(); }
		};
		template<typename T>
		struct size_getter<0, T>{
			difference_type operator()(const this_type& This){ return This.Size; }
		};
		template<unsigned int req_dim_, typename T = void>
		struct gap_getter{
			difference_type operator()(const this_type& This){ return This.Lower.gap<req_dim_ - 1>(); }
		};
		template<typename T>
		struct gap_getter<0, T>{
			difference_type operator()(const this_type& This){ return This.Gap; }
		};
	};
	template<typename iterator_>
	struct lattice_view<iterator_, 0>{
		friend struct lattice_view<iterator_, 1>;
		using this_type = lattice_view<iterator_, 0>;
		using difference_type = int;
		using value_type = typename iterator_::value_type;
		using reference = typename iterator_::reference;
	private:
		iterator_ Begin;
		iterator_ End;
	public:
		lattice_view() = default;
		lattice_view(iterator_ Begin_, iterator_ End_):Begin(Begin_),End(End_){}
		template<typename... args>
		lattice_view(iterator_ Begin_, iterator_ End_, difference_type Size_, args... Others_) = delete;//===ERROR===: Too many argument for lattice constructor.
		template<typename... args>
		lattice_view(iterator_ Begin_, iterator_ End_, std::pair<difference_type, difference_type> SizeGap, args... Others_) = delete;//===ERROR===: Too many argument for lattice constructor.
	public:
		difference_type raw_position(difference_type Ini_){return Ini_;}
	public:
		template<unsigned int dim>
		difference_type size()const = delete;//===ERROR===: Too higher dim is requested.
		template<unsigned int dim>
		difference_type gap()const = delete;//===ERROR===: Too higher dim is requested.
		difference_type lattice_size()const{ return 1; }
		difference_type lattice_step()const{ return 1; }
	private:
		reference direct_at(difference_type DirectPos_){return *std::next(Begin, DirectPos_);}
		template<typename... others>
		reference direct_at(others... Others_)=delete;//===ERROR===: Too much arguments for access position.
	};

	template<typename iterator, typename... others>
	auto make_lattice(iterator Begin, iterator End, others... Others)->lattice_view<iterator, sizeof...(others)>{
		return lattice_view<iterator, sizeof...(others)>(Begin, End, Others...);
	}
}
/*
#include<hmLib/exceptions.hpp>
#include<hmLib/position.hpp>
namespace lattice_views{
		namespace{
			class exception_identifier {};
		}
		using exception = hmLib::exceptions::exception_pattern<exception_identifier>;
		using out_of_range_access = hmLib::exceptions::access::out_of_range_access<exception>;
		using nullptr_dereference = hmLib::exceptions::access::nullptr_dereference<exception>;
		struct invalid_lattice_size : public exception{
			using base_type = exception;
		public:
			explicit invalid_lattice_size(const std::string& Message_) :base_type(Message_.c_str()) {}
			explicit invalid_lattice_size(const char* Message_) :base_type(Message_) {}
		};
	}
	template< typename iterator_>
		void lattice_advance(iterator_& itr, unsigned int latxsize, unsigned int x, unsigned int y){
		std::advance(itr, x + y*latsize);
	}
	template< typename iterator_>
		iterator_ lattice_next(iterator_& itr, unsigned int latxsize, unsigned int x, unsigned int y){
		return std::next(itr, x + y*latsize);
	}
	template<typename iterator_, unsigned int dim_>
	struct lattice_view{
	private:
		using iterator = iterator_;
		using my_type = lattice_view <iterator>;
		using value_type = decltype(*(iterator()));
	private:
		iterator Begin;
		unsigned int Pos;
		unsigned int Size;
		unsigned int Skip;
	public:
		struct row_iterator{
		private:
			using myitr_type = row_iterator;
		private:
			iterator itr;
			unsigned int x;
			unsigned int xsize;
			unsigned int xlatsize;
		public:
			const myitr_type& operator++(){
				if(x == xsize - 1){
					lattice_advance(itr, xlatsize, -(xsize - 1), 1);
					x = 0;
				} else{
					lattice_advance(itr, xlatsize, 1, 0);
					++x;
				}
			}
			myitr_type operator++(int){
				myitr_type Pre(*this);
				this->operator++();
				return Pre;
			}
			const myitr_type& operator--(){
				if(x == 0){
					lattice_advance(itr, xlatsize, -(xsize - 1), 1);
					x = xsize - 1;
				} else{
					lattice_advance(itr, xlatsize, 1, 0);
					--x;
				}
			}
			myitr_type operator--(int){
				myitr_type Pre(*this);
				this->operator--();
				return Pre;
			}
		};
		struct column_iterator{
		private:
			using myitr_type = row_iterator;
		private:
			iterator itr;
			unsigned int y;
			unsigned int ysize;
			unsigned int xlatsize;
		public:
			const myitr_type& operator++(){
				if(y == ysize - 1){
					lattice_advance(itr, xlatsize, 1, -(ysize - 1));
					y = 0;
				} else{
					lattice_advance(itr, xlatsize, 0, 1);
					++y;
				}
			}
			myitr_type operator++(int){
				myitr_type Pre(*this);
				this->operator++();
				return Pre;
			}
			const myitr_type& operator--(){
				if(y == 0){
					lattice_advance(itr, xlatsize, -1, ysize - 1);
					y = ysize - 1;
				} else{
					lattice_advance(itr, xlatsize, 0, -1);
					--y;
				}
			}
			myitr_type operator--(int){
				myitr_type Pre(*this);
				this->operator--();
				return Pre;
			}
		};
		struct point_iterator{
		private:
			using myitr_type = point_iterator;
		private:
			iterator itr;
			unsigned int xlatsize;
		public:
			void advance(int x, int y){
				lattice_advance(itr, xlatsize, x, y);
			}
			myitr_type next(int x, int y){
				myitr_type move_itr(*this);
				return move_itr.advance(x, y);
			}
		private:
			struct x_driver{
			private:
				myitr_type& ritr;
			public:
				x_driver(myitr_type& ritr_) :ritr(ritr_){}
				const myitr_type& operator++(){
					ritr.advance(1, 0);
					return ritr;
				}
				myitr_type operator++(int){
					myitr_type pre(ritr);
					ritr.advance(1, 0);
					return pre;
				}
				const myitr_type& operator--(){
					ritr.advance(-1, 0);
					return ritr;
				}
				myitr_type operator--(int){
					myitr_type pre(ritr);
					ritr.advance(-1, 0);
					return pre;
				}
			};
		public:
			x_driver x;
			y_driver y;
		public:
			point_iterator()
				: xlatsize(0)
				, x(*this)
				, y(*this){
			}
		};
	public:
		value_type& at(unsigned int x, unsigned y){
			hmLib_assert(x<xsize && y<ysize, lattice_views::out_of_range_access("out or range access."));
			return *lattice_next(viewbegin, xlatsize, xpos + x, ypos + y);
		}
		const value_type& at(unsigned int x, unsigned int y)const{
			hmLib_assert(x<xsize && y<ysize, lattice_views::out_of_range_access("out or range access."));
			return *lattice_next(viewbegin, xlatsize, xpos + x, ypos + y);
		}
		value_type& at(pint pos){
			hmLib_assert(0<pos.x && 0<pos.y, lattice_views::out_of_range_access("out or range access."));
			return at(pos.x, pos.y);
		}
		const value_type& at(pint pos)const{
			hmLib_assert(0<pos.x && 0<pos.y, lattice_views::out_of_range_access("out or range access."));
			return at(pos.x, pos.y);
		}
		value_type& operator[](pint pos){ return at(pos); }
		const value_type& operator[](pint pos)const{ return at(pos); }
	public:
		lattice_view(iterator begin_, iterator end_, unsigned int xlatsize_)
			: viewbegin(begin_)
			, viewend(end_)
			, xpos(0)
			, ypos(0){
			unsigned int dist = std::distance(begin_, end_);
			hmLib_assert(dist&xlatsize == 0, invalid_lattice_size, "invalid lattice size is used.");
			ylatsize = dist / xlatsize;
			xsize = xlatsize;
			ysize = ylatsize;
		}
		lattice_view(){
			hmLib_assert(xpos + xsize <= xlatsize && ypos + ysize <= ylatsize, invalid_lattice_size, "invalid lattice size is used.");
		}
	};
	template<typename iterator_>
	lattice_view<iterator_> make_lattice_view(iterator_ begin_, iterator_ end_, unsigned int xlatsize_){
		return lattice_view<iterator_>(begin_, end_, xlatsize_);
	}
	lattice_view<std::vector<int>::iterator> Lattice;
	auto itr = Lattice.point_begin();
	++(itr.x);
	++(itr.y);
	if(itr == Lattice.point_row_end();
}
*/

#
#endif
