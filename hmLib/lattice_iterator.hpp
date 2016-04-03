#ifndef HMLIB_LATTICEITERATOR_INC
#define HMLIB_LATTICEITERATOR_INC 100
#
#include<iterator>
namespace hmLib{
	namespace{
		template<typename iterator_, unsigned int dim_>
		struct basic_lattice_iterator{
			friend struct basic_lattice_iterator<iterator_, dim_ + 1>;
			using iterator_category = std::forward_iterator_tag;
			using difference_type = typename iterator_::difference_type;
			using this_type = basic_lattice_iterator<iterator_, dim_>;
			using iterator_type = basic_lattice_iterator<iterator_, dim_ - 1>;
			constexpr static unsigned int dim(){ return dim_; }
		private:
			iterator_type Itr;
			difference_type Pos;
			difference_type Size;
			difference_type Gap;
		public:
			basic_lattice_iterator() = default;
			basic_lattice_iterator(iterator_type Itr_, difference_type Pos_, difference_type Size_, difference_type Gap_)
				: Itr(Itr_)
				, Pos(Pos_)
				, Size(Size_)
				, Gap(Gap_){}
		private:
			difference_type advance_pos(difference_type& RequestedStep){
				difference_type RawStep = Itr.advance_pos(RequestedStep);

				difference_type NewPos = (Pos + RequestedStep) % Size;
				if(NewPos < 0) NewPos += Size;

				RawStep += (NewPos - Pos) * step_distance();

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
			void advance_itr(difference_type RawStep){ std::advance(Itr.ref(), RawStep); }
		public:
			difference_type pos()const{ return Pos; }
			difference_type size()const{ return Size; }
			difference_type step_size()const{ return Itr.lattice_size(); }
			difference_type lattice_size()const{ return Itr.lattice_size()*size(); }
			difference_type gap()const{ return Gap; }
			difference_type step_distance()const{ return gap() + step_size(); }
			void advance(difference_type Diff){
				auto RawStep = advance_pos(Diff);
				advance_itr(RawStep);
			}
			iterator_& ref(){ return Itr.ref(); }
			const iterator_& ref()const{ return Itr.ref(); }
		};
		template<typename iterator_>
		struct basic_lattice_iterator<iterator_, 1>{
			friend struct basic_lattice_iterator<iterator_, 2>;
			using difference_type = typename iterator_::difference_type;
			using this_type = basic_lattice_iterator<iterator_, 1>;
			using iterator_type = iterator_;
			constexpr static unsigned int dim(){ return 1; }
		private:
			iterator_type Itr;
			difference_type Pos;
			difference_type Size;
			difference_type Gap;
		public:
			basic_lattice_iterator() = default;
			basic_lattice_iterator(iterator_type Itr_, difference_type Pos_, difference_type Size_, difference_type Gap_)
				: Itr(Itr_)
				, Pos(Pos_)
				, Size(Size_)
				, Gap(Gap_){}
		private:
			difference_type advance_pos(difference_type& RequestedStep){
				difference_type RawStep = 0;

				difference_type NewPos = (Pos + RequestedStep) % Size;
				if(NewPos < 0) NewPos += Size;

				RawStep = (NewPos - Pos) * step_distance();

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
			difference_type pos()const{ return Pos; }
			difference_type size()const{ return Size; }
			difference_type step_size()const{ return 1; }
			difference_type lattice_size()const{ return size(); }
			difference_type gap()const{ return Gap; }
			difference_type step_distance()const{ return gap() + step_size(); }
			void advance(difference_type Diff){
				auto RawStep = advance_pos(Diff);
				advance_itr(RawStep);
			}
			iterator_& ref(){ return Itr; }
			const iterator_& ref()const{ return Itr; }
		};
		template<typename iterator_>
		struct basic_lattice_iterator<iterator_, 0>{};

		template<typename this_type, typename iterator_category_ = this_type::iterator_category, bool is_const_ = std::is_const<this_type::value_type>::value, typename reference = this_type::reference, typename pointer = this_type::pointer, typename difference_type = this_type::difference_type>
		struct lattice_iterator_mixin{};
		template<typename this_type, bool is_const_, typename reference, typename pointer,typename difference_type>
		struct lattice_iterator_mixin<this_type, std::output_iterator_tag, is_const_ ,reference, pointer, difference_type>{
			this_type& operator++(){
				static_cast<this_type*>(this)->advance(1);
				return *this;
			}
			this_type operator++(int){
				this_type Prev = *this;
				++(*this);
				return Prev;
			}
			reference operator*(){
				return static_cast<this_type*>(this)->ref().operator*();
			}
			const reference operator*()const{
				return static_cast<this_type*>(this)->ref().operator*();
			}
			pointer operator->(){
				return static_cast<this_type*>(this)->ref().operator->();
			}
			const pointer operator->()const{
				return static_cast<this_type*>(this)->ref().operator->();
			}
		};
		template<typename this_type, bool is_const_, typename reference, typename pointer, typename difference_type>
		struct lattice_iterator_mixin<this_type, std::input_iterator_tag, is_const_, reference, pointer, difference_type>{
			this_type& operator++(){
				static_cast<this_type*>(this)->advance(1);
				return *this;
			}
			this_type operator++(int){
				this_type Prev = *this;
				++(*this);
				return Prev;
			}
			const reference operator*()const{
				return static_cast<this_type*>(this)->ref().operator*();
			}
			const pointer operator->()const{
				return static_cast<this_type*>(this)->ref().operator->();
			}
			friend bool operator==(const this_type& val1, const this_type& val2){
				return val1.ref() == val2.ref();
			}
			friend bool operator!=(const this_type& val1, const this_type& val2){
				return val1.ref() != val2.ref();
			}
		};
		template<typename this_type, typename reference, typename pointer, typename difference_type>
		struct lattice_iterator_mixin<this_type, std::forward_iterator_tag, false, reference, pointer, difference_type>
			: public lattice_iterator_mixin<this_type, std::output_iterator_tag, false, reference, pointer, difference_type>{
			friend bool operator==(const this_type& val1, const this_type& val2){
				return val1.ref() == val2.ref();
			}
			friend bool operator!=(const this_type& val1, const this_type& val2){
				return val1.ref() != val2.ref();
			}
		};
		template<typename this_type, typename reference, typename pointer, typename difference_type>
		struct lattice_iterator_mixin<this_type, std::forward_iterator_tag, true, reference, pointer, difference_type>
			: public lattice_iterator_mixin<this_type, std::input_iterator_tag, true, reference, pointer, difference_type>{
		};
		template<typename this_type, bool is_const_, typename reference, typename pointer, typename difference_type>
		struct lattice_iterator_mixin<this_type, std::bidirectional_iterator_tag, is_const_, reference, pointer, difference_type>
			: public lattice_iterator_mixin<this_type, std::forward_iterator_tag, is_const_, reference, pointer, difference_type>{
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
		template<typename this_type, typename reference, typename pointer, typename difference_type>
		struct lattice_iterator_mixin<this_type, std::random_access_iterator_tag, true, reference, pointer, difference_type>
			: public lattice_iterator_mixin<this_type, std::bidirectional_iterator_tag, true, reference, pointer, difference_type>{
			const reference operator[](difference_type pos)const{
				this_type other = *static_cast<const this_type*>(this);
				other += pos;
				return *other;
			}
			this_type& operator+=(difference_type pos){
				static_cast<this_type*>(this)->advance(pos1);
				return *this;

			}
			friend this_type operator+(const this_type& val, difference_type pos){
				this_type other = val;
				other += pos;
				return other;
			}
			friend this_type operator+(difference_type pos, const this_type& val){
				this_type other = val;
				other += pos;
				return other;
			}
			friend difference_type operator-(const this_type& val1, const this_type& val2){
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
		template<typename this_type, typename reference, typename pointer, typename difference_type>
		struct lattice_iterator_mixin<this_type, std::random_access_iterator_tag, false, reference, pointer, difference_type>
			: public lattice_iterator_mixin<this_type, std::bidirectional_iterator_tag, false, reference, pointer, difference_type>{
			reference operator[](difference_type pos){
				this_type other = *static_cast<this_type*>(this);
				other += pos;
				return *other;
			}
			const reference operator[](difference_type pos)const{
				this_type other = *static_cast<const this_type*>(this);
				other += pos;
				return *other;
			}
			this_type& operator+=(difference_type pos){
				static_cast<this_type*>(this)->advance(pos1);
				return *this;

			}
			friend this_type operator+(const this_type& val, difference_type pos){
				this_type other = val;
				other += pos;
				return other;
			}
			friend this_type operator+(difference_type pos, const this_type& val){
				this_type other = val;
				other += pos;
				return other;
			}
			friend difference_type operator-(const this_type& val1, const this_type& val2){
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
	}
	template<typename iterator_, unsigned int dim_, typename iterator_category_ = iterator_::iterator_category, bool is_const_ = std::is_const<iterator_::value_type>::value>
	struct lattice_iterator
		: public std::iterator<typename iterator_::iterator_category, typename iterator_::value_type, typename iterator_::difference_type, typename iterator_::pointer, typename iterator_::reference>
		, public basic_lattice_iterator<iterator_,dim_>
		, public lattice_iterator_mixin<lattice_iterator<iterator_, dim_, iterator_category_, is_const_>,iterator_category_, is_const_, typename iterator_::reference, typename iterator_::pointer, typename iterator_::difference_type>{
		friend class test_lattice_iterator;
	private:
		using this_type = lattice_iterator<iterator_, dim_, iterator_category_, is_const_>;
	public:
		lattice_iterator() = default;
		template<typename iterator_pos, typename iterator_size, typename iterator_step>
		lattice_iterator(iterator_ itr, iterator_pos begin_pos, iterator_pos end_pos, iterator_size begin_size, iterator_size end_size, iterator_step begin_step, iterator_step end_step)
			: basic_lattice_iterator<iterator_, dim_>(basic_lattice_iterator<iterator_, dim_-1>(itr, begin_pos+1, end_pos, begin_size + 1, end_size, begin_step + 1, end_step), *begin_pos, *begin_size, *begin_step){
		}
		template<typename container_pos, typename container_size, typename container_step>
		lattice_iterator(iterator_ itr, const container_pos& set_pos, const container_size& set_size, const container_step& set_step)
			: lattice_iterator(itr, std::begin(set_pos), std::end(set_pos), std::begin(set_size), std::end(set_size), std::begin(set_step), std::end(set_step)){
		}
	};
	template<typename iterator_, typename iterator_category_, bool is_const_>
	struct lattice_iterator<iterator_, 1, iterator_category_, is_const_>
		: public std::iterator<typename iterator_::iterator_category, typename iterator_::value_type, typename iterator_::difference_type, typename iterator_::pointer, typename iterator_::reference>
		, public basic_lattice_iterator<iterator_, 1>
		, public lattice_iterator_mixin<lattice_iterator<iterator_, 1, iterator_category_, is_const_>, iterator_category_, is_const_, typename iterator_::reference, typename iterator_::pointer, typename iterator_::difference_type>{
		friend class test_lattice_iterator;
	private:
		using this_type = lattice_iterator<iterator_, 1, iterator_category_, is_const_>;
	public:
		lattice_iterator() = default;
		template<typename iterator_pos, typename iterator_size, typename iterator_step>
		lattice_iterator(iterator_ itr, iterator_pos begin_pos, iterator_pos end_pos, iterator_size begin_size, iterator_size end_size, iterator_step begin_step, iterator_step end_step)
			: basic_lattice_iterator<iterator_, 1>(itr, *begin_pos, *begin_size, *begin_step){
		}
		lattice_iterator(iterator_ itr, std::initializer_list<std::size_t> set_pos, std::initializer_list<std::size_t> set_size, std::initializer_list<std::size_t> set_step)
			: lattice_iterator(itr, std::begin(set_pos), std::end(set_pos), std::begin(set_size), std::end(set_size), std::begin(set_step), std::end(set_step)){
		}
	};
	template<typename iterator_, typename iterator_category_, bool is_const_>
	struct lattice_iterator<iterator_, 0, iterator_category_, is_const_>{};
}
#
#endif
