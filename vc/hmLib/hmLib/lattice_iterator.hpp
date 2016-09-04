#ifndef HMLIB_LATTICEITERATOR_INC
#define HMLIB_LATTICEITERATOR_INC 100
#
#include<iterator>
namespace hmLib{
	template<typename iterator_, unsigned int dim_>
	struct basic_lattice_iterator{
		friend struct test_basic_lattice_iterator;
		using iterator_category = std::forward_iterator_tag;
		using difference_type = typename iterator_::difference_type;
		using size_type = std::size_t;
		using this_type = basic_lattice_iterator<iterator_, dim_>;
		using iterater_type = lattice_iterator<iterator_, dim_ - 1>;
		constexpr static unsigned int dim(){ return dim_; }
	private:
		iterater_type Itr;
		size_type Pos;
		size_type Size;
		size_type Step;
	public:
		size_type size()const{ return Size; }
		size_type lattice_size()const{ return size()*Itr.lattice_size(); }
		difference_type advance_pos(difference_type& RequestedStep){
			difference_type RawStep = Itr.advance_pos(RequestedStep);

			size_type NewPos = (Pos + RequestedStep) % Size;
			if(NewPos < 0) NewPos += Size;

			RawStep += (NewPos - Pos) * Step;

			if(RequestedStep > 0 && NewPos < Pos){
				RequestedStep = (RequestedStep % lattice_size()) + 1;
			} else if(RequestedStep < 0 && NewPos > Pos){
				RequestedStep = (RequestedStep % lattice_size()) - 1;
			} else{
				RequestedStep = (RequestedStep % lattice_size());
			}

			Pos = NewPos;

			return RawStep;
		}
		void advance_itr(difference_type RawStep){ Itr.raw_advance(RawStep); }
	};
	template<typename iterator_>
	struct basic_lattice_iterator<iterator_, 1>{
		friend struct test_basic_lattice_iterator;
		using difference_type = typename iterator_::difference_type;
		using size_type = std::size_t;
		using this_type = basic_lattice_iterator<iterator_, 1>;
		using iterater_type = iterator_;
		constexpr static unsigned int dim(){ return 1; }
	private:
		iterater_type Itr;
		size_type Pos;
		size_type Size;
		size_type Step;
	public:
		size_type size()const{ return Size; }
		size_type lattice_size()const{ return size(); }
		difference_type advance_pos(difference_type& RequestedStep){
			difference_type RawStep = 0;

			size_type NewPos = (Pos + RequestedStep) % Size;
			if(NewPos < 0) NewPos += Size;

			RawStep = (NewPos - Pos) * Step;

			if(RequestedStep > 0 && NewPos < Pos){
				RequestedStep = (RequestedStep % lattice_size()) + 1;
			} else if(RequestedStep < 0 && NewPos > Pos){
				RequestedStep = (RequestedStep % lattice_size()) - 1;
			} else{
				RequestedStep = (RequestedStep % lattice_size());
			}

			Pos = NewPos;

			return RawStep;
		}
		void advance_itr(difference_type RawStep){ std::advance(Itr, RawStep); }
	};
	template<typename iterator_>
	struct basic_lattice_iterator<iterator_, 0>{};
/*
	template<typename iterator_, unsigned int dim_, typename iterator_category_ = iterator_::iterator_category>
	struct lattice_iterator{
		using iterator_category = std::forward_iterator_tag;
		using difference_type = typename iterator_::difference_type;
	};

	template<typename iterator_, typename iterator_category = iterator_::iterator_category>
	struct lattice_iterator<iterator_, 0, iterator_category>{};

	template<typename iterator_, typename iterator_category = iterator_::iterator_category>
	struct lattice_iterator<iterator_, 1, iterator_category> : public iterator_{
		using this_type = lattice_iterator<iterator_, 1, iterator_category>;
		using basic_iterator = iterator_;
		constexpr static unsigned int dim(){ return dim_; }
	};
*/
}
#
#endif
