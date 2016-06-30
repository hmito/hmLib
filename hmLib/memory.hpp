#ifndef HMLIB_MEMORY_INC
#define HMLIB_MEMORY_INC 100
#
namespace hmLib{
	template<typename T,typename D = default_deleter<const T>>
	class clone_ptr{
	private:
		typedef T* pointer;
		typedef D deleter;
		typedef clone_ptr<T, D> my_type;
		typedef rvalue_reference<my_type> rvalue_reference;
	private:
		pointer ptr;
		deleter dlt;
	public:
		clone_ptr()
			:ptr(0)
			,dlt(){
		}
		explicit clone_ptr(pointer ptr_)
			:ptr(ptr_)
			,dlt(){
		}
		clone_ptr(pointer ptr_,deleter dlt_)
			:ptr(ptr_)
			,dlt(dlt_){
		}
		explicit clone_ptr(rvalue_reference mptr_)
			:ptr(mptr_.ref.get())
			,dlt(mptr_.ref.get_deleter()) {
			mptr_.ref.release();
		}
		const my_type& operator=(rvalue_reference mptr_) {
			if(this!=&(mptr_.ref)) {
				if(ptr)dlt(ptr);
				ptr=mptr_.ref.get();
				dlt=mptr_.ref.get_deleter();
				mptr_.ref.release();
			}
			return *this;
		}
		template<typename U>
		explicit clone_ptr(hmLib::rvalue_reference<clone_ptr<U, D> > mptr_)
			: ptr(mptr_.ref.get())
			, dlt(mptr_.ref.get_deleter()) {
			mptr_.ref.release();
		}
		template<typename U>
		const my_type& operator=(hmLib::rvalue_reference<clone_ptr<U, D> > mptr_) {
			if(ptr!=mptr_.ref.get()) {
				if(ptr)dlt(ptr);
				ptr=mptr_.ref.get();
				dlt=mptr_.ref.get_deleter();
				mptr_.ref.release();
			}
			return *this;
		}
		~clone_ptr() {
			if(ptr)dlt(ptr);
		}
		void swap(my_type& My_) {
			if(this==&My_)return;

			pointer tmp_ptr=ptr;
			deleter tmp_dlt=dlt;
			ptr=My_.ptr;
			dlt=My_.dlt;
			My_.ptr=tmp_ptr;
			My_.dlt=tmp_dlt;
		}
	private:
		//copy disable
		clone_ptr(const my_type&);
		const my_type& operator=(const my_type& My_);
	public:
		pointer get()const { return ptr; }
		deleter get_deleter()const { return dlt; }
		pointer release() {
			pointer ans=ptr;
			ptr=0;
			return ptr;
		}
		void reset(pointer ptr_=0) {
			if(ptr)dlt(ptr);
			ptr=ptr_;
			dlt=deleter();
		}
		void reset(pointer ptr_, deleter dlt_) {
			if(ptr)dlt(ptr);
			ptr=ptr_;
			dlt=dlt_;
		}
		operator bool() { return ptr!=0; }
		T& operator*() { return *ptr; }
		T* operator->() { return ptr; }
		const T& operator*() const { return *ptr; }
		const T* operator->() const { return ptr; }
	};
	template<typename T, typename D>
	class clone_ptr<T[],D>{
public:
	typedef clone_ptr<_Ty[], _Dx> _Myt;
	typedef _clone_ptr_base<_Ty, _Dx> _Mybase;
	typedef typename _Mybase::pointer pointer;
	typedef _Ty element_type;
	typedef _Dx deleter_type;

	using _Mybase::get_deleter;

	_CONST_FUN clone_ptr() _NOEXCEPT
		: _Mybase(pointer())
		{	// default construct
		static_assert(!is_pointer<_Dx>::value,
			"clone_ptr constructed with null deleter pointer");
		}

	template<class _Uty>
		using _Enable_ctor_reset = enable_if_t<
			is_same<_Uty, pointer>::value
			|| (is_same<pointer, element_type *>::value
			&& is_pointer<_Uty>::value
			&& is_convertible<
				remove_pointer_t<_Uty>(*)[],
				element_type(*)[]
			>::value)>;

	template<class _Uty,
		class = _Enable_ctor_reset<_Uty> >
	explicit clone_ptr(_Uty _Ptr) _NOEXCEPT
		: _Mybase(_Ptr)
		{	// construct with pointer
		static_assert(!is_pointer<_Dx>::value,
			"clone_ptr constructed with null deleter pointer");
		}

	template<class _Uty,
		class = _Enable_ctor_reset<_Uty> >
	clone_ptr(_Uty _Ptr,
		typename _If<is_reference<_Dx>::value, _Dx,
			const typename remove_reference<_Dx>::type&>::type _Dt) _NOEXCEPT
		: _Mybase(_Ptr, _Dt)
		{	// construct with pointer and (maybe const) deleter&
		}

	template<class _Uty,
		class = _Enable_ctor_reset<_Uty> >
	clone_ptr(_Uty _Ptr,
		typename remove_reference<_Dx>::type&& _Dt) _NOEXCEPT
		: _Mybase(_Ptr, _STD move(_Dt))
		{	// construct by moving deleter
		static_assert(!is_reference<_Dx>::value,
			"clone_ptr constructed with reference to rvalue deleter");
		}

	clone_ptr(clone_ptr&& _Right) _NOEXCEPT
		: _Mybase(_Right.release(),
			_STD forward<_Dx>(_Right.get_deleter()))
		{	// construct by moving _Right
		}

	_Myt& operator=(_Myt&& _Right) _NOEXCEPT
		{	// assign by moving _Right
		if (this != &_Right)
			{	// different, do the swap
			reset(_Right.release());
			this->get_deleter() = _STD move(_Right.get_deleter());
			}
		return (*this);
		}

	template<class _Uty,
		class _Ex,
		bool _More,
		class _UP_pointer = typename clone_ptr<_Uty, _Ex>::pointer,
		class _UP_element_type = typename clone_ptr<_Uty, _Ex>::element_type>
		using _Enable_conversion = enable_if_t<
			is_array<_Uty>::value
			&& is_same<pointer, element_type *>::value
			&& is_same<_UP_pointer, _UP_element_type *>::value
			&& is_convertible<_UP_element_type(*)[], element_type(*)[]>::value
			&& _More>;

	template<class _Uty,
		class _Ex,
		class = _Enable_conversion<_Uty, _Ex,
			is_reference<_Dx>::value
			? is_same<_Ex, _Dx>::value
			: is_convertible<_Ex, _Dx>::value> >
		clone_ptr(clone_ptr<_Uty, _Ex>&& _Right) _NOEXCEPT
			: _Mybase(_Right.release(),
				_STD forward<_Ex>(_Right.get_deleter()))
		{	// construct by moving _Right
		}

	template<class _Uty,
		class _Ex,
		class = _Enable_conversion<_Uty, _Ex,
			is_assignable<_Dx&, _Ex&&>::value> >
		_Myt& operator=(clone_ptr<_Uty, _Ex>&& _Right) _NOEXCEPT
		{	// assign by moving _Right
		reset(_Right.release());
		this->get_deleter() = _STD forward<_Ex>(_Right.get_deleter());
		return (*this);
		}

	_CONST_FUN clone_ptr(nullptr_t) _NOEXCEPT
		: _Mybase(pointer())
		{	// null pointer construct
		static_assert(!is_pointer<_Dx>::value,
			"clone_ptr constructed with null deleter pointer");
		}

	_Myt& operator=(nullptr_t) _NOEXCEPT
		{	// assign a null pointer
		reset();
		return (*this);
		}

	void reset(nullptr_t = nullptr_t()) _NOEXCEPT
		{	// establish new null pointer
		reset(pointer());
		}

	void swap(_Myt& _Right) _NOEXCEPT
		{	// swap elements
		_Swap_adl(this->_Myptr(), _Right._Myptr());
		_Swap_adl(this->get_deleter(), _Right.get_deleter());
		}

	~clone_ptr() _NOEXCEPT
		{	// destroy the object
		_Delete();
		}

	_Ty& operator[](size_t _Idx) const
		{	// return reference to object
		return (get()[_Idx]);
		}

	pointer get() const _NOEXCEPT
		{	// return pointer to object
		return (this->_Myptr());
		}

	explicit operator bool() const _NOEXCEPT
		{	// test for non-null pointer
		return (get() != pointer());
		}

	pointer release() _NOEXCEPT
		{	// yield ownership of pointer
		pointer _Ans = get();
		this->_Myptr() = pointer();
		return (_Ans);
		}

	template<class _Uty,
		class = _Enable_ctor_reset<_Uty> >
	void reset(_Uty _Ptr) _NOEXCEPT
		{	// establish new pointer
		pointer _Old = get();
		this->_Myptr() = _Ptr;
		if (_Old != pointer())
			this->get_deleter()(_Old);
	}

	clone_ptr(const _Myt&) = delete;

	_Myt& operator=(const _Myt&) = delete;

	private:
		void _Delete(){	// delete the pointer
		if (get() != pointer())
			this->get_deleter()(get());
		}
	};
}
#ifdef HMLIB_MEMORY_STD
namespace std{
	
}
#endif
#
#endif
