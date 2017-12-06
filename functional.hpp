#ifndef HMLIB_FUNCTIONAL_INC
#define HMLIB_FUNCTIONAL_INC 100
#
namespace hmLib{
	template<typename T>
	class function_ptr;

	template<typename ReturnType,typename ...ArgumentTypes>
	class function_ptr<ReturnType(ArgumentTypes...)>{
		using this_type = function_ptr<ReturnType(ArgumentTypes...)>;
		using pointer = ReturnType(*)(ArgumentTypes...);
		pointer Ptr;
	public:
		function_ptr():Ptr(nullptr){}
		function_ptr(const this_type&) = default;
		this_type& operator=(const this_type&) = default;
		function_ptr(this_type&&) = default;
		this_type& operator=(this_type&&) = default;
		function_ptr(pointer Ptr_) :Ptr(Ptr_){}
		this_type& operator=(pointer Ptr_) noexcept { Ptr = Ptr_; return *this; }
		ReturnType operator()(ArgumentTypes... Args){
			return Ptr(Args...);
		}
	};
}
#
#endif
