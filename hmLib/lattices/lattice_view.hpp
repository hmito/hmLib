#ifndef HMLIB_LATTICEVIEW_INC
#define HMLIB_LATTICEVIEW_INC 100
#
#include<array>
#include<utility>
#include<iterator>
#include<hmLib/exceptions.hpp>
namespace hmLib{
	namespace lattices{
		using point_type = int;
		using difference_type = int;

		struct exception_tag{};
		struct out_of_dim_exception_tag{};
		using exception = hmLib::exceptions::exception_pattern<exception_tag>;
		using out_of_range_access = hmLib::exceptions::access::out_of_range_access<exception>;
		using out_of_dim = hmLib::exceptions::exception_pattern<out_of_dim_exception_tag, exception>;

		template<unsigned int dim_>
		using point = std::array<int, dim_>;
		template<typename ...others>
		auto make_point(others... Others)->point<sizeof...(others)>{
			return point<sizeof...(others)>{Others...};
		}

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
			iterator_base(lower_type Lower_, point_type Pos_) :Lower(Lower_), Pos(Pos_){}
			template<typename... others>
			iterator_base(raw_iterator Itr, base_type& Ref_, point_type Pos_, others... Others)
				: Pos(Pos_)
				, Lower(Itr, Ref_, Others...){}
			template<typename point_iterator>
			iterator_base(raw_iterator Itr, base_type& Ref_, point_iterator Begin, point_iterator End)
				: Pos()
				, Lower(Itr, Ref_, Others...){}
		public:
			point_type sup()const{ return Lower.sup(); }
			raw_iterator& ref(){ return Lower.ref(); }
			template<unsigned int req_dim_>
			point_type pos()const{
				static_assert(req_dim_ < dim_, "requested dim is larger than lattice's dim.");
				return pos_getter<req_dim_>()();
			}
			template<unsigned int req_dim_>
			point_type size()const{
				return Lower.size<req_dim_>();
			}
			template<unsigned int req_dim_>
			point_type gap()const{
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
			point_type Pos;
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
			void advance(point_type Diff){
				auto RawStep = advance_pos(Diff);
				advance_itr(RawStep + Diff*(gap<0>() + size<0>()*lattice_step<0>()), Diff);
			}
			point_type advance_pos(point_type& RequestedStep){
				point_type RawStep = Lower.advance_pos(RequestedStep);

				point_type NewPos = (Pos + RequestedStep) % size<this_dim()>();
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
			void advance_itr(point_type RawStep, point_type ReaminStep){ Lower.advance_itr(RawStep, ReaminStep); }
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
		public:
			iterator_base() :Itr(), Ptr(nullptr), Sup(0), Raw(0){}
			iterator_base(raw_iterator Itr_, base_type& Ref_, point_type Sup_, point_type Raw_)
				: Itr(Itr_)
				, Ptr(&Ref_)
				, Sup(Sup_)
				, Raw(Raw_){}
		public:
			point_type sup()const{ return Sup; }
			raw_iterator& ref(){
				std::advance(Itr, Raw);
				Raw = 0;
				return Itr;
			}
			template<unsigned int req_dim_>
			point_type size()const{
				return Ptr->size<req_dim_>();
			}
			template<unsigned int req_dim_>
			point_type gap()const{
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
			point_type Sup;
			point_type Raw;
		private:
			point_type advance_pos(point_type& RequestedStep){
				return 0;
			}
			void advance_itr(point_type RawStep, point_type RemainStep){
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

	template<typename iterator_, unsigned int dim_>
	struct lattice_view{
		friend struct lattice_view<iterator_, dim_ + 1>;
		using this_type = lattice_view<iterator_, dim_>;
		using lower_type = lattice_view<iterator_, dim_ - 1>;
		using point_type = lattices::point_type;
		using difference_type = lattices::difference_type;
		using value_type = typename iterator_::value_type;
		using reference = typename iterator_::reference;
		using point = lattices::point<dim_>;
		using iterator = lattices::iterator_base<iterator_, this_type, dim_>;
		using raw_iterator = iterator_;
	public:
		static constexpr unsigned int dim(){ return dim_; }
	public:
		lattice_view() = default;
		template<typename... others>
		lattice_view(iterator_ Begin_, iterator_ End_, difference_type Size_, others... Others_)
			:Size(Size_)
			, Gap(0)
			, Lower(Begin_, End_, Others_...){
			static_assert(dim_ == 1 + sizeof...(others), "arguments is too few or many for this dim.");
			hmLib_assert(std::distance(Begin_, End_) >= lattice_step<0>(), lattices::exception, "Too small range for lattice_view.");
		}
		template<typename... others>
		lattice_view(iterator_ Begin_, iterator_ End_, std::pair<difference_type, difference_type> SizeGap, others... Others_)
			:Size(0)
			, Gap(0)
			, Lower(Begin_, End_, Others_...){
			static_assert(dim_ == 1 + sizeof...(others), "arguments is too few or many for this dim.");
			Size = SizeGap.first;
			Gap = SizeGap.second;
			hmLib_assert(std::distance(Begin_, End_) >= lattice_step<0>(), lattices::exception, "Too small range for lattice_view.");
		}
	public:
		template<typename... others>
		difference_type raw_position(difference_type Pos_, others... Others_)const{
			static_assert(dim_ == 1 + sizeof...(others), "the number of arguments is different from that of dim.");
			return raw_position_template(0, Pos_, Others_...);
		}
		difference_type raw_position(const point& Point_)const{
			return raw_position_iterator(0, Point_.begin(), Point_.end());
		}
		template<typename... others>
		reference at(difference_type Pos_, others... Others_){
			static_assert(dim_ == 1 + sizeof...(others), "arguments is too few or many for this dim.");
			return at_template(0, Pos_, Others_...);
		}
		reference at(const point& Point_){
			return at_iterator(0, Point_.begin(), Point_.end());
		}
		reference operator[](const point& Point_){
			return at_iterator(0, Point_.begin(), Point_.end());
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
		template<unsigned int req_dim_>
		difference_type lattice_size()const{
			static_assert(req_dim_ <= dim_, "requested dim is larger than lattice's dim.");
			return lattice_size_getter<req_dim_>()(*this);
		}
		template<unsigned int req_dim_>
		difference_type lattice_step()const{
			static_assert(req_dim_ <= dim_, "requested dim is larger than lattice's dim.");
			return lattice_step_getter<req_dim_>()(*this);
		}
	public:
		raw_iterator raw_begin(){ return Lower.raw_begin(); }
		raw_iterator raw_end(){ return Lower.raw_end(); }
		iterator begin(){ return make_iterator(*this, 0); }
		iterator end(){ return make_iterator(*this, 1); }
	private:
		difference_type Size;
		difference_type Gap;
		lower_type Lower;
	private:
		template<typename... others>
		difference_type raw_position_template(difference_type RawPos_, difference_type Pos_, others... Others_)const{
			hmLib_assert(Pos_ < Size, lattices::out_of_range_access, "The requested position is out of range.");
			return Lower.raw_position_template(RawPos_ + Pos_*lattice_step<0>(), Others_...);
		}
		template<typename iterator>
		difference_type raw_position_iterator(difference_type RawPos_, iterator Begin_, iterator End_)const{
			hmLib_assert(*Begin_<  Size, lattices::out_of_range_access, "The requested position is out of range.");
			RawPos_ += (*Begin_)*lattice_step();
			return Lower.raw_position_iterator(RawPos_, ++Begin_, End_);
		}
		template<typename... others>
		reference at_template(difference_type RawPos_, difference_type Pos_, others... Others_){
			hmLib_assert(Pos_<  Size, lattices::out_of_range_access, "The requested position is out of range.");
			return Lower.at_template(RawPos_ + Pos_*lattice_step<0>(), Others_...);
		}
		template<typename iterator>
		reference at_iterator(difference_type RawPos_, iterator Begin_, iterator End_){
			hmLib_assert(*Begin_<  Size, lattices::out_of_range_access, "The requested position is out of range.");
			RawPos_ += (*Begin_)*lattice_step<0>();
			return Lower.at_iterator(RawPos_, ++Begin_, End_);
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
		template<unsigned int req_dim_, typename T = void>
		struct lattice_size_getter{
			difference_type operator()(const this_type& This){ return This.Lower.lattice_size<req_dim_ - 1>(); }
		};
		template<typename T>
		struct lattice_size_getter<0, T>{
			difference_type operator()(const this_type& This){ return This.Size*This.Lower.lattice_size<0>(); }
		};
		template<unsigned int req_dim_, typename T = void>
		struct lattice_step_getter{
			difference_type operator()(const this_type& This){ return This.Lower.lattice_step<req_dim_ - 1>(); }
		};
		template<typename T>
		struct lattice_step_getter<0, T>{
			difference_type operator()(const this_type& This){ return This.Gap + This.Lower.size<0>()*This.Lower.lattice_step<0>(); }
		};
		template<typename base_type>
		lattices::iterator_base<raw_iterator, base_type, dim_> make_iterator(base_type& Ref_, point_type Sup_){
			return lattices::iterator_base<raw_iterator, base_type, dim_>(Lower.make_iterator(Ref_, Sup_), 0);
		}
	};
	template<typename iterator_>
	struct lattice_view<iterator_, 0>{
		friend struct lattice_view<iterator_, 1>;
		using this_type = lattice_view<iterator_, 0>;
		using point_type = lattices::point_type;
		using difference_type = lattices::difference_type;
		using value_type = typename iterator_::value_type;
		using reference = typename iterator_::reference;
		using point = lattices::point<0>;
		using iterator = lattices::iterator_base<iterator_, this_type, 0>;
		using raw_iterator = iterator_;
	public:
		lattice_view() = default;
		lattice_view(raw_iterator Begin_, raw_iterator End_) :Begin(Begin_), End(End_){}
	public:
		template<unsigned int req_dim_>
		difference_type size()const{ return 1; }
		template<unsigned int req_dim_>
		difference_type lattice_size()const{ return 1; }
		template<unsigned int req_dim_>
		difference_type lattice_step()const{ return 1; }
	public:
		raw_iterator raw_begin(){ return Begin; }
		raw_iterator raw_end(){ return End; }
	private:
		raw_iterator Begin;
		raw_iterator End;
	private:
		difference_type raw_position_template(difference_type RawPos_)const{ return RawPos_; }
		template<typename iterator>
		difference_type raw_position_iterator(difference_type RawPos_, iterator Begin_, iterator End_)const{ return RawPos_; }
		reference at_template(difference_type RawPos_){ return *std::next(Begin, RawPos_); }
		template<typename iterator>
		reference at_iterator(difference_type RawPos_, iterator Begin_, iterator End_){ return *std::next(Begin, RawPos_); }
		template<typename base_type>
		lattices::iterator_base<raw_iterator, base_type, 0> make_iterator(base_type& Ref_, point_type Sup_){
			return lattices::iterator_base<raw_iterator, base_type, 0>(Begin, Ref_, Sup_, (Ref_.gap<0>() + Ref_.size<0>()*Ref_.lattice_step<0>())*Sup_);
		}
	};

	template<typename iterator, typename... others>
	auto make_lattice(iterator Begin, iterator End, others... Others)->lattice_view<iterator, sizeof...(others)>{
		return lattice_view<iterator, sizeof...(others)>(Begin, End, Others...);
	}
}
#
#endif
