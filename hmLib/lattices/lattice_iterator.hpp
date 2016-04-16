#ifndef HMLIB_LATTICEITERATOR_INC
#define HMLIB_LATTICEITERATOR_INC 100
#
#include<iterator>
#include"exceptions.hpp"
#include"point.hpp"
namespace hmLib{
	namespace{
		template<typename iterator_, unsigned int dim_>
		struct basic_lattice_iterator{
			friend struct basic_lattice_iterator<iterator_, dim_ + 1>;
			using iterator_category = std::forward_iterator_tag;
			using lattice_difference_type = typename iterator_::difference_type;
			using this_type = basic_lattice_iterator<iterator_, dim_>;
			using iterator_type = basic_lattice_iterator<iterator_, dim_ - 1>;
			constexpr static unsigned int dim(){ return dim_; }
		private:
			iterator_type Itr;
			lattice_difference_type Pos;
			lattice_difference_type Size;
			lattice_difference_type Gap;
		public:
			basic_lattice_iterator() = default;
			basic_lattice_iterator(iterator_type Itr_, lattice_difference_type Pos_, lattice_difference_type Size_, lattice_difference_type Gap_)
				: Itr(Itr_)
				, Pos(Pos_)
				, Size(Size_)
				, Gap(Gap_){}
		private:
			lattice_difference_type advance_pos(lattice_difference_type& RequestedStep){
				lattice_difference_type RawStep = Itr.advance_pos(RequestedStep);

				lattice_difference_type NewPos = (Pos + RequestedStep) % Size;
				if(NewPos < 0) NewPos += Size;

				RawStep += (NewPos - Pos) * lattice_step();

				if(RequestedStep > 0 && NewPos < Pos){
					RequestedStep = (RequestedStep / Size) + 1;
				} else if(RequestedStep < 0 && NewPos > Pos){
					RequestedStep = (RequestedStep / Size) - 1;
				} else{
					RequestedStep = (RequestedStep / Size);
				}

				Pos = NewPos;

				return RawStep;
			}
			void advance_itr(lattice_difference_type RawStep){ std::advance(Itr.ref(), RawStep); }
		public:
			iterator_& ref(){ return Itr.ref(); }
			const iterator_& ref()const{ return Itr.ref(); }
			template<unsigned int req_dim_>
			lattice_difference_type pos()const{ return pos_getter<req_dim_>()(); }
			template<unsigned int req_dim_>
			lattice_difference_type size()const{ return size_getter<req_dim_>()(); }
			template<unsigned int req_dim_>
			lattice_difference_type gap()const{ return gap_getter<req_dim_>()(); }
			lattice_difference_type lattice_step()const{ return Gap + Itr.lattice_size()*Itr.lattice_step(); }
			lattice_difference_type lattice_size()const{ return Itr.lattice_size()*Size; }
		public:
			void advance(lattice_difference_type Diff){
				auto RawStep = advance_pos(Diff);
				advance_itr(RawStep);
			}
			bool is_equal(const this_type& Other)const{
				if(!Itr.is_equal(Other.Itr))return false;
				return Pos == Other.Pos;
			}
			bool is_less(const this_type& Other)const{
				if(!Itr.is_less(Other.Itr))return false;
				return Pos < Other.Pos;
			}
			lattice_difference_type distance(const this_type& Other)const{
				return (Pos - Other.Pos) * Size + Itr.distance(Other.Itr);
			}
		private:
			template<unsigned int req_dim_, typename T = void>
			struct pos_getter{
				lattice_difference_type operator()(const this_type& This){ return This.Lower.pos<req_dim_ - 1>(); }
			};
			template<typename T>
			struct pos_getter<0, T>{
				lattice_difference_type operator()(const this_type& This){ return This.Pos; }
			};
			template<unsigned int req_dim_, typename T = void>
			struct size_getter{
				lattice_difference_type operator()(const this_type& This){ return This.Lower.size<req_dim_ - 1>(); }
			};
			template<typename T>
			struct size_getter<0, T>{
				lattice_difference_type operator()(const this_type& This){ return This.Size; }
			};
			template<unsigned int req_dim_, typename T = void>
			struct gap_getter{
				lattice_difference_type operator()(const this_type& This){ return This.Lower.gap<req_dim_ - 1>(); }
			};
			template<typename T>
			struct gap_getter<0, T>{
				lattice_difference_type operator()(const this_type& This){ return This.Gap; }
			};
		};
		template<typename iterator_>
		struct basic_lattice_iterator<iterator_, 0>{
			friend struct basic_lattice_iterator<iterator_, 1>;
			using lattice_difference_type = typename iterator_::difference_type;
			using this_type = basic_lattice_iterator<iterator_, 0>;
			using iterator_type = iterator_;
			constexpr static unsigned int dim(){ return 0; }
		private:
			iterator_type Itr;
		public:
			basic_lattice_iterator() = default;
			basic_lattice_iterator(iterator_type Itr_): Itr(Itr_){}
		private:
			lattice_difference_type advance_pos(lattice_difference_type& RequestedStep){return 0;}
			void advance_itr(lattice_difference_type RawStep){ std::advance(Itr, RawStep); }
		public:
			iterator_& ref(){ return Itr; }
			const iterator_& ref()const{ return Itr; }
		public:
			lattice_difference_type lattice_step()const{ return 1; }
			lattice_difference_type lattice_size()const{ return 1; }
			void advance(lattice_difference_type Diff){
				auto RawStep = advance_pos(Diff);
				advance_itr(RawStep);
			}
			bool is_equal(const this_type& Other)const{return true;}
			bool is_less(const this_type& Other)const{ return true; }
			lattice_difference_type distance(const this_type& Other)const{ return 0; }
		};

		template<typename this_type, typename iterator_category_ = this_type::iterator_category, bool is_const_ = std::is_const<this_type::value_type>::value, typename reference = this_type::reference, typename pointer = this_type::pointer, typename difference_type = this_type::difference_type>
		struct lattice_iterator_mixin{};
		template<typename this_type, bool is_const_, typename reference, typename pointer,typename difference_type>
		struct lattice_iterator_mixin<this_type, std::output_iterator_tag, is_const_ ,reference, pointer, difference_type>{
			this_type& operator++(){
				static_cast<this_type*>(this)->advance(1);
				return *static_cast<this_type*>(this);
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
				return static_cast<const this_type*>(this)->ref().operator*();
			}
			pointer operator->(){
				return static_cast<this_type*>(this)->ref().operator->();
			}
			const pointer operator->()const{
				return static_cast<const this_type*>(this)->ref().operator->();
			}
		};
		template<typename this_type, bool is_const_, typename reference, typename pointer, typename difference_type>
		struct lattice_iterator_mixin<this_type, std::input_iterator_tag, is_const_, reference, pointer, difference_type>{
			this_type& operator++(){
				static_cast<this_type*>(this)->advance(1);
				return *static_cast<this_type*>(this);
			}
			this_type operator++(int){
				this_type Prev = *this;
				++(*this);
				return Prev;
			}
			const reference operator*()const{
				return static_cast<const this_type*>(this)->ref().operator*();
			}
			const pointer operator->()const{
				return static_cast<const this_type*>(this)->ref().operator->();
			}
			friend bool operator==(const this_type& val1, const this_type& val2){
				return val1.is_equal(val2);
			}
			friend bool operator!=(const this_type& val1, const this_type& val2){
				return !val1.is_equal(val2);
			}
		};
		template<typename this_type, typename reference, typename pointer, typename difference_type>
		struct lattice_iterator_mixin<this_type, std::forward_iterator_tag, false, reference, pointer, difference_type>
			: public lattice_iterator_mixin<this_type, std::output_iterator_tag, false, reference, pointer, difference_type>{
			friend bool operator==(const this_type& val1, const this_type& val2){
				return val1.is_equal(val2);
			}
			friend bool operator!=(const this_type& val1, const this_type& val2){
				return !val1.is_equal(val2);
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
				return *static_cast<this_type*>(this);
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
				static_cast<this_type*>(this)->advance(pos);
				return static_cast<this_type*>(this);

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
				return val1.distance(val2);
			}
			friend bool operator<(const this_type& val1, const this_type& val2){
				return val1.is_less(val2);
			}
			friend bool operator>(const this_type& val1, const this_type& val2){
				return val2.is_less(val1);
			}
			friend bool operator<=(const this_type& val1, const this_type& val2){
				return val1.is_equal(val2) || val1.is_less(val2);;
			}
			friend bool operator>=(const this_type& val1, const this_type& val2){
				return val1.is_equal(val2) || val2.is_less(val1);;
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
				static_cast<this_type*>(this)->advance(pos);
				return *static_cast<this_type*>(this);

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
				return val1.distance(val2);
			}
			friend bool operator<(const this_type& val1, const this_type& val2){
				return val1.is_less(val2);
			}
			friend bool operator>(const this_type& val1, const this_type& val2){
				return val2.is_less(val1);
			}
			friend bool operator<=(const this_type& val1, const this_type& val2){
				return val1.is_equal(val2) || val1.is_less(val2);;
			}
			friend bool operator>=(const this_type& val1, const this_type& val2){
				return val1.is_equal(val2) || val2.is_less(val1);;
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
		template<typename iterator_pos, typename iterator_size, typename iterator_lattice_step>
		lattice_iterator(iterator_ itr, iterator_pos begin_pos, iterator_pos end_pos, iterator_size begin_size, iterator_size end_size, iterator_lattice_step begin_lattice_step, iterator_lattice_step end_lattice_step)
			: basic_lattice_iterator<iterator_, dim_>(lattice_iterator<iterator_, dim_-1>(itr, begin_pos+1, end_pos, begin_size + 1, end_size, begin_lattice_step + 1, end_lattice_step), *begin_pos, *begin_size, *begin_lattice_step){
		}
//		template<typename container_pos, typename container_size, typename container_lattice_step>
//		lattice_iterator(iterator_ itr, const container_pos& set_pos, const container_size& set_size, const container_lattice_step& set_lattice_step)
//			: lattice_iterator(itr, std::begin(set_pos), std::end(set_pos), std::begin(set_size), std::end(set_size), std::begin(set_lattice_step), std::end(set_lattice_step)){
//		}
		lattice_iterator(iterator_ itr, std::initializer_list<difference_type> set_pos, std::initializer_list<difference_type> set_size, std::initializer_list<difference_type> set_lattice_step)
			: lattice_iterator(itr, std::begin(set_pos), std::end(set_pos), std::begin(set_size), std::end(set_size), std::begin(set_lattice_step), std::end(set_lattice_step)){
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
		template<typename iterator_pos, typename iterator_size, typename iterator_lattice_step>
		lattice_iterator(iterator_ itr, iterator_pos begin_pos, iterator_pos end_pos, iterator_size begin_size, iterator_size end_size, iterator_lattice_step begin_lattice_step, iterator_lattice_step end_lattice_step)
			: basic_lattice_iterator<iterator_, 1>(itr, *begin_pos, *begin_size, *begin_lattice_step){
		}
		lattice_iterator(iterator_ itr, std::initializer_list<difference_type> set_pos, std::initializer_list<difference_type> set_size, std::initializer_list<difference_type> set_lattice_step)
			: lattice_iterator(itr, std::begin(set_pos), std::end(set_pos), std::begin(set_size), std::end(set_size), std::begin(set_lattice_step), std::end(set_lattice_step)){
		}
		lattice_iterator(iterator_ itr, difference_type Pos_, difference_type Size_, difference_type Gap_)
			: basic_lattice_iterator<iterator_, 1>(itr, Pos_, Size_, Gap_){
		}
	};
	template<typename iterator_, typename iterator_category_, bool is_const_>
	struct lattice_iterator<iterator_, 0, iterator_category_, is_const_>{};
}
#
#endif
