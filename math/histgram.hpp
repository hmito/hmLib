#ifndef HMLIB_MATH_HISTGRAM_INC
#define HMLIB_MATH_HISTGRAM_INC
#
#include<vector>
#include"axis.hpp"
namespace hmLib {
	template<typename value_type_, typename grid_policy_ = math::grid_policy::round_grid_tag>
	struct histgram {
		using axis_t = axis<value_type_, grid_policy_>;
		using value_type = value_type_;
		using grid_policy = grid_policy_;
	private:
		axis_t Axis;
		std::vector<unsigned int> Vec;
		unsigned int Underflow;
		unsigned int Overflow;
	public:
		using iterator = std::vector<unsigned int>::const_iterator;
	public:
		histgram() = default;
		histgram(value_type Min_, value_type Max_, unsigned int SepNum_):Axis(Min_,Max_,SepNum_){
			clear();
		}
		explicit histgram(axis_t Axis_):Axis(std::move(Axis_)) {
			clear();
		}
		void clear() {
			Vec.assign(Axis.size(), 0u);
			Underflow = 0;
			Overflow = 0;
		}
		void reset(value_type Min_, value_type Max_, unsigned int SepNum_) {
			Axis.assign(Min_, Max_, SepNum_);
			clear();
		}
		void reset(const axis_t& Axis_) {
			Axis = Axis_;
			clear();
		}
	public:
		void operator()(value_type Val) {
			if(Val < Axis.grid_lower())++Underflow;
			else if(Val >= Axis.grid_upper()) ++Overflow;
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
		iterator begin()const { return Vec.begin(); }
		iterator end()const { return Vec.end(); }
		unsigned int underflow()const { return Underflow; }
		unsigned int overflow()const { return Overflow; }
		unsigned int size()const { return Axis.size(); }
		value_type lower()const { return Axis.lower(); }
		value_type upper()const { return Axis.upper(); }
		const axis_t& axis()const { return Axis; }
	};

	template<typename T, typename grid_policy>
	auto make_histgram(T Lower, T Upper, unsigned int Size, grid_policy GridPolicy, math::make_axis_option Opt = math::make_axis_option::none) {
		using value_type = typename std::decay<T>::type;
		return histgram<value_type, grid_policy>(make_axis(Lower,Upper,Size, GridPolicy, Opt));
	}
	template<typename T>
	auto make_histgram(T Lower, T Upper, unsigned int Size, math::make_axis_option Opt = math::make_axis_option::none) {
		return make_histgram(Lower, Upper, Size, math::grid_policy::round_grid, Opt);
	}
}
#
#endif
