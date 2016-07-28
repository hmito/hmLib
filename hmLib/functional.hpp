#ifndef HMLIB_FUNCTIONAL_INC
#define HMLIB_FUNCTIONAL_INC 100
#
namespace hmLib{
	template<typename T>
	class function_ptr;

	template<typename ReturnType,typename ...ArgumentTypes>
	class function_ptr<ReturnType(ArgumentTypes...)>{
		using pointer = ReturnType(*)(ArgumentTypes...);
		pointer Ptr;
	public:
		function_ptr():functional_ptr(nullptr){}
		function_ptr(pointer Ptr_) :Ptr(Ptr_){}
		ReturnType operator()(ArgumentTypes... Args){
			return Ptr(Args...);
		}
	};
}
#
#endif
