#ifndef HMLIB_ENUMERATORS_ENUMERATORTRAITS_INC
#define HMLIB_ENUMERATORS_ENUMERATORTRAITS_INC 100
#
namespace hmLib{
	template<typename enumerator_tag_, typename value_type_, typename difference_type_ = int, typename reference_ = value_type_&, typename pointer_ = const value_type_&>
	struct enumerator_traits{
	public:
		using enumerator_tag = enumerator_tag_;
		using value_type = value_type_;
		using difference_type = difference_type_;
		using reference = reference_;
		using pointer = pointer_;
	};
}
#
#endif
