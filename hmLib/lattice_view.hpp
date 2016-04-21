#ifndef HMLIB_LATTICEVIEW_INC
#define HMLIB_LATTICEVIEW_INC 100
#
#include<array>
#include<utility>
#include<iterator>
#include<hmLib/exceptions.hpp>
namespace hmLib{
	namespace lattice_views{
		using point_type = int;

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
			using this_type = iterator_pattern<iterator_, base_, dim_>;
			using lower_type = iterator_pattern<iterator_, base_, dim_ - 1>;
			using raw_iterator = iterator_;
			using base_type = base_;
		public:
			iterator_base() = default;
			template<typename... others>
			iterator_base(raw_iterator Itr, base_type& Ref_, point_type Pos_, others... Others)
				: Pos(Pos_)
				, Lower(Itr, Ref_, Others...){
			}
			template<typename point_iterator>
			iterator_base(raw_iterator Itr, base_type& Ref_, point_iterator Begin, point_iterator End)
				: Pos()
				, Lower(Itr, Ref_, Others...){
			}
		public:
			point_type sup()const{ return Lower.sup(); }
			raw_iterator& ref(){ return Lower.ref(); }
			template<unsigned int req_dim_>
			point_type pos()const{
				static_assert(req_dim_ < dim_, "requested dim is larger than the dim");
				return pos_getter<req_dim_>()(); 
			}
			template<unsigned int req_dim_>
			point_type size()const{
				static_assert(req_dim_ < dim_, "requested dim is larger than the dim");
				return Lower.size<req_dim_>(); 
			}
			template<unsigned int req_dim_>
			point_type gap()const{
				static_assert(req_dim_ < dim_, "requested dim is larger than the dim");
				return Lower.size<req_dim_>();
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
			this_tyoe& operator++(){
				advance(1);
				return *this;
			}
			this_type operator++(int){
				this_type pre = *this;
				advance(1);
				return pre;
			}
			this_tyoe& operator--(){
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
				return val1.sup() < val2.sup() || (val1.sup() == val2.sup() && val1.is_lower(val2));
			}
			friend bool operator>(const this_type& val1, const this_type& val2){
				return val1.sup() > val2.sup() || (val1.sup() == val2.sup() && val2.is_lower(val1));
			}
			friend bool operator<=(const this_type& val1, const this_type& val2){
				return val1.sup() < val2.sup() || (val1.sup() == val2.sup() && val1.is_lower_or_equal(val2));
			}
			friend bool operator<=(const this_type& val1, const this_type& val2){
				return val1.sup() > val2.sup() || (val1.sup() == val2.sup() && val2.is_lower_or_equal(val1));
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
				advance_itr(RawStep, Diff);
			}
			point_type advance_pos(point_type& RequestedStep){
				point_type RawStep = Itr.advance_pos(RequestedStep);

				point_type NewPos = (Pos + RequestedStep) % Size;
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
			void advance_itr(point_type RawStep, point_type ReaminStep){ Lower.advance_itr(Itr.ref(), RawStep); }
			bool is_equal(const this_type& Other)const{ return Pos == Other.Pos && Lowrer.is_equal(Other.Lower); }
			bool is_less(const this_type& Other)const{ return Pos == Other.Pos && Lowrer.is_equal(Other.Lower); }
			bool is_less_or_equal(const this_type& Other)const{ return Pos == Other.Pos && Lowrer.is_equal(Other.Lower); }
			difference_type distance_from(const this_type& Other)const{
				return (Pos - Other.Pos) * size<0>() + Itr.distance_from(Other.Itr);
			}

		};
		template<typename iterator_, typename base_>
		struct iterator_pattern<iterator_, base_, 0>{
			friend struct iterator_pattern<iterator_, base_, 1>;
			using this_type = iterator_pattern<iterator_, base_, 0>;
			using raw_iterator = iterator_;
			using base_type = base_;
		private:
			raw_iterator Itr;
			base_type* Ptr;
			point_type Sup;
		public:
			point_type sup()const{ return Sup; }
			raw_iterator& ref(){ return Itr; }
		private:
			point_type advance_pos(point_type& RequestedStep){
				point_type RawStep = 0;

				point_type NewPos = (Pos + RequestedStep) % Size;
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
			void advance_itr(point_type RawStep, point_type ReaminStep){ 
				std::advance(Itr.ref(), RawStep);
				Sup += RemainStep;
			}
			bool is_equal(const this_type& Other)const{ return true; }
			bool is_less(const this_type& Other)const{ return false; }
			bool is_less_or_equal(const this_type& Other)const{ return true; }
		};
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
//		using iterator = 
		using raw_iterator = iterator_;
	public:
		static constexpr unsigned int dim(){ return dim_; }
	private:
		difference_type Size;
		difference_type Gap;
		lower_type Lower;
	public:
		lattice_view() = default;
		template<typename... others>
		lattice_view(iterator_ Begin_, iterator_ End_, difference_type Size_, others... Others_)
			:Size(Size_)
			, Gap(0)
			, Lower(Begin_, End_, Others_...){
			static_assert(dim_ == 1 + sizeof...(others), "arguments is too few or many for this dim.");
			hmLib_assert(std::distance(Begin_, End_) >= lattice_step(), lattices::exception, "Too small range for lattice_view.");
		}
		template<typename... others>
		lattice_view(iterator_ Begin_, iterator_ End_, std::pair<difference_type, difference_type> SizeGap, others... Others_)
			:Size(0)
			, Gap(0)
			, Lower(Begin_, End_, Others_...){
			static_assert(dim_ == 1 + sizeof...(others), "arguments is too few or many for this dim.");
			Size = SizeGap.first;
			Gap = SizeGap.second;
			hmLib_assert(std::distance(Begin_, End_) >= lattice_step(), lattices::exception, "Too small range for lattice_view.");
		}
	public:
		template<typename... others>
		difference_type raw_position(difference_type Pos_, others... Others_)const{
			static_assert(dim_ == 1 + sizeof...(others), "the number of arguments is different from that of dim.");
			return raw_position_template(0, Pos_, Others_...);
		}
		difference_type raw_position(const point_type& Point_)const{
			return raw_position_iterator(0, Point_.begin(), Point_.end());
		}
		template<typename... others>
		reference at(difference_type Pos_, others... Others_){
			static_assert(dim_ == 1 + sizeof...(others), "arguments is too few or many for this dim.");
			return at_template(0, Pos_, Others_...);
		}
		reference at(const point_type& Point_){
			return at_iterator(0, Point_.begin(), Point_.end());
		}
		reference operator[](const point_type& Point_){
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
		difference_type lattice_size()const{ return Size*Lower.lattice_size(); }
		difference_type lattice_step()const{ return Gap + Lower.lattice_size()*Lower.lattice_step(); }
	public:
		raw_iterator raw_begin(){ return Lower.raw_begin(); }
		raw_iterator raw_end(){ return Lower.raw_end(); }
	private:
		template<typename... others>
		difference_type raw_position_template(difference_type RawPos_, difference_type Pos_, others... Others_)const{
			hmLib_assert(Pos_ < Size, lattices::out_of_range_access, "The requested position is out of range.");
			return Lower.raw_position_template(RawPos_ + Pos_*lattice_step(), Others_...);
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
			return Lower.at_template(RawPos_ + Pos_*lattice_step(), Others_...);
		}
		template<typename iterator>
		reference at_iterator(difference_type RawPos_, iterator Begin_, iterator End_){
			hmLib_assert(*Begin_<  Size, lattices::out_of_range_access, "The requested position is out of range.");
			RawPos_ += (*Begin_)*lattice_step();
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
	};
	template<typename iterator_>
	struct lattice_view<iterator_, 0>{
		friend struct lattice_view<iterator_, 1>;
		using this_type = lattice_view<iterator_, 0>;
		using difference_type = int;
		using value_type = typename iterator_::value_type;
		using reference = typename iterator_::reference;
		using raw_iterator = iterator_;
	private:
		raw_iterator Begin;
		raw_iterator End;
	public:
		lattice_view() = default;
		lattice_view(raw_iterator Begin_, raw_iterator End_) :Begin(Begin_), End(End_){}
	public:
		difference_type lattice_size()const{ return 1; }
		difference_type lattice_step()const{ return 1; }
	public:
		raw_iterator raw_begin(){ return Begin; }
		raw_iterator raw_end(){ return End; }
	private:
		difference_type raw_position_template(difference_type RawPos_)const{ return RawPos_; }
		template<typename iterator>
		difference_type raw_position_iterator(difference_type RawPos_, iterator Begin_, iterator End_)const{ return RawPos_; }
		reference at_template(difference_type RawPos_){ return *std::next(Begin, RawPos_); }
		template<typename iterator>
		reference at_iterator(difference_type RawPos_, iterator Begin_, iterator End_){ return *std::next(Begin, RawPos_); }
	};

	template<typename iterator, typename... others>
	auto make_lattice(iterator Begin, iterator End, others... Others)->lattice_view<iterator, sizeof...(others)>{
		return lattice_view<iterator, sizeof...(others)>(Begin, End, Others...);
	}
}
#
#endif
