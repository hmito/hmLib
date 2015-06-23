#ifndef HMLIB_LATTICEVIEW_INC
#define HMLIB_LATTICEVIEW_INC 100
#
#include<iterator>
#include<hmLib/exceptions.hpp>
#include<hmLib/position.hpp>
namespace hmLib{
	namespace lattice_views{
		namespace{
			class exception_identifier {};
		}
		using exception = hmLib::exceptions::exception_pattern<exception_identifier>;
		using out_of_range_access = hmLib::exceptions::access::out_of_range_access<exception> ;
		using nullptr_dereference = hmLib::exceptions::access::nullptr_dereference<exception>;
		struct invalid_lattice_size: public exception{
			using base_type=exception;
		public:
			explicit invalid_lattice_size(const std::string& Message_) :base_type(Message_.c_str()) {}
			explicit invalid_lattice_size(const char* Message_) :base_type(Message_) {}
		};
	}
	template typename iterator_>
	void lattice_advance(iterator_& itr,unsigned int latxsize,unsigned int x, unsigned int y){
		std::advance(itr,x+y*latsize);
	}
	template typename iterator_>
	iterator_ lattice_next(iterator_& itr,unsigned int latxsize,unsigned int x, unsigned int y){
		return std::next(itr,x+y*latsize);
	}
	template<typename iterator_>
	struct lattice_view{
	private:
		using iterator = iterator_;
		using my_type = lattice_view <iterator>;
		using value_type = decltype(*(iterator()));
	private:
		iterator viewbegin;
		iterator viewend;
		unsigned int xlatsize;
		unsigned int ylatsize;
		unsigned int xpos;
		unsigned int ypos;
		unsigned int xsize;
		unsigned int ysize;
	public:
		struct row_iterator{
		private:
			using myitr_type=row_iterator;
		private:
			iterator itr;
			unsigned int x;
			unsigned int xsize;
			unsigned int xlatsize;
		public:
			const myitr_type& operator++(){
				if(x==xsize-1){
					lattice_advance(itr,xlatsize, -(xsize-1), 1);
					x=0;
				}else{
					lattice_advance(itr,xlatsize, 1, 0);
					++x;
				}
			}
			myitr_type operator++(int){
				myitr_type Pre(*this);
				this->operator++();
				return Pre;
			}
			const myitr_type& operator--(){
				if(x==0){
					lattice_advance(itr,xlatsize, -(xsize-1), 1);
					x=xsize-1;
				}else{
					lattice_advance(itr,xlatsize, 1, 0);
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
			using myitr_type=row_iterator;
		private:
			iterator itr;
			unsigned int y;
			unsigned int ysize;
			unsigned int xlatsize;
		public:
			const myitr_type& operator++(){
				if(y==ysize-1){
					lattice_advance(itr,xlatsize, 1,-(ysize-1));
					y=0;
				}else{
					lattice_advance(itr,xlatsize, 0, 1);
					++y;
				}
			}
			myitr_type operator++(int){
				myitr_type Pre(*this);
				this->operator++();
				return Pre;
			}
			const myitr_type& operator--(){
				if(y==0){
					lattice_advance(itr,xlatsize, -1,ysize-1);
					y=ysize-1;
				}else{
					lattice_advance(itr,xlatsize, 0, -1);
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
			using myitr_type=point_iterator;
		private:
			iterator itr;
			unsigned int xlatsize;
		public:
			void advance(int x,int y){
				lattice_advance(itr,xlatsize,x,y);
			}
			myitr_type next(int x, int y){
				myitr_type move_itr(*this);
				return move_itr.advance(x,y);
			}
		private:
			struct x_driver{
			private:
				myitr_type& ritr;
			public:
				x_driver(myitr_type& ritr_):ritr(ritr_){}
				const myitr_type& operator++(){
					ritr.advance(1,0);
					return ritr;
				}
				myitr_type operator++(int){
					myitr_type pre(ritr);
					ritr.advance(1,0);
					return pre;
				}
				const myitr_type& operator--(){
					ritr.advance(-1,0);
					return ritr;
				}
				myitr_type operator--(int){
					myitr_type pre(ritr);
					ritr.advance(-1,0);
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
			return *lattice_next(viewbegin,xlatsize, xpos+x,ypos+y);
		}
		const value_type& at(unsigned int x, unsigned int y)const{
			hmLib_assert(x<xsize && y<ysize, lattice_views::out_of_range_access("out or range access."));
			return *lattice_next(viewbegin,xlatsize, xpos+x,ypos+y);
		}
		value_type& at(pint pos){
			hmLib_assert(0<pos.x && 0<pos.y, lattice_views::out_of_range_access("out or range access."));
			return at(pos.x, pos.y);
		}
		const value_type& at(pint pos)const{
			hmLib_assert(0<pos.x && 0<pos.y, lattice_views::out_of_range_access("out or range access."));
			return at(pos.x, pos.y); 
		}
		value_type& operator[](pint pos){ return at(pos);}
		const value_type& operator[](pint pos)const{ return at(pos); }
	public:
		lattice_view(iterator begin_, iterator end_, unsigned int xlatsize_)
			: viewbegin(begin_)
			, viewend(end_)
			, xpos(0)
			, ypos(0){
			unsigned int dist=std::distance(begin_, end_);
			hmLib_assert(dist&xlatsize==0,invalid_lattice_size,"invalid lattice size is used.");
			ylatsize=dist/xlatsize;
			xsize=xlatsize;
			ysize=ylatsize;
		}
		lattice_view(){
			hmLib_assert(xpos+xsize<=xlatsize && ypos+ysize<=ylatsize,invalid_lattice_size,"invalid lattice size is used.");
		}
	};
	template<typename iterator_>
	lattice_view<iterator_> make_lattice_view(iterator_ begin_, iterator_ end_,unsigned int xlatsize_){
		return lattice_view<iterator_>(begin_,end_,xlatsize_);
	}
		lattice_view<std::vector<int>::iterator> Lattice;
		auto itr=Lattice.point_begin();
		++(itr.x);
		++(itr.y);
		if(itr==Lattice.point_row_end();
	}
}
#
#endif
