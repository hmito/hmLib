#ifndef HMLIB_LATTICEVIEW_INC
#define HMLIB_LATTICEVIEW_INC 100
#
#include<array>
#include<utility>
#include<iterator>
#include<hmLib/exceptions.hpp>
#include"exceptions.hpp"
#include"point.hpp"
namespace hmLib{
	namespace lattice_views{
		template<typename base_type, unsigned int dim_>
		struct iterator_pattern{};
		template<typename base_type>
		struct iterator_pattern<base_type, 0>{
		private:
			base_type Base;
		public:

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
			,Gap(0)
			,Lower(Begin_, End_, Others_...){
			static_assert(dim_ == 1 + sizeof...(others), "arguments is too few or many for this dim.");
			hmLib_assert(std::distance(Begin_, End_) >= lattice_step(), lattices::exception, "Too small range for lattice_view.");
		}
		template<typename... others>
		lattice_view(iterator_ Begin_, iterator_ End_, std::pair<difference_type, difference_type> SizeGap, others... Others_)
			:Size(0)
			,Gap(0)
			,Lower(Begin_, End_, Others_...){
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
		difference_type lattice_step()const{ return Gap+Lower.lattice_size()*Lower.lattice_step(); }
	public:
		iterator_ raw_begin(){ return Lower.raw_begin(); }
		iterator_ raw_end(){ return Lower.raw_end(); }
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
			return Lower.at_template(RawPos_+Pos_*lattice_step(), Others_...);
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
	private:
		iterator_ Begin;
		iterator_ End;
	public:
		lattice_view() = default;
		lattice_view(iterator_ Begin_, iterator_ End_):Begin(Begin_),End(End_){}
	public:
		difference_type lattice_size()const{ return 1; }
		difference_type lattice_step()const{ return 1; }
	public:
		iterator_ raw_begin(){ return Begin; }
		iterator_ raw_end(){ return End; }
	private:
		difference_type raw_position_template(difference_type RawPos_)const{ return RawPos_; }
		template<typename iterator>
		difference_type raw_position_iterator(difference_type RawPos_, iterator Begin_, iterator End_)const{ return RawPos_; }
		reference at_template(difference_type RawPos_){return *std::next(Begin, RawPos_);}
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
