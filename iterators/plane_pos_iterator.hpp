#include<iterator>
#include<utility>
#include"../varray.hpp"
namespace hmLib {
	struct manhattan_plane_pos_iterator {
		using point_type = hmLib::varray<int, 2>;
	private:
		using this_type = manhattan_plane_pos_iterator;
	public:
		using value_type = point_type;
		using reference = const point_type &;
		using pointer = const point_type*;
		using difference_type = int;
		using iterator_category = std::input_iterator_tag;
	private:
		point_type Pos;
	public:
		manhattan_plane_pos_iterator() = default;
		manhattan_plane_pos_iterator(const this_type&) = default;
		explicit manhattan_plane_pos_iterator(point_type Pos_) :Pos(std::move(Pos_)) {}
		this_type& operator=(const this_type&) = default;
		this_type& operator++() {
			if (Pos[0] < 0 && Pos[1] >= 0) {
				++Pos[0];
				++Pos[1];
			} else if (Pos[0] >= 0 && Pos[1] > 0) {
				++Pos[0];
				--Pos[1];
			} else if (Pos[0] > 0 && Pos[1] <= 0) {
				--Pos[0];
				--Pos[1];
			} else if (Pos[0] <= 0 && Pos[1] < 0) {
				--Pos[0];
				++Pos[1];
			}

			if (Pos[0] <= 0 && Pos[1] == 0) {
				--Pos[0];
			}
			return *this;
		}
		this_type operator++(int) {
			this_type Ans = *this;
			operator++();
			return Ans;
		}
		reference operator*()const { return Pos; }
		pointer operator->()const { return &Pos; }
		friend bool operator==(const this_type& v1, const this_type& v2) {
			return v1.Pos == v2.Pos;
		}
		friend bool operator!=(const this_type& v1, const this_type& v2) {
			return v1.Pos != v2.Pos;
		}
	};
	struct chebyshev_plane_pos_iterator {
		using point_type = hmLib::varray<int, 2>;
	private:
		using this_type = chebyshev_plane_pos_iterator;
	public:
		using value_type = point_type;
		using reference = const point_type &;
		using pointer = const point_type*;
		using difference_type = int;
		using iterator_category = std::input_iterator_tag;
	private:
		point_type Pos;
	public:
		chebyshev_plane_pos_iterator() = default;
		chebyshev_plane_pos_iterator(const this_type&) = default;
		explicit chebyshev_plane_pos_iterator(point_type Pos_) :Pos(std::move(Pos_)) {}
		this_type& operator=(const this_type&) = default;
		this_type& operator++() {
			if (Pos[0] <= Pos[1] && Pos[1] < -Pos[0]) {
				++Pos[1];
			} else if (-Pos[1] <= Pos[0] && Pos[0] < Pos[1]) {
				++Pos[0];
			} else if (-Pos[0] < Pos[1] && Pos[1] <= Pos[0]) {
				--Pos[1];
			} else if (Pos[1] < Pos[0] && Pos[0] <= -Pos[1]) {
				--Pos[0];
			}

			if (Pos[0] == Pos[1] && Pos[0] <= 0) {
				--Pos[0];
				--Pos[1];
			}
			return *this;
		}
		this_type operator++(int) {
			this_type Ans = *this;
			operator++();
			return Ans;
		}
		reference operator*()const { return Pos; }
		pointer operator->()const { return &Pos; }
		friend bool operator==(const this_type& v1, const this_type& v2) {
			return v1.Pos == v2.Pos;
		}
		friend bool operator!=(const this_type& v1, const this_type& v2) {
			return v1.Pos != v2.Pos;
		}
	};
	struct manhattan_plane_pos_range {
	private:
		using this_type = manhattan_plane_pos_range;
	public:
		using iterator = manhattan_plane_pos_iterator;
		using point_type = typename iterator::point_type;
	private:
		unsigned int Begin;
		unsigned int End;
	public:
		manhattan_plane_pos_range() = default;
		manhattan_plane_pos_range(const this_type&) = default;
		explicit manhattan_plane_pos_range(unsigned int Begin_, unsigned int End_) :Begin(Begin_), End(End_) {}
		explicit manhattan_plane_pos_range(unsigned int Size_) :Begin(0), End(Size_) {}
		iterator begin()const { return iterator(point_type{ -static_cast<int>(Begin),0 }); }
		iterator end()const { return iterator(point_type{ -static_cast<int>(End),0 }); }
	};
	struct chebyshev_plane_pos_range {
	private:
		using this_type = chebyshev_plane_pos_range;
	public:
		using iterator = chebyshev_plane_pos_iterator;
		using point_type = typename iterator::point_type;
	private:
		unsigned int Begin;
		unsigned int End;
	public:
		chebyshev_plane_pos_range() = default;
		chebyshev_plane_pos_range(const this_type&) = default;
		explicit chebyshev_plane_pos_range(unsigned int Begin_, unsigned int End_) :Begin(Begin_), End(End_) {}
		explicit chebyshev_plane_pos_range(unsigned int Size_) :Begin(0), End(Size_) {}
		iterator begin()const { return iterator(point_type{ -static_cast<int>(Begin),-static_cast<int>(Begin) }); }
		iterator end()const { return iterator(point_type{ -static_cast<int>(End),-static_cast<int>(End) }); }
	};
}
