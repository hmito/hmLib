#ifndef HMLIB_LATTICEITERATOR_INC
#define HMLIB_LATTICEITERATOR_INC 100
#
#include<iterator>
namespace hmLib{
	template<typename iterator_, unsigned int dim_, typename iterator_category_ = iterator_::iterator_category>
	struct lattice_iterator{};
	
	template<typename iterator_, unsigned int dim_>
	struct lattice_iterator<iterator_, dim_, std::forward_iterator_tag>{
		using iterator_category = std::forward_iterator_tag;
		using difference_type = typename iterator_::difference_type;
		using size_type = std::size_t;
		using this_type = lattice_iterator<iterator_, dim_, iterator_category>;
		using basic_iterator = lattice_iterator<iterator_, dim_ - 1, iterator_category>;
		constexpr static unsigned int dim(){ return dim_; }
	private:
		basic_iterator Itr;
		unsigned int Pos;
		size_type Size;
		size_type Step;
	public:
		size_type size()const{ return Size; }
		size_type lattice_size()const{ return size()*Itr.lattice_size(); }
		difference_type raw_count(difference_type& RequestedStep){
			auto LowerStep = Itr.raw_count(RequestedStep);
			
			difference_type NewPos = Pos;
			NewPos -= (Pos + RequestedStep) % Size;


		}
		void raw_advance(difference_type RawStep){ Itr.raw_advance(RawStep); }
		this_type& operator++(){
			++Itr;
			++Pos;

			if(Pos == Size){
				std::advance(Itr, LatticeSize - Size);
				Pos = 0;
			}

			return *this;
		}
		this_type operator++(int){
			this_type Prev = *this;
			this->operator++();
			return Prev;
		}
	};
	template<typename iterator_, typename iterator_category = iterator_::iterator_category>
	struct lattice_iterator<iterator_, 0, iterator_category>{};

	template<typename iterator_, typename iterator_category = iterator_::iterator_category>
	struct lattice_iterator<iterator_, 1, iterator_category> : public iterator_{
		using this_type = lattice_iterator<iterator_, 1, iterator_category>;
		using basic_iterator = iterator_;
		constexpr static unsigned int dim(){ return dim_; }
	};
}
#
#endif
