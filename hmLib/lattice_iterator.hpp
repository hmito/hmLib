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
	template<typename iterator_>
	struct basic_lattice_iterator<iterator_, 0>{};

	template<typename this_type, typename iterator_category_ = this_type::iterator_category, bool is_const_ = std::is_const<this_type::value_type>::value>
	struct lattice_iterator_mixin{};
	template<typename this_type, bool is_const_>
	struct lattice_iterator_mixin<this_type, std::output_iterator_tag, is_const_>{
		this_type& operator++(){
			static_cast<this_type*>(this)->advance(1);
			return *this;
		}
		this_type operator++(int){
			this_type Prev = *this;
			++(*this);
			return Prev;
		}
		this_type::reference operator*(){
			return static_cast<this_type*>(this)->ref().operator*();
		}
		const this_type::reference operator*()const{
			return static_cast<this_type*>(this)->ref().operator*();
		}
		this_type::pointer operator->(){
			return static_cast<this_type*>(this)->ref().operator->();
		}
		const this_type::pointer operator->()const{
			return static_cast<this_type*>(this)->ref().operator->();
		}
	};
	template<typename this_type, bool is_const_>
	struct lattice_iterator_mixin<this_type, std::input_iterator_tag, is_const_>{
		this_type& operator++(){
			static_cast<this_type*>(this)->advance(1);
			return *this;
		}
		this_type operator++(int){
			this_type Prev = *this;
			++(*this);
			return Prev;
		}
		const this_type::reference operator*()const{
			return static_cast<this_type*>(this)->ref().operator*();
		}
		const this_type::pointer operator->()const{
			return static_cast<this_type*>(this)->ref().operator->();
		}
		friend bool operator==(const this_type& val1, const this_type& val2){
			return val1.ref() == val2.ref();
		}
		friend bool operator!=(const this_type& val1, const this_type& val2){
			return val1.ref() != val2.ref();
		}
	};
	template<typename this_type>
	struct lattice_iterator_mixin<this_type, std::forward_iterator_tag, false>
		: public lattice_iterator_mixin<this_type, std::output_iterator_tag, false>{
		friend bool operator==(const this_type& val1, const this_type& val2){
			return val1.ref() == val2.ref();
		}
		friend bool operator!=(const this_type& val1, const this_type& val2){
			return val1.ref() != val2.ref();
		}
	};
	template<typename this_type>
	struct lattice_iterator_mixin<this_type, std::forward_iterator_tag, true>
		: public lattice_iterator_mixin<this_type, std::input_iterator_tag, true>{
	};
	template<typename this_type, bool is_const_>
	struct lattice_iterator_mixin<this_type, std::bidirectional_iterator_tag, is_const_>
		: public lattice_iterator_mixin<this_type, std::forward_iterator_tag, is_const_>{
		this_type& operator--(){
			static_cast<this_type*>(this)->advance(-1);
			return *this;
		}
		this_type operator--(int){
			this_type Prev = *this;
			--(*this);
			return Prev;
		}
	};
	template<typename this_type>
	struct lattice_iterator_mixin<this_type, std::random_access_iterator_tag, true>
		: public lattice_iterator_mixin<this_type, std::bidirectional_iterator_tag, true>{
		const this_type::reference operator[](this_type::difference_type pos)const{
			this_type other = *static_cast<const this_type*>(this);
			other += pos;
			return *other;
		}
		this_type& operator+=(this_type::difference_type pos){
			static_cast<this_type*>(this)->advance(pos1);
			return *this;

		}
		friend this_type operator+(const this_type& val, this_type::difference_type pos){
			this_type other = val;
			other += pos;
			return other;
		}
		friend this_type operator+(this_type::difference_type pos, const this_type& val){
			this_type other = val;
			other += pos;
			return other;
		}
		friend this_type::difference_type operator-(const this_type& val1, const this_type& val2){
			return val1.ref() - val2.ref();
		}
		friend bool operator<(const this_type& val1, const this_type& val2){
			return val1.ref() < val2.ref();
		}
		friend bool operator>(const this_type& val1, const this_type& val2){
			return val1.ref() > val2.ref();
		}
		friend bool operator<=(const this_type& val1, const this_type& val2){
			return val1.ref() <= val2.ref();
		}
		friend bool operator>=(const this_type& val1, const this_type& val2){
			return val1.ref() >= val2.ref();
		}
	};
	template<typename this_type>
	struct lattice_iterator_mixin<this_type, std::random_access_iterator_tag, false>
		: public lattice_iterator_mixin<this_type, std::bidirectional_iterator_tag, false>{
		this_type::reference operator[](this_type::difference_type pos){
			this_type other = *static_cast<this_type*>(this);
			other += pos;
			return *other;
		}
		const this_type::reference operator[](this_type::difference_type pos)const{
			this_type other = *static_cast<const this_type*>(this);
			other += pos;
			return *other;
		}
		this_type& operator+=(this_type::difference_type pos){
			static_cast<this_type*>(this)->advance(pos1);
			return *this;

		}
		friend this_type operator+(const this_type& val, this_type::difference_type pos){
			this_type other = val;
			other += pos;
			return other;
		}
		friend this_type operator+(this_type::difference_type pos, const this_type& val){
			this_type other = val;
			other += pos;
			return other;
		}
		friend this_type::difference_type operator-(const this_type& val1, const this_type& val2){
			return val1.ref() - val2.ref();
		}
		friend bool operator<(const this_type& val1, const this_type& val2){
			return val1.ref() < val2.ref();
		}
		friend bool operator>(const this_type& val1, const this_type& val2){
			return val1.ref() > val2.ref();
		}
		friend bool operator<=(const this_type& val1, const this_type& val2){
			return val1.ref() <= val2.ref();
		}
		friend bool operator>=(const this_type& val1, const this_type& val2){
			return val1.ref() >= val2.ref();
		}
	};

	template<typename iterator_, unsigned int dim_, typename iterator_category_ = iterator_::iterator_category, bool is_const_ = std::is_const<iterator_::value_type>::value>
	struct lattice_iterator
		: public std::iterator<iterator_::iterator_category, iterator_::value_type, iterator_::difference_type, iterator_::pointer, iterator_::reference>
		, public basic_lattice_iterator<iterator_,dim_>
		, public lattice_iterator_mixin<lattice_iterator<iterator_, dim_, iterator_category_, is_const_>,iterator_category_, is_const_>{
	private:
		using this_type = lattice_iterator<iterator_, dim_, iterator_category_, is_const_>;
	public:
		lattice_iterator() = default;
		template<typename iterator_pos, typename iterator_size, typename iterator_step>
		lattice_iterator(iterator_ itr, iterator_pos begin_pos, iterator_pos end_pos, iterator_size begin_size, iterator_size end_size, iterator_step begin_step, iterator_step end_step)
			: basic_lattice_iterator<iterator_, dim_>(basic_lattice_iterator<iterator_, dim_-1>(itr, begin_pos+1, end_pos, begin_step + 1, end_size, begin_step + 1, end_step), *begin_pos, *begin_size, *begin_step){
		}
		template<typename container_pos, typename container_size, typename container_step>
		lattice_iterator(iterator_ itr, const container_pos& set_pos, const container_size& set_size, const container_step& set_step)
			: lattice_iterator(itr, std::begin(set_pos), std::end(set_pos), std::begin(set_size), std::end(set_size), std::begin(set_step), std::end(set_step)){
		}
	};
	template<typename iterator_, typename iterator_category_ = iterator_::iterator_category, bool is_const_ = std::is_const<iterator_::value_type>::value>
	struct lattice_iterator<iterator_, 1, iterator_category_, is_const_>
		: public std::iterator<iterator_::iterator_category, iterator_::value_type, iterator_::difference_type, iterator_::pointer, iterator_::reference>
		, public basic_lattice_iterator<iterator_, 1>
		, public lattice_iterator_mixin<lattice_iterator<iterator_, 1, iterator_category_, is_const_>, iterator_category_, is_const_>{
	private:
		using this_type = lattice_iterator<iterator_, 1, iterator_category_, is_const_>;
	public:
		lattice_iterator() = default;
		template<typename iterator_pos, typename iterator_size, typename iterator_step>
		lattice_iterator(iterator_ itr, iterator_pos begin_pos, iterator_pos end_pos, iterator_size begin_size, iterator_size end_size, iterator_step begin_step, iterator_step end_step)
			: basic_lattice_iterator<iterator_, 1>(itr, *begin_pos, *begin_size, *begin_step){
		}
		template<typename container_pos, typename container_size, typename container_step>
		lattice_iterator(iterator_ itr, const container_pos& set_pos, const container_size& set_size, const container_step& set_step)
			: lattice_iterator(itr, std::begin(set_pos), std::end(set_pos), std::begin(set_size), std::end(set_size), std::begin(set_step), std::end(set_step)){
		}
	};
	template<typename iterator_, typename iterator_category_ = iterator_::iterator_category, bool is_const_ = std::is_const<iterator_::value_type>::value>
	struct lattice_iterator<iterator_, 0, iterator_category_, is_const_>{};
}
#
#endif
