#ifndef HMLIB_UNIQUERESOURCE_INC
#define HMLIB_UNIQUERESOURCE_INC 100
#include<boost/optional.hpp>
#include"exceptions.hpp"
#
namespace hmLib{
	namespace{
		struct resource_exception_identifier{};
	}
	using resrouce_exepction = exceptions::exception_pattern<resource_exception_identifier>;
	using resource_nullptr_dereference = exceptions::access::nullptr_dereference<resrouce_exepction>;

	template<typename value_type_, typename deleter_>
	struct unique_resource{
	public:
		using this_type = unique_resource<value_type_, deleter_>;
		using value_type = value_type_;
		using optvalue_type = boost::optional<value_type>;
		using deleter = deleter_;
	private:
		optvalue_type OptValue;
		deleter Deleter;
	public:
		unique_resource() = default;
		unique_resource(value_type Value_, deleter Deleter_ = deleter())
			: OptValue(std::move(Value_))
			, Deleter(std::move(Deleter_)){}
		unique_resource(const this_type&) = delete;
		this_type& operator = (const this_type&) = delete;
		unique_resource(this_type&& Other_)noexcept{
			swap(Other_);
		}
		this_type& operator = (this_type&& Other_)noexcept{
			if(this != &Other_){
				swap(Other_);
			}

			return *this;
		}
		~unique_resource(){ reset(); }
	public:
		operator bool()noexcept{ return static_cast<bool>(OptValue); }
		void reset(){
			if(OptValue){
				Deleter(*OptValue);
				OptValue.reset();
			}
		}
		void reset(value_type Value_, deleter Deleter_ = deleter()){
			if(OptValue){
				Deleter(*OptValue);
			}

			OptValue.emplace(std::move(Value_));
			Deleter = std::move(Deleter_);
		}
		value_type release(){
			if(!OptValue)return value_type();
			value_type Ans = std::move(*OptValue);
			OptValue.reset();
			return std::move(Ans);
		}
		void swap(this_type& Other_){
			OptValue.swap(Other_.OptValue);
			std::swap(Deleter, Other_.Deleter);
		}
		const value_type& get()const{
			hmLib_assert(OptValue, resource_nullptr_dereference, "Accecced unique_resource hold no resource.");
			return *OptValue;
		}
		const value_type& operator*()const noexcept{ return *OptValue; }
		const value_type* operator->()const noexcept{ return &(*OptValue); }
	public:
		friend bool operator==(const this_type& v1, const this_type& v2){
			return v1.ref() == v2.ref();
		}
		friend bool operator!=(const this_type& v1, const this_type& v2){
			return v1.ref() != v2.ref();
		}
	};
	template<typename value_type_, typename deleter_>
	unique_resource<value_type_, deleter_> make_resource(value_type_&& Value_, deleter_&& Deleter_){
		return unique_resource<value_type_, deleter_>(std::forward<value_type_>(Value_), std::forward<deleter_>(Deleter_));
	}
}
#
#endif
