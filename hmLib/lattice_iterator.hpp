#ifndef HMLIB_LATTICEITERATOR_INC
#define HMLIB_LATTICEITERATOR_INC 100
#
#include<iterator>
namespace hmLib{
	template<typename iterator_, unsigned int dim_>
	struct basic_lattice_iterator{
		friend class test_basic_lattice_iterator;
		friend struct basic_lattice_iterator<iterator_, dim_+1>;
		using iterator_category = std::forward_iterator_tag;
		using difference_type = typename iterator_::difference_type;
		using size_type = std::size_t;
		using this_type = basic_lattice_iterator<iterator_, dim_>;
		using iterator_type = basic_lattice_iterator<iterator_, dim_ - 1>;
		constexpr static unsigned int dim(){ return dim_; }
	private:
		iterator_type Itr;
		size_type Pos;
		size_type Size;
		size_type Step;
	public:
		basic_lattice_iterator() = default;
		basic_lattice_iterator(iterator_type Itr_, size_type Pos_, size_type Size_, size_type Step_)
			: Itr(Itr_)
			, Pos(Pos_)
			, Size(Size_)
			, Step(Step_){
		}
	private:
		difference_type advance_pos(difference_type& RequestedStep){
			difference_type RawStep = 0;

			size_type NewPos = (Pos + RequestedStep) % Size;
			if(NewPos < 0) NewPos += Size;

			RawStep = (NewPos - Pos) * Step;

			if(RequestedStep > 0 && NewPos < Pos){
				RequestedStep = (RequestedStep / lattice_size()) + 1;
			} else if(RequestedStep < 0 && NewPos > Pos){
				RequestedStep = (RequestedStep / lattice_size()) - 1;
			} else{
				RequestedStep = (RequestedStep / lattice_size());
			}

			Pos = NewPos;

			return RawStep;
		}
		void advance_itr(difference_type RawStep){ std::advance(Itr, RawStep); }
	public:
		size_type size()const{ return Size; }
		size_type lattice_size()const{ return size()*Itr.lattice_size(); }
		void advance(difference_type Diff){
			auto RawStep = advance_pos(Diff);
			advance_itr(RawStep);
		}
		iterator_& ref(){return Itr.ref();}
		const iterator_& ref()const{return Itr.ref();}
	};
	template<typename iterator_>
	struct basic_lattice_iterator<iterator_, 1>{
		friend class test_basic_lattice_iterator;
		friend struct basic_lattice_iterator<iterator_, 2>;
		using difference_type = typename iterator_::difference_type;
		using size_type = std::size_t;
		using this_type = basic_lattice_iterator<iterator_, 1>;
		using iterator_type = iterator_;
		constexpr static unsigned int dim(){ return 1; }
	private:
		iterator_type Itr;
		size_type Pos;
		size_type Size;
		size_type Step;
	public:
		basic_lattice_iterator() = default;
		basic_lattice_iterator(iterator_type Itr_, size_type Pos_, size_type Size_, size_type Step_)
			: Itr(Itr_)
			, Pos(Pos_)
			, Size(Size_)
			, Step(Step_){
		}
	private:
		difference_type advance_pos(difference_type& RequestedStep){
			difference_type RawStep = 0;

			size_type NewPos = (Pos + RequestedStep) % Size;
			if(NewPos < 0) NewPos += Size;

			RawStep = (NewPos - Pos) * Step;

			if(RequestedStep > 0 && NewPos < Pos){
				RequestedStep = (RequestedStep / lattice_size()) + 1;
			} else if(RequestedStep < 0 && NewPos > Pos){
				RequestedStep = (RequestedStep / lattice_size()) - 1;
			} else{
				RequestedStep = (RequestedStep / lattice_size());
			}

			Pos = NewPos;

			return RawStep;
		}
		void advance_itr(difference_type RawStep){ std::advance(Itr, RawStep); }
	public:
		size_type size()const{ return Size; }
		size_type lattice_size()const{ return size(); }
		void advance(difference_type Diff){
			auto RawStep = advance_pos(Diff);
			advance_itr(RawStep);
		}
		iterator_& ref(){return Itr;}
		const iterator_& ref()const{return Itr;}
	};

	template<typename iterator_, unsigned int dim_, typename iterator_category_ = iterator_::iterator_category>
	struct lattice_iterator
		: public std::iterator<iterator_::iterator_category,iterator_::value_type,iterator_::difference_type,iterator_::pointer,iterator_::reference>{
	};

	template<typename iterator_, unsigned int dim_>
	struct lattice_iterator<iterator_, dim_, std::output_iterator_tag>
		: public std::iterator<iterator_::iterator_category, iterator_::value_type, iterator_::difference_type, iterator_::pointer, iterator_::reference>
		, private basic_lattice_iterator<iterator_, dim_>{
	private:
		using this_type = lattice_iterator<iterator_, dim_, iterator_category>;
		using base_type = basic_lattice_iterator<iterator_, dim_>;
	public:
		this_type& operator++(){
			base_type::advance(1);
			return *this;
		}
		this_type operator++(int){
			this_type Prev = *this;
			base_type::advance(1);
			return Prev;
		}
		reference operator*(){
			return base_type::ref().operator*();
		}
	};
	template<typename iterator_, unsigned int dim_>
	struct lattice_iterator<iterator_, dim_, std::input_iterator_tag>
		: public std::iterator<iterator_::iterator_category, iterator_::value_type, iterator_::difference_type, iterator_::pointer, iterator_::reference>
		, private basic_lattice_iterator<iterator_, dim_>{
	private:
		using this_type = lattice_iterator<iterator_, dim_, iterator_category>;
		using base_type = basic_lattice_iterator<iterator_, dim_>;
	public:
		this_type& operator++(){
			base_type::advance(1);
			return *this;
		}
		this_type operator++(int){
			this_type Prev = *this;
			base_type::advance(1);
			return Prev;
		}
		const reference operator*()const{
			return base_type::ref().operator*();
		}
		const pointer operator->()const{
			return base_type::ref().operator->();
		}
		friend bool operator==(const this_type& val1, const this_type& val2){
			return val1.ref() == val2.ref();
		} 
		friend bool operator!=(const this_type& val1, const this_type& val2){
			return val1.ref() != val2.ref();
		}
	};
	template<typename iterator_, unsigned int dim_>
	struct lattice_iterator<iterator_, dim_, std::forward_iterator_tag>
		: public std::iterator<iterator_::iterator_category, iterator_::value_type, iterator_::difference_type, iterator_::pointer, iterator_::reference>
		, private basic_lattice_iterator<iterator_, dim_>{
	private:
		using this_type = lattice_iterator<iterator_, dim_, iterator_category>;
		using base_type = basic_lattice_iterator<iterator_, dim_>;
	public:
		this_type& operator++(){
			base_type::advance(1);
			return *this;
		}
		this_type operator++(int){
			this_type Prev = *this;
			base_type::advance(1);
			return Prev;
		}
		reference operator*(){
			return base_type::ref().operator*();
		}
		const reference operator*()const{
			return base_type::ref().operator*();
		}
		pointer operator->(){
			return base_type::ref().operator->();
		}
		const pointer operator->()const{
			return base_type::ref().operator->();
		}
		friend bool operator==(const this_type& val1, const this_type& val2){
			return val1.ref() == val2.ref();
		} 
		friend bool operator!=(const this_type& val1, const this_type& val2){
			return val1.ref() != val2.ref();
		}
	};
	template<typename iterator_, unsigned int dim_>
	struct lattice_iterator<iterator_, dim_, std::bidirectional_iterator_tag>
		: public std::iterator<iterator_::iterator_category, iterator_::value_type, iterator_::difference_type, iterator_::pointer, iterator_::reference>
		, private basic_lattice_iterator<iterator_, dim_>{
	private:
		using this_type = lattice_iterator<iterator_, dim_, iterator_category>;
		using base_type = basic_lattice_iterator<iterator_, dim_>;
	public:
		this_type& operator++(){
			base_type::advance(1);
			return *this;
		}
		this_type operator++(int){
			this_type Prev = *this;
			this->operator++();
			return Prev;
		}
		this_type& operator--(){
			base_type::advance(-1);
			return *this;
		}
		this_type operator--(int){
			this_type Prev = *this;
			this->operator--();
			return Prev;
		}
		reference operator*(){
			return base_type::ref().operator*();
		}
		const reference operator*()const{
			return base_type::ref().operator*();
		}
		pointer operator->(){
			return base_type::ref().operator->();
		}
		const pointer operator->()const{
			return base_type::ref().operator->();
		}
		friend bool operator==(const this_type& val1, const this_type& val2){
			return val1.ref() == val2.ref();
		} 
		friend bool operator!=(const this_type& val1, const this_type& val2){
			return val1.ref() != val2.ref();
		}
	};
	template<typename iterator_, unsigned int dim_>
	struct lattice_iterator<iterator_, dim_, std::random_access_iterator_tag>
		: public std::iterator<iterator_::iterator_category, iterator_::value_type, iterator_::difference_type, iterator_::pointer, iterator_::reference>
		, private basic_lattice_iterator<iterator_, dim_>{
	private:
		using this_type = lattice_iterator<iterator_, dim_, iterator_category>;
		using base_type = basic_lattice_iterator<iterator_, dim_>;
	public:
		this_type& operator++(){
			base_type::advance(1);
			return *this;
		}
		this_type operator++(int){
			this_type Prev = *this;
			this->operator++();
			return Prev;
		}
		this_type& operator--(){
			base_type::advance(-1);
			return *this;
		}
		this_type operator--(int){
			this_type Prev = *this;
			this->operator--();
			return Prev;
		}
		reference operator*(){
			return base_type::ref().operator*();
		}
		const reference operator*()const{
			return base_type::ref().operator*();
		}
		pointer operator->(){
			return base_type::ref().operator->();
		}
		const pointer operator->()const{
			return base_type::ref().operator->();
		}
		friend bool operator==(const this_type& val1, const this_type& val2){
			return val1.ref() == val2.ref();
		} 
		friend bool operator!=(const this_type& val1, const this_type& val2){
			return val1.ref() != val2.ref();
		}
	};
}
#
#endif
