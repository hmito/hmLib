#ifndef HMLIB_LATTICES_ITERATORBASE_INC
#define HMLIB_LATTICES_ITERATORBASE_INC 100
#
#include<iterator>
#include"exceptions.hpp"
namespace hmLib{
	namespace lattices{
		template<typename iterator_, typename base_, unsigned int dim_>
		struct iterator_base
			: public std::iterator<typename iterator_::iterator_category, typename iterator_::value_type, typename iterator_::difference_type, typename iterator_::pointer, typename iterator_::reference>{
			friend struct iterator_base<iterator_, base_, dim_ + 1>;
			using this_type = iterator_base<iterator_, base_, dim_>;
			using lower_type = iterator_base<iterator_, base_, dim_ - 1>;
			using raw_iterator = iterator_;
			using base_type = base_;
		public:
			static constexpr unsigned int dim(){ return dim_; }
			static constexpr unsigned int this_dim(){ return base_::dim() - dim_; }
		public:
			iterator_base() = default;
			iterator_base(lower_type Lower_, index_type Pos_) :Lower(Lower_), Pos(Pos_){}
			template<typename... others>
			iterator_base(raw_iterator Itr, base_type& Ref_, index_type Pos_, others... Others)
				: Pos(Pos_)
				, Lower(Itr, Ref_, Others...){}
			template<typename point_iterator>
			iterator_base(raw_iterator Itr, base_type& Ref_, point_iterator Begin, point_iterator End)
				: Pos()
				, Lower(Itr, Ref_, Others...){}
		public:
			index_type sup()const{ return Lower.sup(); }
			raw_iterator& ref(){ return Lower.ref(); }
			template<unsigned int req_dim_>
			index_type pos()const{
				static_assert(req_dim_ < dim_, "requested dim is larger than lattice's dim.");
				return pos_getter<req_dim_>()();
			}
			template<unsigned int req_dim_>
			index_type size()const{
				return Lower.size<req_dim_>();
			}
			template<unsigned int req_dim_>
			index_type gap()const{
				return Lower.gap<req_dim_>();
			}
			template<unsigned int req_dim_>
			difference_type lattice_size()const{
				return Lower.lattice_size<req_dim_>();
			}
			template<unsigned int req_dim_>
			difference_type lattice_step()const{
				return Lower.lattice_step<req_dim_>();
			}
		public:
			reference operator*(){
				return ref().operator*();
			}
			pointer operator->(){
				return ref().operator->();
			}
			reference operator[](difference_type pos){
				this_type other = *this;
				other += pos;
				return *other;
			}
			this_type& operator++(){
				advance(1);
				return *this;
			}
			this_type operator++(int){
				this_type pre = *this;
				advance(1);
				return pre;
			}
			this_type& operator--(){
				advance(-1);
				return *this;
			}
			this_type operator--(int){
				this_type pre = *this;
				advance(-1);
				return pre;
			}
			this_type& operator+=(difference_type pos){
				advance(pos);
				return *this;
			}
			this_type& operator-=(difference_type pos){
				advance(-pos);
				return *this;
			}
			friend this_type operator+(const this_type& val, difference_type pos){
				this_type other = val;
				other += pos;
				return other;
			}
			friend this_type operator-(const this_type& val, difference_type pos){
				this_type other = val;
				other -= pos;
				return other;
			}
			friend this_type operator+(difference_type pos, const this_type& val){
				this_type other = val;
				other += pos;
				return other;
			}
			friend difference_type operator-(const this_type& val1, const this_type& val2){
				return val1.distance_from(val2);
			}
			friend bool operator==(const this_type& val1, const this_type& val2){
				return val1.sup() == val2.sup() && val1.is_equal(val2);
			}
			friend bool operator!=(const this_type& val1, const this_type& val2){
				return !(val1.sup() == val2.sup() && val1.is_equal(val2));
			}
			friend bool operator<(const this_type& val1, const this_type& val2){
				return val1.sup() < val2.sup() || (val1.sup() == val2.sup() && val1.is_less(val2));
			}
			friend bool operator>(const this_type& val1, const this_type& val2){
				return val1.sup() > val2.sup() || (val1.sup() == val2.sup() && val2.is_less(val1));
			}
			friend bool operator<=(const this_type& val1, const this_type& val2){
				return val1.sup() < val2.sup() || (val1.sup() == val2.sup() && val1.is_less_or_equal(val2));
			}
			friend bool operator>=(const this_type& val1, const this_type& val2){
				return val1.sup() > val2.sup() || (val1.sup() == val2.sup() && val2.is_less_or_equal(val1));
			}
		private:
			index_type Pos;
			lower_type Lower;
		private:
			template<unsigned int req_dim_, typename T = void>
			struct pos_getter{
				difference_type operator()(const this_type& This){ return This.Lower.pos<req_dim_ - 1>(); }
			};
			template<typename T>
			struct pos_getter<0, T>{
				difference_type operator()(const this_type& This){ return This.Pos; }
			};
		private:
			void advance(index_type Diff){
				auto RawStep = advance_pos(Diff);
				advance_itr(RawStep + Diff*(gap<0>() + size<0>()*lattice_step<0>()), Diff);
			}
			index_type advance_pos(index_type& RequestedStep){
				index_type RawStep = Lower.advance_pos(RequestedStep);

				index_type NewPos = (Pos + RequestedStep) % size<this_dim()>();
				if(NewPos < 0) NewPos += size<this_dim()>();

				RawStep += (NewPos - Pos) * lattice_step<this_dim()>();

				if(RequestedStep > 0 && NewPos < Pos){
					RequestedStep = (RequestedStep / size<this_dim()>()) + 1;
				} else if(RequestedStep < 0 && NewPos > Pos){
					RequestedStep = (RequestedStep / size<this_dim()>()) - 1;
				} else{
					RequestedStep = (RequestedStep / size<this_dim()>());
				}

				Pos = NewPos;

				return RawStep;
			}
			void advance_itr(index_type RawStep, index_type ReaminStep){ Lower.advance_itr(RawStep, ReaminStep); }
			bool is_equal(const this_type& Other)const{ return Pos == Other.Pos && Lower.is_equal(Other.Lower); }
			bool is_less(const this_type& Other)const{ return Pos < Other.Pos || (Pos == Other.Pos && Lower.is_less(Other.Lower)); }
			bool is_less_or_equal(const this_type& Other)const{ return Pos < Other.Pos || (Pos == Other.Pos && Lower.is_less_or_equal(Other.Lower)); }
			difference_type distance_from(const this_type& Other)const{
				return (Pos - Other.Pos) * lattice_size<this_dim() + 1>() + Lower.distance_from(Other.Lower);
			}
		};
		template<typename iterator_, typename base_>
		struct iterator_base<iterator_, base_, 0>{
			friend struct iterator_base<iterator_, base_, 1>;
			using this_type = iterator_base<iterator_, base_, 0>;
			using raw_iterator = iterator_;
			using base_type = base_;
			using difference_type = int;
		public:
			iterator_base() :Itr(), Ptr(nullptr), Sup(0), Raw(0){}
			iterator_base(raw_iterator Itr_, base_type& Ref_, index_type Sup_, index_type Raw_)
				: Itr(Itr_)
				, Ptr(&Ref_)
				, Sup(Sup_)
				, Raw(Raw_){}
		public:
			index_type sup()const{ return Sup; }
			raw_iterator& ref(){
				std::advance(Itr, Raw);
				Raw = 0;
				return Itr;
			}
			template<unsigned int req_dim_>
			index_type size()const{
				return Ptr->size<req_dim_>();
			}
			template<unsigned int req_dim_>
			index_type gap()const{
				return Ptr->gap<req_dim_>();
			}
			template<unsigned int req_dim_>
			difference_type lattice_size()const{
				return Ptr->lattice_size<req_dim_>();
			}
			template<unsigned int req_dim_>
			difference_type lattice_step()const{
				return Ptr->lattice_step<req_dim_>();
			}
		private:
			raw_iterator Itr;
			base_type* Ptr;
			index_type Sup;
			index_type Raw;
		private:
			index_type advance_pos(index_type& RequestedStep){
				return 0;
			}
			void advance_itr(index_type RawStep, index_type RemainStep){
				Raw += RawStep;
				Sup += RemainStep;
			}
			bool is_equal(const this_type& Other)const{ return true; }
			bool is_less(const this_type& Other)const{ return false; }
			bool is_less_or_equal(const this_type& Other)const{ return true; }
			difference_type distance_from(const this_type& Other)const{
				return (Sup - Other.Sup) * Ptr->lattice_size<0>();
			}
		};
	}
}
#
#endif
