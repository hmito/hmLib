#ifndef HMLIB_UNIQUERESOURCE_INC
#define HMLIB_UNIQUERESOURCE_INC 100
#
namespace hmLib{
	template<typename value_type_, value_type_ nullvalue_, typename releaser_>
	struct unique_resource{
	public:
		using this_type = unique_resource<value_type_, releaser_>;
		using value_type = value_type_;
		using releaser = releaser_;
	private:
		value_type Value;
		releaser Releaser;
	public:
		unique_resource():Value(nullvalue_){}
		unique_resource(value_type Value_, releaser Releaser_ = releaser())
			: Value(std::move(Value_))
			, Releaser(std::move(Releaser_)){
		}
		unique_resource(const this_type&) = delete;
		this_type& opearator=(const this_type&) = delete;
		unique_resource(this_type&&) = default;
		this_type& opearator=(this_type&&) = default;
		~unique_resource(){clear();}
	public:
		operator bool()const{return Value != nullvalue_;}
		void reset(){if(Value!=nullvalue_)Releaser(Value);}
		void reset(value_type Value_, releaser Releaser_ = releaser()){
			reset();
			Value = std::move(Value_);
			Releaser = std::move(Releaser_);
		}
		value_type release(){
			value_type Ans = std::move(Value);
			Value = nullvalue_;
			return std::move(Ans);
		}
		void swap(this_type& Other){
			std::swap(Value,Other.Value);
			std::swap(Releaser,Other.Releaser);
		}
		value_type get(){return Value;}
		const value_type& ref()const{return Value;}
	public:
		friend bool operator==(const this_type& v1, const this_type& v2){
			return v1.ref() == v2.ref();
		}
		friend bool operator!=(const this_type& v1, const this_type& v2){
			return v1.ref() != v2.ref();
		}
	};
	template<typename value_type_, value_type_ nullvalue_, typename releaser_>
	unique_resource<value_type_,nullvalue_,releaser_> make_resource(value_type Value_, ){}
	template<typename value_type_, value_type_ nullvalue_, typename releaser_>
	struct shared_resource{
	public:
		using this_type = unique_resource<value_type_, releaser_>;
		using value_type = value_type_;
		using releaser = releaser_;
	private:
		value_type Value;
		releaser Releaser;
	public:
		unique_resource():Value(nullvalue_){}
		unique_resource(value_type Value_, releaser Releaser_ = releaser())
			: Value(std::move(Value_))
			, Releaser(std::move(Releaser_)){
		}
		unique_resource(const this_type&) = delete;
		this_type& opearator=(const this_type&) = delete;
		unique_resource(this_type&&) = default;
		this_type& opearator=(this_type&&) = default;
		~unique_resource(){clear();}
	public:
		operator bool()const{return Value != nullvalue_;}
		void reset(){if(Value!=nullvalue_)Releaser(Value);}
		void reset(value_type Value_, releaser Releaser_ = releaser()){
			reset();
			Value = std::move(Value_);
			Releaser = std::move(Releaser_);
		}
		value_type release(){
			value_type Ans = std::move(Value);
			Value = nullvalue_;
			return std::move(Ans);
		}
		void swap(this_type& Other){
			std::swap(Value,Other.Value);
			std::swap(Releaser,Other.Releaser);
		}
		value_type get(){return Value;}
		const value_type& ref()const{return Value;}
	public:
		friend bool operator==(const this_type& v1, const this_type& v2){
			return v1.ref() == v2.ref();
		}
		friend bool operator!=(const this_type& v1, const this_type& v2){
			return v1.ref() != v2.ref();
		}
	};
}
#
#endif
