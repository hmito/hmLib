#ifndef HMLIB_MATH_HISTGRAM_INC
#define HMLIB_MATH_HISTGRAM_INC
#
#include<vector>
#include"axis.hpp"
namespace hmLib {
	template<typename range_axis_type_>
	struct histgram {
		using range_axis_type = range_axis_type_;
		using value_type = typename range_axis_type::value_type;
		using grid_policy = typename range_axis_type::grid_policy;
	private:
		range_axis_type Axis;
		std::vector<unsigned int> Vec;
		unsigned int Underflow;
		unsigned int Overflow;
	public:
		using iterator = std::vector<unsigned int>::iterator;
		using const_iterator = std::vector<unsigned int>::const_iterator;
	public:
		histgram() = default;
		explicit histgram(range_axis_type Axis_)
			:Axis(std::move(Axis_)),Vec(Axis.size(), 0u), Underflow(0), Overflow(0) {
		}
		void clear() {
			if (Axis.empty())return;
			Vec.assign(Axis.size(), 0u);
			Underflow = 0;
			Overflow = 0;
		}
		void reset(const range_axis_type& Axis_) {
			Axis = Axis_;
			clear();
		}
	public:
		void operator()(value_type Val) {
			if(Val < Axis.lower())++Underflow;
			else if(Val >= Axis.upper()) ++Overflow;
			else ++Vec[Axis.index(Val)];
		}
		template<typename input_iterator>
		void operator()(input_iterator Beg, input_iterator End) {
			for(; Beg != End; ++Beg) {
				operator()(*Beg);
			}
		}
		template<typename input_iterator, typename accessor>
		void operator()(input_iterator Beg, input_iterator End, accessor&& Acs) {
			for(; Beg != End; ++Beg) {
				operator()(Acs(*Beg));
			}
		}
	public:
		unsigned int& operator[](unsigned int n){return Vec[n];}
		const unsigned int& operator[](unsigned int n)const{return Vec[n];}
		unsigned int& at(unsigned int n){return Vec.at(n);}
		const unsigned int& at(unsigned int n)const{return Vec.at(n);}
		unsigned int& underflow() { return Underflow; }
		const unsigned int& underflow()const { return Underflow; }
		unsigned int& overflow() { return Overflow; }
		const unsigned int& overflow()const { return Overflow; }
		iterator begin() { return Vec.begin(); }
		iterator end() { return Vec.end(); }
		const_iterator begin()const { return cbegin(); }
		const_iterator end()const { return cend(); }
		const_iterator cbegin()const { return Vec.begin(); }
		const_iterator cend()const { return Vec.end(); }
		unsigned int size()const { return Vec.size(); }
		const std::vector<unsigned int>& data()&{return Vec;}
		std::vector<unsigned int>&& data()&&{return std::move(Vec);}
		value_type lower()const { return Axis.lower(); }
		value_type upper()const { return Axis.upper(); }
		const range_axis_type& axis()const { return Axis; }
	};
	template<typename T, typename grid_adjuster>
	auto make_histgram(T Lower, T Upper, unsigned int Size, grid_adjuster GridAdjuster, math::range_axis_option Opt = math::range_axis_option::none) {
		auto Axis = make_range_axis(Lower,Upper,Size+1, GridAdjuster, Opt);
		return histgram<decltype(Axis)>(Axis);
	}
}
#
#endif
