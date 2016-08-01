#ifndef HMLIB_EITHER_INC
#define HMLIB_EITHER_INC 100
#
#include<utility>
namespace hmLib{
	template<typename value_type_, typename alternate_type_>
	struct either{
	public:
		using value_type = value_type_;
		using alternate_type = alternate_type_;
		using my_type = either<value_type, alternate_type>;
	private:
		bool IsAlternate;
		value_type Value;
		alternate_type Alternate;
	public:
		either()noexcept:IsAlternate(false) = default;
		either(const my_type& My_) = default;
		my_type& operator=(const my_type& My_) = default;
		either(my_type&& My_)noexcept = default;
		my_type& operator=(my_type&& My_)noexcept = default;
		either(value_type Value_) :IsAlternate(false), Value(std::move(Value_)), Alternate(){}
		my_type& operator=(const value_type& Value_){
			Value = Value_;
			IsAlternate = false;
			return *this;
		}
		either(alternate_type Alternate_) :IsAlternate(true), Value(), Alternate(std::move(Alternate_)){}
		my_type& operator=(const alternate_type& Alternate_){
			Alternate = Alternate_;
			IsAlternate = true;
			return *this;
		}
	public:
		operator bool()const{ return !IsAlternate; }
		value_type& operator *(){ return Value; }
		value_type* operator->(){ return &Value; }
		const value_type& operator *()const{ return Value; }
		const value_type* operator->()const{ return &Value; }
		alternate_type alternate()const{ return Alternate; }
	};
	template<typename alternate_type_>
	struct either<void, alternate_type_>{
	public:
		using value_type = value_type_;
		using alternate_type = alternate_type_;
		using my_type = either<value_type, alternate_type>;
	private:
		bool IsAlternate;
		alternate_type Alternate;
	public:
		either()noexcept : IsAlternate(false) = default;
		either(const my_type& My_) = default;
		my_type& operator=(const my_type& My_) = default;
		either(my_type&& My_)noexcept = default;
		my_type& operator=(my_type&& My_)noexcept = default;
		either(alternate_type Alternate_) :IsAlternate(true), Value(), Alternate(std::move(Alternate_)){}
		my_type& operator=(const alternate_type& Alternate_){
			Alternate = Alternate_;
			IsAlternate = true;
			return *this;
		}
	public:
		operator bool()const{ return !IsAlternate; }
		alternate_type alternate()const{ return Alternate; }
	};
}
#
#endif
