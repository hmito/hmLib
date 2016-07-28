#ifndef HMLIB_LATTICES_LATTICEVIEW_INC
#define HMLIB_LATTICES_LATTICEVIEW_INC 100
#
#include<array>
#include<utility>
#include<iterator>
#include"../exceptions.hpp"
#include"point.hpp"
#include"exceptions.hpp"
#include"iterator_base.hpp"
namespace hmLib{
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
