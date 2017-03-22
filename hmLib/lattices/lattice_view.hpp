#ifndef HMLIB_LATTICES_LATTICEVIEW_INC
#define HMLIB_LATTICES_LATTICEVIEW_INC 100
#
#include<array>
#include<utility>
#include<iterator>
#include"utility.hpp"
#include"exceptions.hpp"
#include"iterator_base.hpp"
#include"lattice_indexer.hpp"
namespace hmLib{
	template<typename iterator_, unsigned int dim_>
	struct lattice_view{
		friend struct lattice_view<iterator_, dim_ + 1>;
		using this_type = lattice_view<iterator_, dim_>;
		using indexer = lattices::lattice_indexer<dim_>;
		using size_type = lattices::size_type;
		using diff_type = lattices::diff_type;
		using index_type = lattices::index_type;
		using value_type = typename iterator_::value_type;
		using reference = typename iterator_::reference;
		using point = lattices::point<dim_>;
		using iterator = lattices::iterator_base<iterator_, this_type, dim_>;
		using raw_iterator = iterator_;
		struct locator{};
	public:
		static constexpr unsigned int dim(){ return dim_; }
	public:
		lattice_view() = default;
		template<typename... others>
		lattice_view(iterator_ Begin_, iterator_ End_, size_type Size_, others... Others_)
			: Size(Size_)
			, Gap(0)
			, Lower(Begin_, End_, Others_...){
			static_assert(dim_ == 1 + sizeof...(others), "arguments is too few or many for this dim.");
			hmLib_assert(std::distance(Begin_, End_) >= lattice_step<0>(), lattices::exception, "Too small range for lattice_view.");
		}
	public:
		template<typename... others>
		index_type raw_index(index_type Pos_, others... Others_)const{
			static_assert(dim_ == 1 + sizeof...(others), "the number of arguments is different from that of dim.");
			return raw_index_template(0, Pos_, Others_...);
		}
		index_type raw_index(const point& Point_)const{
			return raw_index_iterator(0, Point_.begin(), Point_.end());
		}
		template<typename... others>
		reference at(index_type Pos_, others... Others_){
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
		raw_iterator raw_begin(){ return Lower.raw_begin(); }
		raw_iterator raw_end(){ return Lower.raw_end(); }
		iterator begin(){ return make_iterator(*this, 0); }
		iterator end(){ return make_iterator(*this, 1); }
		locator 
	private:
		index_type Size;
		index_type Gap;
		lower_type Lower;
	private:
		template<typename... others>
		index_type raw_index_template(index_type RawPos_, index_type Pos_, others... Others_)const{
			hmLib_assert(Pos_ < Size, lattices::out_of_range_access, "The requested position is out of range.");
			return Lower.raw_index_template(RawPos_ + Pos_*lattice_step<0>(), Others_...);
		}
		template<typename iterator>
		index_type raw_index_iterator(index_type RawPos_, iterator Begin_, iterator End_)const{
			hmLib_assert(*Begin_<  Size, lattices::out_of_range_access, "The requested position is out of range.");
			RawPos_ += (*Begin_)*lattice_step();
			return Lower.raw_index_iterator(RawPos_, ++Begin_, End_);
		}
		template<typename... others>
		reference at_template(index_type RawPos_, index_type Pos_, others... Others_){
			hmLib_assert(Pos_<  Size, lattices::out_of_range_access, "The requested position is out of range.");
			return Lower.at_template(RawPos_ + Pos_*lattice_step<0>(), Others_...);
		}
		template<typename iterator>
		reference at_iterator(index_type RawPos_, iterator Begin_, iterator End_){
			hmLib_assert(*Begin_<  Size, lattices::out_of_range_access, "The requested position is out of range.");
			RawPos_ += (*Begin_)*lattice_step<0>();
			return Lower.at_iterator(RawPos_, ++Begin_, End_);
		}
	private:
		template<unsigned int req_dim_, typename T = void>
		struct size_getter{
			index_type operator()(const this_type& This){ return This.Lower.size<req_dim_ - 1>(); }
		};
		template<typename T>
		struct size_getter<0, T>{
			index_type operator()(const this_type& This){ return This.Size; }
		};
		template<unsigned int req_dim_, typename T = void>
		struct gap_getter{
			index_type operator()(const this_type& This){ return This.Lower.gap<req_dim_ - 1>(); }
		};
		template<typename T>
		struct gap_getter<0, T>{
			index_type operator()(const this_type& This){ return This.Gap; }
		};
		template<unsigned int req_dim_, typename T = void>
		struct lattice_size_getter{
			index_type operator()(const this_type& This){ return This.Lower.lattice_size<req_dim_ - 1>(); }
		};
		template<typename T>
		struct lattice_size_getter<0, T>{
			index_type operator()(const this_type& This){ return This.Size*This.Lower.lattice_size<0>(); }
		};
		template<unsigned int req_dim_, typename T = void>
		struct lattice_step_getter{
			index_type operator()(const this_type& This){ return This.Lower.lattice_step<req_dim_ - 1>(); }
		};
		template<typename T>
		struct lattice_step_getter<0, T>{
			index_type operator()(const this_type& This){ return This.Gap + This.Lower.size<0>()*This.Lower.lattice_step<0>(); }
		};
		template<typename base_type>
		lattices::iterator_base<raw_iterator, base_type, dim_> make_iterator(base_type& Ref_, index_type Sup_){
			return lattices::iterator_base<raw_iterator, base_type, dim_>(Lower.make_iterator(Ref_, Sup_), 0);
		}
	};

	template<typename iterator_, unsigned int dim_>
	struct sublattice_view{};

	template<typename iterator, typename... others>
	auto make_lattice(iterator Begin, iterator End, others... Others)->lattice_view<iterator, sizeof...(others)>{
		return lattice_view<iterator, sizeof...(others)>(Begin, End, Others...);
	}
}
#
#endif
