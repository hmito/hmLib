#ifndef HMLIB_UNIQUERESOURCE_INC
#define HMLIB_UNIQUERESOURCE_INC 100
#include<boost/optional.hpp>
#
namespace hmLib{
	template<typename value_type_, typename releaser_>
	struct unique_resource{
	public:
		using this_type = unique_resource<value_type_, releaser_>;
		using value_type = value_type_;
		using optvalue_type = boost::optional<value_type>;
		using releaser = releaser_;
	private:
		optvalue_type OptValue;
		releaser Releaser;
	public:
		unique_resource() = default;
		unique_resource(value_type Value_, releaser Releaser_ = releaser())
			: OptValue(std::move(Value_))
			, Releaser(std::move(Releaser_)){}
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
		operator bool(){ return static_cast<bool>(OptValue); }
		void reset(){
			if(OptValue){
				Releaser(*OptValue);
				OptValue.reset();
			}
		}
		void reset(value_type Value_, releaser Releaser_ = releaser()){
			if(OptValue){
				Releaser(*OptValue);
			}

			OptValue.emplace(std::move(Value_));
			Releaser = std::move(Releaser_);
		}
		value_type release(){
			if(!OptValue)return value_type();
			value_type Ans = std::move(*OptValue);
			OptValue.reset();
			return std::move(Ans);
		}
		void swap(this_type& Other_){
			OptValue.swap(Other_.OptValue);
			std::swap(Releaser, Other_.Releaser);
		}
		value_type get(){return *OptValue;}
		const value_type& ref()const{return *OptValue;}
	public:
		friend bool operator==(const this_type& v1, const this_type& v2){
			return v1.ref() == v2.ref();
		}
		friend bool operator!=(const this_type& v1, const this_type& v2){
			return v1.ref() != v2.ref();
		}
	};
	template<typename value_type_, typename releaser_>
	unique_resource<value_type_, releaser_> make_resource(value_type_ Value_, releaser_ Releaser_){
		return unique_resource<value_type_, releaser_>(std::move(Value_), std::move(Releaser_));
	}
}
#
#endif
